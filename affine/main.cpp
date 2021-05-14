#include <cstring>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

void reverseMatrix(vector<vector<double>> vec) {
  int size = vec.size();
  double inv_a[size][size]; //ここに逆行列が入る

  double *a = vec[0].data();
  //単位行列を作る
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      inv_a[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }
  for (int i = 0; i < size; i++) {
    double buf = 1 / a[i][i];
    for (int j = 0; j < size; j++) {
      a[i][j] *= buf;
      inv_a[i][j] *= buf;
    }
    for (int j = 0; j < size; j++) {
      if (i != j) {
        buf = a[j][i];
        for (int k = 0; k < size; k++) {
          a[j][k] -= a[i][k] * buf;
          inv_a[j][k] -= inv_a[i][k] * buf;
        }
      }
    }
  }
}

void createMatrix(vector<vector<double>> &vec, int angle, int x, int y) {
  vec.push_back({cos(angle), -sin(angle), (double)x});
  vec.push_back({sin(angle), cos(angle), (double)y});
  vec.push_back({0, 0, 1});
  return;
}

int main(int argc, char *argv[]) {

  string path = "./sample.jpg";
  Mat src, gray;
  src = imread(path);

  cvtColor(src, gray, COLOR_RGB2GRAY);

  Mat dst(gray.size(), gray.type());
  //
  vector<vector<double>> matrix;
  createMatrix(matrix, 45, 0, 0);
  reverseMatrix(3, matrix);

  for (int c = 0; c < gray.cols; c++) {
    for (int r = 0; r < gray.rows; r++) {
      unsigned char pt = gray.at<unsigned char>(r, c);
      int idx = r * dst.step + c * dst.elemSize();
      int dstPt = pt - 40;
      if (dstPt < 0) {
        dstPt = 0;
      }
      dst.data[idx] = (unsigned char)dstPt;
    }
  }
  imshow("gray", gray);
  imshow("dst", dst);

  waitKey(0);
}
