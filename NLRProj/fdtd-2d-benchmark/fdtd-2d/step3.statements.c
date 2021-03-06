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

#pragma scop
for ( i0 = 0; i0 <= 4; i0++) 
{
    for ( i1 = 0; i1 <= 31; i1++) 
        A142631328 [( 8*i1 ) / 8] = 
        A142647808 [( 8*i0 ) / 8] ; 
    for ( i1 = 0; i1 <= 991; i1++) 
        A142631328 [( 256 + 8*i1 ) / 8] = 
        A142631328 [( 256 + 8*i1 ) / 8] -
	0.5 *
        (A142639552 [( 256 + 8*i1 ) / 8] - 
        A142639552 [( 8*i1 ) / 8]) ; 
    for ( i1 = 0; i1 <= 31; i1++) 
        for ( i2 = 0; i2 <= 30; i2++) 
            A142623104 [( 8 + 256*i1 + 8*i2 ) / 8] = 
            A142623104 [( 8 + 256*i1 + 8*i2 ) / 8] -
	    0.5 * 
            (A142639552 [( 8 + 256*i1 + 8*i2 ) / 8] - 
            A142639552 [( 256*i1 + 8*i2 ) / 8]) ; 
    for ( i1 = 0; i1 <= 30; i1++) 
        for ( i2 = 0; i2 <= 30; i2++) 
            A142639552 [( 256*i1 + 8*i2 ) / 8] = 
            A142639552 [( 256*i1 + 8*i2 ) / 8] -
	    0.7 * 
            (A142623104 [( 8 + 256*i1 + 8*i2 ) / 8] - 
            A142623104 [( 256*i1 + 8*i2 ) / 8] + 
            A142631328 [( 256 + 256*i1 + 8*i2 ) / 8] - 
            A142631328 [( 256*i1 + 8*i2 ) / 8]) ; 
}
#pragma endscop
  print_array(32,32,A142623104, A142631328,A142639552);
}
