#include<stdio.h>

#include<cuda_runtime.h>
#include "device_launch_parameters.h"
#include <ctime>
#include <iostream>

using namespace std;

#define BLOCK 16
#define WIDTH 1024


float* d_A, * d_B, * d_C;

__global__ void d_multiply0(float* A, float* B, float* C) {
	unsigned int r = blockDim.y * blockIdx.y + threadIdx.y;
	unsigned int c = blockDim.x * blockIdx.x + threadIdx.x;
	unsigned int i;
	float tmp;
	tmp = 0.0f;
	for (i = 0; i < WIDTH; i++) {
		tmp += A[WIDTH * r + i] * B[WIDTH * i + c];
	}
	C[WIDTH * r + c] = tmp;

}
float ha_A[WIDTH * WIDTH];
float ha_B[WIDTH * WIDTH];
float ha_C[WIDTH * WIDTH];
void d_multiply() {

	clock_t start = clock();
	size_t size = sizeof(float) * WIDTH * WIDTH;
	cudaMemcpy(d_A, ha_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, ha_B, size, cudaMemcpyHostToDevice);

	dim3 grid(WIDTH / BLOCK, WIDTH / BLOCK);
	dim3 block(BLOCK, BLOCK);
	d_multiply0 << < grid, block >> > (d_A, d_B, d_C);
	cudaMemcpy(ha_C, d_C, size, cudaMemcpyDeviceToHost);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	clock_t end = clock();
	cout << "CUDA計算時間:" << (double)(end - start) << endl;
}

float* d_process() {



	unsigned int i;

	cudaMalloc((void**)&d_A, sizeof(float) * WIDTH * WIDTH);
	cudaMalloc((void**)&d_B, sizeof(float) * WIDTH * WIDTH);
	cudaMalloc((void**)&d_C, sizeof(float) * WIDTH * WIDTH);

	for (i = 0; i < WIDTH * WIDTH; i++) {
		ha_A[i] = (float)i;
		ha_B[i] = (float)i;
	}
	d_multiply();
	return &ha_C[0];
}