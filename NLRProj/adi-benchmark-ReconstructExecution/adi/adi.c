/**
 * adi.c: This file is part of the PolyBench/C 3.2 test suite.
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

/* Include benchmark-specific header. */
/* Default data type is double, default size is 10x1024x1024. */
#include "adi.h"
#include <gentrace.h>
extern FILE* fid;

/* Array initialization. */
static
void init_array (int n,
		 DATA_TYPE POLYBENCH_2D(X,N,N,n,n),
		 DATA_TYPE POLYBENCH_2D(A,N,N,n,n),
		 DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      {
	TnsMemWr(X[i][j]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(1)) + TnsMemC(1)) / TnsMemC(n);
	fprintf(fid, "Line%i \n", __LINE__-1);
	TnsMemWr(A[i][j]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(2)) + TnsMemC(2)) / TnsMemC(n);
	fprintf(fid, "Line%i \n", __LINE__-1);
	TnsMemWr(B[i][j]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(3)) + TnsMemC(3)) / TnsMemC(n);
	fprintf(fid, "Line%i \n", __LINE__-1);
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE POLYBENCH_2D(X,N,N,n,n))

{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      fprintf(stderr, "%0.2lf ", X[i][j]);
      if ((i * N + j) % 20 == 0) { fprintf(stderr,"\n"); }
    }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_adi(int tsteps,
		int n,
		DATA_TYPE POLYBENCH_2D(X,N,N,n,n),
		DATA_TYPE POLYBENCH_2D(A,N,N,n,n),
		DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
  int t, i1, i2;

#pragma scop
  for (t = 0; t < _PB_TSTEPS; t++)
    {
      for (i1 = 0; i1 < _PB_N; i1++)
	for (i2 = 1; i2 < _PB_N; i2++)
	  {
	    TnsMemWr(X[i1][i2]) = TnsMem(X[i1][i2]) - TnsMem(X[i1][i2-1]) * TnsMem(A[i1][i2]) / TnsMem(B[i1][i2-1]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
	    TnsMemWr(B[i1][i2]) = TnsMem(B[i1][i2]) - TnsMem(A[i1][i2]) * TnsMem(A[i1][i2]) / TnsMem(B[i1][i2-1]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
	  }

      for (i1 = 0; i1 < _PB_N; i1++) {
	TnsMemWr(X[i1][_PB_N-1]) = TnsMem(X[i1][_PB_N-1]) / TnsMem(B[i1][_PB_N-1]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
	}

      for (i1 = 0; i1 < _PB_N; i1++)
	for (i2 = 0; i2 < _PB_N-2; i2++) 
	{
	  TnsMemWr(X[i1][_PB_N-i2-2]) = (TnsMem(X[i1][_PB_N-2-i2]) - TnsMem(X[i1][_PB_N-2-i2-1]) * TnsMem(A[i1][_PB_N-i2-3])) / TnsMem(B[i1][_PB_N-3-i2]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
 	}

      for (i1 = 1; i1 < _PB_N; i1++)
	for (i2 = 0; i2 < _PB_N; i2++) {
	  TnsMemWr(X[i1][i2]) = TnsMem(X[i1][i2]) - TnsMem(X[i1-1][i2]) * TnsMem(A[i1][i2]) / TnsMem(B[i1-1][i2]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
	  TnsMemWr(B[i1][i2]) = TnsMem(B[i1][i2]) - TnsMem(A[i1][i2]) * TnsMem(A[i1][i2]) / TnsMem(B[i1-1][i2]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
	}

      for (i2 = 0; i2 < _PB_N; i2++)
	{
	TnsMemWr(X[_PB_N-1][i2]) = TnsMem(X[_PB_N-1][i2]) / TnsMem(B[_PB_N-1][i2]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
	}

      for (i1 = 0; i1 < _PB_N-2; i1++)
	for (i2 = 0; i2 < _PB_N; i2++)
	{
	  TnsMemWr(X[_PB_N-2-i1][i2]) = (TnsMem(X[_PB_N-2-i1][i2]) - TnsMem(X[_PB_N-i1-3][i2]) * TnsMem(A[_PB_N-3-i1][i2])) / TnsMem(B[_PB_N-2-i1][i2]);
	    fprintf(fid,"Line%i \n", __LINE__ -1);
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
  POLYBENCH_2D_ARRAY_DECL(X, DATA_TYPE, N, N, n, n);
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N, N, n, n);


  /* Initialize array(s). */
  init_array (n, POLYBENCH_ARRAY(X), POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_adi (tsteps, n, POLYBENCH_ARRAY(X),
	      POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  print_array(n, POLYBENCH_ARRAY(X));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(X);
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
