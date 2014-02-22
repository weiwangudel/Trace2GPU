/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include "utils.h"

const char * mystrdup(const char * s)
{
    char * n = (char *)GENALLOCATM(strlen(s)+1);
    strcpy(n,s);
    return n;
}

void fatal(const char * fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    vfprintf(stderr,fmt,args);
    fputs("\n",stderr);
    exit(1);
}


static volatile int sigs_usr1_caught = 0;
static volatile int sigs_usr2_caught = 0;

void sigs_handler(int s)
{
    if ( s == SIGUSR1 )
        sigs_usr1_caught = 1;
    else if ( s == SIGUSR2 )
        sigs_usr2_caught = 1;
    signal(s,sigs_handler);
}

void sigs_setup()
{
    signal(SIGUSR1,sigs_handler);
    signal(SIGUSR2,sigs_handler);
}

int sigs_usr1()
{
    int r = sigs_usr1_caught;
    sigs_usr1_caught = 0;
    return r;
}
int sigs_usr2()
{
    int r = sigs_usr2_caught;
    sigs_usr2_caught = 0;
    return r;
}
