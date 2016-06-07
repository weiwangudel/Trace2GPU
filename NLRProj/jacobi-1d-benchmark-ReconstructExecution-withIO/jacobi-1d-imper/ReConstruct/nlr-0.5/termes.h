/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#ifndef _TERMES_H_INCLUDED
#define _TERMES_H_INCLUDED


#include <stdio.h>
#include <unistd.h>


typedef long long int value_t;

enum e_functype { F_CST, F_BIN, F_SYM };
typedef struct s_func  {
    enum e_functype type;
    union u_funcbody {
        value_t cst;
        struct s_funcbin { /* may be expanded to accomodate higher degrees */
            struct s_func * exp0;
            struct s_func * exp1;
        } bin;
        const char * sym;
    } u;
} func_t;

typedef struct s_dumpfmt {
    /* functions */
    int hexa; /* 0=decimal, 1=valeurs, constantes, 2=tout */
    int color;
    const char * indice;
    /* tuples */
    const char * separ;
    const char * termin;
    /* terms */
    int delim;
    /* common */
    unsigned int indent;
    unsigned int hmax;
} dumpfmt_t;

#define DUMPFMT_DEFAULTS {                             \
        INITFIELD(hexa,1),                             \
        INITFIELD(color,0), /*isatty(1)*/              \
        INITFIELD(indice,"i"),                         \
        INITFIELD(separ," , "),                        \
        INITFIELD(termin,";"),                         \
        INITFIELD(delim,0),                            \
        INITFIELD(indent,2),                           \
            INITFIELD(hmax,5)                          \
}

typedef enum { M_POLY, M_LINEAR, M_CONSTANT } model_t;


typedef func_t * (*func_unif_t)(const func_t * f1, int p1,
                                const func_t * f2, int p2);

typedef func_t * (*func_unif3_t)(const func_t * f1, int p1,
                                 const func_t * f2, int p2,
                                 const func_t * f3, int p3);

typedef struct s_tuple {
    unsigned int len;
    func_t * funcs [];
} tuple_t;

enum e_termtype { T_TUPLE, T_LOOP };
typedef struct s_term {
    enum e_termtype type;
    union u_termbody {
        tuple_t * tuple;
        struct s_termloop {
            func_t * upper;
            struct s_term * sub;
        } loop;
    } u;
    struct s_term * sib;
} term_t;



func_t *
func_constant(value_t cst);

func_t *
func_binary(func_t * l, func_t * r);

func_t *
func_symbol(const char * s);

func_t *
func_dup(const func_t * f);

void
term_desc(const term_t * t, char * str, unsigned int len, unsigned int * n);

void
func_dump(const func_t * f, FILE * fd, int depth,
          const dumpfmt_t * fmt, int inval);

void
func_dump_smart(const func_t * f, FILE * fd, int depth,
                const dumpfmt_t * fmt, int inval);

void
func_free(func_t * f);

func_t *
func_unif_poly(const func_t * f1, int p1, const func_t * f2, int p2);
func_t *
func_unif3_poly(const func_t * f1, int p1,
                const func_t * f2, int p2,
                const func_t * f3, int p3);

func_t *
func_unif_affine(const func_t * f1, int p1, const func_t * f2, int p2);
func_t *
func_unif3_affine(const func_t * f1, int p1,
                  const func_t * f2, int p2,
                  const func_t * f3, int p3);

func_t *
func_unif_constant(const func_t * f1, int p1, const func_t * f2, int p2);
func_t *
func_unif3_constant(const func_t * f1, int p1,
                    const func_t * f2, int p2,
                    const func_t * f3, int p3);

int
func_isomorphic(const func_t * f1, const func_t * f2);

int
func_match(const func_t * f, int pos, const func_t * g);

int
func_mayfollow(const func_t * big, const func_t * small, value_t n);

value_t
func_eval(const func_t * f, value_t * v, unsigned int depth);

tuple_t *
tuple_new(unsigned int l);

tuple_t *
tuple_dup(const tuple_t * t);

void
tuple_dump(const tuple_t * t, FILE * fd, int depth,
           void (*fdump)(const func_t * f, FILE * fd, int depth,
                         const dumpfmt_t * fmt, int inval),
           const dumpfmt_t * fmt);

void
tuple_dump_raw(const tuple_t * t, FILE * fd);

tuple_t *
tuple_parse(const char * s);

void
tuple_free(tuple_t * t);

tuple_t *
tuple_unif(const tuple_t * t1, int p1, const tuple_t * t2, int p2,
           func_unif_t unif);
tuple_t *
tuple_unif3(const tuple_t * t1, int p1,
            const tuple_t * t2, int p2,
            const tuple_t * t3, int p3,
            func_unif3_t unif);

int
tuple_isomorphic(const tuple_t * t1, const tuple_t * t2);

int
tuple_match(const tuple_t * t, int pos, const tuple_t * g);

int
tuple_mayfollow(const tuple_t * big, const tuple_t * small, value_t n);


term_t *
term_tuple(tuple_t * tuple, term_t * sib);

term_t *
term_loop(func_t * upper, term_t * sub, term_t * sib);

term_t *
term_dup(const term_t * t);

term_t *
term_text(model_t values, model_t bounds, FILE * fd);

int
term_dump(const term_t * t, model_t values, model_t bounds,
          FILE * fd, const dumpfmt_t * fmt);

int
term_gobble(const term_t * t, model_t values, model_t bounds,
            FILE * fd, const dumpfmt_t * fmt);


void
generic_dump(term_t * g, unsigned int gpos, FILE * fd);

void
term_free(term_t * t);

term_t *
term_unif(const term_t * t1, int p1, const term_t * t2, int p2,
          func_unif_t unif_value, func_unif_t unif_bound);
#define term_unif_01(t1,t2,uv,ub) term_unif(t1,0,t2,1,uv,ub)
term_t *
term_unif3(const term_t * t1, int p1,
           const term_t * t2, int p2,
           const term_t * t3, int p3,
           func_unif3_t unif_value, func_unif3_t unif_bound);

int
term_isomorphic(const term_t * t1, const term_t * t2);

int
term_match(const term_t * t, int pos, const term_t * g);

int
term_mayfollow_match(const term_t * big, const term_t * small, value_t n);

int
term_mayfollow(const term_t * big, const term_t * small);

term_t *
term_triplet(const term_t * f, const term_t * s, const term_t * t,
             func_unif3_t u_value, func_unif3_t u_bound);

int
term_follows(const term_t * l, const term_t * t);


#endif
