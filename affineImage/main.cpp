#include <iostream>
#include "halcon_logic.h"
#include "cpu_logic.h"
#include "gpu_logic.cuh"
using namespace std;

int main() {
	cout << "cpu" << endl;
	cpu_affine();
	cout << "gpu" << endl;
	try {
		gpu_affine();
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	cout << "halcon" << endl;
	h_affine();
	cout << "halcon gpu" << endl;
	h_gpu_affine();
}