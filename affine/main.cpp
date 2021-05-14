#include <cstring>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

const double PI = 3.14159265358979323846;

vector<vector<double>> reverse(vector<vector<double>> vec) {
  int size = vec.size();
  // 単位行列作成
  vector<vector<double>> inv_a(size, vector<double>(size, 0));
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      inv_a[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }

  //掃き出し法
  for (int i = 0; i < size; i++) {
    double buf = 1 / vec[i][i];
    for (int j = 0; j < size; j++) {
      vec[i][j] *= buf;
      inv_a[i][j] *= buf;
    }
    for (int j = 0; j < size; j++) {
      if (i != j) {
        buf = vec[j][i];
        for (int k = 0; k < size; k++) {
          vec[j][k] -= vec[i][k] * buf;
          inv_a[j][k] -= inv_a[i][k] * buf;
        }
      }
    }
  }
  return inv_a;
}

double deg2rad(double degree) { return degree * PI / 180.0; }

vector<vector<double>> createMatrix(double angle, int x, int y) {
  vector<vector<double>> vec{{cos(angle), -sin(angle), (double)x},
                             {sin(angle), cos(angle), (double)y},
                             {0, 0, 1}};
  return vec;
}

int main(int argc, char *argv[]) {
  string path = "./sample.jpg";
  Mat src, gray;
  src = imread(path);

  cvtColor(src, gray, COLOR_RGB2GRAY);
  Mat dst(gray.size(), gray.type());
  //
  vector<vector<double>> matrix = createMatrix(deg2rad(1), 0, 0);
  for (int r = 0; r < matrix.size(); r++) {
    for (int c = 0; c < matrix[0].size(); c++) {
      cout << matrix[r][c] << endl;
    }
  }

  cout << "type:" << gray.size() << " type:" << gray.type() << endl;
  cout << "rows:" << gray.rows << " cols:" << gray.cols << endl;
  cout << "step:" << gray.step << " ele:" << gray.elemSize();
  cout << " chan:" << gray.channels() << endl;

  vector<vector<double>> rm = reverse(matrix);
  for (int r = 0; r < rm.size(); r++) {
    for (int c = 0; c < rm[0].size(); c++) {
      cout << rm[r][c] << ",";
    }
    cout << endl;
  }
  cout << endl;

  for (int r = 0; r < gray.rows; r++) {
    for (int c = 0; c < gray.cols; c++) {
      double refX = rm[0][0] * c + rm[0][1] * r;
      double refY = rm[1][0] * c + rm[1][1] * r;
      int rX = (int)(refX + 0.5);
      int rY = (int)(refY + 0.5);
      unsigned char val = 0;
      if (rX >= 0 && rY >= 0 && rX < gray.cols && rY < gray.rows) {
        val = gray.at<unsigned char>(rY, rX);
      }
      int idx = r * gray.step + c;
      dst.data[idx] = val;
    }
  imshow("dst", dst);

  waitKey(0);
}
