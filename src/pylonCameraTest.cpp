#include "pylonCamera.hpp"

#include "time.h"
#include "stdlib.h"
#include <string.h>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
	cout << "Start Program" << endl;
//	PylonInitialize();
	pyloncamera PylonCamera;
	PylonCamera.cameraInit();
	cout << "init pylon camera finished" << endl;
	Mat Frame;
	namedWindow("Frame");
	while(1){
	//	cout << "Start Get Frame" << endl;
		Frame = PylonCamera.getCameraImg();
		if(!Frame.empty()){
			imshow("Frame", Frame);
			waitKey(50);
		}
		//usleep(50*1000);
	}
}
