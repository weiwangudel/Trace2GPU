/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#ifndef _ITERS_H_INCLUDED
#define _ITERS_H_INCLUDED

#include <stdio.h>
#include "termes.h"

typedef struct s_iter {
    unsigned int size;
    unsigned int len;
    unsigned int * indices;
    const term_t ** termes;
} iter_t;


iter_t *
iter_start(const term_t * t);

iter_t *
iter_after(const term_t * t);

int
iter_void(const iter_t * it);

void
iter_free(iter_t * it);

void
iter_dump(const iter_t * it, FILE * fd);

tuple_t *
iter_get(const iter_t * it);

int
iter_advance(iter_t * it, int wraproot);


iter_t *
term_next(const term_t * big, const term_t * small);

iter_t *
term_next_complete(const term_t * big, const term_t * small);


#endif
