#include <math.h>
#include <stdio.h>

static
void print_array(int nx,
		 int ny,
		 double * ex,
		 double * ey,
		 double * hz)
{
  int i, j;

  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {
      fprintf(stderr, "%0.2lf ", ex[i*nx+j]);
      fprintf(stderr, "%0.2lf ", ey[i*nx+j]);
      fprintf(stderr, "%0.2lf ", hz[i*nx+j]);
      if ((i * nx + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}



int main () {
int i0,i1,i2;
double    A142647808[5];
double    A142623104[1024]; 
double    A142631328[1024]; 
double    A142639552[1024]; 

for ( i0 = 0; i0 <= 4; i0++) 
    A142647808 [( 8*i0 ) / 8] = (double) i0;
for ( i0 = 0; i0 < 32; i0++) 
  for ( i1 = 0; i1 < 32; i1++) 
    A142623104 [i0*32+i1] = ((double) i0*(i1+1)) / 32;
for ( i0 = 0; i0 < 32; i0++) 
  for ( i1 = 0; i1 < 32; i1++) 
    A142631328 [i0*32+i1] = ((double) i0*(i1+2)) / 32;
for ( i0 = 0; i0 < 32; i0++) 
  for ( i1 = 0; i1 < 32; i1++) 
    A142639552 [i0*32+i1] = ((double) i0*(i1+3)) / 32;

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

#pragma acc data copyin(A142631328, A142647808, A142623104) copy(A142639552)
{
#pragma acc kernels
{
for (c1 = 0; c1 <= 4; c1++) {
  for (c3 = 0; c3 <= 31; c3++) {
    A142631328[(1*c3)]=A142647808[(1*c1)];
  }
  for (c3 = 0; c3 <= 991; c3++) {
    A142631328[(32+1*c3)]=A142631328[(32+1*c3)]-0.5*(A142639552[(32+1*c3)]-A142639552[(1*c3)]);
  }
  for (c3 = 0; c3 <= 31; c3++) {
    for (c5 = 0; c5 <= 30; c5++) {
      A142623104[(1+32*c3+1*c5)]=A142623104[(1+32*c3+1*c5)]-0.5*(A142639552[(1+32*c3+1*c5)]-A142639552[(32*c3+1*c5)]);
    }
  }
  for (c3 = 0; c3 <= 30; c3++) {
    for (c5 = 0; c5 <= 30; c5++) {
      A142639552[(32*c3+1*c5)]=A142639552[(32*c3+1*c5)]-0.7*(A142623104[(1+32*c3+1*c5)]-A142623104[(32*c3+1*c5)]+A142631328[(32+32*c3+1*c5)]-A142631328[(32*c3+1*c5)]);
    }
  }
}
}
}
  print_array(32,32,A142623104, A142631328,A142639552);
}
