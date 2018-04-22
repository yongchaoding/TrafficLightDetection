#include "FrameLoad.hpp"
#include "trafficLight.hpp"
using namespace std;
using namespace cv;

#define DEBUG_CAMERA 0
#define DEBUG_VIDEO 1
#define DEBUG_IMAGE 0
#define DEBUG_IMAGES 0

int main(int argc, char** argv){
	FrameLoad frameLoad;
	TrafficLight trafficlight;

	if(DEBUG_CAMERA == 1){
		frameLoad.CameraLoad(0);
	}
	else if(DEBUG_VIDEO == 1){
		frameLoad.videoLoad("../../VideoData/sample33.avi");
	}
	else if(DEBUG_IMAGE == 1){
		frameLoad.singleImageLoad("../../ImgData/1.png");
	}
	else if(DEBUG_IMAGES == 1){
		frameLoad.multiImageLoad("../../ImgData/%01d.png");
	}

	long nFrame = frameLoad.getFrameCount();
	Mat frame;
	Mat Image;
	Mat redImage;
	Mat greenImage;
	Mat lightImage;
	while(nFrame){
		if(nFrame != INF){
			nFrame --;
		}
		frame = frameLoad.getFrame();
		/*
		frameLoad.showFrame("Frame", frame);
		if(DEBUG_IMAGE){
			frameLoad.show(0);
		}
		else if(DEBUG_VIDEO){
			frameLoad.show(20);
		}
		else if(DEBUG_IMAGES){
			frameLoad.show(200);
		}
		*/
		frame.copyTo(Image);
		redImage.create(frame.rows, frame.cols, CV_8UC1);
		greenImage.create(frame.rows, frame.cols, CV_8UC1);

		lightImage.create(frame.rows, frame.cols, CV_8UC1);

		trafficlight.LightDetection(Image, redImage, greenImage);
		trafficlight.LightDetection(Image, lightImage);
	
		frameLoad.showFrame("redImage", redImage);
		frameLoad.showFrame("greenImage", greenImage);
		frameLoad.showFrame("lightImage", lightImage);
			
		frameLoad.show(20);
	}
}
