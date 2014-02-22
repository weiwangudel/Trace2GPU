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

#pragma acc data copy(M)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 99; c1++) {
  for (c3 = 0; c3 <= 99; c3++) {
    M[6296640+800*c1+8*c3]=M[6296640+800*c1+8*c3]+M[6457440+8*c1]+M[6459840+8*c3]+M[6458240+8*c1]+M[6459040+8*c3];
  }
}
for (c1 = 0; c1 <= 99; c1++) {
  for (c3 = 0; c3 <= 99; c3++) {
    M[6456640+8*c1]=M[6456640+8*c1]+M[6296624]+M[6296640+8*c1+800*c3]+M[6461440+8*c3];
  }
}
for (c1 = 0; c1 <= 99; c1++) {
  M[6456640+8*c1]=M[6456640+8*c1]+M[6462240+8*c1];
}
for (c1 = 0; c1 <= 99; c1++) {
  for (c3 = 0; c3 <= 99; c3++) {
    M[6460640+8*c1]=M[6460640+8*c1]+M[6296616]+M[6296640+800*c1+8*c3]+M[6456640+8*c3];
  }
}
}
}
