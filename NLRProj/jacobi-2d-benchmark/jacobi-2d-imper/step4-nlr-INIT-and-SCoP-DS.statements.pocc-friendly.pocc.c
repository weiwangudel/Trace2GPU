#include <math.h>
#include<stdio.h>

static
void print_array(int n,
                 double *A)

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, "%0.2lf ", A[i*n+j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}



int main() {

int i0,i1,t;

double A166830464[10000];
double A166910496[10000];

for ( i0 = 0; i0 <= 9999; i0++) 
{
    A166830464 [( 8*i0 ) / 8] = ( (double) ((i0/100) * (i0%100+2)) + 2 ) / 100 ;
    A166910496 [( 8*i0 ) / 8] = ( (double) ((i0/100) * (i0%100+3)) + 3 ) / 100 ;
}

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

#pragma acc data copyin(A166910496) copy(A166830464)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 1; c1++) {
  for (c3 = 0; c3 <= 97; c3++) {
    for (c5 = 0; c5 <= 97; c5++) {
      A166910496[(101+100*c3+1*c5)]=0.2*(A166830464[(101+100*c3+1*c5)]+A166830464[(100+100*c3+1*c5)]+A166830464[(102+100*c3+1*c5)]+A166830464[(201+100*c3+1*c5)]+A166830464[(1+100*c3+1*c5)]);
    }
  }
  for (c3 = 0; c3 <= 97; c3++) {
    for (c5 = 0; c5 <= 97; c5++) {
      A166830464[(101+100*c3+1*c5)]=A166910496[(101+100*c3+1*c5)];
    }
  }
}
}
}
  print_array(100,  A166830464); 
  
}
