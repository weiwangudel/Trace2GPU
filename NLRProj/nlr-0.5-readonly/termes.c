/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#include "termes.h"
#include "utils.h"


/* a dictionary to hold symbols */
struct dico {
    const char * entry;
    struct dico * next;
};

static struct dico * symbols = NULL;

static const char *
dico_find(const char * s)
{
    struct dico * prev = NULL;
    struct dico * curr = symbols;
    int cmp;
    while ( curr != NULL && (cmp=strcmp(curr->entry,s)) < 0 )
    {
        prev = curr;
        curr = curr->next;
    }
    if ( curr == NULL )
    {   /* not found, insert at the end */
        struct dico * n = (struct dico *)malloc(sizeof(struct dico));
        n->entry = mystrdup(s);
        n->next = NULL;
        if ( prev != NULL )
            prev->next = n;
        else
            symbols = n;
        return n->entry;
    }
    else if ( cmp > 0 )
    {   /* not found, insert in the middle */
        struct dico * n = (struct dico *)malloc(sizeof(struct dico));
        n->entry = mystrdup(s);
        n->next = curr;
        if ( prev != NULL )
            prev->next = n;
        else
            symbols = n;
        return n->entry;
    }
    else /* cmp == 0 : found */
    {
        return curr->entry;
    }
}



static func_t * func_repo = NULL;

static func_t * func_repo_get()
{
    if ( func_repo == NULL )
        return (func_t*)GENALLOCPTR(sizeof(func_t));
    else
    {
        func_t * r = func_repo;
        func_repo = func_repo->u.bin.exp0;
        return r;
    }
}
static void func_repo_put(func_t * f)
{
    if ( f->type == F_BIN )
    {
        func_repo_put(f->u.bin.exp0);
        func_repo_put(f->u.bin.exp1);
    }
    f->u.bin.exp0 = func_repo;
    func_repo = f;
}


func_t *
func_constant(value_t cst)
{
    func_t * n = func_repo_get(); /*GENALLOCATM(sizeof(func_t));*/
    n->type = F_CST;
    n->u.cst = cst;
    return n;
}

func_t *
func_binary(func_t * l, func_t * r)
{
    func_t * n = func_repo_get(); /*GENALLOCPTR(sizeof(func_t));*/
    n->type = F_BIN;
    n->u.bin.exp0 = l;
    n->u.bin.exp1 = r;
    return n;
}

func_t *
func_symbol(const char * s)
{
    func_t * n = func_repo_get(); /*GENALLOCATM(sizeof(func_t));*/
    n->type = F_SYM;
    n->u.sym = dico_find(s);
    return n;
}

void
func_free(func_t * f)
{
    func_repo_put(f);
    /*
    switch ( f->type )
    {
        case F_CST:
            break;
        case F_BIN:
            func_free(f->u.bin.exp0);
            func_free(f->u.bin.exp1);
            break;
    }
    GENFREE(f);
    */
}


func_t *
func_dup(const func_t * f)
{
    func_t * r = NULL;
    switch ( f->type )
    {
        case F_CST:
            r = func_constant(f->u.cst);
            break;
        case F_BIN:
            r = func_binary(func_dup(f->u.bin.exp0),func_dup(f->u.bin.exp1));
            break;
        case F_SYM:
            r = func_symbol(f->u.sym);
            break;
    }
    return r;
}

void
func_dump(const func_t * f, FILE * fd, int depth,
          const dumpfmt_t * fmt, int inval)
{
    switch ( f->type )
    {
        case F_CST:
            fprintf(fd,"%lld",f->u.cst);
            break;
        case F_BIN:
            fprintf(fd,"(");
            func_dump(f->u.bin.exp0,fd,depth-1,fmt,inval);
            fprintf(fd,") + i%d*(",depth);
            func_dump(f->u.bin.exp1,fd,depth-1,fmt,inval);
            fprintf(fd,")");
            break;
        case F_SYM:
            fprintf(fd,"%s",f->u.sym);
            break;
    }
}

static struct outputdescr {
    const char * pref;
    const char * suff;
} ansisequences [] = {
    { "\033[31m", "\033[0m" }, /* red */
    { "\033[32m", "\033[0m" }, /* green */
    { "\033[34m", "\033[0m" }, /* blue */
    { "\033[35m", "\033[0m" }, /* magenta */
    { "\033[33m", "\033[0m" }, /* yellow */
    { "\033[36m", "\033[0m" }, /* cyan */
    { "\033[1m", "\033[0m" },  /* bright */
    { NULL , NULL }
};

static void
highlight_indice(int i, int color, const char * name, FILE * fd)
{
    extern int fileno(FILE *);
    //if ( isatty(fileno(fd)) )
    if ( color )
    {
        int p = 0;
        while ( p < i && ansisequences[p].pref != NULL )
            ++p;
        if ( ansisequences[p].pref == NULL )
            --p;
        fprintf(fd,"%s%s%d%s",
                ansisequences[p].pref,name,i,ansisequences[p].suff);
    }
    else
    {
        fprintf(fd,"%s%d",name,i);
    }
}

