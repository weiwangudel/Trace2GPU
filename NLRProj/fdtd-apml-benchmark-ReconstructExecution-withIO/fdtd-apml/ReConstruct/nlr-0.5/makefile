
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -O3
OBJS=termes xio txml iters args utils

all: nlr

nlr: ${OBJS:%=%.o}


clean:
	rm -f nlr ${OBJS:%=%.o}
