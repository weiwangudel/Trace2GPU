#include <stdio.h>

#define DATA_TYPE double

static
void print_array(int nx,
		 int ny,
		 double *ex,
		 double *ey,
		 double *hz)
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

int main()
{
    double  A158667264 [ 40 / 8];
    double  A158642560 [ 8192 / 8];
    double  A158650784 [ 8192 / 8];
    double  A158659008 [ 8192 / 8];

    int i0;
    int i1;
    int i2;

    #pragma scop
    for ( i0 = 0 ; i0 <= 4 ; i0++) 
    {
        A158667264 [( 8*i0 ) / 8] 
        =(DATA_TYPE)    1*i0 
        ;
    }
    for ( i0 = 0 ; i0 <= 31 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 31 ; i1++) 
        {
            A158642560 [( 256*i0 + 8*i1 ) / 8] 
            =((DATA_TYPE)        1*i0 
            *(        1*i1 
            +        1 
            ))/        32 
            ;
            A158650784 [( 256*i0 + 8*i1 ) / 8] 
            =((DATA_TYPE)        1*i0 
            *(        1*i1 
            +        2 
            ))/        32 
            ;
            A158659008 [( 256*i0 + 8*i1 ) / 8] 
            =((DATA_TYPE)        1*i0 
            *(        1*i1 
            +        3 
            ))/        32 
            ;
        }
    }
    for ( i0 = 0 ; i0 <= 4 ; i0++) 
    {
        for ( i1 = 0 ; i1 <= 31 ; i1++) 
        {
            A158650784 [( 8*i1 ) / 8] 
            =        A158667264 [( 8*i0 ) / 8] 
            ;
        }
        for ( i1 = 0 ; i1 <= 991 ; i1++) 
        {
            A158650784 [( 256 + 8*i1 ) / 8] 
            =        A158650784 [( 256 + 8*i1 ) / 8] 
            -        0.500000 
            *(        A158659008 [( 256 + 8*i1 ) / 8] 
            -        A158659008 [( 8*i1 ) / 8] 
            );
        }
        for ( i1 = 0 ; i1 <= 31 ; i1++) 
        {
            for ( i2 = 0 ; i2 <= 30 ; i2++) 
            {
                A158642560 [( 8 + 256*i1 + 8*i2 ) / 8] 
                =            A158642560 [( 8 + 256*i1 + 8*i2 ) / 8] 
                -            0.500000 
                *(            A158659008 [( 8 + 256*i1 + 8*i2 ) / 8] 
                -            A158659008 [( 256*i1 + 8*i2 ) / 8] 
                );
            }
        }
        for ( i1 = 0 ; i1 <= 30 ; i1++) 
        {
            for ( i2 = 0 ; i2 <= 30 ; i2++) 
            {
                A158659008 [( 256*i1 + 8*i2 ) / 8] 
                =            A158659008 [( 256*i1 + 8*i2 ) / 8] 
                -            0.700000 
                *(            A158642560 [( 8 + 256*i1 + 8*i2 ) / 8] 
                -            A158642560 [( 256*i1 + 8*i2 ) / 8] 
                +            A158650784 [( 256 + 256*i1 + 8*i2 ) / 8] 
                -            A158650784 [( 256*i1 + 8*i2 ) / 8] 
                );
            }
        }
    }
    #pragma endscop

    print_array(32,32,A158642560,A158650784,A158659008);
}
