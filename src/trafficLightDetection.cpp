#include "FrameLoad.hpp"

using namespace std;
using namespace cv;

#define DEBUG_CAMERA 0
#define DEBUG_VIDEO 1
#define DEBUG_IMAGE 0
#define DEBUG_IMAGES 0

int main(int argc, char** argv){
	FrameLoad frameLoad;
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

	nFrame = getFrameCount();
	Mat frame;
	while(nFrame){
		if(nFrame != INF){
			nFrame --;
		}
		frame = frameLoad.getFrame();
		showFrame("Frame", frame);
		show();
	}
}