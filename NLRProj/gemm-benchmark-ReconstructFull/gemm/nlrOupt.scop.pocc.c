#include <math.h>


#include <stdio.h>
#include <math.h>

static
void print_array(double*  C)
{
  int i, j;

  for (i = 0; i < 10; i++)
    for (j = 0; j < 10; j++) {
        fprintf (stderr, "%0.2lf ", C[i*10+j]);
        if ((i * 10 + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}



int main() 
{
    double A160661888[100];
    double A160662720[100];
    double A160663552[100];
    int i0, i1, i2;

    for ( i0 = 0; i0 <= 99; i0++) 
        A160661888 [( 8*i0 ) / 8] = 
        ( (double) ((i0 / 10) * (i0 % 10)) ) / 10;  
    for ( i0 = 0; i0 <= 99; i0++) 
        A160662720 [( 8*i0 ) / 8] = 
        ( (double) ((i0 / 10) * (i0 % 10)) ) / 10;  
    for ( i0 = 0; i0 <= 99; i0++) 
        A160663552 [( 8*i0 ) / 8] = 
        ( (double) ((i0 / 10) * (i0 % 10)) ) / 10;  

    double A3216936832 = 2123;
    double A3216936840 = 32412;

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

#pragma acc data copyin(A160662720, A160663552) copy(A160661888)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 9; c1++) {
  for (c3 = 0; c3 <= 9; c3++) {
    A160661888[(10*c1+c3)]=A160661888[(10*c1+c3)]*A3216936832;
    for (c5 = 0; c5 <= 9; c5++) {
      A160661888[(10*c1+c3)]=A160661888[(10*c1+c3)]+A3216936840*A160662720[(10*c1+c5)]*A160663552[(c3+10*c5)];
    }
  }
}
}
}

    //verify results
    print_array(A160661888);
}
