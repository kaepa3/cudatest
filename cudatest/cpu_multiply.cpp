#include<stdio.h>
#include <time.h> 
#include <iostream>
using namespace std;

#define WIDTH 1024
float h_A[WIDTH * WIDTH];
float h_B[WIDTH * WIDTH];
float h_C[WIDTH * WIDTH];

void h_multiply(float* A, float* B, float* C)
{
	unsigned int r, c, i;
	float tmp;
	for (r = 0; r < WIDTH; r++) {
		for (c = 0; c < WIDTH; c++) {
			tmp = 0.0;
			for (i = 0; i < WIDTH; i++) {
				tmp += A[WIDTH * r + i] * B[WIDTH * i + c];
			}
			C[WIDTH * r + c] = tmp;
		}
	}
}

float* h_process() {
	unsigned int i;
	for (i = 0; i < (WIDTH * WIDTH); i++) {
		h_A[i] = (float)i;
		h_B[i] = (float)i;
	}
	clock_t start = clock();
	h_multiply(h_A, h_B, h_C);
	clock_t end = clock();
	cout << "ホスト計算結果:" << (double)(end - start) << endl;
	return &h_C[0];
}


