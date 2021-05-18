#include "CudaProcess.cuh"
#include "cpu_multiply.h"
#include "cuda_multiply.cuh"
#include <iostream>
#include <string>

using namespace std;
using std::string;

int main() {
	cout << "process" << endl;
	Process();
	cout << "cpu" << endl;
	float* h_ptr = h_process();
	cout << "cuda" << endl;
	float* d_ptr = d_process();

	for (int i = 0; i < 1024 * 1024; i++) {
		float diff = fabsf(h_ptr[i] - d_ptr[i]);
		string hoge = to_string(h_ptr[i]);
		if (diff >= DBL_EPSILON) {
			cout << "error index:" << i << " host:" << h_ptr[i] << " cuda:" << d_ptr[i] << " diff:" << endl;
			break;
		}
	}
}