void
func_dump_smart_aux(const func_t * f, FILE * fd, int depth,
                    int * flags, int len,
                    const dumpfmt_t * fmt, int inval,
                    int * pfirst)
{
    switch ( f->type )
    {
        case F_CST:
        {
            if ( f->u.cst != 0 )
            {
                if ( ! *pfirst )
                    fprintf(fd," + ");
                int ffirst = 0;
                 //FIXME: si c'est le premier coeff...
                /*
                if ( f->u.cst == 1 )
                    ffirst = 1;
                else
                */
                int hexa = fmt->hexa;
                if ( fmt->hexa == 1 ) /* constants only */
                {
                    //hexa = hexa && inval;// to avoid hex in bounds
                    for ( int i=depth+1 ; hexa && i<len ; i++ )
                        if ( flags[i] )
                            hexa = 0;
                }
                fprintf(fd,(hexa?"0x%llx":"%lld"),f->u.cst);
                for ( int i=depth+1 ; i<len ; i++ )
                    if ( flags[i] )
                    {
                        //fprintf(fd,"*i%d",i);
                        if ( ! ffirst )
                            fprintf(fd,"*");
                        highlight_indice(i,fmt->color,fmt->indice,fd);
                        ffirst = 0;
                    }
                *pfirst = 0;
            }
            break;
        }
        case F_BIN:
            flags[depth] = 0;
            func_dump_smart_aux(f->u.bin.exp0,fd,depth-1,flags,len,
                                fmt,inval,pfirst);
            flags[depth] = 1;
            func_dump_smart_aux(f->u.bin.exp1,fd,depth-1,flags,len,
                                fmt,inval,pfirst);
            break;
        case F_SYM:
            /* Doesn't happen anyway */
            break;
    }
}

void
func_dump_smart(const func_t * f, FILE * fd, int depth,
                const dumpfmt_t * fmt, int inval)
{
    if ( f->type == F_SYM )
    {
        if ( fmt->color )
            fprintf(fd,"\033[01m%s\033[0m",f->u.sym);
        else
            fprintf(fd,"%s",f->u.sym);
    }
    else
    {
        int flags[depth+1];
        int first = 1;
        func_dump_smart_aux(f,fd,depth,flags,depth+1,fmt,inval,&first);
        if ( first )
            fprintf(fd,"0");
    }
}

static func_t *
func_unif_interpole(value_t x1, value_t y1, value_t x2, value_t y2)
{
     /* avoid all these / ( __divdi3) and % (__moddi3) because
        gprof tells me that these are where most of the work is done */
    if ( x1 == 0 && x2 == 1 )
    {
        return func_binary(func_constant(y1),func_constant(y2-y1));
    }
    else
    {
        value_t dx = x2 - x1;
        value_t dy = y2 - y1;
        if ( dy % dx != 0 )
            return NULL;
        value_t dz = x2*y1 - x1*y2;
        if ( dz % dx != 0 )
            return NULL;
        return func_binary(func_constant(dz/dx),func_constant(dy/dx));
    }
}

static func_t *
func_unif3_interpole(value_t x1, value_t y1,
                     value_t x2, value_t y2,
                     value_t x3, value_t y3)
{
     /* avoid all these / ( __divdi3) and % (__moddi3) because
        gprof tells me that these are where most of the work is done */
    if ( x1 == 0 && x2 == 1 && x3 == 2)
    {
        if ( y2-y1 == y3-y2 )
            return func_binary(func_constant(y1),func_constant(y2-y1));
        else
            return NULL;
    }
    else
    {
        value_t dx = x2 - x1;
        value_t dy = y2 - y1;
        if ( dy % dx != 0 )
            return NULL;
        value_t dz = x2*y1 - x1*y2;
        if ( dz % dx != 0 )
            return NULL;
        if ( dy*x3 + dz == dx*y3 )
            return func_binary(func_constant(dz/dx),func_constant(dy/dx));
        else
            return NULL;
    }
}

