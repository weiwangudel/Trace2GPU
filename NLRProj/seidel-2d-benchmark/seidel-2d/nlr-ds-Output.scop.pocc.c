#include <math.h>
#include<stdio.h>


static
void print_array(int n,
                 double *A)

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, "%0.2lf ", A[i*32+j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}



int main()
{
int i0, i1;
int t;
double A148943232[1024];

for ( i0 = 0; i0 <= 1023; i0++) 
    A148943232 [( 8*i0 ) / 8] =  ( (double) ((i0/32) * (i0%32+2)) + 2 ) / 32 ;
 
  for (t = 0; t<2; t++)
  {
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
  int c1, c3;

#pragma acc data copy(A148943232)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 29; c1++) {
  for (c3 = 0; c3 <= 29; c3++) {
    A148943232[(33+32*c1+1*c3)]=(A148943232[(32*c1+1*c3)]+A148943232[(1+32*c1+1*c3)]+A148943232[(2+32*c1+1*c3)]+A148943232[(32+32*c1+1*c3)]+A148943232[(33+32*c1+1*c3)]+A148943232[(34+32*c1+1*c3)]+A148943232[(64+32*c1+1*c3)]+A148943232[(65+32*c1+1*c3)]+A148943232[(66+32*c1+1*c3)])/9.0;
  }
}
}
}
  }
 
  print_array(32, A148943232); 

}
