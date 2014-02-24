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
  int c1, c3, c5;

#pragma acc data copy(M)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 9; c1++) {
  for (c3 = 0; c3 <= 9; c3++) {
    M[137859456+80*c1+8*c3]=M[137859456+80*c1+8*c3]+M[3219761584];
    for (c5 = 0; c5 <= 9; c5++) {
      M[137859456+80*c1+8*c3]=M[137859456+80*c1+8*c3]+M[3219761592]+M[137860288+80*c1+8*c5]+M[137861120+8*c3+80*c5];
    }
  }
}
}
}
