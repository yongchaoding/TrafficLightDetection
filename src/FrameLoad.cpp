#include "FrameLoad.hpp"

using namespace std;
using namespace cv;

void FrameLoad::CameraLoad(int CameraDev){
	loadMode = CAMERA;
	capture.open(0);
	if(!capture.isOpened()){
		perror("CAMERA open error");
		exit(-1);
	}
}

void FrameLoad::videoLoad(std::string VideoFile){
	loadMode = VIDEO;
	capture.open(VideoFile.c_str());
	printf("VideoFile: %s\n", VideoFile.c_str());
	if(!capture.isOpened()){
		perror("VIDEO open error");
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
		perror("Images open error");
		exit(-1);
	}
}


long FrameLoad::getFrameCount(){
	long nFrame;
	switch (loadMode)
	{
		case CAMERA:
			nFrame = INF;
			break;
		case VIDEO:
			nFrame = static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
			break;
		case IMAGE:
			nFrame = 1;
			break;
		case IMAGES:
			nFrame = static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
			//nFrame = ImagesNum;
			break;
		default:
			break;
	}
	return nFrame;
}


Mat FrameLoad::getFrame(){
	Mat frame;
	if(loadMode == CAMERA){

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
    //resize(frame, frame, Size(), 0.3, 0.3);
	return frame;
}

void FrameLoad::showFrame(std::string WinName, Mat Frame){
	namedWindow(WinName.c_str(), CV_WINDOW_AUTOSIZE);
	imshow(WinName.c_str(), Frame);
}

void FrameLoad::show(int time){
	waitKey(time);
}
