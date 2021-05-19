#include <stdio.h>

#include<cuda_runtime.h>
#include "device_launch_parameters.h"
#include "HalconCpp.h"
#include "common_val.h"

using namespace HalconCpp;
using namespace std;


unsigned char* d_src, * d_dst;
double* d_matrix;

__global__ void  d_affine(unsigned char* src, double* matrix, int width, int height, int dstWidth, int dstHeight, unsigned char* dst) {

	unsigned int r = blockDim.y * blockIdx.y + threadIdx.y;
	unsigned int c = blockDim.x * blockIdx.x + threadIdx.x;
	if ((c < dstWidth) && (r < dstHeight)) {

		double refX = matrix[0] * (c - ((double)dstWidth / 2)) +
			matrix[1] * (r - ((double)dstHeight / 2)) +
			((double)width / 2);
		double refY = matrix[3] * (c - ((double)dstWidth / 2)) +
			matrix[4] * (r - ((double)dstHeight / 2)) +
			((double)height / 2);
		int rX = (int)(refX + 0.5);
		int rY = (int)(refY + 0.5);
		unsigned char val = 0;
		if (rX >= 0 && rY >= 0 && rX < width && rY < height) {

			int idxRef = rY * width + rX;
			val = src[idxRef];
		}
		int idx = (r * dstWidth) + c;
		if (idx < dstWidth * dstHeight)
		{
			dst[idx] = val;
		}

	}
	return;
}

#define BLOCKWIDTH 	64
#define BLOCKHEIGHT 16
void gpu_affine() {

	HObject image;
	HalconCpp::ReadImage(&image, READIMAGE);
	HObject imgR, imgG, imgB;
	Decompose3(image, &imgR, &imgG, &imgB);
	HObject gray;
	Rgb3ToGray(imgR, imgG, imgB, &gray);

	HTuple p, tp, w, h;
	GetImagePointer1(gray, &p, &tp, &w, &h);

	HTuple hommat2dIdentity;
	HomMat2dIdentity(&hommat2dIdentity);

	HTuple hommat2dscale;
	HomMat2dScale(hommat2dIdentity, 2, 2, 0, 0, &hommat2dscale);

	HTuple hommat2dinvert;
	HomMat2dInvert(hommat2dscale, &hommat2dinvert);

	HTuple hommat2drotate;
	double rad = (45.0 / 180.0) * PI;
	HomMat2dRotate(hommat2dinvert, rad, 0, 0, &hommat2drotate);

	unsigned char* ptr = (unsigned char*)p.L();
	int length = w.I() * h.I();
	int length_2 = w.I() * 2 * h.I() * 2;
	size_t matrixSize = sizeof(double) * 6;
	unsigned char* buffer = new unsigned char[length_2];

	cudaMalloc((void**)&d_src, length);
	cudaMalloc((void**)&d_dst, length_2);
	cudaMalloc((void**)&d_matrix, matrixSize);

	clock_t  start = clock();

	cudaMemcpy(d_src, ptr, length, cudaMemcpyHostToDevice);
	cudaMemcpy(d_dst, buffer, length_2, cudaMemcpyHostToDevice);
	cudaMemcpy(d_matrix, hommat2drotate.ToDArr(), matrixSize, cudaMemcpyHostToDevice);

	int width = w.I() * 2;
	int height = h.I() * 2;
	
	dim3 block(BLOCKWIDTH, BLOCKHEIGHT);
	dim3 grid((width + BLOCKWIDTH-1)/ BLOCKWIDTH, (height + BLOCKHEIGHT-1) / BLOCKHEIGHT);
	double s = HSystem::CountSeconds(); 
	d_affine << < grid, block >> > (d_src, d_matrix, w.I(), h.I(), w.I() * 2, h.I() * 2, d_dst);
	double e = HSystem::CountSeconds(); 
	cout << (e-s) * 1000 << ":only" << endl;


	cudaMemcpy(buffer, d_dst, length_2, cudaMemcpyDeviceToHost);
	cudaFree(d_src);
	cudaFree(d_dst);
	cudaFree(d_matrix);
	clock_t  end = clock();
	cout << end - start << ":" << CLOCKS_PER_SEC << endl;
	HImage dst;
	GenImage1(&dst, "byte", w.I() * 2, h.I() * 2, (Hlong)buffer);
	WriteImage(dst, "tiff", 0, "gpu_affine");

	//for (int i = 0; i < width; i++)
	//{
	//	cout << "[" << +buffer[i] << "]";
	//}
	//cout << endl;

	delete[] buffer;
}
