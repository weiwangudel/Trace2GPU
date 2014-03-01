#include <math.h>
#include <stdio.h>

static
void print_array(int n,
		 double *X)

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, "%0.2lf ", X[i*n+j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}

int main() {

int i0,i1,i2;
double    A141898112[1024]; 
double    A141906336[1024]; 
double    A141914560[1024];

for ( i0 = 0; i0 <= 31; i0++) 
  for ( i1 = 0; i1 <= 31; i1++) 
    A141898112 [i0*32+i1] =  ((double) i0 * (i1+1) + 1)/32;
for ( i0 = 0; i0 <= 31; i0++) 
  for ( i1 = 0; i1 <= 31; i1++) 
    A141906336 [i0*32+i1] =  ((double) i0 * (i1+2) + 2)/32;
for ( i0 = 0; i0 <= 31; i0++) 
  for ( i1 = 0; i1 <= 31; i1++) 
    A141914560 [i0*32+i1] =  ((double) i0 * (i1+3) + 3)/32;

#ifdef ceild
# undef ceild
#endif
#ifdef floord
# undef floord
#endif
#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))
  int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;
  int c1, c3, c5;
#pragma acc data copyin(A141906336, A141914560) copy(A141898112)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 4; c1++) {
  for (c3 = 0; c3 <= 31; c3++) {
    for (c5 = 0; c5 <= 30; c5++) {
      A141898112[(1+32*c3+1*c5)]=A141898112[(1+32*c3+1*c5)]-A141898112[(32*c3+1*c5)]*A141906336[(1+32*c3+1*c5)]/A141914560[(32*c3+1*c5)];
      A141914560[(1+32*c3+1*c5)]=A141914560[(1+32*c3+1*c5)]-A141906336[(1+32*c3+1*c5)]*A141906336[(1+32*c3+1*c5)]/A141914560[(32*c3+1*c5)];
    }
  }
  for (c3 = 0; c3 <= 31; c3++) {
    A141898112[(31+32*c3)]=A141898112[(31+32*c3)]/A141914560[(31+32*c3)];
  }
  for (c3 = 0; c3 <= 31; c3++) {
    for (c5 = 0; c5 <= 29; c5++) {
      A141898112[(30+32*c3+-1*c5)]=(A141898112[(30+32*c3+-1*c5)]-A141898112[(29+32*c3+-1*c5)]*A141906336[(29+32*c3+-1*c5)])/A141914560[(29+32*c3+-1*c5)];
    }
  }
  for (c3 = 0; c3 <= 991; c3++) {
    A141898112[(32+1*c3)]=A141898112[(32+1*c3)]-A141898112[(1*c3)]*A141906336[(32+1*c3)]/A141914560[(1*c3)];
    A141914560[(32+1*c3)]=A141914560[(32+1*c3)]-A141906336[(32+1*c3)]*A141906336[(32+1*c3)]/A141914560[(1*c3)];
  }
  for (c3 = 0; c3 <= 31; c3++) {
    A141898112[(992+1*c3)]=A141898112[(992+1*c3)]/A141914560[(992+1*c3)];
  }
  for (c3 = 0; c3 <= 29; c3++) {
    for (c5 = 0; c5 <= 31; c5++) {
      A141898112[(960+-32*c3+1*c5)]=(A141898112[(960+-32*c3+1*c5)]-A141898112[(928+-32*c3+1*c5)]*A141906336[(928+-32*c3+1*c5)])/A141914560[(960+-32*c3+1*c5)];
    }
  }
}
}
}

    print_array(32, A141898112);


}
