/**
 * fdtd-2d.c: This file is part of the PolyBench/C 3.2 test suite.
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
extern FILE * fid;

/* Include benchmark-specific header. */
/* Default data type is double, default size is 50x1000x1000. */
#include "fdtd-2d.h"


/* Array initialization. */
static
void init_array (int tmax,
		 int nx,
		 int ny,
		 DATA_TYPE POLYBENCH_2D(ex,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(ey,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(hz,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_1D(_fict_,TMAX,tmax))
{
  int i, j;

  for (i = 0; i < tmax; i++) {
    TnsMemWr(_fict_[i]) = (DATA_TYPE) i;
    fprintf(fid, "\n");
  }
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++)
      {
	TnsMemWr(ex[i][j]) = ((DATA_TYPE) i*(j+1)) / nx;
    	fprintf(fid, "\n");
      }
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) 
	{
	TnsMemWr(ey[i][j]) = ((DATA_TYPE) i*(j+2)) / ny;
    	fprintf(fid, "\n");
	}
     TnsMemWr(ey[0][2]);
     fprintf(fid, "\n");
     
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++)  {
	TnsMemWr(hz[i][j]) = ((DATA_TYPE) i*(j+3)) / nx;
    	fprintf(fid, "\n");
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nx,
		 int ny,
		 DATA_TYPE POLYBENCH_2D(ex,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(ey,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(hz,NX,NY,nx,ny))
{
  int i, j;

  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {
      fprintf(stderr, DATA_PRINTF_MODIFIER, ex[i][j]);
      fprintf(stderr, DATA_PRINTF_MODIFIER, ey[i][j]);
      fprintf(stderr, DATA_PRINTF_MODIFIER, hz[i][j]);
      if ((i * nx + j) % 20 == 0) fprintf(stderr, "\n");
    }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_fdtd_2d(int tmax,
		    int nx,
		    int ny,
		    DATA_TYPE POLYBENCH_2D(ex,NX,NY,nx,ny),
		    DATA_TYPE POLYBENCH_2D(ey,NX,NY,nx,ny),
		    DATA_TYPE POLYBENCH_2D(hz,NX,NY,nx,ny),
		    DATA_TYPE POLYBENCH_1D(_fict_,TMAX,tmax))
{
  int t, i, j;

#pragma scop

  for(t = 0; t < _PB_TMAX; t++)
    {
      for (j = 0; j < _PB_NY; j++) {
	TnsMemWr(ey[0][j]) = TnsMem(_fict_[t]);
	fprintf(fid,"\n");
      }
      for (i = 1; i < _PB_NX; i++)
	for (j = 0; j < _PB_NY; j++) {
	  TnsMemWr(ey[i][j]) = TnsMem(ey[i][j]) - 0.5*(TnsMem(hz[i][j])-TnsMem(hz[i-1][j]));
	  fprintf(fid,"\n");
      }
      for (i = 0; i < _PB_NX; i++)
	for (j = 1; j < _PB_NY; j++) {
	  TnsMemWr(ex[i][j]) = TnsMem(ex[i][j]) - 0.5*(TnsMem(hz[i][j])-TnsMem(hz[i][j-1]));
	  fprintf(fid,"\n");
      }
      for (i = 0; i < _PB_NX - 1; i++)
	for (j = 0; j < _PB_NY - 1; j++) {
	  TnsMemWr(hz[i][j]) = TnsMem(hz[i][j]) - 0.7*  (TnsMem(ex[i][j+1]) - TnsMem(ex[i][j]) +
				       TnsMem(ey[i+1][j]) - TnsMem(ey[i][j]));
	  fprintf(fid,"\n");
      }
    }

#pragma endscop
}


int main(int argc, char** argv)
{
  fid = fopen("memory.txt", "w+t");
  /* Retrieve problem size. */
  int tmax = TMAX;
  int nx = NX;
  int ny = NY;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(ex,DATA_TYPE,NX,NY,nx,ny);
  POLYBENCH_2D_ARRAY_DECL(ey,DATA_TYPE,NX,NY,nx,ny);
  POLYBENCH_2D_ARRAY_DECL(hz,DATA_TYPE,NX,NY,nx,ny);
  POLYBENCH_1D_ARRAY_DECL(_fict_,DATA_TYPE,TMAX,tmax);

  /* Initialize array(s). */
  init_array (tmax, nx, ny,
	      POLYBENCH_ARRAY(ex),
	      POLYBENCH_ARRAY(ey),
	      POLYBENCH_ARRAY(hz),
	      POLYBENCH_ARRAY(_fict_));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_fdtd_2d (tmax, nx, ny,
		  POLYBENCH_ARRAY(ex),
		  POLYBENCH_ARRAY(ey),
		  POLYBENCH_ARRAY(hz),
		  POLYBENCH_ARRAY(_fict_));


  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  print_array(nx, ny, POLYBENCH_ARRAY(ex),
				    POLYBENCH_ARRAY(ey),
				    POLYBENCH_ARRAY(hz));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(ex);
  POLYBENCH_FREE_ARRAY(ey);
  POLYBENCH_FREE_ARRAY(hz);
  POLYBENCH_FREE_ARRAY(_fict_);

  return 0;
}
