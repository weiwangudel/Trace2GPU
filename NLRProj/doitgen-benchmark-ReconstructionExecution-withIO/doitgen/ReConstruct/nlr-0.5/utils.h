/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#ifndef _UTILS_H_INCLUDED
#define _UTILS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

/* This macro ensures that initializations can be compiled in C and C++ */
#ifdef __cplusplus
#define INITFIELD(name,val) val
#else
#define INITFIELD(name,val) . name = val
#endif


/* This was done to try Boehm's GC (consistently slower) */
/* #define GENALLOCPTR(size) malloc(size) */
/* #define GENALLOCATM(size) malloc(size) */
/* ...ATM is for zones containing no pointer */

static inline void *
GENALLOCPTR(size_t size)
{
    register void * value = malloc(size);
    if (value == 0)
    {
        perror("malloc");
        abort();
    }
    return value;
}
static inline void *
GENALLOCATM(size_t size)
{
    return GENALLOCPTR(size);
}
static inline void *
GENREALLOC(void * ptr, size_t size)
{
    register void * value = realloc(ptr,size);
    if (value == 0)
    {
        perror("realloc");
        abort();
    }
    return value;
}
static inline void
GENFREE(void * ptr)
{
    free(ptr);
}

const char * mystrdup(const char * s);

void fatal(const char * fmt, ...);

void sigs_setup();
int sigs_usr1();
int sigs_usr2();


#endif
