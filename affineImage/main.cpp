#include <iostream>
#include "halcon_logic.h"
#include "cpu_logic.h"
#include "gpu_logic.cuh"
using namespace std;

int main() {
	cout << "halcon" << endl;
	h_affine();
	cout << "cpu" << endl;
	cpu_affine();
	cout << "gpu" << endl;
	gpu_affine();
}