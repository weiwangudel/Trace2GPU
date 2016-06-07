/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#ifndef _ARGS_H_INCLUDED
#define _ARGS_H_INCLUDED

#include <stdio.h>
#include "termes.h"

typedef term_t * (*read_term_t)(model_t,model_t,FILE *);
typedef int (*write_term_t)(const term_t *, model_t, model_t,
                            FILE *, const dumpfmt_t *);

typedef struct s_param {
    struct {
        model_t values;
        model_t bounds;
        read_term_t r_term;
        write_term_t w_term;
        unsigned int maxstack;
        unsigned int flushstack;
    } general;
    struct {
        unsigned int k;
        func_unif_t unif_bound;
        func_unif3_t unif3_bound;
        func_unif_t unif_value;
        func_unif3_t unif3_value;
    } terms;
    struct {
        int slowstart;
        int checkfollow;
    } bir;
    struct {
        unsigned int p;
        unsigned int d;
        unsigned int w;
    } iki;
    struct {
        unsigned int verbosity;
        unsigned int flags;
        unsigned int every;
        unsigned int size;
    } debug;
    dumpfmt_t dump;
} param_t;

#define PARAM_DUMP(p,n) ( (p)->debug.flags && (p)->debug.every \
                          && ( (n) % (p)->debug.every == 0 ) )


/* FIXME: INITFIELD doesn't work for nested initialization */
#ifdef __cplusplus
#define PARAM_DEFAULTS { \
        {                                       \
            INITFIELD(values,M_POLY),           \
                INITFIELD(bounds,M_POLY),       \
                INITFIELD(r_term,term_text),    \
                INITFIELD(w_term,term_dump),    \
                INITFIELD(maxstack,100000),     \
                INITFIELD(flushstack,10000)     \
                },                              \
        {                                       \
            INITFIELD(k,1),                     \
                INITFIELD(unif_bound,func_unif_poly),   \
                INITFIELD(unif3_bound,func_unif3_poly), \
                INITFIELD(unif_value,func_unif_poly),   \
                INITFIELD(unif3_value,func_unif3_poly)  \
                },                                      \
        {                                               \
            INITFIELD(slowstart,0),                     \
                INITFIELD(checkfollow,0)                \
                },                                      \
        {                                               \
            INITFIELD(p,0),                             \
                INITFIELD(d,0),                         \
                INITFIELD(w,0)                          \
                },                                      \
        {                                               \
            INITFIELD(verbosity,0x0),                   \
                INITFIELD(flags,0x1),                   \
                INITFIELD(every,-1),                    \
                INITFIELD(size,5)                       \
                },                                      \
            INITFIELD(dump,DUMPFMT_DEFAULTS)            \
}
#else
#define PARAM_DEFAULTS { \
        .general = {                            \
            INITFIELD(values,M_POLY),           \
                INITFIELD(bounds,M_POLY),       \
                INITFIELD(r_term,term_text),    \
                INITFIELD(w_term,term_dump),    \
                INITFIELD(maxstack,100000),     \
                INITFIELD(flushstack,10000)     \
                },                            \
    .terms = { \
        INITFIELD(k,1),                       \
        INITFIELD(unif_bound,func_unif_poly),   \
        INITFIELD(unif3_bound,func_unif3_poly), \
        INITFIELD(unif_value,func_unif_poly),   \
        INITFIELD(unif3_value,func_unif3_poly)  \
     }, \
    .bir = { \
        INITFIELD(slowstart,0),  \
        INITFIELD(checkfollow,0) \
     }, \
    .iki = { \
        INITFIELD(p,0), \
        INITFIELD(d,0), \
        INITFIELD(w,0)  \
     }, \
    .debug = { \
        INITFIELD(verbosity,0x0),               \
        INITFIELD(flags,0x1),                   \
        INITFIELD(every,-1),                    \
        INITFIELD(size,5)                       \
     }, \
         INITFIELD(dump,DUMPFMT_DEFAULTS)       \
}
#endif

void
scan_args(int argc, char ** argv, param_t * p);



#endif
