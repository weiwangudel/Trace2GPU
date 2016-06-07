/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#include "iters.h"
#include "utils.h"

static inline iter_t *
iter_init()
{
    iter_t * it = (iter_t*)GENALLOCPTR(sizeof(iter_t));
    it->len = 0;
    it->size = 8;
    it->indices = (unsigned int *)GENALLOCPTR(it->size*sizeof(unsigned int));
    it->termes = (const term_t **)GENALLOCPTR(it->size*sizeof(term_t *));
    return it;
}

static inline void
iter_app(iter_t * it, unsigned int indice, const term_t * t)
{
    if ( it->size <= it->len )
    {
        it->size += 8;
        it->indices = (unsigned int *)GENREALLOC(it->indices,
                                                 it->size*sizeof(unsigned int));
        it->termes = (const term_t **)GENREALLOC(it->termes,
                                                 it->size*sizeof(term_t *));
    }
    it->indices[it->len] = indice;
    it->termes[it->len] = t;
    ++(it->len);
}

static inline void
iter_next(iter_t * it)
{
    unsigned int last = it->len - 1;
    ++(it->indices[last]);
    it->termes[last] = it->termes[last]->sib;
}

static inline void
iter_complete(iter_t * it)
{
    /* First make sure we are left on/after an horizontal move */
    if ( it->len %2 == 0 )
    {
        unsigned int pos = it->len;
        it->indices[pos] = 0;
        it->termes[pos] = it->termes[pos-1];
        ++(it->len);
    }
    const term_t * current = it->termes[it->len-1];
    while ( current->type != T_TUPLE )
    {
        current = current->u.loop.sub;
        iter_app(it,0,current); /* vertical */
        iter_app(it,0,current); /* horizontal */
    }
}


iter_t *
iter_start(const term_t * t)
{
    iter_t * it = iter_init();
    iter_app(it,0,t); /* horizontal */
    iter_complete(it);
    return it;
}

iter_t *
iter_after(const term_t * t)
{
    value_t ups = t->u.loop.upper->u.cst;
    iter_t * it = iter_init();
    iter_app(it,0,t); /* horizontal */
    iter_app(it,ups+1,t->u.loop.sub); /* vertical */
    iter_complete(it);
    return it;
}

void
iter_free(iter_t * it)
{
    GENFREE(it->indices);
    GENFREE(it->termes);
    GENFREE(it);
}

void
iter_dump(const iter_t * it, FILE * fd)
{
    for ( unsigned int i=0 ; i<it->len ; i++ )
    {
        if ( i % 2 == 0 )
            fprintf(fd,"[%d]",it->indices[i]);
        else
            fprintf(fd,"(%d)",it->indices[i]);
        if ( it->termes[i]->type == T_LOOP )
            fprintf(fd,"<L>");
        else
            fprintf(fd,"<T>");
    }
    fprintf(fd,"\n");
}

int
iter_void(const iter_t * it)
{
    return it->len == 0;
}


static value_t
iter_eval_func(const iter_t * it, const func_t * f)
{
    unsigned int depth = (it->len-1) / 2;
    value_t vector [ depth+1 ]; /* because depth can be 0 */
    for ( unsigned int i=1 ; i<it->len ; i+=2 )
        vector[ (i-1)/2 ] = it->indices[i];
    return func_eval(f,vector,depth);
}

tuple_t *
iter_get(const iter_t * it)
{
    /*FIXME: vector is recomputed for each element */
    const tuple_t * s = it->termes[it->len-1]->u.tuple;
    tuple_t * r = tuple_new(s->len);
    for ( unsigned int i=0 ; i<s->len ; i++ )
    {
        if ( s->funcs[i]->type == F_SYM )
            r->funcs[i] = func_symbol(s->funcs[i]->u.sym);
        else
            r->funcs[i] = func_constant(iter_eval_func(it,s->funcs[i]));
    }
    return r;
}


static int
iter_advance_term(iter_t * it, int wraproot)
{
    while ( 1 )
    {
        if ( it->len == 0 )
        {
            return 0;
        }
        else if ( it->len % 2 == 1 ) /* last move is horizontal */
        {
            if ( it->termes[it->len-1]->sib != NULL )
            {
                //fprintf(stderr,"[[HSIB]]\n");
                ++( it->indices[it->len-1] );
                it->termes[it->len-1] = it->termes[it->len-1]->sib;
                //iter_complete(it);
                return 1;
            }
            else
            {
                //fprintf(stderr,"[[HBAK]]\n");
                --(it->len);
            }
        }
        else /* last move is vertical */
        {
            /* vertical move actually point to thefirst child */
            const term_t * t = it->termes[it->len-2];
            if ( t->type == T_LOOP )
            {
                func_t * u = t->u.loop.upper;
                value_t upb = iter_eval_func(it,u);
                //fprintf(stderr,"[[NEXT,u=%lld]]\n",upb);
                if ( it->indices[it->len-1] < upb
                     || ( it->len == 2 && wraproot ) )
                {
                    ++( it->indices[it->len-1] );
                    iter_app(it,0,it->termes[it->len-1]); // !?!
                    //iter_complete(it);
                    return 1;
                }
                else
                {
                    --(it->len);
                }
            }
            else
            {
                //fprintf(stderr,"[[VBAK]]\n");
                --(it->len);
            }
        }
    }
}


int
iter_advance(iter_t * it, int wraproot)
{
    int r = iter_advance_term(it,wraproot);
    if ( r )
        iter_complete(it);
    return r;
}


static int
func_eq(const func_t * big, const iter_t * sofar,
        const func_t * small, unsigned int depth)
{
    if ( depth == 0 )
    {
        value_t e = iter_eval_func(sofar,big);
        return small->u.cst == e;
    }
    else
    {
        return func_eq(big->u.bin.exp0,sofar,
                       small->u.bin.exp0,depth-1)
            && func_eq(big->u.bin.exp1,sofar,
                       small->u.bin.exp1,depth-1);
    }
}

