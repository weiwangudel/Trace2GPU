all:
	g++ -O3 main_test.cpp  -o main_test.exe
openmp:
	g++ -fopenmp -O3 main_test.cpp  -o omp_main_test.exe
acc_opencl:
	rm *hmp*  &
	hmpp --openacc-target=OPENCL g++ -O3 -lOpenCL -o main_test.exe main_test.cpp
acc_cuda:
	rm *hmp* &
	hmpp --openacc-target=CUDA g++ -O3 -o main_test.exe main_test.cpp
clean:
	rm	*cuda*
