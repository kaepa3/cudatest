#include <iostream>
#include <math.h>
#include <string>
#include<vector>
#include "HalconCpp.h"



using namespace HalconCpp;
using namespace std;


//// reverse matrix
//vector<vector<double>> reverse(vector<vector<double>> vec) {
//    int size = vec.size();
//    // 単位行列作成
//    vector<vector<double>> inv_a(size, vector<double>(size, 0));
//    for (int i = 0; i < size; i++) {
//        for (int j = 0; j < size; j++) {
//            inv_a[i][j] = (i == j) ? 1.0 : 0.0;
//        }
//    }
//
//    //掃き出し法
//    for (int i = 0; i < size; i++) {
//        double buf = 1 / vec[i][i];
//        for (int j = 0; j < size; j++) {
//            vec[i][j] *= buf;
//            inv_a[i][j] *= buf;
//        }
//        for (int j = 0; j < size; j++) {
//            if (i != j) {
//                buf = vec[j][i];
//                for (int k = 0; k < size; k++) {
//                    vec[j][k] -= vec[i][k] * buf;
//                    inv_a[j][k] -= inv_a[i][k] * buf;
//                }
//            }
//        }
//    }
//    return inv_a;
//}
//
////degree to radian 
//double deg2rad(double degree) { return degree * PI / 180.0; }
//
//vector<vector<double>> createMatrix(double angle, int x, int y) {
//    vector<vector<double>> vec{ {cos(angle), -sin(angle), (double)x},
//                               {sin(angle), cos(angle), (double)y},
//                               {0, 0, 1} };
//    return vec;
//}

//// affine trans image
//Mat affine(Mat gray, vector<vector<double>> rm) {
//    Mat dst(gray.size(), gray.type());
//
//    for (int r = 0; r < gray.rows; r++) {
//        for (int c = 0; c < gray.cols; c++) {
//
//            double refX = rm[0][0] * (c - ((double)gray.cols / 2)) +
//                rm[0][1] * (r - ((double)gray.rows / 2)) +
//                ((double)dst.cols / 2);
//            double refY = rm[1][0] * (c - ((double)gray.cols / 2)) +
//                rm[1][1] * (r - ((double)gray.rows / 2)) +
//                ((double)dst.rows / 2);
//            int rX = (int)(refX + 0.5);
//            int rY = (int)(refY + 0.5);
//            int ch = gray.channels();
//            if (ch == 1) {
//                uchar val = 0;
//                if (rX >= 0 && rY >= 0 && rX < gray.cols && rY < gray.rows) {
//                    val = gray.at<uchar>(rY, rX);
//                }
//                int idx = r * gray.step + (c * gray.elemSize());
//                dst.data[idx] = val;
//            }
//            else {
//                Vec3b val{ 0, 0, 0 };
//                if (rX >= 0 && rY >= 0 && rX < gray.cols && rY < gray.rows) {
//                    val = gray.at<Vec3b>(rY, rX);
//                }
//                for (int chIdx = 0; chIdx < gray.channels(); chIdx++) {
//                    int idx = r * gray.step + (c * gray.elemSize()) + chIdx;
//                    dst.data[idx] = val[chIdx];
//                }
//            }
//        }
//    }
//    return dst;
//}

// display mat status
//void status(Mat gray) {
//    cout << "---------------------------" << endl;
//    cout << "type:" << gray.size() << " type:" << gray.type() << endl;
//    cout << "rows:" << gray.rows << " cols:" << gray.cols << endl;
//    cout << "step:" << gray.step << " ele:" << gray.elemSize();
//    cout << " chan:" << gray.channels() << endl;
//}

// affine trans image
vector<byte>  affine(double* matrix, byte* dataArray, int width, int height) {

	vector<byte> dstData(width * height, 0);
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {

			double refX = matrix[0] * (c - ((double)width / 2)) +
				matrix[1] * (r - ((double)height / 2)) +
				((double)width / 2);
			double refY = matrix[3] * (c - ((double)width / 2)) +
				matrix[4] * (r - ((double)height / 2)) +
				((double)width / 2);
			int rX = (int)(refX + 0.5);
			int rY = (int)(refY + 0.5);
			byte val = 0;
			if (rX >= 0 && rY >= 0 && rX < width && rY < height) {

				int idx = rY * width + rX;
				val = dataArray[idx];
			}
			int idx = r * width + c;
			dstData[idx] = val;
		}
	}
	return dstData;
}

const double PIValue = 3.14159265358979323846;
// main Function
void cpu_affine() {

	char cdir[255];
	GetCurrentDirectory(255, cdir);
	cout << "Current Directory : " << cdir << endl;

	HObject reg;
	GenRectangle1(&reg, 10, 10, 20, 20);
	HObject img;
	ReadImage(&img, "sample");
	HObject imgR, imgG, imgB, tmp;
	Decompose3(img, &imgR, &imgG, &imgB);
	Rgb3ToGray(imgR, imgG, imgB, &img);
	//行列の作成
	HTuple hommat2d;
	HomMat2dIdentity(&hommat2d);
	HTuple w, h;
	GetImageSize(img, &w, &h);
	HTuple hommat2dRotate;
	HomMat2dRotate(hommat2d, 90 / 180 * PIValue, h / 2, w / 2, &hommat2dRotate);
	HTuple hommat2dInvert;
	HomMat2dInvert(hommat2dRotate, &hommat2dInvert);
	HTuple  tp, width, height;
	//	GetImagePointer1(img, (Hlong*)&ptr, &tp, &width, &height);
	HImage hImg(img);
	//unsigned char* ptr;
	HTuple ptr = hImg.GetImagePointer1(&tp, &width, &height);
	//unsigned char* pdata = static_cast<unsigned char*>(ptr);
	cout << "tp;" << ptr.TupleTypeElem().ToString()  << endl;
	
	byte a= ((byte*)ptr.PArr())[220];

	double* dptr = hommat2dInvert.ToDArr();
//	affine(dptr, (byte*)ptr, width.I(), height.I());
	cout << hommat2dInvert.Length() << endl;
	for (int i = 0; i < hommat2dInvert.Length(); i++) {
		cout << "tuple:" << dptr[i] << endl;
	}

	cout << tp.S() << endl;
	cout << width.I() << endl;
	cout << height.I() << endl;
}
