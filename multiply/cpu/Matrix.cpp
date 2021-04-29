#include <stdio.h>

#define WIDTH 1024

float h_A[WIDTH * WIDTH];
float h_B[WIDTH * WIDTH];
float h_C[WIDTH * WIDTH];

void h_multiply(float *A, float *B, float *C);

int main() {
  unsigned int i;

  for (i = 0; i < (WIDTH * WIDTH); i++) {
    h_A[i] = (float)i;
    h_B[i] = (float)i;
  }

  h_multiply(h_A, h_B, h_C);
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