func_t *
func_unif_poly(const func_t * f1, int p1, const func_t * f2, int p2)
{
    if ( f1->type != f2->type )
        return NULL;
    else
    {
        func_t * r = NULL;
        switch ( f1->type /* which is the same as f2->type */ )
        {
            case F_CST:
            {
                r = func_unif_interpole((value_t)p1,f1->u.cst,
                                        (value_t)p2,f2->u.cst);
                /*r = func_binary(func_constant(f1->u.cst),
                  func_constant(f2->u.cst - f1->u.cst));*/
                break;
            }
            case F_BIN:
            {
                func_t * g0 = NULL;
                func_t * g1 = NULL;
                if ( (g0=func_unif_poly(f1->u.bin.exp0,p1,
                                        f2->u.bin.exp0,p2)) != NULL
                     &&
                     (g1=func_unif_poly(f1->u.bin.exp1,p1,
                                        f2->u.bin.exp1,p2)) != NULL )
                {
                    r = func_binary(g0,g1);
                }
                else
                {
                    if ( g0 != NULL ) func_free(g0);
                    if ( g1 != NULL ) func_free(g1);
                    r = NULL;
                }
                break;
            }
            case F_SYM:
            {
                if ( f1->u.sym == f2->u.sym )
                    r = func_symbol(f1->u.sym);
                else
                    r = NULL;
            }
        }
        return r;
    }
}
func_t *
func_unif3_poly(const func_t * f1, int p1,
                const func_t * f2, int p2,
                const func_t * f3, int p3)
{
    if ( f1->type != f2->type || f1->type != f3->type )
        return NULL;
    else
    {
        func_t * r = NULL;
        switch ( f1->type /* which is the same as f2->type */ )
        {
            case F_CST:
            {
                r = func_unif3_interpole((value_t)p1,f1->u.cst,
                                         (value_t)p2,f2->u.cst,
                                         (value_t)p3,f3->u.cst);
                break;
            }
            case F_BIN:
            {
                func_t * g0 = NULL;
                func_t * g1 = NULL;
                if ( (g0=func_unif3_poly(f1->u.bin.exp0,p1,
                                         f2->u.bin.exp0,p2,
                                         f3->u.bin.exp0,p3)) != NULL
                     &&
                     (g1=func_unif3_poly(f1->u.bin.exp1,p1,
                                         f2->u.bin.exp1,p2,
                                         f3->u.bin.exp1,p3)) != NULL )
                {
                    r = func_binary(g0,g1);
                }
                else
                {
                    if ( g0 != NULL ) func_free(g0);
                    if ( g1 != NULL ) func_free(g1);
                    r = NULL;
                }
                break;
            }
            case F_SYM:
            {
                if ( f1->u.sym == f2->u.sym && f1->u.sym == f3->u.sym )
                    r = func_symbol(f1->u.sym);
                else
                    r = NULL;
            }
        }
        return r;
    }
}

func_t *
func_unif_affine(const func_t * f1, int p1, const func_t * f2, int p2)
{
    if ( f1->type != f2->type )
        return NULL;
    else
    {
        func_t * r = NULL;
        switch ( f1->type /* which is the same as f2->type */ )
        {
            case F_CST:
            {
                r = func_unif_interpole((value_t)p1,f1->u.cst,
                                        (value_t)p2,f2->u.cst);
                /*r = func_binary(func_constant(f1->u.cst),
                  func_constant(f2->u.cst - f1->u.cst));*/
                break;
            }
            case F_BIN:
            {
                func_t * g0 = NULL;
                if ( f1->u.bin.exp1->u.cst == f2->u.bin.exp1->u.cst
                     &&
                     (g0=func_unif_affine(f1->u.bin.exp0,p1,
                                          f2->u.bin.exp0,p2)) != NULL )
                {
                    r = func_binary(g0,func_constant(f1->u.bin.exp1->u.cst));
                }
                else
                {
                    if ( g0 != NULL ) func_free(g0);
                    r = NULL;
                }
                break;
            }
            case F_SYM:
            {
                if ( f1->u.sym == f2->u.sym )
                    r = func_symbol(f1->u.sym);
                else
                    r = NULL;
            }
        }
        return r;
    }
}
func_t *
func_unif3_affine(const func_t * f1, int p1,
                  const func_t * f2, int p2,
                  const func_t * f3, int p3)
{
    if ( f1->type != f2->type || f1->type != f3->type )
        return NULL;
    else
    {
        func_t * r = NULL;
        switch ( f1->type /* which is the same as f2->type */ )
        {
            case F_CST:
            {
                r = func_unif3_interpole((value_t)p1,f1->u.cst,
                                         (value_t)p2,f2->u.cst,
                                         (value_t)p3,f3->u.cst);
                /*r = func_binary(func_constant(f1->u.cst),
                  func_constant(f2->u.cst - f1->u.cst));*/
                break;
            }
            case F_BIN:
            {
                func_t * g0 = NULL;
                if ( f1->u.bin.exp1->u.cst == f2->u.bin.exp1->u.cst
                     &&
                     f1->u.bin.exp1->u.cst == f3->u.bin.exp1->u.cst
                     &&
                     (g0=func_unif3_affine(f1->u.bin.exp0,p1,
                                           f2->u.bin.exp0,p2,
                                           f3->u.bin.exp0,p3)) != NULL )
                {
                    r = func_binary(g0,func_constant(f1->u.bin.exp1->u.cst));
                }
                else
                {
                    if ( g0 != NULL ) func_free(g0);
                    r = NULL;
                }
                break;
            }
            case F_SYM:
            {
                if ( f1->u.sym == f2->u.sym && f1->u.sym == f3->u.sym )
                    r = func_symbol(f1->u.sym);
                else
                    r = NULL;
            }
        }
        return r;
    }
}

