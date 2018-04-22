#include "FrameLoad.hpp"

using namespace std;
using namespace cv;

FrameLoad(){
	LoadMode loadMode;
}

void CameraLoad(int CameraDev){
	loadMode = CAMERA;
	capture.open(0);
	if(!capture.isOpened()){
		perror("CAMERA:%d open error", CameraDev);
		exit(-1);
	}
}

void FrameLoad::videoLoad(std::string VideoFile){
	loadMode = VIDEO;
	capture.open(VideoFile.c_str());
	if(!capture.isOpened()){
		perror("%s open error", VideoFile.c_str());
		exit(-1);
	}
}

void FrameLoad::singleImageLoad(std::string ImageFile){
	loadMode = IMAGE;
	this->ImageFile = ImageFile;
}

void FrameLoad::multiImageLoad(std::string ImagesPath){
	loadMode = IMAGES;
	// ImagesPath: img_%02d.jpg
	capture.open(ImagesPath.c_str());
	if(!capture.isOpened()){
		perror("%s open error", VideoFile.c_str());
		exit(-1);
	}
}


long FrameLoad::getFrameCount(){
	long nFrame;
	switch loadMode{
		case CAMERA:
			nFrame = INF;
			break;
		case VIDEO:
			nFrame = long nFrame=static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
			break;
		case IMAGE:
			nFrame = 1;
			break;
		case IMAGES:
			nFrame = long nFrame=static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
			//nFrame = ImagesNum;
			break;
		default:
			break;
	}
	return nFrame;
}


Mat FrameLoad::getFrame(){
	Mat frame;
	if(loadMode == CAMREA){

	}
	else if(loadMode == VIDEO){
		capture >> frame;
	}
	else if(loadMode == IMAGE){
		frame = imread(ImageFile.c_str());
	}
	else if(loadMode == IMAGES){
		capture >> frame;
	}
	else{
		perror("LOADMODE ERROR!\n");
	}
	return frame;
}

void FrameLoad::showFrame(str::string WinName, Mat Frame){
	namedWindow(WinName.c_str(), CV_WINDOW_AUTOSIZE);
	imshow(WinName.c_str(), Frame);
}

void FrameLoad::show(){
	waitKey(10);
}