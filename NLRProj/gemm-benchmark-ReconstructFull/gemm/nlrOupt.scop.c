

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

    #pragma scop
    for ( i0 = 0; i0 <= 9; i0++) 
        for ( i1 = 0; i1 <= 9; i1++) 
        {
            A160661888 [( 10*i0 + i1 ) ] = 
            A160661888 [( 10*i0 + i1 ) ] * 
            A3216936832 ; 
            for ( i2 = 0; i2 <= 9; i2++) 
                A160661888 [( 10*i0 + i1 ) ] = 
                A160661888 [( 10*i0 + i1 ) ] + 
                A3216936840 * 
                A160662720 [( 10*i0 + i2 ) ] * 
                A160663552 [( i1 + 10*i2 ) ] ; 
	}
    #pragma endscop

    //verify results
    print_array(A160661888);
}
