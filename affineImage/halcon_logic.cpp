#include "HalconCpp.h"
#include "common_val.h"

using namespace HalconCpp;
using namespace std;

void h_affine(int a = 0) {

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


	HTuple hommat2rotate;
	double rad = (45.0 / 180.0) * PI;
	HomMat2dRotate(hommat2dIdentity, rad, h / 2, w / 2, &hommat2rotate);

	HTuple hommat2dscale;
	HomMat2dScale(hommat2rotate, 2, 2, 0, 0, &hommat2dscale);


	HObject affineImage;

	clock_t  start = clock();
	double s = HSystem::CountSeconds();
	AffineTransImageSize(gray, &affineImage, hommat2dscale, "nearest_neighbor", w * 2, h * 2);
	double e = HSystem::CountSeconds();
	clock_t  end = clock();
	cout << end - start << ":" << CLOCKS_PER_SEC << ";" << e - s << endl;
	if (a == 0)
	{
		WriteImage(affineImage, "tiff", 0, "affineImage_gpu");
	}
	else {
		WriteImage(affineImage, "tiff", 0, "affineImage");
	}
}

void h_gpu_affine() {
	HTuple deviceIdentifier, info, deviceHandle;
	QueryAvailableComputeDevices(&deviceIdentifier);
	GetComputeDeviceInfo(deviceIdentifier[0], "name", &info);
	OpenComputeDevice(0, &deviceHandle);
	ActivateComputeDevice(deviceHandle);
	InitComputeDevice(deviceHandle, "affine_trans_image");
	InitComputeDevice(deviceHandle, "affine_trans_image_size");
	h_affine(2);
}