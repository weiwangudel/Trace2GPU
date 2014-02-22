
#include "termes.h"
#include "iters.h"
#include "utils.h"
#include "xio.h"
#include "args.h"

#ifdef MTRACE_PLEASE
#include <mcheck.h>
#endif


typedef unsigned long long int count_t;

typedef struct s_stack {
    term_t * term;
    count_t coverage;
    unsigned int position;
    struct s_stack * prev;
} stack_t;


void
stack_free(stack_t * s)
{
    if ( s != NULL )
    {
        s->term->sib = NULL; /* Hack */
        term_free(s->term);
        stack_free(s->prev);
        GENFREE(s);
    }
}

stack_t *
stack_push(term_t * t, count_t c, stack_t * s)
{
    stack_t * n = (stack_t *)GENALLOCPTR(sizeof(stack_t));
    n->term = t;
    n->coverage = c;
    n->prev = s;
    if ( s != NULL )
    {
        s->term->sib = t;
        n->position = s->position + 1;
    }
    else
        n->position = 0;
    return n;
}

void
stack_write(const stack_t * s, FILE * fd,
            const param_t * params)
{
    /* trivial, since stack contents are reverse linked */
    if ( s == NULL )
        fprintf(fd,"(null stack)");
    else
    {
        const stack_t * f = s;
        while ( f->prev != NULL )
            f = f->prev;
        params->general.w_term(f->term,
                               params->general.values,
                               params->general.bounds,
                               fd,
                               &(params->dump));
    }
}


stack_t *
stack_split(stack_t * s, FILE * fd,
            const param_t * params)
{
    if ( s->position <= params->general.flushstack )
    {
        stack_write(s,fd,params);
        return NULL;
    }
    else
    {
        stack_t * curr = s;
        while ( curr->position > params->general.flushstack )
        {
            curr->position -= params->general.flushstack;
            curr = curr->prev;
        }
        curr->prev->term->sib = NULL;
        stack_write(curr->prev,fd,params);
        stack_free(curr->prev);
        curr->prev = NULL;
        return s;
    }
}

void
stack_debug(const stack_t * s, FILE * fd, unsigned int size,
            const param_t * params)
{
    if ( s != NULL && size > 0 )
    {
        stack_debug(s->prev,fd,size-1,params);
        const char * fmt;
        if ( s->prev == NULL )
            fmt = "=== %d =================\nTerm: ";
        else if ( size == 1 )
            fmt = "... %d .................\nTerm: ";
        else
            fmt = "--- %d -----------------\nTerm: ";
        fprintf(fd,fmt,s->position);
        term_t * tsib = s->term->sib;
        s->term->sib = NULL; // avoid walk-up of the stack with term chains
        term_dump(s->term,
                  params->general.values,params->general.bounds,
                  fd,&(params->dump));
        s->term->sib = tsib;
        fprintf(fd,"Coverage: %lld\n",s->coverage);
    }
}



stack_t *
stack_reduce(stack_t * s, const param_t * params)
{
    const unsigned int K = params->terms.k;
    stack_t * precs [3*K];
    precs[0] = s;
    unsigned int i;
    for ( i=1 ; (precs[i-1]->prev!=NULL && i<3*K) ; i++ )
    {
        precs[i] = precs[i-1]->prev;
        /* NEW */
        if ( (i+1) % 3 == 0 )
        {
            unsigned int breadth = (i+1) / 3;
            /* Temporarily remove chaining */
            precs[breadth]->term->sib = NULL;
            precs[breadth+breadth]->term->sib = NULL;

            /* Test for 3-alignment */
            term_t * triplet =
                term_triplet(precs[breadth+breadth+breadth-1]->term,
                             precs[breadth+breadth-1]->term,
                             precs[breadth-1]->term,
                             params->terms.unif3_value,
                             params->terms.unif3_bound);
            if ( triplet != NULL )
            {
                count_t n = 0;
                for ( unsigned int j=0 ; j<=i ; j++ )
                    n += precs[j]->coverage;
                /* Replace triplet by a loop */
                stack_t * newstack = stack_push(triplet,n,precs[i]->prev);
                precs[i]->prev = NULL;
                stack_free(s);
                /* Recurse and return */
                return stack_reduce(newstack,params);
            }

            /* Otherwise, reset chaining */
            precs[breadth]->term->sib = precs[breadth-1]->term;
            precs[breadth+breadth]->term->sib = precs[breadth+breadth-1]->term;
        }
        /* ADD */
        if ( i <= K )
        {
            /* Temporarily remove chaining */
            precs[i]->term->sib = NULL;

            /* Test for followup */
            if ( term_follows(precs[i]->term,precs[i-1]->term) )
            {
                count_t n = 0;
                for ( unsigned int j=0 ; j<i ; j++ )
                    n += precs[j]->coverage;
                /* Remove all but first term and increase loop width */
                precs[i-1]->prev = NULL;
                stack_free(s);
                ++( precs[i]->term->u.loop.upper->u.cst );
                precs[i]->coverage += n;
                /* Recurse and return */
                return stack_reduce(precs[i],params);
            }

            /* Otherwise, reset chaining */
            precs[i]->term->sib = precs[i-1]->term;
        }
    }
    return s;
}


