/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#ifndef _XIO_H_INCLUDED
#define _XIO_H_INCLUDED

#include "termes.h"


typedef enum e_lextype {
    LX_CHR = 0, LX_NBR = 1, LX_EOF = 2
} lextype_t;

typedef struct s_lexer {
    FILE * fd;
    lextype_t type;
    union {
        value_t nbr;
        char chr[256];
    } u;
} lexer_t;


int
term_export(const term_t * t,
            model_t values, model_t bounds,
            FILE * fd,
            const dumpfmt_t * fmt);

term_t *
term_import(model_t values, model_t bounds,
            FILE * fd);

term_t *
term_import_piecewise(model_t values, model_t bounds,
                      FILE * fd);



#endif
