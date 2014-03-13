/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "termes.h"
#include "xio.h"

#define lexer_is_char(l) ((l)->type==LX_CHR)
#define lexer_is_number(l) ((l)->type==LX_NBR)
#define lexer_eof(l) ((l)->type==LX_EOF)
#define lexer_is(l,s) (lexer_is_char(l)&&strcmp(l->u.chr,s)==0)
#define lexer_number(l) ((l)->u.nbr)
#define lexer_string(l) ((l)->u.chr)

static inline void
lexer_debug(const lexer_t * l)
{
    {l=l;}
#if 0
    static char * lexnames [] = { "CHR", "NBR", "EOF" };
    fprintf(stderr,"--> [%s]",lexnames[l->type]);
    if ( lexer_is_char(l) )
        fprintf(stderr,"%s",l->u.chr);
    else if ( lexer_is_number(l) )
        fprintf(stderr,"%lld",lexer_number(l));
    fprintf(stderr,"\n");
#endif
}

static void
lexer_advance(lexer_t * l)
{
    if ( lexer_eof(l) )
    {
        l->type = LX_EOF;
        lexer_debug(l);
        return;
    }
    char c = fgetc(l->fd);
    while ( c != EOF && isspace(c) )
        c = fgetc(l->fd);
    if ( feof(l->fd) )
    {
        l->type = LX_EOF;
        lexer_debug(l);
        return;
    }
    char buffer[256];
    unsigned int next = 0;
    while ( next < 255 && c != EOF && ! isspace(c) )
    {
        buffer[next++] = c;
        c = fgetc(l->fd);
    }
    if ( next == 255 )
        fatal("Token with more than 255 chars. Abort.\n");
    if ( c == EOF && next == 0 )
    {
        l->type = LX_EOF;
        lexer_debug(l);
        return;
    }
    buffer[next] = '\0';
    char * endp;
    errno = 0;
    value_t val = strtoll(buffer,&endp,0);
    if ( (errno == ERANGE ) || ( errno != 0 && val == 0 ) )
        fatal("Number out of range");
    if ( endp == buffer || *endp != '\0' )
    {
        l->type = LX_CHR;
        strncpy(l->u.chr,buffer,255);
        l->u.chr[strlen(l->u.chr)] = '\0';
    }
    else
    {
        l->type = LX_NBR;
        l->u.nbr = val;
    }
    lexer_debug(l);
}

static void
lexer_init(lexer_t * l, FILE * fd)
{
    l->fd = fd;
    lexer_advance(l);
}

lexer_t *
lexer_new(FILE * fd)
{
    lexer_t * l = (lexer_t*)GENALLOCATM(sizeof(lexer_t));
    lexer_init(l,fd);
    return l;
}

void
lexer_free(lexer_t * l)
{
    GENFREE(l);
}



typedef void (*func_export_t)(const func_t * f,
                              unsigned int depth,
                              FILE * fd);

typedef func_t * (*func_import_t)(unsigned int depth,
                                  lexer_t * l);



static void
func_export_poly_aux(const func_t * f, unsigned int depth, FILE * fd)
{
    if ( depth == 0 )
    {
        fprintf(fd," %lld",f->u.cst);
    }
    else
    {
        func_export_poly_aux(f->u.bin.exp0,depth-1,fd);
        func_export_poly_aux(f->u.bin.exp1,depth-1,fd);
    }
}

static void
func_export_poly(const func_t * f, unsigned int depth, FILE * fd)
{
    /* FIXME */
    fprintf(fd," P");
    func_export_poly_aux(f,depth,fd);
}

static void
func_export_linear_aux(const func_t * f, unsigned int depth, FILE * fd)
{
    if ( depth == 0 )
    {
        fprintf(fd," %lld",f->u.cst);
    }
    else
    {
        func_export_linear_aux(f->u.bin.exp0,depth-1,fd);
        fprintf(fd," %lld",f->u.bin.exp1->u.cst);
    }
}

static void
func_export_linear(const func_t * f, unsigned int depth, FILE * fd)
{
    /* FIXME */
    fprintf(fd," A");
    func_export_linear_aux(f,depth,fd);
}

static void
func_export_constant(const func_t * f, unsigned int depth, FILE * fd)
{
    {depth=depth;}
    fprintf(fd," C %lld",f->u.cst);
}

static func_export_t exporters [] = {
#ifdef __cplusplus
    func_export_poly, func_export_linear, func_export_constant
#else
    [M_POLY]= func_export_poly,
    [M_LINEAR] = func_export_linear,
    [M_CONSTANT] = func_export_constant
#endif
};

static void
func_export(model_t model, const func_t * f, unsigned int depth, FILE * fd)
{
    if ( f->type == F_SYM )
        fprintf(fd," S %s",f->u.sym);
    else
        exporters[model](f,depth,fd);
}

static func_t *
func_import_poly_aux(unsigned int depth, lexer_t * l)
{
    if ( depth == 0 )
    {
        if ( ! lexer_is_number(l) )
            fatal("Syntax error: expecting number");
        value_t v = lexer_number(l);
        lexer_advance(l);
        return func_constant(v);
    }
    else
    {
        func_t * t0 = func_import_poly_aux(depth-1,l);
        func_t * t1 = func_import_poly_aux(depth-1,l);
        return func_binary(t0,t1);
    }
}

static func_t *
func_import_poly(unsigned int depth, lexer_t * l)
{
    if ( ! lexer_is(l,"P") )
        fatal("Syntax error: expecting polynomial");
    lexer_advance(l);
    return func_import_poly_aux(depth,l);
}

