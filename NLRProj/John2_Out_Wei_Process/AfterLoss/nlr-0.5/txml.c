/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#include <stdio.h>
#include <string.h>

#include "termes.h"
#include "txml.h"
#include "utils.h"

/* ------------------------------------------------------------
 *
 * A utility function called in several places
 *
 * ------------------------------------------------------------ */

static func_t *
func_zero(unsigned int depth)
{
    if ( depth == 0 )
        return func_constant(0);
    else
        return func_binary(func_zero(depth-1),func_constant(0));
}

/* ------------------------------------------------------------
 *
 * Changing everything to be affine (if possible).
 * This changes the original term/tuples/funcs in place
 *
 * ------------------------------------------------------------ */

static int
func_is_constant_aux(const func_t * f, value_t * v, unsigned int pathsum)
{
    if ( f->type == F_CST )
    {
        if ( pathsum == 0 )
        {
            *v = f->u.cst;
            return 1;
        }
        else
            return (f->u.cst == 0);
    }
    else
    {
        return func_is_constant_aux(f->u.bin.exp0,v,pathsum)
            && func_is_constant_aux(f->u.bin.exp1,v,pathsum+1);
    }
}

static int
func_is_constant(const func_t * f, value_t * v)
{
    return func_is_constant_aux(f,v,0);
}
static int
func_affinify(func_t * f)
{
    switch ( f->type )
    {
        case F_BIN:
        {
            int aff = func_affinify(f->u.bin.exp0);
            value_t v;
            aff = aff && func_is_constant(f->u.bin.exp1,&v);
            func_free(f->u.bin.exp1);
            f->u.bin.exp1 = func_constant(v);
            return aff;
        }
        case F_CST:
        case F_SYM:
            return 1;
    }
    return 1; /* for the compiler */
}
static int
term_affinify(term_t * t)
{
    int aff = 1;
    if ( t != NULL )
    {
        switch ( t->type )
        {
            case T_TUPLE:
                for ( unsigned int i=0 ; i<t->u.tuple->len ; i++ )
                    aff = aff && func_affinify(t->u.tuple->funcs[i]);
                break;
            case T_LOOP:
                aff = aff && func_affinify(t->u.loop.upper);
                aff = aff && term_affinify(t->u.loop.sub);
                break;
            default:
                break;
        }
        aff = aff && term_affinify(t->sib);
    }
    return aff;
}

/* ------------------------------------------------------------
 *
 * Normalization: if the body of a loop, or the top level
 * sequence of terms, is a mix between tuples and loops,
 * then wrap each tuple in an artificial loop. Note that all
 * functions are affine in the input term.
 *
 * ------------------------------------------------------------ */

static term_t *
term_norm_body(term_t * t, unsigned int depth, unsigned int * maxdepth);

static term_t *
term_norm_body_walk(term_t * t, unsigned int depth, unsigned int * maxdepth)
{
    if ( t == NULL )
        return NULL;
    else
    {
        term_t * sib = term_norm_body_walk(t->sib,depth,maxdepth);
        term_t * r = NULL;
        switch ( t->type )
        {
            case T_TUPLE:
            {
                tuple_t * tu = t->u.tuple;
                for ( unsigned int i=0 ; i<tu->len ; i++ )
                    if( tu->funcs[i]->type != F_SYM )
                        tu->funcs[i] = func_binary(tu->funcs[i],
                                                   func_constant(0));
                t->sib = NULL;
                if ( (depth+1) > *maxdepth )
                    *maxdepth = depth+1;
                return term_loop(func_zero(depth),t,sib);
            }
            case T_LOOP:
            {
                t->u.loop.sub = term_norm_body(t->u.loop.sub,depth+1,maxdepth);
                t->sib = sib;
                return t;
            }
            default:
                fatal("This should not happen (term_norm_body_walk)");
                return NULL;
        }
        return r;
    }
}
static term_t *
term_norm_body(term_t * t, unsigned int depth, unsigned int * maxdepth)
{
    if ( t->type == T_TUPLE && t->sib == NULL )
    {/* The body is made of a single tuple */
        if ( depth > *maxdepth )
            *maxdepth = depth;
        return t;
    }
    else
    {
        return term_norm_body_walk(t,depth,maxdepth);
    }
}

