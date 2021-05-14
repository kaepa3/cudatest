#include "CudaProcess.cuh"
#include "cpu_multiply.h"
#include "cuda_multiply.cuh"
#include <iostream>

using namespace std;

int main() {
	cout << "process" << endl;
	Process();
	cout << "cpu" << endl;
	h_process();
	cout << "cuda" << endl;
	d_process();

}