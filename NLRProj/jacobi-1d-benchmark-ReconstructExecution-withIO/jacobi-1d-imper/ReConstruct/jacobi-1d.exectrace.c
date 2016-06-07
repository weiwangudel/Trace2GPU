/**
 * jacobi-1d-imper.c: This file is part of the PolyBench/C 3.2 test suite.
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
extern FILE* fid;

/* Include benchmark-specific header. */
/* Default data type is double, default size is 100x10000. */
#include "jacobi-1d-imper.h"


/* Array initialization. */
static
void init_array (int n,
		 DATA_TYPE POLYBENCH_1D(A,N,n),
		 DATA_TYPE POLYBENCH_1D(B,N,n))
{
  int i;

  for (i = 0; i < n; i++)
      {
	TnsMemWr(A[i]) = ((DATA_TYPE) TnsMemIter(i)+ TnsMemC(2)) / TnsMemC(n);
	fprintf(fid, "Line%i ", __LINE__-1);
	TnsMemWr(B[i]) = ((DATA_TYPE) TnsMemIter(i)+ TnsMemC(3)) / TnsMemC(n);
	fprintf(fid, "Line%i ", __LINE__-1);
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_1D(A,N,n))

{
  int i;

  for (i = 0; i < n; i++)
    {
      fprintf(stderr, DATA_PRINTF_MODIFIER, TnsMem(A[i]));
      fprintf(fid, "Line%i ", __LINE__-1);
      if (TnsMemIter(i) % TnsMemC(20) == TnsMemC(0)) fprintf(stderr, "\n");
      fprintf(fid, "Line%i ", __LINE__-1);
    }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_jacobi_1d_imper(int tsteps,
			    int n,
			    DATA_TYPE POLYBENCH_1D(A,N,n),
			    DATA_TYPE POLYBENCH_1D(B,N,n))
{
  int t, i, j;

#pragma scop
  for (t = 0; t < _PB_TSTEPS; t++)
    {
      for (i = 1; i < _PB_N - 1; i++) {
	TnsMemWr(B[i]) = TnsMemC(0.33333) * (TnsMem(A[i-1]) + TnsMem(A[i]) + TnsMem(A[i + 1]));
	fprintf(fid, "Line%i ", __LINE__ - 1);
      }
      for (j = 1; j < _PB_N - 1; j++) {
	TnsMemWr(A[j]) = TnsMem(B[j]);
	fprintf(fid, "Line%i ", __LINE__ - 1);
      }
    }
#pragma endscop

}


int main(int argc, char** argv)
{
  fid = fopen("memory.txt", "w+t");
  /* Retrieve problem size. */
  int n = N;
  int tsteps = TSTEPS;

  /* Variable declaration/allocation. */
  POLYBENCH_1D_ARRAY_DECL(A, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(B, DATA_TYPE, N, n);


  /* Initialize array(s). */
  init_array (n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_jacobi_1d_imper (tsteps, n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  (print_array(n, POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