func_t *
func_unif_constant(const func_t * f1, int p1, const func_t * f2, int p2)
{
    {p1=p1;p2=p2;}
    if ( f1->type != f2->type )
        return NULL;
    else
    {
        func_t * r = NULL;
        switch ( f1->type /* which is the same as f2->type */ )
        {
            case F_CST:
            {
                if ( f1->u.cst == f2->u.cst )
                    r = func_constant(f1->u.cst);
                else
                    r = NULL;
                break;
            }
            case F_BIN:
            {
                r = NULL;
                break;
            }
            case F_SYM:
            {
                if ( f1->u.sym == f2->u.sym )
                    r = func_symbol(f1->u.sym);
                else
                    r = NULL;
            }
        }
        return r;
    }
}
func_t *
func_unif3_constant(const func_t * f1, int p1,
                    const func_t * f2, int p2,
                    const func_t * f3, int p3)
{
    {p1=p1;p2=p2;p3=p3;}
    if ( f1->type != f2->type || f1->type != f3->type )
        return NULL;
    else
    {
        func_t * r = NULL;
        switch ( f1->type /* which is the same as f2->type */ )
        {
            case F_CST:
            {
                if ( f1->u.cst == f2->u.cst && f1->u.cst == f3->u.cst )
                    r = func_constant(f1->u.cst);
                else
                    r = NULL;
                break;
            }
            case F_BIN:
            {
                r = NULL;
                break;
            }
            case F_SYM:
            {
                if ( f1->u.sym == f2->u.sym && f1->u.sym == f3->u.sym )
                    r = func_symbol(f1->u.sym);
                else
                    r = NULL;
            }
        }
        return r;
    }
}

int
func_isomorphic(const func_t * f1, const func_t * f2)
{
    if ( f1->type == F_SYM && f2->type == F_SYM )
    {
        return f1->u.sym == f2->u.sym;
    }
    else
    {
        return 1; /* ? */
    }
}

int
func_match(const func_t * f, int pos, const func_t * g)
{
    if ( f->type == F_SYM && g->type == F_SYM )
    {
        return f->u.sym == g->u.sym;
    }
    else if ( f->type == F_CST
         && g->type == F_BIN
         && g->u.bin.exp0->type == F_CST
         && g->u.bin.exp1->type == F_CST )
    {
        return f->u.cst == (g->u.bin.exp0->u.cst + pos*g->u.bin.exp1->u.cst);
    }
    else if ( f->type == F_CST && g->type == F_CST )
    {
        return f->u.cst == g->u.cst;
    }
    else if ( f->type == F_BIN && g->type == F_BIN )
    {
        return func_match(f->u.bin.exp0,pos,g->u.bin.exp0)
            && func_match(f->u.bin.exp1,pos,g->u.bin.exp1);
    }
    else
    {
        return 0;
    }
}

int
func_mayfollow(const func_t * big, const func_t * small, value_t n)
{
    if ( small->type == F_SYM && big->type == F_SYM )
    {
        return small->u.sym == big->u.sym;
    }
    else if ( small->type == F_SYM || big->type == F_SYM )
    {
        return 0;
    }
    else if ( small->type == F_CST )
    {
        const func_t * sub = big;
        while ( sub->u.bin.exp0->type != F_CST )
            sub = sub->u.bin.exp0;
        return
            (sub->u.bin.exp0->u.cst + n*sub->u.bin.exp1->u.cst)
            ==
            small->u.cst;
    }
    else
    {
        return func_mayfollow(big->u.bin.exp0,small->u.bin.exp0,n)
            && func_mayfollow(big->u.bin.exp1,small->u.bin.exp1,n);
    }
}


value_t
func_eval(const func_t * f, value_t * v, unsigned int depth)
{
    /*FIXME: doesn't work with affine/constant funcs, or symbols */
    if ( depth == 0 )
        return f->u.cst;
    else
        return func_eval(f->u.bin.exp0,v,depth-1)
            + func_eval(f->u.bin.exp1,v,depth-1) * v[depth-1];
}


static tuple_t * tuple_repos [] = {
    /* 0*/ NULL, NULL, NULL, NULL,
    /* 4*/ NULL, NULL, NULL, NULL,
    /* 8*/ NULL, NULL, NULL, NULL,
    /*12*/ NULL, NULL, NULL, NULL
};

static tuple_t * tuple_repo_get(unsigned int l)
{
    if ( l > 16 || tuple_repos[l-1] == NULL )
    {
        tuple_t * n = (tuple_t*)GENALLOCPTR(sizeof(tuple_t)+l*sizeof(func_t *));
        n->len = l;
        for ( unsigned int i=0 ; i<n->len ; i++ )
            n->funcs[i] = NULL;
        return n;
    }
    else
    {
        tuple_t * r = tuple_repos[l-1];
        tuple_repos[l-1] = (tuple_t *)tuple_repos[l-1]->funcs[0];
        return r;
    }
}
static void tuple_repo_put(tuple_t * t)
{
    for ( unsigned int i=0 ; i<t->len ; i++ )
        if ( t->funcs[i] != NULL )
            func_free(t->funcs[i]);
    if ( t->len > 16 )
        GENFREE(t);
    else
    {
        t->funcs[0] = (func_t *)tuple_repos[t->len-1];
        tuple_repos[t->len-1] = t;
    }
}



tuple_t *
tuple_new(unsigned int l)
{
    return tuple_repo_get(l);
    /*
    tuple_t * n = GENALLOCPTR(sizeof(tuple_t)+l*sizeof(func_t *));
    n->len = l;
    for ( unsigned int i=0 ; i<n->len ; i++ )
        n->funcs[i] = NULL;
    return n;
    */
}

void
tuple_free(tuple_t * t)
{
    tuple_repo_put(t);
    /*
    for ( unsigned int i=0 ; i<t->len ; i++ )
        if ( t->funcs[i] != NULL )
            func_free(t->funcs[i]);
    GENFREE(t);
    */
}

