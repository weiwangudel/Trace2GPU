/**
 * gemm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>
#include <gentrace.h>
/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "gemm.h"


extern FILE* fid;
/* Array initialization. */
static
void init_array(int ni, int nj, int nk,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
		DATA_TYPE POLYBENCH_2D(A,NI,NK,ni,nk),
		DATA_TYPE POLYBENCH_2D(B,NK,NJ,nk,nj))
{
  int i, j;

  TnsMemWr(*alpha) = TnsMemC(32412);fprintf(fid, "Line%i \n", __LINE__); 
  TnsMemWr(*beta) = TnsMemC(2123);fprintf(fid, "Line%i \n", __LINE__); 
  for (i = 0; i < ni; i++)
  {
    for (j = 0; j < nj; j++) 
    {
      TnsMemWr(C[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(ni); fprintf(fid, "Line%i \n", __LINE__);
    }
  }
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
    {
      TnsMemWr(A[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(ni);fprintf(fid, "Line%i \n", __LINE__);
    }
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
    {
      TnsMemWr(B[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(ni);fprintf(fid, "Line%i \n", __LINE__);
    }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni, int nj,
		 DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, C[i][j]);
	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gemm(int ni, int nj, int nk,
		 DATA_TYPE *alpha,
		 DATA_TYPE *beta,
		 DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
		 DATA_TYPE POLYBENCH_2D(A,NI,NK,ni,nk),
		 DATA_TYPE POLYBENCH_2D(B,NK,NJ,nk,nj))
{
  int i, j, k;

#pragma scop
  /* C := alpha*A*B + beta*C */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	TnsMemWr(C[i][j]) = TnsMem(C[i][j]) * TnsMem(*beta);  fprintf(fid, "Line%i \n", __LINE__);
	for (k = 0; k < _PB_NK; ++k) {
	  TnsMemWr(C[i][j]) = TnsMem(C[i][j]) + TnsMem(*alpha) * TnsMem(A[i][k]) * TnsMem(B[k][j]); fprintf(fid,"Line%i \n", __LINE__);
        }
      }
#pragma endscop

}

int main(int argc, char** argv)
{
   fid = fopen("memory.txt", "w+t");
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;
  int nk = NK;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NI,NJ,ni,nj);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NI,NK,ni,nk);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,NK,NJ,nk,nj);

  /* Initialize array(s). */
  init_array (ni, nj, nk, &alpha, &beta,
	      POLYBENCH_ARRAY(C),
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_gemm (ni, nj, nk,
	       &alpha, &beta,
	       POLYBENCH_ARRAY(C),
	       POLYBENCH_ARRAY(A),
	       POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  //polybench_prevent_dce(print_array(ni, nj,  POLYBENCH_ARRAY(C)));
  print_array(ni, nj,  POLYBENCH_ARRAY(C));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
