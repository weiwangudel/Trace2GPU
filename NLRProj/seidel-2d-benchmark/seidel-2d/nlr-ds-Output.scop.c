#include<stdio.h>


static
void print_array(int n,
                 double *A)

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, "%0.2lf ", A[i*32+j]);
      if ((i * n + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}



int main()
{
int i0, i1;
int t;
double A148943232[1024];

for ( i0 = 0; i0 <= 1023; i0++) 
    A148943232 [( 8*i0 ) / 8] =  ( (double) ((i0/32) * (i0%32+2)) + 2 ) / 32 ;
 
  for (t = 0; t<2; t++)
  {
    #pragma scop
    for ( i0 = 0; i0 <= 29; i0++) 
        for ( i1 = 0; i1 <= 29; i1++) 
        {
            A148943232 [( 264 + 256*i0 + 8*i1 ) / 8] = 
            (A148943232 [( 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 8 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 16 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 256 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 264 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 272 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 512 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 520 + 256*i0 + 8*i1 ) / 8] + 
            A148943232 [( 528 + 256*i0 + 8*i1 ) / 8]) / 9.0 ; 
        }
    #pragma endscop
  }
 
  print_array(32, A148943232); 

}
