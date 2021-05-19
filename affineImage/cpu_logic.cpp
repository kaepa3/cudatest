#include "HalconCpp.h"
#include "common_val.h"

using namespace HalconCpp;
using namespace std;

void affine(unsigned char* src, double* matrix, int width, int height, int dstWidth, int dstHeight, unsigned char* dst) {

	for (int r = 0; r < dstHeight - 1; r++) {
		for (int c = 0; c < dstWidth - 1; c++) {
			if (r == 1) {
				int hoage = 1;
			}
			double refX = matrix[0] * (c - ((double)dstWidth / 2)) +
				matrix[1] * (r - ((double)dstHeight / 2)) +
				((double)width / 2);
			double refY = matrix[3] * (c - ((double)dstWidth / 2)) +
				matrix[4] * (r - ((double)dstHeight / 2)) +
				((double)height / 2);
			int rX = (int)(refX + 0.5);
			int rY = (int)(refY + 0.5);
			unsigned char val = 0;
			int idxRert = rY * width + rX;
			if (rX >= 0 && rY >= 0 && rX < width && rY < height) {

				int idxRef = rY * width + rX;
				val = src[idxRef];
			}
			else {
				int asdga = 0;
			}
			int idx = r * dstWidth + c;
			dst[idx] = val;
		}
	}
	return;
}

void cpu_affine() {

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
	HomMat2dRotate(hommat2dinvert, rad, h / 2, w / 2, &hommat2drotate);


	unsigned char* ptr = (unsigned char*)p.L();
	int length = w.I() * 2 * h.I() * 2;
	unsigned char* buffer = new unsigned char[length];
	clock_t  start = clock();
	affine(ptr, hommat2drotate.DArr(), w.I(), h.I(), w.I() * 2, h.I() * 2, buffer);
	clock_t  end = clock();
	cout << end - start << ":" << CLOCKS_PER_SEC << endl;
	HImage dst;
	GenImage1(&dst, "byte", w.I() * 2, h.I() * 2, (Hlong)buffer);
	delete[] buffer;
	WriteImage(dst, "tiff", 0, "cpu_affine");

}

