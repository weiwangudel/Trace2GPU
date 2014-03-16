/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#ifndef _TXML_H_INCLUDED
#define _TXML_H_INCLUDED

#include <stdio.h>

int
term_txml(const term_t * t, model_t values, model_t bounds,
          FILE * fd, const dumpfmt_t * fmt);

#endif
