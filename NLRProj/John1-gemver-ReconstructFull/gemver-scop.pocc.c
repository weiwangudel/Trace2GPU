#include <math.h>
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

#pragma acc data copyin(A, u1, v1, u2, v2, x, y, z) copy(w)
{
#pragma acc kernels
{
if ((N >= 1)) {
  for (c1 = 0; c1 <= (N + -1); c1++) {
    for (c3 = 0; c3 <= (N + -1); c3++) {
      A[c1][c3]=A[c1][c3]+u1[c1]*v1[c3]+u2[c1]*v2[c3];
    }
  }
  for (c1 = 0; c1 <= (N + -1); c1++) {
    for (c3 = 0; c3 <= (N + -1); c3++) {
      x[c1]=x[c1]+beta*A[c3][c1]*y[c3];
    }
  }
  for (c1 = 0; c1 <= (N + -1); c1++) {
    x[c1]=x[c1]+z[c1];
  }
  for (c1 = 0; c1 <= (N + -1); c1++) {
    for (c3 = 0; c3 <= (N + -1); c3++) {
      w[c1]=w[c1]+alpha*A[c1][c3]*x[c3];
    }
  }
}
}
}