tuple_t *
tuple_dup(const tuple_t * t)
{
    tuple_t * n = tuple_new(t->len);
    for ( unsigned int i=0 ; i<t->len ; i++ )
        n->funcs[i] = func_dup(t->funcs[i]);
    return n;
}

void
tuple_dump(const tuple_t * t, FILE * fd, int depth,
           void (*fdump)(const func_t * f, FILE * fd, int depth,
                         const dumpfmt_t * fmt, int inval),
           const dumpfmt_t * fmt)
{
    /*FIXME: use fmt->separ and fmt->termin*/
    if ( t->len <= fmt->hmax )
    {
        fprintf(fd,"%*s",depth*(fmt->indent),"");
        fprintf(fd,"val ");
        for ( unsigned int f=0 ; f<t->len ; f++ )
        {
            if ( f > 0 )
                fprintf(fd," , ");
            fdump(t->funcs[f],fd,depth-1,fmt,1);
        }
        fprintf(fd,"\n");
    }
    else
    {
        for ( unsigned int f=0 ; f<t->len ; f++ )
        {
            fprintf(fd,"%*s",depth*(fmt->indent),"");
            fprintf(fd,( f==0 ? "val " : "  , " ));
            fdump(t->funcs[f],fd,depth-1,fmt,1);
            fprintf(fd,"\n");
        }
    }
}

void
tuple_dump_raw(const tuple_t * t, FILE * fd)
{
    const char * prefix = "";
    for ( unsigned int f=0 ; f<t->len ; f++ )
    {
        switch ( t->funcs[f]->type    )
        {
            case F_CST:
                fprintf(fd,"%s%lld",prefix,t->funcs[f]->u.cst);
                break;
            case F_SYM:
                fprintf(fd,"%s%s",prefix,t->funcs[f]->u.sym);
                break;
            case F_BIN:
                fatal("tuple_dump_raw called on non-constant");
                break;
        }
        prefix = " ";
    }
}


static int
tuple_parse_aux_number(const char * start, char ** endptr,
                       value_t * n)
{
    *n = strtoll(start,endptr,0);
    if ( ( n == 0 && start == *endptr ) // no number
         || ( **endptr != '\0' && ! isspace(**endptr) ) // bad suffix
         || ( errno != 0 ) ) // under/over-flow 
        return 0;
    else
        return 1;
}

static int
tuple_parse_aux_symbol(const char * start, char ** endptr,
                       char * buffer, unsigned int len)
{
    static const char * acc =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        ".-_:/@=";
    const char * p = start;
    char * r = buffer;
    if ( ! isalnum(*p) )
        return 0;
    *r++ = *p++;
    while ( ! isspace(*p) && --len > 1 )
    {
        if ( strchr(acc,*p) != NULL )
            *r++ = *p++;
        else
            return 0;
    }
    if ( isspace(*p) )
    {
        *endptr = (char *)p; // Merde...
        *r = '\0';
        return 1;
    }
    else
        return 0;
}

tuple_t *
tuple_parse_aux(unsigned int pos, const char * s)
{
    const char * start = s;

    while ( isspace(*start) )
        ++start;
    if ( *start == '\0' )
        return ( pos > 0 ? tuple_new(pos) : NULL );
    errno = 0;
    char * endptr = NULL;
    value_t n;
    char buffer[256];
    func_t * f;
    if ( tuple_parse_aux_number(start,&endptr,&n) )
        f = func_constant(n);
    else if ( tuple_parse_aux_symbol(start,&endptr,buffer,256) )
        f = func_symbol(buffer);
    else
        return NULL;
    tuple_t * t = tuple_parse_aux(pos+1,endptr);
    if ( t == NULL )
        return NULL;
    t->funcs[pos] = f;
    return t;
}


tuple_t *
tuple_parse(const char * s)
{
    return tuple_parse_aux(0,s);
}

tuple_t *
tuple_unif(const tuple_t * t1, int p1,
           const tuple_t * t2, int p2,
           func_unif_t unif)
{
    if ( t1->len != t2->len )
        return NULL;
    else
    {
        func_t * fu [ t1->len ];
        for ( unsigned int i=0 ; i<t1->len ; i++ )
        {
            if ( (fu[i]=unif(t1->funcs[i],p1,t2->funcs[i],p2)) == NULL )
            {
                for ( unsigned int j=0 ; j<i ; j++ )
                    func_free(fu[j]);
                return NULL;
            }
        }
        tuple_t * u = tuple_new(t1->len);
        for ( unsigned int i=0 ; i<t1->len ; i++ )
            u->funcs[i] = fu[i];
        return u;
    }
}
tuple_t *
tuple_unif3(const tuple_t * t1, int p1,
            const tuple_t * t2, int p2,
            const tuple_t * t3, int p3,
            func_unif3_t unif)
{
    if ( t1->len != t2->len || t1->len != t3->len )
        return NULL;
    else
    {
        func_t * fu [ t1->len ];
        for ( unsigned int i=0 ; i<t1->len ; i++ )
        {
            if ( (fu[i]=unif(t1->funcs[i],p1,
                             t2->funcs[i],p2,
                             t3->funcs[i],p3)) == NULL )
            {
                for ( unsigned int j=0 ; j<i ; j++ )
                    func_free(fu[j]);
                return NULL;
            }
        }
        tuple_t * u = tuple_new(t1->len);
        for ( unsigned int i=0 ; i<t1->len ; i++ )
            u->funcs[i] = fu[i];
        return u;
    }
}

