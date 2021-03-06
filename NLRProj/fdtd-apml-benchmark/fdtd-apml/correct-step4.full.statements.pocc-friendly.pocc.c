#include <math.h>
#include <stdio.h>

static
void print_array(int cz,
		 int cxm,
		 int cym,
		 double *Bza,
		 double *Ex,
		 double *Ey,
		 double *Hz)
{
  int i, j, k;

  for (i = 0; i <= cz; i++)
    for (j = 0; j <= cym; j++)
      for (k = 0; k <= cxm; k++) {
	fprintf(stderr, "%0.2lf ", Bza[i*33*33+j*33+k]);
	fprintf(stderr, "%0.2lf ", Ex[i*33*33+j*33+k]);
	fprintf(stderr, "%0.2lf ", Ey[i*33*33+j*33+k]);
	fprintf(stderr, "%0.2lf ", Hz[i*33*33+j*33+k]);
	if ((i * cxm + j) % 20 == 0) fprintf(stderr, "\n");
      }
  fprintf(stderr, "\n");
}




int main () 
{
    double A3219962056 = 2341;
    double A3219962064 = 42;
    double A3219961912 = A3219962056;
    double A3219961904 = A3219962064;

    double A167451136[264/8]; 
    double A167451488[264/8];
    double A167451776[264/8];
    double A167452128[264/8];
    double A167452448[264/8];
    double A167452800[264/8];
    double A167424928[8712/8];
    double A167416192[8712/8];
    double A3075694624[287496/8];
    double A3075403808[287496/8];
    double A3075112992[287496/8];
    double A167433664[8712/8];
    double A167442400[8712/8];
    double A3075985440[287496/8];

    int i0,i1,i2;

    for ( i0 = 0; i0 <= 32; i0++) 
    {
        A167451136 [( 8*i0 ) / 8] = ((double) i0+1) / 32;
        A167451488 [( 8*i0 ) / 8] = ((double) i0+2) / 32;
    }
    for ( i0 = 0; i0 <= 32; i0++) 
    {
        A167451776 [( 8*i0 ) / 8] = ((double) i0+3) / 32;
        A167452128 [( 8*i0 ) / 8] = ((double) i0+4) / 32;
    }
    for ( i0 = 0; i0 <= 32; i0++) 
    {
        A167452448 [( 8*i0 ) / 8] = ((double) i0+5) / 32;
        A167452800 [( 8*i0 ) / 8] = ((double) i0+6) / 32;
    }
    for ( i0 = 0; i0 <= 32; i0++) 
      for ( i1 = 0; i1 <= 32; i1++) 
      {
        A167424928 [i0*33+i1] = ((double) i0*(i1+1)+10)/32;  
        A167416192 [i0*33+i1] = ((double) i0*(i1+2)+11)/32;
        for ( i2 = 0; i2 <= 32; i2++) 
	{
            A3075694624 [i0*33*33+i1*33+i2] = ((double) i0*(i1+3)+i2+1)/32;
            A3075403808 [i0*33*33+i1*33+i2] = ((double) i0*(i1+4)+i2+2)/32;
            A3075112992 [i0*33*33+i1*33+i2] = ((double) i0*(i1+5)+i2+3)/32;
	}
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
#pragma acc data copyin(A167433664, A3075694624, A3075403808, A167442400, A167452448, A167452800,A167451776, A167452128, A167451488, A167451136, A167424928, A167416192) copy(A3075985440, A3075112992)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 31; c1++) {
  for (c3 = 0; c3 <= 31; c3++) {
    for (c5 = 0; c5 <= 31; c5++) {
      A167433664[(33*c1+1*c3)]=A3075694624[(1089*c1+33*c3+1*c5)]-A3075694624[(33+1089*c1+33*c3+1*c5)]+A3075403808[(1+1089*c1+33*c3+1*c5)]-A3075403808[(1089*c1+33*c3+1*c5)];
      A167442400[(33*c1+1*c3)]=(A167452448[(1*c3)]/A167452800[(1*c3)])*A3075985440[(1089*c1+33*c3+1*c5)]-(A3219961904/A167452800[(1*c3)])*A167433664[(33*c1+1*c3)];
      A3075112992[(1089*c1+33*c3+1*c5)]=(A167451776[(1*c5)]/A167452128[(1*c5)])*A3075112992[(1089*c1+33*c3+1*c5)]+(A3219961912*A167451488[(1*c1)]/A167452128[(1*c5)])*A167442400[(33*c1+1*c3)]-(A3219961912*A167451136[(1*c1)]/A167452128[(1*c5)])*A3075985440[(1089*c1+33*c3+1*c5)];
      A3075985440[(1089*c1+33*c3+1*c5)]=A167442400[(33*c1+1*c3)];
    }
    A167433664[(33*c1+1*c3)]=A3075694624[(32+1089*c1+33*c3)]-A3075694624[(65+1089*c1+33*c3)]+A167424928[(33*c1+1*c3)]-A3075403808[(32+1089*c1+33*c3)];
    A167442400[(33*c1+1*c3)]=(A167452448[(1*c3)]/A167452800[(1*c3)])*A3075985440[(32+1089*c1+33*c3)]-(A3219961904/A167452800[(1*c3)])*A167433664[(33*c1+1*c3)];
    A3075112992[(32+1089*c1+33*c3)]=(A167451776[(32)]/A167452128[(32)])*A3075112992[(32+1089*c1+33*c3)]+(A3219961912*A167451488[(1*c1)]/A167452128[(32)])*A167442400[(33*c1+1*c3)]-(A3219961912*A167451136[(1*c1)]/A167452128[(32)])*A3075985440[(32+1089*c1+33*c3)];
    A3075985440[(32+1089*c1+33*c3)]=A167442400[(33*c1+1*c3)];
    for (c5 = 0; c5 <= 31; c5++) {
      A167433664[(33*c1+1*c3)]=A3075694624[(1056+1089*c1+1*c5)]-A167416192[(33*c1+1*c5)]+A3075403808[(1057+1089*c1+1*c5)]-A3075403808[(1056+1089*c1+1*c5)];
      A167442400[(33*c1+1*c3)]=(A167452448[(32)]/A167452800[(1*c3)])*A3075985440[(1089*c1+33*c3+1*c5)]-(A3219961904/A167452800[(1*c3)])*A167433664[(33*c1+1*c3)];
      A3075112992[(1056+1089*c1+1*c5)]=(A167451776[(1*c5)]/A167452128[(1*c5)])*A3075112992[(1056+1089*c1+1*c5)]+(A3219961912*A167451488[(1*c1)]/A167452128[(1*c5)])*A167442400[(33*c1+1*c3)]-(A3219961912*A167451136[(1*c1)]/A167452128[(1*c5)])*A3075985440[(1056+1089*c1+1*c5)];
      A3075985440[(1056+1089*c1+1*c5)]=A167442400[(33*c1+1*c3)];
    }
    A167433664[(33*c1+1*c3)]=A3075694624[(1088+1089*c1)]-A167416192[(32+33*c1)]+A167424928[(32+33*c1)]-A3075403808[(1088+1089*c1)];
    A167442400[(33*c1+1*c3)]=(A167452448[(32)]/A167452800[(32)])*A3075985440[(1088+1089*c1)]-(A3219961904/A167452800[(32)])*A167433664[(33*c1+1*c3)];
    A3075112992[(1088+1089*c1)]=(A167451776[(32)]/A167452128[(32)])*A3075112992[(1088+1089*c1)]+(A3219961912*A167451488[(1*c1)]/A167452128[(32)])*A167442400[(33*c1+1*c3)]-(A3219961912*A167451136[(1*c1)]/A167452128[(32)])*A3075985440[(1088+1089*c1)];
    A3075985440[(1088+1089*c1)]=A167442400[(33*c1+1*c3)];
  }
}
}
}
    print_array(32,32,32, A3075985440, A3075694624, A3075403808, A3075112992);

}
