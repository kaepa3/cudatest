#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {

  string path = "./sample.jpg";
  Mat src, gray;
  src = imread(path);

  cvtColor(src, gray, COLOR_RGB2GRAY);
  Mat dst(gray.size(), gray.type());
  for (int c = 0; c < gray.cols; c++) {
    for (int r = 0; r < gray.rows; r++) {
      unsigned char pt = gray.at<unsigned char>(r, c);
      int idx = r * dst.step + c * dst.elemSize();
      int dstPt = pt - 40;
	  if(dstPt < 0){
		  dstPt =0;
	  }
      dst.data[idx] = (unsigned char)dstPt;
    }
  }
  imshow("gray", gray);
  imshow("dst", dst);

  waitKey(0);
}