int
tuple_isomorphic(const tuple_t * t1, const tuple_t * t2)
{
    if ( t1->len != t2->len )
        return 0;
    else
        return 1;
}

int
tuple_match(const tuple_t * t, int pos, const tuple_t * g)
{
    if ( t->len != g->len )
        return 0;
    else
    {
        for ( unsigned int i=0 ; i<t->len ; i++ )
            if ( ! func_match(t->funcs[i],pos,g->funcs[i]) )
                return 0;
        return 1;
    }
}

int
tuple_mayfollow(const tuple_t * big, const tuple_t * small, value_t n)
{
    if ( big->len != small->len )
        return 0;
    else
    {
        for ( unsigned int i=0 ; i<big->len ; i++ )
            if ( ! func_mayfollow(big->funcs[i],small->funcs[i],n) )
                return 0;
        return 1;
    }
}


static term_t * term_repo = NULL;

static term_t * term_repo_get()
{
    if ( term_repo == NULL )
        return (term_t*)GENALLOCPTR(sizeof(term_t));
    else
    {
        term_t * r = term_repo;
        term_repo = term_repo->sib;
        return r;
    }
}
static void term_repo_put(term_t * t)
{
    if ( t->sib != NULL )
        term_repo_put(t->sib);
    switch ( t->type )
    {
        case T_TUPLE:
            tuple_free(t->u.tuple);
            break;
        case T_LOOP:
            func_free(t->u.loop.upper);
            term_repo_put(t->u.loop.sub);
            break;
    }
    t->sib = term_repo;
    term_repo = t;
}

term_t *
term_tuple(tuple_t * tuple, term_t * sib)
{
    term_t * n = term_repo_get(); /*GENALLOCPTR(sizeof(term_t));*/
    n->type = T_TUPLE;
    n->u.tuple = tuple;
    n->sib = sib;
    return n;
}

term_t *
term_loop(func_t * upper, term_t * sub, term_t * sib)
{
    term_t * n = term_repo_get(); /*GENALLOCPTR(sizeof(term_t));*/
    n->type = T_LOOP;
    n->u.loop.upper = upper;
    n->u.loop.sub = sub;
    n->sib = sib;
    return n;
}

void
term_free(term_t * t)
{
    term_repo_put(t);
    /*
    if ( t->sib != NULL )
        term_free(t->sib);
    switch ( t->type )
    {
        case T_TUPLE:
            tuple_free(t->u.tuple);
            break;
        case T_LOOP:
            func_free(t->u.loop.upper);
            term_free(t->u.loop.sub);
            break;
    }
    GENFREE(t);
    */
}

term_t *
term_dup(const term_t * t)
{
    term_t * r = NULL;
    term_t * nsib = ( t->sib == NULL ? NULL : term_dup(t->sib) );
    switch ( t->type )
    {
        case T_TUPLE:
            r = term_tuple(tuple_dup(t->u.tuple),nsib);
            break;
        case T_LOOP:
            r = term_loop(func_dup(t->u.loop.upper),
                          term_dup(t->u.loop.sub),
                          nsib);
            break;
    }
    return r;
}

term_t *
term_text(model_t values, model_t bounds, FILE * fd)
{
    {values=values;bounds=bounds;}
    char buffer[4096];
    while ( 1 )
    {
        char * r = fgets(buffer,sizeof(buffer),fd);
        if ( r != NULL )
        {
            tuple_t * t = tuple_parse(buffer);
            if ( t != NULL )
                return term_tuple(t,NULL);
            else
                fprintf(stderr,"Line '%s' ignored\n",buffer);
        }
        else if ( feof(fd) )
            return NULL;
        else if ( ferror(fd) && errno == EINTR )
            /* silently restart */;
        else
            fatal("I/O error: %s",strerror(errno));
    }
    return NULL;
}



/* FIXME */
void
term_dump_aux(const term_t * t, FILE * fd, int depth,
              const dumpfmt_t * fmt)
{
    void (*dumpers [2])(const func_t * f, FILE * fd, int depth,
                        const dumpfmt_t * fmt, int inval) = {
        func_dump , func_dump_smart
    };
    const int dumper = 1;
    switch ( t->type )
    {
        case T_TUPLE:
            tuple_dump(t->u.tuple,fd,depth,dumpers[dumper],fmt);
            break;
        case T_LOOP:
            fprintf(fd,"%*s",depth*(fmt->indent),"");
            fprintf(fd,"for ");
            highlight_indice(depth,fmt->color,fmt->indice,fd);
            fprintf(fd," = 0 to ");
            dumpers[dumper](t->u.loop.upper,fd,depth-1,fmt,0);
            if ( 0 ) /* FIXME */
            {
                char buffer[1024];
                unsigned int n = 0;
                term_desc(t,buffer,1024,&n);
                buffer[n] = 0;
                fprintf(fd," %*s",n,buffer);
            }
            fprintf(fd,"\n");
            term_dump_aux(t->u.loop.sub,fd,depth+1,fmt);
            if ( fmt->delim )
                fprintf(fd,"%*sdone\n",depth*(fmt->indent),"");
            break;
    }
    if ( t->sib != NULL )
        term_dump_aux(t->sib,fd,depth,fmt);
}

