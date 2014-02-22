/*
 * Copyright (C) 2008 Alain Ketterlin, Philippe Clauss
 * Contact: alain@dtp-info.u-strasbg.fr
 *
 */

#include <getopt.h>
#include <string.h>
#include <stdarg.h>

#include "args.h"
#include "utils.h"
#include "xio.h"
#include "txml.h"

static model_t
scan_model(const char * s)
{
    if ( strcmp(s,"poly") == 0 )
        return M_POLY;
    else if ( strcmp(s,"affine") == 0 )
        return M_LINEAR;
    else if ( strcmp(s,"constant") == 0 )
        return M_CONSTANT;
    else
        fatal("Unknown model '%s'\n",s);
    return M_CONSTANT; /* never reached */
}

static func_unif_t
scan_model2(const char * s)
{
    if ( strcmp(s,"poly") == 0 )
        return func_unif_poly;
    else if ( strcmp(s,"affine") == 0 )
        return func_unif_affine;
    else if ( strcmp(s,"constant") == 0 )
        return func_unif_constant;
    else
        fatal("Unknown model '%s'\n",s);
    return NULL; /* never reached */
}

static func_unif3_t
scan_model3(const char * s)
{
    if ( strcmp(s,"poly") == 0 )
        return func_unif3_poly;
    else if ( strcmp(s,"affine") == 0 )
        return func_unif3_affine;
    else if ( strcmp(s,"constant") == 0 )
        return func_unif3_constant;
    else
        fatal("Unknown model '%s'\n",s);
    return NULL; /* never reached */
}

static int
scan_int(const char * s)
{
    int i;
    if ( sscanf(s,"%d",&i) != 1 )
        fatal("Expecting an integer, not '%s'\n",s);
    return i;
}

static unsigned int
scan_posint(const char * s, int nonzero)
{
    int i = scan_int(s);
    if ( nonzero && i <= 0 )
        fatal("Integer parameter (%s) must be positive\n",s);
    return i;
}

static int
scan_yesno(char c)
{
    return strchr("1yYoOtT+",c) != NULL;
}

static unsigned int
scan_onestring(const char * opt, ...)
{
    va_list args;
    const char * s;
    unsigned int p = 0;
    int r = -1;
    va_start(args,opt);
    while ( ( r == -1 ) && (s=va_arg(args,const char *)) != NULL )
    {
        if ( strcmp(opt,s) == 0 )
            r = p;
        else
            ++p;
    }
    va_end(args);
    if ( r == -1 )
        fatal("Unknown value '%s'\n",opt);
    return r;
}

static void
scan_dumpfmt(const char * opt, param_t * p)
{
    unsigned int n;
    if ( strcmp(opt,"none") == 0 ) p->dump.hexa = 0;
    else if ( strcmp(opt,"const") == 0 ) p->dump.hexa = 1;
    else if ( strcmp(opt,"all") == 0 ) p->dump.hexa = 2;
    else if ( strcmp(opt,"short") == 0 ) p->dump.delim = 0;
    else if ( strcmp(opt,"delim") == 0 ) p->dump.delim = 1;
    else if ( strcmp(opt,"line") == 0 ) p->dump.hmax = ~0;
    else if ( sscanf(opt,"%u",&n) == 1 ) p->dump.indent = n;
    else p->dump.indice = mystrdup(opt);
}

