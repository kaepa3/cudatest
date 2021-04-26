#include "CudaProcess.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
using namespace std;

__global__ void kernel(int* pSrc1, int* pSrc2, int* pResult, int length)
{
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	if (idx >= length) {
		return;
	}

	pResult[idx] = pSrc1[idx] + pSrc2[idx];
	return;
}
int main() {
	Process();
}

void Process() {
	int length = 1024;
	size_t size = sizeof(int) * length;

	int* pHostSrc1;
	int* pHostSrc2;
	int* pHostResult;

	int* pDevSrc1;
	int* pDevSrc2;
	int* pDevResult;

	cudaMallocHost(&pHostSrc1, size);
	cudaMallocHost(&pHostSrc2, size);
	cudaMallocHost(&pHostResult, size);

	cudaMalloc(&pDevSrc1, size);
	cudaMalloc(&pDevSrc2, size);
	cudaMalloc(&pDevResult, size);

	//
	pHostSrc1[2] = 3;
	pHostSrc2[2] = 5;

	cudaMemcpy(pDevSrc1, pHostSrc1, size, cudaMemcpyHostToDevice);
	cudaMemcpy(pDevSrc2, pHostSrc2, size, cudaMemcpyHostToDevice);

	dim3 block(128, 1, 1);
	dim3 grid((length + 128 - 1) / 128, 1, 1);
	kernel << <grid, block >> > (pDevSrc1, pDevSrc2, pDevResult, length);
	cudaMemcpy(pHostResult, pDevResult, size, cudaMemcpyDeviceToHost);


	cout << pHostSrc1[2] << ":" << pHostSrc2[2] << ":" << pHostResult[2] << endl;
	cout << "end" << endl;

	cudaFree(pDevSrc1);
	cudaFree(pDevSrc2);
	cudaFree(pDevResult);

	cudaFreeHost(pHostSrc1);
	cudaFreeHost(pHostSrc2);
	cudaFreeHost(pHostResult);

	cudaDeviceReset();


}