static int
term_eq(const term_t * big, const iter_t * sofar,
        const term_t * small, unsigned int depth)
{
    /* Returns true of false depending on match */
    if ( big == NULL && small == NULL )
    {
        return 1;
    }
    else if ( big == NULL || small == NULL )
    {
        return 0;
    }
    int r = 0;
    if ( big->type != small->type )
    {
        r = 0;
    }
    else if ( big->type == T_TUPLE ) /* == small->type */
    {
        if ( big->u.tuple->len != small->u.tuple->len )
        {
            r = 0;
        }
        else
        {
            r = 1;
            for ( unsigned int i=0 ; i<big->u.tuple->len ; i++ )
                r = r && func_eq(big->u.tuple->funcs[i],sofar,
                                 small->u.tuple->funcs[i],depth);
        }
    }
    else /* big->type == small->type == T_LOOP */
    {
        r = func_eq(big->u.loop.upper,sofar,
                    small->u.loop.upper,depth)
            && term_eq(big->u.loop.sub,sofar,
                       small->u.loop.sub,depth+1);
    }
    r = r && term_eq(big->sib,sofar,small->sib,depth);
    return r;
}

static value_t
term_eq_top(const term_t * big, const term_t * small, const iter_t * sofar)
{
    /* Returns -1 for complete difference, 0 for exact match, n for
       match on everything except the upper bound on the topmost loop
       (which applies only to loops, of course) */
    /* This is very similar to term_eq, except that:
       1) it doesn't continue on siblings
       2) it discriminates between equality and less-than-ity
       3) depth is always 0 (that's small's depth)
    */
    if ( big->type != small->type )
    {
        return -1;
    }
    else if ( big->type == T_TUPLE ) /* == small->type */
    {
        if ( big->u.tuple->len != small->u.tuple->len )
        {
            return -1;
        }
        else
        {
            for ( unsigned int i=0 ; i<big->u.tuple->len ; i++ )
                if ( ! func_eq(big->u.tuple->funcs[i],sofar,
                               small->u.tuple->funcs[i],0) )
                    return -1;
            return 0;
        }
    }
    else /* big->type == small->type == T_LOOP */
    {
        if ( ! term_eq(big->u.loop.sub,sofar,
                       small->u.loop.sub,1) )
        {
            return -1;
        }
        else
        {
            value_t upb = iter_eval_func(sofar,big->u.loop.upper);
            value_t ups = small->u.loop.upper->u.cst;
            if ( upb == ups )
                return 0;
            else if ( upb > ups )
                return ups+1;
            else
                return -1;
        }
    }
}


static iter_t *
term_next_walk(iter_t * sofar, const term_t * small)
{
#if 0
    fprintf(stderr,"====================\n");
    fprintf(stderr,"SOFAR = ");
    iter_dump(sofar,stderr);
    //fprintf(stderr,"\n");
    term_dump(sofar->termes[sofar->len-1],stderr);
    fprintf(stderr,"\n");
    fprintf(stderr,"SMALL =\n");
    if ( small != NULL )
        term_dump(small,stderr);
    else
        fprintf(stderr,"(null)");
    fprintf(stderr,"\n");
#endif
    if ( small == NULL )
        return sofar;
    else if ( iter_void(sofar) )
        return NULL;
    else
    {
        value_t e = term_eq_top(sofar->termes[sofar->len-1],small,sofar);
#if 0
        fprintf(stderr,"TEQOP = %lld\n",e);
#endif
        if ( e == -1 )
        {
            const term_t * last = sofar->termes[sofar->len-1];
            if ( last->type == T_TUPLE )
                return NULL;
            else
            {
                const term_t * next = last->u.loop.sub;
                iter_app(sofar,0,next);
                iter_app(sofar,0,next);
                return term_next_walk(sofar,small);
            }
        }
        else if ( e == 0 )
        {
            const term_t * savetop_term = sofar->termes[0];
            value_t savetop_ind = sofar->indices[1];
            iter_advance_term(sofar,0/*wraproot*/);
            if ( iter_void(sofar) )
            {
                /* We've exhausted the topmost loop, start another cycle */
                const term_t * topsub = savetop_term->u.loop.sub;
                iter_app(sofar,0,savetop_term);
                iter_app(sofar,savetop_ind+1,topsub);
                iter_app(sofar,0,topsub);
            }
            return term_next_walk(sofar,small->sib);
        }
        else /* e == -1, meaning that not all cycles have been eaten */
        {
            /* e is the next cycle to start */
            const term_t * next = sofar->termes[sofar->len-1]->u.loop.sub;
            iter_app(sofar,e,next);
            iter_app(sofar,0,next);
            /* Why couldn't we find a few more cycles inside small? */
            return term_next_walk(sofar,small->sib);
        }
    }
}

iter_t *
term_next(const term_t * big, const term_t * small)
{
    if ( big->type == T_TUPLE )
        return NULL;
    iter_t * sofar = iter_init();
    value_t n = big->u.loop.upper->u.cst+1;
    const term_t * sub = big->u.loop.sub;
    iter_app(sofar,0,big);
    iter_app(sofar,n,sub);
    iter_app(sofar,0,sub);
    iter_t * next = term_next_walk(sofar,small);
    if ( next == NULL )
    {
        iter_free(sofar);
        return NULL;
    }
    else
    {
        // iter_advance(sofar);
        return sofar;
    }
}

iter_t *
term_next_complete(const term_t * big, const term_t * small)
{
    iter_t * r = term_next(big,small);
    if ( r != NULL )
        iter_complete(r);
    return r;
}
