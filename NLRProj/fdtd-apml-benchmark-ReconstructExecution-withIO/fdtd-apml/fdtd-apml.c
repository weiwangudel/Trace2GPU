/**
 * fdtd-apml.c: This file is part of the PolyBench/C 3.2 test suite.
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
extern FILE *fid;
/* Include benchmark-specific header. */
/* Default data type is double, default size is 256x256x256. */
#include "fdtd-apml.h"


/* Array initialization. */
static
void init_array (int cz,
		 int cxm,
		 int cym,
		 DATA_TYPE *mui,
		 DATA_TYPE *ch,
		 DATA_TYPE POLYBENCH_2D(Ax,CZ+1,CYM+1,cz+1,cym+1),
		 DATA_TYPE POLYBENCH_2D(Ry,CZ+1,CYM+1,cz+1,cym+1),
		 DATA_TYPE POLYBENCH_3D(Ex,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		 DATA_TYPE POLYBENCH_3D(Ey,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		 DATA_TYPE POLYBENCH_3D(Hz,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		 DATA_TYPE POLYBENCH_1D(czm,CZ+1,cz+1),
		 DATA_TYPE POLYBENCH_1D(czp,CZ+1,cz+1),
		 DATA_TYPE POLYBENCH_1D(cxmh,CXM+1,cxm+1),
		 DATA_TYPE POLYBENCH_1D(cxph,CXM+1,cxm+1),
		 DATA_TYPE POLYBENCH_1D(cymh,CYM+1,cym+1),
		 DATA_TYPE POLYBENCH_1D(cyph,CYM+1,cym+1))
{
  int i, j, k;
  TnsMemWr(*mui) = TnsMemC(2341);
  fprintf(fid, "Line%i \n", __LINE__ - 1);
  
  TnsMemWr(*ch) = TnsMemC(42);
  fprintf(fid, "Line%i \n", __LINE__ - 1);
  
  for (i = 0; i <= cz; i++)
    {
      TnsMemWr(czm[i]) = ((DATA_TYPE) TnsMemIter(i) + TnsMemC(1)) / TnsMemC(cxm);
      fprintf(fid, "Line%i \n", __LINE__ - 1);
      TnsMemWr(czp[i]) = ((DATA_TYPE) TnsMemIter(i) + TnsMemC(2)) / TnsMemC(cxm);
      fprintf(fid, "Line%i \n", __LINE__ - 1);
    }
  for (i = 0; i <= cxm; i++)
    {
      TnsMemWr(cxmh[i]) = ((DATA_TYPE) TnsMemIter(i) + TnsMemC(3)) / TnsMemC(cxm);
      fprintf(fid, "Line%i \n", __LINE__ - 1);
      TnsMemWr(cxph[i]) = ((DATA_TYPE) TnsMemIter(i) + TnsMemC(4)) / TnsMemC(cxm);
      fprintf(fid, "Line%i \n", __LINE__ - 1);
    }
  for (i = 0; i <= cym; i++)
    {
      TnsMemWr(cymh[i]) = ((DATA_TYPE) TnsMemIter(i) + TnsMemC(5)) / TnsMemC(cxm);
      fprintf(fid, "Line%i \n", __LINE__ - 1);
      TnsMemWr(cyph[i]) = ((DATA_TYPE) TnsMemIter(i) + TnsMemC(6)) / TnsMemC(cxm);
      fprintf(fid, "Line%i \n", __LINE__ - 1);
    }

  for (i = 0; i <= cz; i++)
    for (j = 0; j <= cym; j++)
      {
	TnsMemWr(Ry[i][j]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(1)) + TnsMemC(10)) / TnsMemC(cym);
        fprintf(fid, "Line%i \n", __LINE__ - 1);
	TnsMemWr(Ax[i][j]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(2)) + TnsMemC(11)) / TnsMemC(cym);
        fprintf(fid, "Line%i \n", __LINE__ - 1);
	for (k = 0; k <= cxm; k++)
	  {
	    TnsMemWr(Ex[i][j][k]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(3)) + TnsMemIter(k) + TnsMemC(1)) / TnsMemC(cxm);
            fprintf(fid, "Line%i \n", __LINE__ - 1);
	    TnsMemWr(Ey[i][j][k]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(4)) + TnsMemIter(k) + TnsMemC(2)) / TnsMemC(cym);
            fprintf(fid, "Line%i \n", __LINE__ - 1);
	    TnsMemWr(Hz[i][j][k]) = ((DATA_TYPE) TnsMemIter(i)*(TnsMemIter(j)+TnsMemC(5)) + TnsMemIter(k) + TnsMemC(3)) / TnsMemC(cz);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
	  }
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int cz,
		 int cxm,
		 int cym,
		 DATA_TYPE POLYBENCH_3D(Bza,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		 DATA_TYPE POLYBENCH_3D(Ex,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		 DATA_TYPE POLYBENCH_3D(Ey,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		 DATA_TYPE POLYBENCH_3D(Hz,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1))
{
  int i, j, k;

  for (i = 0; i <= cz; i++)
    for (j = 0; j <= cym; j++)
      for (k = 0; k <= cxm; k++) {
	fprintf(stderr, DATA_PRINTF_MODIFIER, TnsMem(Bza[i][j][k]));
  	fprintf(fid, "Line%i \n", __LINE__ - 1);
	fprintf(stderr, DATA_PRINTF_MODIFIER, TnsMem(Ex[i][j][k]));
  	fprintf(fid, "Line%i \n", __LINE__ - 1);
	fprintf(stderr, DATA_PRINTF_MODIFIER, TnsMem(Ey[i][j][k]));
  	fprintf(fid, "Line%i \n", __LINE__ - 1);
	fprintf(stderr, DATA_PRINTF_MODIFIER, TnsMem(Hz[i][j][k]));
  	fprintf(fid, "Line%i \n", __LINE__ - 1);
	if ((TnsMemIter(i) * TnsMemC(cxm) + TnsMemIter(j)) % TnsMemC(20) == TnsMemC(0)) fprintf(stderr, "\n");
  	fprintf(fid, "Line%i \n", __LINE__ - 1);
      }
  fprintf(stderr, "\n");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_fdtd_apml(int cz,
		      int cxm,
		      int cym,
		      DATA_TYPE mui,
		      DATA_TYPE ch,
		      DATA_TYPE POLYBENCH_2D(Ax,CZ+1,CYM+1,cz+1,cym+1),
		      DATA_TYPE POLYBENCH_2D(Ry,CZ+1,CYM+1,cz+1,cym+1),
		      DATA_TYPE POLYBENCH_2D(clf,CYM+1,CXM+1,cym+1,cxm+1),
		      DATA_TYPE POLYBENCH_2D(tmp,CYM+1,CXM+1,cym+1,cxm+1),
		      DATA_TYPE POLYBENCH_3D(Bza,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		      DATA_TYPE POLYBENCH_3D(Ex,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		      DATA_TYPE POLYBENCH_3D(Ey,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		      DATA_TYPE POLYBENCH_3D(Hz,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1),
		      DATA_TYPE POLYBENCH_1D(czm,CZ+1,cz+1),
		      DATA_TYPE POLYBENCH_1D(czp,CZ+1,cz+1),
		      DATA_TYPE POLYBENCH_1D(cxmh,CXM+1,cxm+1),
		      DATA_TYPE POLYBENCH_1D(cxph,CXM+1,cxm+1),
		      DATA_TYPE POLYBENCH_1D(cymh,CYM+1,cym+1),
		      DATA_TYPE POLYBENCH_1D(cyph,CYM+1,cym+1))
{
  int iz, iy, ix;
  
  for (iz = 0; iz <= _PB_CZ; iz++)
  {
    for (iy = 0; iy <= _PB_CYM; iy++)
    {
	TnsMemWr(clf[iz][iy]) = TnsMemC(0.0);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
	TnsMemWr(tmp[iz][iy]) = TnsMemC(0.0);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
 	for (ix = 0; ix <= _PB_CXM; ix++)	
	{
	    TnsMemWr(Bza[iz][iy][ix]) = TnsMemC(0.0);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
	}
    }
  }


#pragma scop
  for (iz = 0; iz < _PB_CZ; iz++)
    {
      for (iy = 0; iy < _PB_CYM; iy++)
	{
	  for (ix = 0; ix < _PB_CXM; ix++)
	    {
	      TnsMemWr(clf[iz][iy]) = TnsMem(Ex[iz][iy][ix]) - TnsMem(Ex[iz][iy+1][ix]) + TnsMem(Ey[iz][iy][ix+1]) - TnsMem(Ey[iz][iy][ix]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	      TnsMemWr(tmp[iz][iy]) = (TnsMem(cymh[iy]) / TnsMem(cyph[iy])) * TnsMem(Bza[iz][iy][ix]) - (TnsMemC(ch) / TnsMem(cyph[iy])) * TnsMem(clf[iz][iy]);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	      TnsMemWr(Hz[iz][iy][ix]) = (TnsMem(cxmh[ix]) /TnsMem(cxph[ix])) * TnsMem(Hz[iz][iy][ix]) 		+ (TnsMemC(mui) * TnsMem(czp[iz]) / TnsMem(cxph[ix])) * TnsMem(tmp[iz][iy]) 		- (TnsMemC(mui) * TnsMem(czm[iz]) / TnsMem(cxph[ix])) * TnsMem(Bza[iz][iy][ix]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	      TnsMemWr(Bza[iz][iy][ix]) = TnsMem(tmp[iz][iy]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	    }
	  TnsMemWr(clf[iz][iy]) = TnsMem(Ex[iz][iy][_PB_CXM]) - TnsMem(Ex[iz][iy+1][_PB_CXM]) + TnsMem(Ry[iz][iy]) - TnsMem(Ey[iz][iy][_PB_CXM]);
	  fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  TnsMemWr(tmp[iz][iy]) = (TnsMem(cymh[iy]) / TnsMem(cyph[iy])) * TnsMem(Bza[iz][iy][_PB_CXM]) - (TnsMemC(ch) / TnsMem(cyph[iy])) * TnsMem(clf[iz][iy]);
	  fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  TnsMemWr(Hz[iz][iy][_PB_CXM]) = (TnsMem(cxmh[_PB_CXM]) / TnsMem(cxph[_PB_CXM])) * TnsMem(Hz[iz][iy][_PB_CXM])  + (TnsMemC(mui) * TnsMem(czp[iz]) / TnsMem(cxph[_PB_CXM])) * TnsMem(tmp[iz][iy])  - (TnsMemC(mui) * TnsMem(czm[iz]) / TnsMem(cxph[_PB_CXM])) * TnsMem(Bza[iz][iy][_PB_CXM]);
	  fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  TnsMemWr(Bza[iz][iy][_PB_CXM]) = TnsMem(tmp[iz][iy]);
	  fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  for (ix = 0; ix < _PB_CXM; ix++)
	    {
	      TnsMemWr(clf[iz][iy]) = TnsMem(Ex[iz][_PB_CYM][ix]) - TnsMem(Ax[iz][ix]) + TnsMem(Ey[iz][_PB_CYM][ix+1]) - TnsMem(Ey[iz][_PB_CYM][ix]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	      TnsMemWr(tmp[iz][iy]) = (TnsMem(cymh[_PB_CYM]) / TnsMem(cyph[iy])) * TnsMem(Bza[iz][iy][ix]) - (TnsMemC(ch) / TnsMem(cyph[iy])) * TnsMem(clf[iz][iy]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	      TnsMemWr(Hz[iz][_PB_CYM][ix]) = (TnsMem(cxmh[ix]) / TnsMem(cxph[ix])) * TnsMem(Hz[iz][_PB_CYM][ix])		+ (TnsMemC(mui) * TnsMem(czp[iz]) / TnsMem(cxph[ix])) * TnsMem(tmp[iz][iy])		- (TnsMemC(mui) * TnsMem(czm[iz]) / TnsMem(cxph[ix])) * TnsMem(Bza[iz][_PB_CYM][ix]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	      TnsMemWr(Bza[iz][_PB_CYM][ix]) = TnsMem(tmp[iz][iy]);
	      fprintf(fid, "Line%i \n", __LINE__ - 1);
	      
	    }
	  TnsMemWr(clf[iz][iy]) = TnsMem(Ex[iz][_PB_CYM][_PB_CXM]) - TnsMem(Ax[iz][_PB_CXM]) + TnsMem(Ry[iz][_PB_CYM]) - TnsMem(Ey[iz][_PB_CYM][_PB_CXM]);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  TnsMemWr(tmp[iz][iy]) = (TnsMem(cymh[_PB_CYM]) / TnsMem(cyph[_PB_CYM])) * TnsMem(Bza[iz][_PB_CYM][_PB_CXM]) - (TnsMemC(ch) / TnsMem(cyph[_PB_CYM])) * TnsMem(clf[iz][iy]);
	    fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  TnsMemWr(Hz[iz][_PB_CYM][_PB_CXM]) = (TnsMem(cxmh[_PB_CXM]) / TnsMem(cxph[_PB_CXM])) * TnsMem(Hz[iz][_PB_CYM][_PB_CXM])	    + (TnsMemC(mui) * TnsMem(czp[iz]) / TnsMem(cxph[_PB_CXM])) * TnsMem(tmp[iz][iy])	    - (TnsMemC(mui) * TnsMem(czm[iz]) / TnsMem(cxph[_PB_CXM])) * TnsMem(Bza[iz][_PB_CYM][_PB_CXM]);
	  fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	  TnsMemWr(Bza[iz][_PB_CYM][_PB_CXM]) = TnsMem(tmp[iz][iy]);
	  fprintf(fid, "Line%i \n", __LINE__ - 1);
	  
	}
    }
#pragma endscop

}


int main(int argc, char** argv)
{

  fid = fopen("memory.txt", "w+t");
  /* Retrieve problem size. */
  int cz = CZ;
  int cym = CYM;
  int cxm = CXM;

  /* Variable declaration/allocation. */
  DATA_TYPE mui;
  DATA_TYPE ch;
  POLYBENCH_2D_ARRAY_DECL(Ax,DATA_TYPE,CZ+1,CYM+1,cz+1,cym+1);
  POLYBENCH_2D_ARRAY_DECL(Ry,DATA_TYPE,CZ+1,CYM+1,cz+1,cym+1);
  POLYBENCH_2D_ARRAY_DECL(clf,DATA_TYPE,CYM+1,CXM+1,cym+1,cxm+1);
  POLYBENCH_2D_ARRAY_DECL(tmp,DATA_TYPE,CYM+1,CXM+1,cym+1,cxm+1);
  POLYBENCH_3D_ARRAY_DECL(Bza,DATA_TYPE,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1);
  POLYBENCH_3D_ARRAY_DECL(Ex,DATA_TYPE,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1);
  POLYBENCH_3D_ARRAY_DECL(Ey,DATA_TYPE,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1);
  POLYBENCH_3D_ARRAY_DECL(Hz,DATA_TYPE,CZ+1,CYM+1,CXM+1,cz+1,cym+1,cxm+1);
  POLYBENCH_1D_ARRAY_DECL(czm,DATA_TYPE,CZ+1,cz+1);
  POLYBENCH_1D_ARRAY_DECL(czp,DATA_TYPE,CZ+1,cz+1);
  POLYBENCH_1D_ARRAY_DECL(cxmh,DATA_TYPE,CXM+1,cxm+1);
  POLYBENCH_1D_ARRAY_DECL(cxph,DATA_TYPE,CXM+1,cxm+1);
  POLYBENCH_1D_ARRAY_DECL(cymh,DATA_TYPE,CYM+1,cym+1);
  POLYBENCH_1D_ARRAY_DECL(cyph,DATA_TYPE,CYM+1,cym+1);

  /* Initialize array(s). */
  init_array (cz, cxm, cym, &mui, &ch,
  	      POLYBENCH_ARRAY(Ax),
  	      POLYBENCH_ARRAY(Ry),
  	      POLYBENCH_ARRAY(Ex),
  	      POLYBENCH_ARRAY(Ey),
  	      POLYBENCH_ARRAY(Hz),
  	      POLYBENCH_ARRAY(czm),
  	      POLYBENCH_ARRAY(czp),
  	      POLYBENCH_ARRAY(cxmh),
  	      POLYBENCH_ARRAY(cxph),
  	      POLYBENCH_ARRAY(cymh),
  	      POLYBENCH_ARRAY(cyph));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_fdtd_apml (cz, cxm, cym, mui, ch,
  		    POLYBENCH_ARRAY(Ax),
  		    POLYBENCH_ARRAY(Ry),
  		    POLYBENCH_ARRAY(clf),
  		    POLYBENCH_ARRAY(tmp),
  		    POLYBENCH_ARRAY(Bza),
  		    POLYBENCH_ARRAY(Ex),
  		    POLYBENCH_ARRAY(Ey),
  		    POLYBENCH_ARRAY(Hz),
  		    POLYBENCH_ARRAY(czm),
  		    POLYBENCH_ARRAY(czp),
  		    POLYBENCH_ARRAY(cxmh),
  		    POLYBENCH_ARRAY(cxph),
  		    POLYBENCH_ARRAY(cymh),
  		    POLYBENCH_ARRAY(cyph));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  (print_array(cz, cxm, cym,
  				    POLYBENCH_ARRAY(Bza),
  				    POLYBENCH_ARRAY(Ex),
  				    POLYBENCH_ARRAY(Ey),
  				    POLYBENCH_ARRAY(Hz)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(Ax);
  POLYBENCH_FREE_ARRAY(Ry);
  POLYBENCH_FREE_ARRAY(clf);
  POLYBENCH_FREE_ARRAY(tmp);
  POLYBENCH_FREE_ARRAY(Bza);
  POLYBENCH_FREE_ARRAY(Ex);
  POLYBENCH_FREE_ARRAY(Ey);
  POLYBENCH_FREE_ARRAY(Hz);
  POLYBENCH_FREE_ARRAY(czm);
  POLYBENCH_FREE_ARRAY(czp);
  POLYBENCH_FREE_ARRAY(cxmh);
  POLYBENCH_FREE_ARRAY(cxph);
  POLYBENCH_FREE_ARRAY(cymh);
  POLYBENCH_FREE_ARRAY(cyph);

  return 0;
}

