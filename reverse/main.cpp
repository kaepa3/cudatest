#include <cstdio>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

vector<vector<double>> reverse(vector<vector<double>> vec) {
  int size = vec.size();
  // 単位行列作成
  vector<vector<double>> inv_a(size, vector<double>(size,0));
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

int main(int argc, char *argv[]) {

  // vector<double> vec(val, val + sizeof val / sizeof val[0]);
  vector<vector<double>> vec{
      {1, 0, 1},
      {-2, 1, 0},
      {2, -1, 1},
  };
  reverse(vec);
  cout << "elj" << endl;

  return 0;
}
