1@ /**
2@  * gemm.c: This file is part of the PolyBench/C 3.2 test suite.
3@  *
4@  *
5@  * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
6@  * Web address: http://polybench.sourceforge.net
7@  */
8@ #include <stdio.h>
9@ #include <unistd.h>
10@ #include <string.h>
11@ #include <math.h>
12@ 
13@ /* Include polybench common header. */
14@ #include <polybench.h>
15@ #include <gentrace.h>
16@ /* Include benchmark-specific header. */
17@ /* Default data type is double, default size is 4000. */
18@ #include "gemm.h"
19@ 
20@ 
21@ extern FILE* fid;
22@ /* Array initialization. */
23@ static
24@ void init_array(int ni, int nj, int nk,
25@ 		DATA_TYPE *alpha,
26@ 		DATA_TYPE *beta,
27@ 		DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
28@ 		DATA_TYPE POLYBENCH_2D(A,NI,NK,ni,nk),
29@ 		DATA_TYPE POLYBENCH_2D(B,NK,NJ,nk,nj))
30@ {
31@   int i, j;
32@ 
33@   TnsMemWr(*alpha) = TnsMemC(32412);fprintf(fid, "Line%i \n", __LINE__); 
34@   TnsMemWr(*beta) = TnsMemC(2123);fprintf(fid, "Line%i \n", __LINE__); 
35@   for (i = 0; i < ni; i++)
36@   {
37@     for (j = 0; j < nj; j++) 
38@     {
39@       TnsMemWr(C[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(ni); fprintf(fid, "Line%i \n", __LINE__);
40@     }
41@   }
42@   for (i = 0; i < ni; i++)
43@     for (j = 0; j < nk; j++)
44@     {
45@       TnsMemWr(A[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(ni);fprintf(fid, "Line%i \n", __LINE__);
46@     }
47@   for (i = 0; i < nk; i++)
48@     for (j = 0; j < nj; j++)
49@     {
50@       TnsMemWr(B[i][j]) = ((DATA_TYPE) TnsMemIter(i)*TnsMemIter(j)) / TnsMemC(ni);fprintf(fid, "Line%i \n", __LINE__);
51@     }
52@ }
53@ 
54@ 
55@ /* DCE code. Must scan the entire live-out data.
56@    Can be used also to check the correctness of the output. */
57@ static
58@ void print_array(int ni, int nj,
59@ 		 DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj))
60@ {
61@   int i, j;
62@ 
63@   for (i = 0; i < ni; i++)
64@     for (j = 0; j < nj; j++) {
65@ 	fprintf (stderr, DATA_PRINTF_MODIFIER, C[i][j]);
66@ 	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
67@     }
68@   fprintf (stderr, "\n");
69@ }
70@ 
71@ 
72@ /* Main computational kernel. The whole function will be timed,
73@    including the call and return. */
74@ static
75@ void kernel_gemm(int ni, int nj, int nk,
76@ 		 DATA_TYPE *alpha,
77@ 		 DATA_TYPE *beta,
78@ 		 DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
79@ 		 DATA_TYPE POLYBENCH_2D(A,NI,NK,ni,nk),
80@ 		 DATA_TYPE POLYBENCH_2D(B,NK,NJ,nk,nj))
81@ {
82@   int i, j, k;
83@ 
84@ #pragma scop
85@   /* C := alpha*A*B + beta*C */
86@   for (i = 0; i < _PB_NI; i++)
87@     for (j = 0; j < _PB_NJ; j++)
88@       {
89@ 	TnsMemWr(C[i][j]) = TnsMem(C[i][j]) * TnsMem(*beta);  fprintf(fid, "Line%i \n", __LINE__);
90@ 	for (k = 0; k < _PB_NK; ++k) {
91@ 	  TnsMemWr(C[i][j]) = TnsMem(C[i][j]) + TnsMem(*alpha) * TnsMem(A[i][k]) * TnsMem(B[k][j]); fprintf(fid,"Line%i \n", __LINE__);
92@         }
93@       }
94@ #pragma endscop
95@ 
96@ }
97@ 
98@ int main(int argc, char** argv)
99@ {
100@    fid = fopen("memory.txt", "w+t");
101@   /* Retrieve problem size. */
102@   int ni = NI;
103@   int nj = NJ;
104@   int nk = NK;
105@ 
106@   /* Variable declaration/allocation. */
107@   DATA_TYPE alpha;
108@   DATA_TYPE beta;
109@   POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NI,NJ,ni,nj);
110@   POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NI,NK,ni,nk);
111@   POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,NK,NJ,nk,nj);
112@ 
113@   /* Initialize array(s). */
114@   init_array (ni, nj, nk, &alpha, &beta,
115@ 	      POLYBENCH_ARRAY(C),
116@ 	      POLYBENCH_ARRAY(A),
117@ 	      POLYBENCH_ARRAY(B));
118@ 
119@   /* Start timer. */
120@   polybench_start_instruments;
121@ 
122@   /* Run kernel. */
123@   kernel_gemm (ni, nj, nk,
124@ 	       &alpha, &beta,
125@ 	       POLYBENCH_ARRAY(C),
126@ 	       POLYBENCH_ARRAY(A),
127@ 	       POLYBENCH_ARRAY(B));
128@ 
129@   /* Stop and print timer. */
130@   polybench_stop_instruments;
131@   polybench_print_instruments;
132@ 
133@   /* Prevent dead-code elimination. All live-out data must be printed
134@      by the function call in argument. */
135@   //polybench_prevent_dce(print_array(ni, nj,  POLYBENCH_ARRAY(C)));
136@   print_array(ni, nj,  POLYBENCH_ARRAY(C));
137@ 
138@   /* Be clean. */
139@   POLYBENCH_FREE_ARRAY(C);
140@   POLYBENCH_FREE_ARRAY(A);
141@   POLYBENCH_FREE_ARRAY(B);
142@ 
143@   return 0;
144@ }
