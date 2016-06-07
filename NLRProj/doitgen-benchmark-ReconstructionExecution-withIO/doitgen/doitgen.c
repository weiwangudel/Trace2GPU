/**
 * doitgen.c: This file is part of the PolyBench/C 3.2 test suite.
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
/* Default data type is double, default size is 4000. */
#include "doitgen.h"


/* Array initialization. */
static
void init_array(int nr, int nq, int np,
		DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np))
{
  int i, j, k;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++) {
	TnsMemWr(A[i][j][k]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j) + TnsMemIter(k)) / TnsMemC(np);
	fprintf(fid, "Line%i \n", __LINE__ - 1);
      }
  for (i = 0; i < np; i++)
    for (j = 0; j < np; j++) {
      TnsMemWr(C4[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(np);
	fprintf(fid, "Line%i \n", __LINE__ - 1);
    }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nr, int nq, int np,
		 DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np))
{
  int i, j, k;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, TnsMem(A[i][j][k]));
	fprintf(fid, "Line%i \n", __LINE__ - 1);
	if (TnsMemIter(i) % TnsMemC(20) == TnsMemC(0)) fprintf (stderr, "\n");
	fprintf(fid, "Line%i \n", __LINE__ - 1);
      }
  fprintf (stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_doitgen(int nr, int nq, int np,
		    DATA_TYPE POLYBENCH_3D(A,NR,NQ,NP,nr,nq,np),
		    DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
		    DATA_TYPE POLYBENCH_3D(sum,NR,NQ,NP,nr,nq,np))
{
  int r, q, p, s;

#pragma scop
  for (r = 0; r < _PB_NR; r++)
    for (q = 0; q < _PB_NQ; q++)  {
      for (p = 0; p < _PB_NP; p++)  {
	TnsMemWr(sum[r][q][p]) = TnsMemC(0);
	fprintf(fid, "Line%i \n", __LINE__ - 1);
	for (s = 0; s < _PB_NP; s++) {
	  TnsMemWr(sum[r][q][p]) = TnsMem(sum[r][q][p]) + TnsMem(A[r][q][s]) * TnsMem(C4[s][p]);
	fprintf(fid, "Line%i \n", __LINE__ - 1);
	}
      }
      for (p = 0; p < _PB_NR; p++) {
	TnsMemWr(A[r][q][p]) = TnsMem(sum[r][q][p]);
	fprintf(fid, "Line%i \n", __LINE__ - 1);
	}
    }
#pragma endscop

}


int main(int argc, char** argv)
{
  fid = fopen("memory.txt", "w+t");
  /* Retrieve problem size. */
  int nr = NR;
  int nq = NQ;
  int np = NP;

  /* Variable declaration/allocation. */
  POLYBENCH_3D_ARRAY_DECL(A,DATA_TYPE,NR,NQ,NP,nr,nq,np);
  POLYBENCH_3D_ARRAY_DECL(sum,DATA_TYPE,NR,NQ,NP,nr,nq,np);
  POLYBENCH_2D_ARRAY_DECL(C4,DATA_TYPE,NP,NP,np,np);

  /* Initialize array(s). */
  init_array (nr, nq, np,
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(C4));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_doitgen (nr, nq, np,
		  POLYBENCH_ARRAY(A),
		  POLYBENCH_ARRAY(C4),
		  POLYBENCH_ARRAY(sum));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  (print_array(nr, nq, np,  POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(sum);
  POLYBENCH_FREE_ARRAY(C4);

  return 0;
}
