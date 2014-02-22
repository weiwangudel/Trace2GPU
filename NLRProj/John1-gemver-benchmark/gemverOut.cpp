// JohnOut.cpp : Defines the entry point for the console application.
//



#include <stdio.h>
//#include <unistd.h>
#include <string.h>
#include <math.h>

//#include "instrument.h"


/* Default problem size. */
#ifndef N
# define N 100
#endif

/* Default data type is double. */
#ifndef DATA_TYPE
# define DATA_TYPE double
#endif


//TNS MEM
FILE* fid = NULL;

//double* TnsMem(double* pData)
//{
//	fprintf(fid, "Read@ %X " , (unsigned int)pData);
//	return pData;
//}

double& TnsMem(double& rData)
{
	fprintf(fid, "Read 0x%X " , &rData);
	return rData;
}

int& TnsMem(int& rData)
{
	fprintf(fid, "Read 0x%X " , &rData);
	return rData;
}

double& TnsMemWr(double& rData)
{
	fprintf(fid, "Write 0x%X " , &rData);
	return rData;
}




/* Array declaration. Enable malloc if POLYBENCH_TEST_MALLOC. */
DATA_TYPE alpha;
DATA_TYPE beta;
#ifndef POLYBENCH_TEST_MALLOC
DATA_TYPE A[N][N];
DATA_TYPE B[N][N];
DATA_TYPE x[N];
DATA_TYPE u1[N];
DATA_TYPE u2[N];
DATA_TYPE v2[N];
DATA_TYPE v1[N];
DATA_TYPE w[N];
DATA_TYPE y[N];
DATA_TYPE z[N];
#else
DATA_TYPE** A = (DATA_TYPE**)malloc(N * sizeof(DATA_TYPE*));
DATA_TYPE** B = (DATA_TYPE**)malloc(N * sizeof(DATA_TYPE*));
DATA_TYPE* x = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* u1 = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* u2 = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* v1 = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* v2 = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* w = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* y = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
DATA_TYPE* z = (DATA_TYPE*)malloc(N * sizeof(DATA_TYPE));
{
  int i;
  for (i = 0; i < N; ++i)
    {
      (A[(i)]) = (DATA_TYPE*)malloc((N) * sizeof(DATA_TYPE));
      (B[(i)]) = (DATA_TYPE*)malloc((N) * sizeof(DATA_TYPE));
    }
}
#endif

inline
void init_array()
{
  int i, j;

  (alpha) = 43532;//	fprintf(fid, "Line@ %i \n" , __LINE__);
  (beta) = 12313; //  fprintf(fid, "Line@ %i \n" , __LINE__);
  for (i = 0; i < N; i++)
    {
      (u1[(i)]) = (i);						//fprintf(fid, "Line@ %i \n" , __LINE__);
      (u2[(i)]) = ((i)+1)/(N)/2.0;			//fprintf(fid, "Line@ %i \n" , __LINE__);
      (v1[(i)]) = ((i)+1)/(N)/4.0;			//fprintf(fid, "Line@ %i \n" , __LINE__);
      (v2[(i)]) = ((i)+1)/(N)/6.0;			//fprintf(fid, "Line@ %i \n" , __LINE__);
      (y[(i)]) = ((i)+1)/(N)/8.0;			//fprintf(fid, "Line@ %i \n" , __LINE__);
      (z[(i)]) = ((i)+1)/(N)/9.0;			//fprintf(fid, "Line@ %i \n" , __LINE__);
      (x[(i)]) = 0.0;								//fprintf(fid, "Line@ %i \n" , __LINE__);
      (w[(i)]) = 0.0;								//fprintf(fid, "Line@ %i \n" , __LINE__);
      for (j = 0; j < N; j++)
	  {
		(A[(i)][(j)]) = (((DATA_TYPE) (i))*(j)) / (N); //	fprintf(fid, "Line@ %i \n" , __LINE__);
	  }
    }
}

/* Define the live-out variables. Code is not executed unless
   POLYBENCH_DUMP_ARRAYS is defined. */
inline
void print_array(int argc, char** argv)
{
  int i, j;
#ifndef POLYBENCH_DUMP_ARRAYS
 
#endif
    {
      for (i = 0; i < N; i++) {
	fprintf(stderr, "%0.2lf ", w[i]);
	if (i%80 == 20) fprintf(stderr, "\n");
      }
      fprintf(stderr, "\n");
    }
}


int main(int argc, char** argv)
{
	fid = fopen("memory.txt", "w+t");

  int i, j;
  int n = (N);

  /* Initialize array. */
  init_array();

  /* Start timer. */
  //polybench_start_instruments;

#pragma scop
#pragma live-out w

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
	{
      TnsMemWr(A[(i)][(j)]) = TnsMem(A[(i)][(j)]) + TnsMem(u1[(i)]) * TnsMem(v1[(j)]) + TnsMem(u2[(i)]) * TnsMem(v2[(j)]);	fprintf(fid, "Line@ %i \n" , __LINE__);
	}

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
	{
      TnsMemWr(x[(i)]) = TnsMem(x[(i)]) + TnsMem(beta) * TnsMem(A[(j)][(i)]) * TnsMem(y[(j)]);	fprintf(fid, "Line@ %i \n" , __LINE__);
	}

  for (i = 0; i < N; i++)
  {
    TnsMemWr(x[(i)]) = TnsMem(x[(i)]) + TnsMem(z[(i)]);	fprintf(fid, "Line@ %i \n" , __LINE__);
  }

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
	{
      TnsMemWr(w[(i)]) = TnsMem(w[(i)]) +  TnsMem(alpha) * TnsMem(A[(i)][(j)]) * TnsMem(x[(j)]);	fprintf(fid, "Line@ %i \n" , __LINE__);
	}

#pragma endscop

  /* Stop and print timer. */
  //polybench_stop_instruments;
  //polybench_print_instruments;

  print_array(argc, argv);

  fclose(fid);
  return 0;
}