/* ------------------------------------------------------------
 *
 * Change of variable
 *
 * ------------------------------------------------------------ */

static void
func_inc(func_t * f)
{
    func_t * c = f;
    while ( c->type != F_CST )
        c = c->u.bin.exp0;
    ++ (c->u.cst);
}
static void
func_add(func_t * f, const func_t * a)
{
    if ( f->type == F_CST )
        f->u.cst += a->u.cst;
    else
    {
        func_add(f->u.bin.exp0,a->u.bin.exp0);
        func_add(f->u.bin.exp1,a->u.bin.exp1);
    }
}
static void
func_subscaled(func_t * f, value_t s, const func_t * g)
{
    if ( f->type == F_CST )
        f->u.cst -= s*g->u.cst;
    else
    {
        func_subscaled(f->u.bin.exp0,s,g->u.bin.exp0);
        func_subscaled(f->u.bin.exp1,s,g->u.bin.exp1);
    }
}

static value_t
func_txml_constant(const func_t * f)
{
    const func_t * c = f;
    while ( c->type != F_CST )
        c = c->u.bin.exp0;
    return c->u.cst;
}

static void
func_txml_change(func_t * f, int depth, func_t ** vch)
{
    if ( depth >= 0 )
    {
#if 0
        static const dumpfmt_t fmt = DUMPFMT_DEFAULTS;
        fprintf(stdout,"|||f=");
        func_dump(f,stdout,depth,&fmt,0);
        fprintf(stdout,"\n");
        fprintf(stdout,"|||v=");
        func_dump(vch[depth],stdout,depth,&fmt,0);
        fprintf(stdout,"\n");
        fprintf(stdout,"|||c=%lld\n",f->u.bin.exp1->u.cst);
#endif

        func_subscaled(f->u.bin.exp0,f->u.bin.exp1->u.cst,vch[depth]);
        func_txml_change(f->u.bin.exp0,depth-1,vch);
    }
}

/* ------------------------------------------------------------
 *
 * Output XML, by traversing a normalized term,
 * changing variables on the fly
 *
 * ------------------------------------------------------------ */

static void
func_txml(func_t * f, FILE * fd, int depth, func_t ** vch)
{
    static const dumpfmt_t fmt = DUMPFMT_DEFAULTS;
#if 0
    fprintf(stderr,"===\n");
    for ( unsigned int i=0 ; i<=depth ; i++ )
    {
        fprintf(stderr,"vch[%d]=",i);
        func_dump(vch[i],stderr,i,&fmt,0);
        fprintf(stderr,"\n");
    }
#endif

    /* This only gets "real" functions, no symbols */
    func_txml_change(f,depth-1,vch);
    fprintf(fd,"<fctAff cste=\"%lld\">\n",func_txml_constant(f));
    fprintf(fd,"<!-- ");
    func_dump_smart(f,fd,depth-1,&fmt,0);
    fprintf(fd," -->\n");
    func_t * c = f;
    for ( unsigned int i=depth ; i>0 ; i-- )
    {
        fprintf(fd,"<monome coeff=\"%lld\" var=\"i%d\"/>\n",
                c->u.bin.exp1->u.cst,
                i-1);
        c = c->u.bin.exp0;
    }
    fprintf(fd,"</fctAff>\n");
}


