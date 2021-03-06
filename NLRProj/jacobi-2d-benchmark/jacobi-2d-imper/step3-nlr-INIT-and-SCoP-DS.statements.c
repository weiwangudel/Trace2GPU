#include<stdio.h>

static
void print_array(int n,
                 double *A)

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, "%0.2lf ", A[i*n+j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}



int main() {

int i0,i1,t;

double A166830464[10000];
double A166910496[10000];

for ( i0 = 0; i0 <= 9999; i0++) 
{
    A166830464 [( 8*i0 ) / 8] = ( (double) ((i0/100) * (i0%100+2)) + 2 ) / 100 ;
    A166910496 [( 8*i0 ) / 8] = ( (double) ((i0/100) * (i0%100+3)) + 3 ) / 100 ;
}

#pragma scop
for (t = 0; t < 2; t++) {  
  for ( i0 = 0; i0 <= 97; i0++) 
      for ( i1 = 0; i1 <= 97; i1++) 
          A166910496 [( 808 + 800*i0 + 8*i1 ) / 8] = 0.2 * (
          A166830464 [( 808 + 800*i0 + 8*i1 ) / 8] + 
          A166830464 [( 800 + 800*i0 + 8*i1 ) / 8] + 
          A166830464 [( 816 + 800*i0 + 8*i1 ) / 8] + 
          A166830464 [( 1608 + 800*i0 + 8*i1 ) / 8] + 
          A166830464 [( 8 + 800*i0 + 8*i1 ) / 8]) ; 
  for ( i0 = 0; i0 <= 97; i0++) 
      for ( i1 = 0; i1 <= 97; i1++) 
          A166830464 [( 808 + 800*i0 + 8*i1 ) / 8] = 
          A166910496 [( 808 + 800*i0 + 8*i1 ) / 8] ; 
 }
#pragma endscop
  print_array(100,  A166830464); 
  
}