void
scan_args(int argc, char ** argv, param_t * p)
{
    static const char * options = "hi:o:a:k:b:v:s:f:p:d:w:x:y:g:u:e:m:";
    static struct { char c; const char * m; } optdoc[] = {
        { 'h',
          "\tPrints this help\n" },
        { 'i',
          "\tInput format. One of 'text', 'xio'.\n" },
        { 'o',
          "\tOuput format. One of 'text', 'xio', 'xml', 'null'.\n" },
        { 'a',
          "\tText output format: followed by one of:\n"
          "\t\tnone, const, all: what to print in hexadecimal\n"
          "\t\tshort, delim: do you want \"done\" at end of loops\n"
          "\t\t(a positive number): indent level, in spaces\n"
          "\t\tanything else: name of loop indices\n"
          "\tYou may have to give this option several times\n" },
        { 'k',
          "\tLoop max branching factor (a positive number).\n" },
        { 'b',
          "\tModel of loop bounds. One of 'poly', 'affine', 'constant'.\n" },
        { 'v',
          "\tModel of values. One of 'poly', 'affine', 'constant'.\n" },
        { 's',
          "\t(bir) Deprecated. Requires \"slow start\". Forget it.\n" },
        { 'f',
          "\t(bir) \"Check followup\". Handle with care.\n" },
        { 'p',
          "\t(iki) How far shall we go to find a predictor.\n" },
        { 'd',
          "\t(iki) A what distance shall we predict.\n" },
        { 'w',
          "\t(iki) How many values shall we predict.\n" },
        { 'x',
          "\t(iki) Max stack size, after which it is flushed (see -y).\n" },
        { 'y',
          "\t(iki) Size of stack bottom flushed when max size reached.\n" },
        { 'g',
          "\tEnable debugging on stderr, param is a bitset.\n"
          "\t0x2: output the stack after each value\n"
          "\t0x1: print a progress counter\n"
          "\t(see -e to find out how often this happens)\n"},
        { 'u',
          "\tWhat to dump: a bit field (program specific).\n"},
        { 'e',
          "\tNumber of items after which the stack is dumped.\n" },
        { 'm',
          "\tMax number of stack items to dump.\n" },
        { '\0',0 }
    };
    char c;

    while ( (c=getopt(argc,argv,options)) != -1 )
    {
        switch ( c )
        {
            case 'i':
            {
                static read_term_t readers [] =
                    { term_text, term_import_piecewise };
                p->general.r_term =
                    readers[scan_onestring(optarg,"text","xio",NULL)];
                break;
            }
            case 'o':
            {
                static write_term_t writers [] =
                    { term_gobble, term_dump, term_export, term_txml };
                p->general.w_term =
                    writers[scan_onestring(optarg,
                                           "null","text","xio","xml",NULL)];
                break;
            }
            case 'k':
                p->terms.k = scan_posint(optarg,1);
                break;
            case 'b':
                p->general.bounds = scan_model(optarg);
                p->terms.unif_bound = scan_model2(optarg);
                p->terms.unif3_bound = scan_model3(optarg);
                break;
            case 'v':
                p->general.values = scan_model(optarg);
                p->terms.unif_value = scan_model2(optarg);
                p->terms.unif3_value = scan_model3(optarg);
                break;
            case 's':
                p->bir.slowstart = scan_yesno(optarg[0]);
                break;
            case 'f':
                p->bir.checkfollow = scan_yesno(optarg[0]);
                break;
            case 'p':
                p->iki.p = scan_posint(optarg,1);
                if ( p->iki.w == 0 )
                    p->iki.w = 1;
                break;
            case 'd':
                p->iki.d = scan_posint(optarg,0);
                break;
            case 'w':
                p->iki.w = scan_posint(optarg,1);
                break;
            case 'a':
                scan_dumpfmt(optarg,p);
                break;
            case 'x':
                p->general.maxstack = scan_posint(optarg,1);
                break;
            case 'y':
                p->general.flushstack = scan_posint(optarg,1);
                break;
            case 'g':
                p->debug.verbosity = scan_posint(optarg,0);
                break;
            case 'u':
                p->debug.flags = scan_posint(optarg,0);
                break;
            case 'e':
                p->debug.every = scan_posint(optarg,1);
                break;
            case 'm':
                p->debug.size = scan_posint(optarg,1);
                break;
            case 'h':
            {
                const char * op = options;
                fprintf(stderr
                        ,"Usage: %s [options]\n"
                        "\tReads numeric data from standard input."
                        " Options are:\n\n"
                        ,argv[0]);
                while ( *op != '\0' )
                {
                    int d = 0;
                    while ( optdoc[d].c != '\0' && optdoc[d].c != *op )
                        d++;
                    fprintf(stderr,"-%c",*op++);
                    if ( *op == ':' )
                    {
                        fprintf(stderr," <param>");
                        ++op;
                    }
                    fprintf(stderr,"\n");
                    if ( optdoc[d].c == '\0' )
                        fprintf(stderr,"(missing documentation: complain.)\n");
                    else
                        fprintf(stderr,"%s",optdoc[d].m);
                }
                fprintf(stderr,"\n");
                exit(1);
            }
            default:
                fprintf(stderr,"Don't play with options\n");
                break;
        }
    }
    if ( optind < argc )
    {
        fprintf(stderr,"Warning: params");
        for ( int i=optind ; i<argc ; i++ )
            fprintf(stderr," '%s'",argv[i]);
        fprintf(stderr," ignored (use stdin to provide data)\n");
    }
}