void term_desc(const term_t * t, char * str, unsigned int len, unsigned int * n)
{
    switch ( t->type )
    {
        case T_TUPLE:
            if ( *n < len )
                str[*n] = 'V';
            ++ (*n);
            break;
        case T_LOOP:
            if ( *n < len )
                str[*n] = 'F';
            ++ (*n);
            term_desc(t->u.loop.sub,str,len,n);
            if ( *n < len )
                str[*n] = 'D';
            ++ (*n);
            break;
    }
    if ( t->sib != NULL )
        term_desc(t->sib,str,len,n);
}

int
term_dump(const term_t * t, model_t values, model_t bounds,
          FILE * fd, const dumpfmt_t * fmt)
{
    {values=values;bounds=bounds;}
    term_dump_aux(t,fd,0,fmt);
    return 1; /* beuh */
}

int
term_gobble(const term_t * t, model_t values, model_t bounds,
            FILE * fd, const dumpfmt_t * fmt)
{
    {t=t;values=values;bounds=bounds;fd=fd;fmt=fmt;}
    return 1; /* beuh */
}

void
generic_dump(term_t * g, unsigned int gpos, FILE * fd)
{
    /*
    func_t upperbound = { .type=F_CST, .u={ .cst=gpos } };
    */
    func_t upperbound;
    upperbound.type = F_CST;
    upperbound.u.cst = gpos;
    /*
    term_t wrapper = { .type=T_LOOP,
                      .u={ .loop={&upperbound,g} },
                      .sib=NULL};
    */
    term_t wrapper;
    wrapper.type = T_LOOP;
    wrapper.u.loop.upper = &upperbound;
    wrapper.u.loop.sub = g;
    wrapper.sib = NULL;
    dumpfmt_t fmt = DUMPFMT_DEFAULTS;
    term_dump_aux(&wrapper,fd,0,&fmt);
}

term_t *
term_unif(const term_t * t1, int p1, const term_t * t2, int p2,
          func_unif_t u_value, func_unif_t u_bound)
{
    term_t * usib = NULL;
    term_t * r = NULL;

    if ( t1->type != t2->type )
        return NULL;

    if ( ( t1->sib == NULL && t2->sib == NULL )
         ||
         ( t1->sib != NULL && t2->sib != NULL
           &&
           (usib=term_unif(t1->sib,p1,t2->sib,p2,u_value,u_bound)) != NULL ) )
        ; /* OK */
    else
        return NULL;

    switch ( t1->type /* which is the same as t2->type */ )
    {
        case T_TUPLE:
        {
            tuple_t * utuple = tuple_unif(t1->u.tuple,p1,
                                          t2->u.tuple,p2,
                                          u_value);
            if ( utuple == NULL )
            {
                if ( usib != NULL ) term_free(usib);
                r = NULL;
            }
            else
                r = term_tuple(utuple,usib);
            break;
        }
        case T_LOOP:
        {
            func_t * uupper = NULL;
            term_t * usub = NULL;
            if ( (usub=term_unif(t1->u.loop.sub,p1,
                                 t2->u.loop.sub,p2,
                                 u_value,u_bound)) != NULL
                 &&
                 (uupper=u_bound(t1->u.loop.upper,p1,
                                 t2->u.loop.upper,p2)) != NULL )
            {
                r = term_loop(uupper,usub,usib);
            }
            else
            {
                if ( uupper != NULL ) func_free(uupper);
                if ( usub != NULL ) term_free(usub);
                if ( usib != NULL ) term_free(usib);
                r = NULL;
            }
            break;
        }
    }
    return r;
}
term_t *
term_unif3(const term_t * t1, int p1,
           const term_t * t2, int p2,
           const term_t * t3, int p3,
           func_unif3_t u_value, func_unif3_t u_bound)
{
    term_t * usib = NULL;
    term_t * r = NULL;

    if ( t1->type != t2->type || t1->type != t3->type )
        return NULL;

    if ( ( t1->sib == NULL && t2->sib == NULL && t3->sib == NULL )
         ||
         ( t1->sib != NULL && t2->sib != NULL && t3->sib != NULL
           &&
           (usib=term_unif3(t1->sib,p1,t2->sib,p2,t3->sib,p3,
                            u_value,u_bound)) != NULL ) )
        ; /* OK */
    else
        return NULL;

    switch ( t1->type /* which is the same as t2->type */ )
    {
        case T_TUPLE:
        {
            tuple_t * utuple = tuple_unif3(t1->u.tuple,p1,
                                           t2->u.tuple,p2,
                                           t3->u.tuple,p3,
                                           u_value);
            if ( utuple == NULL )
            {
                if ( usib != NULL ) term_free(usib);
                r = NULL;
            }
            else
                r = term_tuple(utuple,usib);
            break;
        }
        case T_LOOP:
        {
            func_t * uupper = NULL;
            term_t * usub = NULL;
            if ( (usub=term_unif3(t1->u.loop.sub,p1,
                                  t2->u.loop.sub,p2,
                                  t3->u.loop.sub,p3,
                                  u_value,u_bound)) != NULL
                 &&
                 (uupper=u_bound(t1->u.loop.upper,p1,
                                 t2->u.loop.upper,p2,
                                 t3->u.loop.upper,p3)) != NULL )
            {
                r = term_loop(uupper,usub,usib);
            }
            else
            {
                if ( uupper != NULL ) func_free(uupper);
                if ( usub != NULL ) term_free(usub);
                if ( usib != NULL ) term_free(usib);
                r = NULL;
            }
            break;
        }
    }
    return r;
}

