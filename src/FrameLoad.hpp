#ifndef FRAMELOAD_HPP
#define FRAMELOAD_HPP

#include "iostream"
#include "limits.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define INF LONG_MAX

class FrameLoad{
public:
	FrameLoad();
	~FrameLoad() {}
	void CameraLoad(int CameraDev);
	void videoLoad(std::string VideoFile);
	void singleImageLoad(std::string ImageFile);
	void multiImageLoad(std::string ImagesPath);
	long getFrameCount();
	Mat getFrame();

	void showFrame(str::string WinName, Mat Frame);
	void show();
private:
	enum LoadMode{CAMERA, VIDEO, IMAGE, IMAGES};
	VideoCapture capture;
	int ImagesNum;
	std::string ImageFile;
}

#endif