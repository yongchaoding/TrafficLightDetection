#ifndef FRAMELOAD_HPP
#define FRAMELOAD_HPP

#include "iostream"
#include "limits.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define INF LONG_MAX

enum LoadMode {CAMERA, VIDEO, IMAGE, IMAGES};

class FrameLoad{
public:
	FrameLoad() {}
	~FrameLoad() {}
	void CameraLoad(int CameraDev);
	void videoLoad(std::string VideoFile);
	void singleImageLoad(std::string ImageFile);
	void multiImageLoad(std::string ImagesPath);
	long getFrameCount();
	Mat getFrame();

	void showFrame(std::string WinName, Mat Frame);
	void show(int time);
private:
	LoadMode loadMode;
	VideoCapture capture;
	int ImagesNum;
	std::string ImageFile;
};

#endif
