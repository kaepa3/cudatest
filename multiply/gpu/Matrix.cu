#include <cstdio>
#include <stdio.h>
#include <cuda_runtime.h>
#include <timer.h>

#define BLOCK 16
#define WIDTH 1024

float h_A[WIDTH * WIDTH];
float h_B[WIDTH * WIDTH];
float h_C[WIDTH * WIDTH];

float *d_A, *d_B, *d_C;

void h_multiply(float *A, float *B, float *C);
__global__ void d_multiply(float *A, float *B, float *C);

int main() {
  unsigned int i;

  cudaMalloc((void**)&d_A, sizeof(float) * WIDTH * WIDTH);
  cudaMalloc((void**)&d_B, sizeof(float) * WIDTH * WIDTH);
  cudaMalloc((void**)&d_C, sizeof(float) * WIDTH * WIDTH);

  for (i = 0; i < (WIDTH * WIDTH); i++) {
    h_A[i] = (float)i;
    h_B[i] = (float)i;
  }

  StartTimer();

  cudaMemcpy(d_A, h_A, sizeof(float) * WIDTH * WIDTH), cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, sizeof(float) * WIDTH * WIDTH), cudaMemcpyHostToDevice);

  dim3 grid(WIDTH/BLOCK, WIDTH/BLOCK);
  dim3 block(BLOCK, BLOCK);

  d_multiply0 <<<grid,block >>>(d_A,d_B,d_C);

  cudaMemcpy(h_B, d_B, sizeof(float) * WIDTH * WIDTH), cudaMemcpyDeviceToHost);

  printf("デバイス計算時間: %f(ms)", GetTimer());
  printf("デバイス計算結果: %f\n", h_C[WIDTH*WIDTH-1]);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);



//
  StartTimer();
  h_multiply(h_A, h_B, h_C);
  printf("ホスト計算時間：%f\n", );
  printf("ホスト計算結果：%f\n", h_C[WIDTH * WIDTH - 1]);
}
void h_multiply(float *A, float *B, float *C) {
  unsigned int r, c, i;
  float tmp;
  for (int r = 0; r < WIDTH; r++) {
    for (int c = 0; c < WIDTH; c++) {
      tmp = 0.0;
      for (int i = 0; i < WIDTH; i++) {
        tmp += A[WIDTH * r + i] * B[WIDTH * i + c];
      }
      C[WIDTH * r + c] = tmp;
    }
  }
}

__global__ void d_multiply0(float *A, float *B, float *C){
  unsigned int r= blockDim.y * blockIdx.y + threadIdx.y;
  unsigned int c= blockDim.x * blockIdx.x + threadIdx.x;
  unsigned int i;
  flaot tmp;
  tmp =0.0f;
  for(i=0; i <WIDTH; i++){
    tmp +=A[WIDTH * r + i] * B[WIDTH * i + c];
  }
  C[WIDTH * r + c] = tmp;

}

__global__ void d_multiply1(float *A, float *B, float *C){
  unsigned int r= blockDim.y * blockIdx.y + threadIdx.y;
  unsigned int c= blockDim.x * blockIdx.x + threadIdx.x;
  unsigned int i,j;

  float tmp;
  __shared__ float s_A[BLOCK][BLOCK];
  __shared__ float s_B[BLOCK][BLOCK];
  tmp = 0.0f;

  for (i =0; i< WIDTH, i += BLOCK){
    s_A[thredIdx.y][threadIdx.x] = A[WIDTH * r + i + threadIdx.x];
    s_B[thredIdx.y][threadIdx.x] = B[WIDTH * (i + threadIdx.y) + c ];
    __syncthreads();
    for(j=0; j < BLOCK; j++){
    tmp+= s_A[thredIdx.y][j] * s_B[j][threadIdx.x];
    __syncthreads();
  }
  c[WIDTH * r + c] = tmp;


}