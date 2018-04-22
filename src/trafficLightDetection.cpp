#include "FrameLoad.hpp"
#include "trafficLight.hpp"
#include "time.h"
#include "stdlib.h"
#include <string.h>
#include <fstream>

using namespace std;
using namespace cv;

#define DEBUG_CAMERA 0
#define DEBUG_VIDEO 0
#define DEBUG_IMAGE 0
#define DEBUG_IMAGES 1

int main(int argc, char** argv){
	FrameLoad frameLoad;
	TrafficLight trafficlight;

	if(argc != 2){
		printf("Please input the video to detect!\n");
		exit(0);
	}

	fstream fi("../frame.txt", ios::in|ios::out);
	if(!fi){
		perror("Can not open file");
		exit(1);
	}
	int startID = 0;
	fi >> startID;
	printf("startID is %d\n", startID);
	fi.close();

	fstream fo("../frame.txt", ios::in|ios::out|ios::trunc);
	char videoName[128];
	strcpy(videoName, argv[1]);

	if(DEBUG_CAMERA == 1){
		frameLoad.CameraLoad(0);
	}
	else if(DEBUG_VIDEO == 1){
		frameLoad.videoLoad(string(videoName));
	}
	else if(DEBUG_IMAGE == 1){
		frameLoad.singleImageLoad("../../ImgData/1.png");
	}
	else if(DEBUG_IMAGES == 1){
		frameLoad.multiImageLoad(string(videoName));
	}

	long nFrame = frameLoad.getFrameCount();
	Mat frame;
	Mat Image;
	Mat redImage;
	Mat greenImage;
	Mat lightImage;

	int frameNum = 0;
	clock_t start, end;
	while(nFrame){
		frameNum ++;
		start = clock();
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

		//trafficlight.LightDetection(Image, redImage, greenImage);
		trafficlight.LightDetection(Image, lightImage);
		
		trafficlight.ClosingOperation(lightImage);
		vector<Point> pointLight;
		pointLight = trafficlight.LightBoundingBox(lightImage);
		trafficlight.BoundingBoxShow(Image, pointLight);
		trafficlight.LightExtract(Image, pointLight, &startID);

		end = clock();
		float TimeSpend = (double)(end - start) / CLOCKS_PER_SEC;
		float frequency = 1/TimeSpend;
		printf("FrameNum: %d, HZ is %f\n", frameNum, frequency); 
		//frameLoad.showFrame("redImage", redImage);
		//frameLoad.showFrame("greenImage", greenImage);
		//frameLoad.showFrame("lightImage", lightImage);
			
		//frameLoad.show(20);
	}
	fo << startID;
	fo.close();
}