int
term_isomorphic(const term_t * t1, const term_t * t2)
{
    if ( t1->type != t2->type )
        return 0;
    switch ( t1->type )
    {
        case T_TUPLE:
            if ( ! tuple_isomorphic(t1->u.tuple,t2->u.tuple) )
                return 0;
            break;
        case T_LOOP:
            if ( ! term_isomorphic(t1->u.loop.sub,t2->u.loop.sub) )
                return 0;
            break;
    }
    if ( t1->sib == NULL && t2->sib == NULL )
        return 1;
    else if ( t1->sib == NULL || t2->sib == NULL )
        return 0;
    else
        return term_isomorphic(t1->sib,t2->sib);
}

int
term_match(const term_t * t, int pos, const term_t * g)
{
    int r = 0;
    if ( t->type != g->type )
        return 0;

    if ( ( t->sib == NULL && g->sib == NULL )
         ||
         ( t->sib != NULL && g->sib != NULL
           && term_match(t->sib,pos,g->sib) ) )
        r = 1; /* OK */
    else
        return 0;

    switch ( t->type /* which is the same as g->type */ )
    {
        case T_TUPLE:
            r = r && tuple_match(t->u.tuple,pos,g->u.tuple);
            break;
        case T_LOOP:
            r = r
                && func_match(t->u.loop.upper,pos,g->u.loop.upper)
                && term_match(t->u.loop.sub,pos,g->u.loop.sub);
            break;
    }
    return r;
}

int
term_mayfollow_match(const term_t * big, const term_t * small, value_t n)
{
    if ( big == NULL && small == NULL )
        return 1;
    else if ( big == NULL && small != NULL )
        return 0;
    else if ( big != NULL && small == NULL )
        return 1;
    else if ( big->type != small->type )
        return 0;
    else if ( ! term_mayfollow_match(big->sib,small->sib,n) )
        return 0;
    else
    {
        int r = 0;
        switch ( big->type )
        {
            case T_TUPLE:
                r = tuple_mayfollow(big->u.tuple,small->u.tuple,n);
                break;
            case T_LOOP:
                r = func_mayfollow(big->u.loop.upper,small->u.loop.upper,n)
                    && term_mayfollow_match(big->u.loop.sub,
                                            small->u.loop.sub,
                                            n);
                break;
        }
        return r;
    }
}

int
term_mayfollow(const term_t * big, const term_t * small)
{
    const term_t * dlb = big;
    const term_t * dls = small;
    unsigned int db = 0;
    unsigned int ds = 0;
    while ( dlb->type != T_TUPLE )
    {
        dlb = dlb->u.loop.sub;
        ++db;
    }
    while ( dls->type != T_TUPLE )
    {
        dls = dls->u.loop.sub;
        ++ds;
    }
    if ( db <= ds )
        return 0;
    value_t n = big->u.loop.upper->u.cst + 1;
    dlb = big;
    while ( db > ds )
    {
        dlb = dlb->u.loop.sub;
        --db;
    }
    return term_mayfollow_match(dlb,small,n);
}

term_t *
term_triplet(const term_t * f, const term_t * s, const term_t * t,
             func_unif3_t u_value, func_unif3_t u_bound)
{
    /*
    term_t * u = term_unif(f,0,s,1,u_value,u_bound);
    if ( u == NULL )
    {
        return NULL;
    }
    else
    {
        term_t * triplet = term_loop(func_constant(1),u,NULL);
        if ( term_match(t,2,triplet->u.loop.sub) )
        {
            triplet->u.loop.upper->u.cst = 2;
            return triplet;
        }
        else
        {
            term_free(triplet);
            return NULL;
        }
    }
    */
    term_t * u = term_unif3(f,0,s,1,t,2,u_value,u_bound);
    if ( u == NULL )
    {
        return NULL;
    }
    else
    {
        return term_loop(func_constant(2),u,NULL);
    }
}

int
term_follows(const term_t * l, const term_t * t)
{
    int r = 0;
    switch ( l->type )
    {
        case T_TUPLE:
            r = 0;
            break;
        case T_LOOP:
        {
            value_t n = l->u.loop.upper->u.cst; /* must be constant here */
            r = term_match(t,n+1,l->u.loop.sub);
        }
    }
    return r;
}


