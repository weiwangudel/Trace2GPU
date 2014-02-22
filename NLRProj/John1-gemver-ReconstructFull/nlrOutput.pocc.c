#include <math.h>
//v1: This version mainly was nlr output
// With manual editing such as array declaration and operator copy
// as well as some disambiguation of variable such as i0 = A[0-9]*
// But this version contains / 8 and * 8 operation, which is not 
// allowed by PoCC.
// v2: this version tries to remove /8 in scop


#include <stdio.h>
#include <math.h>

inline
void print_array(double* w)
{
  int i, j;
    {
      for (i = 0; i < 100; i++) {
        fprintf(stderr, "%0.2lf ", w[i]);
        if (i%80 == 20) fprintf(stderr, "\n");
      }
      fprintf(stderr, "\n");
    }
}


int main() {

  double A134685824[100]; 
  double A134686624[100]; 
  double A134688224[100]; 
  double A134687424[100]; 
  double A134689824[100]; 
  double A134690624[100]; 
  double A134685024[100]; 
  double A134689024[100]; 
  double A134525024[10000];
  int i0,i1;
  int A3213166120, A3213166124;

double A134525000 = 43532;
double A134525008 = 12313; 
for ( i0 = 0; i0 <= 99; i0++) 
{
    A3213166120 = i0;
    A134685824 [( 8*i0 ) / 8] = 
    A3213166120 ; 
    A134686624 [( 8*i0 ) / 8] = 
    (A3213166120 +1)/100/2.0 ; 
    A134688224 [( 8*i0 ) / 8] = 
    (A3213166120 +1)/100/4.0 ; 
    A134687424 [( 8*i0 ) / 8] = 
    (A3213166120 +1)/100/6.0 ; 
    A134689824 [( 8*i0 ) / 8] = 
    (A3213166120 +1)/100/8.0 ; 
    A134690624 [( 8*i0 ) / 8] = 
    (A3213166120 +1)/100/9.0 ; 
    A134685024 [( 8*i0 ) / 8] =  0.0;
    A134689024 [( 8*i0 ) / 8] =  0.0;
    for ( i1 = 0; i1 <= 99; i1++) 
        A134525024 [( 800*i0 + 8*i1 ) / 8] = (double) (A3213166120 * i1) / 100;
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
  int c1, c3;

#pragma acc data copyin(A134525024, A134685824, A134688224, A134686624, A134687424, A134685024, A134689824, A134690624) copy(A134689024)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 99; c1++) {
  for (c3 = 0; c3 <= 99; c3++) {
    A134525024[(100*c1+1*c3)]=A134525024[(100*c1+1*c3)]+A134685824[(1*c1)]*A134688224[(1*c3)]+A134686624[(1*c1)]*A134687424[(1*c3)];
  }
}
for (c1 = 0; c1 <= 99; c1++) {
  for (c3 = 0; c3 <= 99; c3++) {
    A134685024[(1*c1)]=A134685024[(1*c1)]+A134525008*A134525024[(1*c1+100*c3)]*A134689824[(1*c3)];
  }
}
for (c1 = 0; c1 <= 99; c1++) {
  A134685024[(1*c1)]=A134685024[(1*c1)]+A134690624[(1*c1)];
}
for (c1 = 0; c1 <= 99; c1++) {
  for (c3 = 0; c3 <= 99; c3++) {
    A134689024[(1*c1)]=A134689024[(1*c1)]+A134525000*A134525024[(100*c1+1*c3)]*A134685024[(1*c3)];
  }
}
}
}

// verify computing results
  print_array(A134689024);
}


