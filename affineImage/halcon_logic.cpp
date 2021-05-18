#include "HalconCpp.h"

using namespace HalconCpp;
void h_affine() {

	HObject image;
	HalconCpp::ReadImage(&image, "sample.jpg");
	HObject imgR, imgG, imgB;
	Decompose3(image, &imgR, &imgG, &imgB);
	HObject gray;
	Rgb3ToGray(imgR, imgG, imgB, &gray);

	HTuple p, tp, w, h;
	GetImagePointer1(gray, &p, &tp, &w, &h);

	HTuple hommat2dIdentity;
	HomMat2dIdentity(&hommat2dIdentity);
	HTuple hommat2rotate;
	double rad = (45.0 / 180.0) * PI;
	HomMat2dRotate(hommat2dIdentity,rad, h / 2, w / 2, &hommat2rotate);


	HObject affineImage;
	AffineTransImage(gray, &affineImage, hommat2rotate, "nearest_neighbor", "false");
	WriteImage(affineImage, "tiff", 0, "affineImage");


	unsigned char* buffer = new unsigned char[w.I() * h.I()];
	if (buffer != NULL) {

		unsigned char* ptr = (unsigned char*)p.L();
		for (int i = 0; i < w.I() - 1; i++) {
			for (int j = 0; j < h.I() - 1; j++) {
				int idx = (j * w.I() + i);
				unsigned char val = (unsigned char)(ptr[idx] + 10);
				if (val < ptr[idx] + 10) {
					val = 255;
				}
				buffer[idx] = val;
			}
		}
		HImage dst;
		GenImage1(&dst, "byte", w, h, (Hlong)buffer);
		WriteImage(dst, "tiff", 0, "writeTest");
		delete[] buffer;
	}

}