static func_t *
func_import_linear_aux(unsigned int depth, lexer_t * l)
{
    if ( depth == 0 )
    {
        if ( ! lexer_is_number(l) )
            fatal("Syntax error: expecting number");
        value_t v = lexer_number(l);
        lexer_advance(l);
        return func_constant(v);
    }
    else
    {
        func_t * t0 = func_import_linear_aux(depth-1,l);
        if ( ! lexer_is_number(l) )
            fatal("Syntax error: expecting number");
        value_t v = lexer_number(l);
        lexer_advance(l);
        return func_binary(t0,func_constant(v));
    }
}

static func_t *
func_import_linear(unsigned int depth, lexer_t * l)
{
    if ( ! lexer_is(l,"A") )
        fatal("Syntax error: expecting polynomial");
    lexer_advance(l);
    return func_import_linear_aux(depth,l);
}

static func_t *
func_import_constant(unsigned int depth, lexer_t * l)
{
    {depth=depth;}
    if ( ! lexer_is(l,"C") )
        fatal("Syntax error: expecting polynomial");
    lexer_advance(l);
    if ( ! lexer_is_number(l) )
        fatal("Syntax error: expecting number");
    value_t v = lexer_number(l);
    lexer_advance(l);
    return func_constant(v);
}

static func_import_t importers [] = {
#ifdef __cplusplus
    func_import_poly, func_import_linear, func_import_constant
#else
    [M_POLY]= func_import_poly,
    [M_LINEAR] = func_import_linear,
    [M_CONSTANT] = func_import_constant
#endif
};

static func_t *
func_import(model_t model, unsigned int depth, lexer_t * l)
{
    if ( lexer_is(l,"S") )
    {
        lexer_advance(l);
        if ( ! lexer_is_char(l) )
            fatal("Syntax error: expecting symbole");
        func_t * fs = func_symbol(lexer_string(l));
        lexer_advance(l);
        return fs;
    }
    else
        return importers[model](depth,l);
}

static void
tuple_export(const tuple_t * t, unsigned int depth, model_t model,
             FILE * fd)
{
    fprintf(fd," %d",t->len);
    for ( unsigned int i=0 ; i<t->len ; i++ )
        func_export(model,t->funcs[i],depth,fd);
}

static tuple_t *
tuple_import(unsigned int depth, model_t model,
             lexer_t * l)
{
    if ( ! lexer_is_number(l) )
        fatal("Syntax error: expecting number");
    tuple_t * t = tuple_new(lexer_number(l));
    lexer_advance(l);
    for ( unsigned int i=0 ; i<t->len ; i++ )
        t->funcs[i] = func_import(model,depth,l);
    return t;
}

static void
term_export_aux(const term_t * t, unsigned int depth,
                model_t values, model_t bounds,
                // func_export_t x_value, func_export_t x_bound,
                FILE * fd,
                const dumpfmt_t * fmt)
{
    if ( t == NULL )
        fprintf(fd," N");
    else if ( t->type == T_LOOP )
    {
        fprintf(fd," L");
        func_export(bounds,t->u.loop.upper,depth,fd);
        term_export_aux(t->u.loop.sub,depth+1,values,bounds,fd,fmt);
        term_export_aux(t->sib,depth,values,bounds,fd,fmt);
    }
    else /* t->type == T_TUPLE */
    {
        fprintf(fd," T");
        tuple_export(t->u.tuple,depth,values,fd);
        term_export_aux(t->sib,depth,values,bounds,fd,fmt);
    }
}

int
term_export(const term_t * t,
            model_t values, model_t bounds,
            // func_export_t x_value, func_export_t x_bound,
            FILE * fd,
            const dumpfmt_t * fmt)
{
    term_export_aux(t,0,values,bounds,fd,fmt);
    fprintf(fd,"\n");
    return 1; /* beuh */
}


static term_t *
term_import_aux(unsigned int depth,
                model_t values, model_t bounds,
                // func_import_t i_value, func_import_t i_bound,
                lexer_t * l)
{
    if ( lexer_is(l,"N") )
    {
        lexer_advance(l);
        return NULL;
    }
    else if ( lexer_is(l,"L") )
    {
        lexer_advance(l);
        func_t * u = func_import(bounds,depth,l);
        term_t * sub = term_import_aux(depth+1,values,bounds,l);
        term_t * sib = term_import_aux(depth,values,bounds,l);
        return term_loop(u,sub,sib);
    }
    else if ( lexer_is(l,"T") )
    {
        lexer_advance(l);
        tuple_t * t = tuple_import(depth,values,l);
        term_t * sib = term_import_aux(depth,values,bounds,l);
        return term_tuple(t,sib);
    }
    else
    {
        fatal("Syntax error: expected N or L or T (@%ld)",ftell(l->fd));
        return NULL; /* never reached */
    }
}

term_t *
term_import(model_t values, model_t bounds,
            // func_import_t i_value, func_import_t i_bound,
            FILE * fd)
{
    static FILE * from = NULL;
    static lexer_t lexer;
    if ( from == NULL )
    {
        from = fd;
        lexer_init(&lexer,from);
    }
    else if ( fd != from )
        fatal("Don't mess with input file(s)");
    term_t * t = lexer_eof(&lexer)
        ? NULL : term_import_aux(0,values,bounds,&lexer);
    return t;
}

term_t *
term_import_piecewise(model_t values, model_t bounds,
                      FILE * fd)
{
    static term_t * next = NULL;
    if ( next == NULL )
    {
        next = term_import(values,bounds,fd);
        if ( next == NULL )
            return NULL;
    }
    term_t * r = next;
    next = next->sib;
    r->sib = NULL;
    return r;
}

/*
static term_t *
term_import_lex(func_import_t i_value, func_import_t i_bound,
                lexer_t * l)
{
    return term_import_aux(0,i_value,i_bound,l);
}
*/