static void
tuple_txml(const tuple_t * t, FILE * fd, int depth, func_t ** vch)
{
    const char * polytrace_prefix = "polytrace:";
    fprintf(fd,"<tuple");
    /* <tuple> xml attributes */
    for ( unsigned int i=0 ; i<t->len ; i++ )
        if ( t->funcs[i]->type == F_SYM )
        {
            const char * s = t->funcs[i]->u.sym;
            if ( strncmp(s,polytrace_prefix,strlen(polytrace_prefix)) == 0 )
            {
                const char * start = s+strlen(polytrace_prefix);
                const char * equal = strchr(start,'=');
                int alen = equal-start;
                fprintf(fd,
                        " %.*s=\"%s\"",
                        alen,start,equal+1);
                        
            }
            
        }
    fprintf(fd,">\n");
    /* <tag> elements */
    for ( unsigned int i=0 ; i<t->len ; i++ )
        if ( t->funcs[i]->type == F_SYM )
        {
            const char * s = t->funcs[i]->u.sym;
            if ( strncmp(s,polytrace_prefix,strlen(polytrace_prefix)) != 0 )
            {
                const char * start = s;
                const char * equal = strchr(start,'=');
                int alen = equal-start;
                if ( equal != NULL )
                    fprintf(fd,
                            "<tag name=\"%.*s\" value=\"%s\"/>\n",
                            alen,start,equal+1);
                else
                    fprintf(fd,"<tag name=\"%s\"/>\n",s);
                        
            }
            
        }
    /* <fctAff> element< */
    for ( unsigned int i=0 ; i<t->len ; i++ )
        if ( t->funcs[i]->type != F_SYM )
            func_txml(t->funcs[i],fd,depth,vch);
    fprintf(fd,"</tuple>\n");
}


static void
term_txml_aux(const term_t * t, FILE * fd, int depth, func_t ** vch)
{
    switch ( t->type )
    {
        case T_TUPLE:
            tuple_txml(t->u.tuple,fd,depth,vch);
            break;
        case T_LOOP:
            fprintf(fd,"<for indice=\"i%d\">\n",depth);
            fprintf(fd,"<binf>\n");
            func_txml(vch[depth],fd,depth,vch);
            fprintf(fd,"</binf>\n");
            func_t * up = func_dup(t->u.loop.upper);
            func_add(up,vch[depth]);
            fprintf(fd,"<bsup>\n");
            func_txml(up,fd,depth,vch);
            fprintf(fd,"</bsup>\n");
            vch[depth+1] = func_zero(depth+1);
            term_txml_aux(t->u.loop.sub,fd,depth+1,vch);
            fprintf(fd,"</for>\n");
            if ( t->sib != NULL )
            {
                func_free(vch[depth]);
                vch[depth] = up;
                func_inc(vch[depth]);
                term_txml_aux(t->sib,fd,depth,vch);
            }
            //func_free(up);
            break;
    }
}

static int
term_txml_1(const term_t * t, FILE * fd, unsigned int maxdepth)
{
    fprintf(fd,"<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n");
    fprintf(fd,"<!DOCTYPE trace SYSTEM \"xmlTrace.dtd\">\n");
    fprintf(fd,"<trace prof=\"%d\">\n",maxdepth);
    for ( unsigned int i=0 ; i<maxdepth ; i++ )
        fprintf(fd,"<var indice=\"i%d\"/>",i);
    func_t * vch [ maxdepth+1 ];
    vch[0] = func_zero(0);
    term_txml_aux(t,fd,0,vch);
    fprintf(fd,"</trace>\n");
    return 1; /* beuh */
}

/* ------------------------------------------------------------
 *
 * The main driving routine
 *
 * ------------------------------------------------------------ */

int
term_txml(const term_t * t, model_t values, model_t bounds,
          FILE * fd, const dumpfmt_t * fmt)
{
    {values=values;bounds=bounds;fmt=fmt;}

    term_t * normal = term_dup(t);
    if ( ! term_affinify(normal) )
        fatal("Some functions are not linear, cannot convert to xml");
    /*OK: term_dump(term_aff,values,bounds,stderr,fmt);*/

    unsigned int max = 0;
    normal = term_norm_body(normal,0,&max);
    /* OK:term_dump(normal,M_LINEAR,M_LINEAR,stderr,fmt);*/

    term_txml_1(normal,fd,max);
    term_free(normal);
    return 1;
}
