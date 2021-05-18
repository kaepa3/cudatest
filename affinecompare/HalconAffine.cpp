#include "HalconCpp.h"
#include <iostream>
#include <Windows.h>

using namespace HalconCpp;
using namespace std;

void h_affine()
{
    char cdir[255];
    GetCurrentDirectory(255, cdir);
    cout << "Current Directory : " << cdir << endl;

    HObject reg;
    GenRectangle1(&reg, 10, 10, 20, 20);
    HImage img;
    ReadImage(&img, "sample");
    //s—ñ‚Ìì¬
    HTuple hommat2d;
    HomMat2dIdentity(&hommat2d);
    HTuple w, h;
    GetImageSize(img, &w, &h);
    HTuple hommat2dRotate;
    HomMat2dRotate(hommat2d, 0.78, h / 2, w / 2, &hommat2dRotate);

    HObject transImg;
    clock_t start_clock, end_clock;
    start_clock = clock();
    AffineTransImage(img, &transImg, hommat2dRotate, "nearest_neighbor", "false");
    end_clock = clock();

    cout << "clock:" << (end_clock - start_clock) / CLOCKS_PER_SEC << " cps:" << CLOCKS_PER_SEC << endl;
    WriteImage(transImg, "tiff", 0, "hAffine");
}
