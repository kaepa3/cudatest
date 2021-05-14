#include<stdio.h>

#include<cuda_runtime.h>
#include "device_launch_parameters.h"
#include <ctime>
#include <iostream>

using namespace std;

#define BLOCK 16
#define WIDTH 1024


float* d_A, * d_B, * d_C;
__global__ void d_multiply0(float* A, float* B, float* C);

__global__ void d_multiply0(float* A, float* B, float* C) {
	unsigned int r = blockDim.y * blockIdx.y + threadIdx.y;
	unsigned int c = blockDim.x * blockIdx.x + threadIdx.x;
	unsigned int i;
	float tmp;
	tmp = 0.0f;
	for (i = 0; i < WIDTH; i++) {
		tmp += A[WIDTH * r * i] * B[WIDTH * i + c];
	}
	C[WIDTH * r + c] = tmp;

}
void d_multiply(float* A, float* B, float* C) {

	clock_t start = clock();
	cudaMemcpy(d_A, A, sizeof(float) * WIDTH * WIDTH, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, sizeof(float) * WIDTH * WIDTH, cudaMemcpyHostToDevice);

	dim3 grid(WIDTH / BLOCK, WIDTH / BLOCK);
	dim3 block(BLOCK, BLOCK);
	d_multiply0 << < grid, block >> > (A, B, C);
	cudaMemcpy(C, d_C, sizeof(float) * WIDTH * WIDTH, cudaMemcpyDeviceToHost);
	clock_t end = clock();
	cout << "CUDA計算時間:" << (double)(end - start) << endl;
}
float ha_A[WIDTH * WIDTH];
float ha_B[WIDTH * WIDTH];
float ha_C[WIDTH * WIDTH];
void d_process() {



	unsigned int i;

	cudaMalloc((void**)&d_A, sizeof(float) * WIDTH * WIDTH);
	cudaMalloc((void**)&d_B, sizeof(float) * WIDTH * WIDTH);
	cudaMalloc((void**)&d_C, sizeof(float) * WIDTH * WIDTH);

	for (i = 0; i < WIDTH * WIDTH; i++) {
		ha_A[i] = (float)i;
		ha_B[i] = (float)i;
	}
	d_multiply(ha_A, ha_B, ha_C);
}