void /* ?!? */
stack_predict(stack_t * s, const param_t * params, unsigned long long int at)
{
    stack_t * current = s;
    iter_t * predictor = NULL;
    for ( unsigned int i=1
              ; i<=params->iki.p
              && current->prev!=NULL
              /* Use the last found : && predictor==NULL */
              ; i++ )
    {
        current = current->prev;
        term_t * big = current->term;
        term_t * small = big->sib;
        /* Temporarily break chaining */
        big->sib = NULL;
        iter_t * thisone = term_next_complete(big,small);
        if ( thisone != NULL )
        {
            if ( predictor != NULL )
                iter_free(predictor);
            predictor = thisone;
        }
#if 0
        if ( predictor != NULL )
        {
            fprintf(stderr,"=== PREDICTOR\n");
            term_dump(big,stderr);
            iter_dump(predictor,stderr);
        }
#endif
        big->sib = small;
    }
    /* Fall back on the top loop's next turn */
    if ( predictor == NULL && s->term->type == T_LOOP )
    {
        predictor = iter_after(s->term);
#if 0
        fprintf(stderr,"=== FALLBACK\n");
        term_dump(s->term,stderr);
        iter_dump(predictor,stderr);
#endif
    }
    /* Let's predict if we can */
    if ( predictor != NULL )
    {
        for ( unsigned int i=0 ; i<params->iki.d ; i++ )
            iter_advance(predictor,1);
        for ( unsigned int i=0 ; i<params->iki.w ; i++ )
        {
	    /* This test is stupid */
	    if ( iter_void(predictor) )
            {
                fprintf(stderr,"P:%lld:%lld:PASS\n",
                        at,at+1+params->iki.d+i);
            }
            else
            {
                tuple_t * t = iter_get(predictor);
                fprintf(stderr,"P:%lld:%lld:#",
                        at,at+1+params->iki.d+i);
                tuple_dump_raw(t,stderr);
                fprintf(stderr,"\n");
                tuple_free(t);
                iter_advance(predictor,1);
            }
        }
        iter_free(predictor);
    }
    else
    {
        for ( unsigned int i=0 ; i<params->iki.w ; i++ )
        {
            fprintf(stderr,"P:%lld:%lld:PASS\n",
                    at,at+1+params->iki.d+i);
        }
    }
}



int
main(int argc, char ** argv)
{
    param_t params = PARAM_DEFAULTS;
    params.dump.color = isatty(1);
    stack_t * s = NULL;
    term_t * one;
    unsigned long long int count = 0;

#ifdef MTRACE_PLEASE
    mtrace();
#endif

    sigs_setup();
    scan_args(argc,argv,&params);

    while ( (one=params.general.r_term(params.general.values,
                                       params.general.bounds,
                                       stdin)) != NULL )
    {
        ++count;
        if ( ( params.debug.verbosity & 0x1 && count % 1000 == 0 )
             || sigs_usr1() )
            fprintf(stderr,"%-20lld\r",count);

        if ( params.iki.p > 0 )
        {
            fprintf(stderr,"O:%lld:%lld:#",count,count);
            tuple_dump_raw(one->u.tuple,stderr);
            fprintf(stderr,"\n");
        }

        s = stack_push(one,1ULL,s);
        s = stack_reduce(s,&params);

        if ( params.iki.p > 0 )
            stack_predict(s,&params,count);

        if ( (s->position+1) >= params.general.maxstack )
            s = stack_split(s,stdout,&params);
        if ( ( params.debug.every > 0 && count % params.debug.every == 0 )
             || sigs_usr2() )
            stack_debug(s,stderr,params.debug.size,&params);
    }

    stack_write(s,stdout,&params);
    stack_free(s);
    return 0;
}
