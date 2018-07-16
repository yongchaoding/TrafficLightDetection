#ifndef PYLON_CAMERA_HPP
#define PYLON_CAMERA_HPP
#include <iostream>
#include <string.h>
#include <pylon/PylonIncludes.h>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv2.hpp>
#include <opencv2/core.hpp>

using namespace Pylon;
using namespace cv;
using namespace std;

class pyloncamera{
public:
	pyloncamera();
	~pyloncamera();
	int cameraInit();
	Mat getCameraImg();
	int saveCameraImg();
private:
	PylonAutoInitTerm autoInitTerm;
	CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
	CImageFormatConverter formatConverter;
	CPylonImage pylonImage;
	VideoWriter cvVideoCreator;
	int grabbedlmages = 0;
	string videoFileName;
	static const uint32_t c_countOfImagesToGrab = 10;
	CGrabResultPtr ptrGrabResult;
}

#endif
