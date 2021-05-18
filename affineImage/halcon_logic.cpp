#include "HalconCpp.h"

using namespace HalconCpp;
using namespace std;

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

	HTuple hommat2dscale;
	HomMat2dScale(hommat2rotate, 2, 2, 0, 0, &hommat2dscale);


	HObject affineImage;
	
	clock_t  start = clock();
	AffineTransImage(gray, &affineImage, hommat2dscale, "nearest_neighbor", "false");
	clock_t  end = clock();
	cout << end - start << ":" << CLOCKS_PER_SEC << endl;
	WriteImage(affineImage, "tiff", 0, "affineImage");



}