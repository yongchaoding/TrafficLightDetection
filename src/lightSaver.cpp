#include "FrameLoad.hpp"
#include "trafficLight.hpp"
#include "receiver.hpp"

#include "time.h"
#include "stdlib.h"
#include <string.h>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace cv;
using namespace exlcm;
void *lcmHandler(void* arg);

receiver Receiver;
msgImagePath imagePath;
vector<int> imageMsg;

int main(int argc, char** argv){
    FrameLoad frameLoad;
    TrafficLight trafficlight;

    int ret;
    if(argc != 2){
                printf("Please input the video to detect!\n");
                exit(0);
    }

    pthread_t id_lcmhandler;
    ret=pthread_create(&id_lcmhandler,NULL,lcmHandler,(void *)NULL);

    char videoName[128];
    strcpy(videoName, argv[1]);
    frameLoad.videoLoad(string(videoName));
    long nFrame = frameLoad.getFrameCount();

    Mat frame;
    Mat Image;
    Mat lightImage;

    int frameNum = 0;
    int startID = 0;
    clock_t start, end;
    while(nFrame){
        vector<string> ImagePath;
        startID = 0;
        frameNum ++;
        start = clock();
        // Design for web camera
        if(nFrame != INF){
            nFrame --;
        }
        frame = frameLoad.getFrame();
        frame.copyTo(Image);
        lightImage.create(frame.rows, frame.cols, CV_8UC1);

        trafficlight.LightDetection(Image, lightImage);

        trafficlight.ClosingOperation(lightImage);
        vector<Point> pointLight;
        pointLight = trafficlight.LightBoundingBox(lightImage);

        ImagePath = trafficlight.LightExtract(Image, pointLight, &startID);

        int pathNum = ImagePath.size();

        imagePath.timestamp = 0;
        imagePath.ImageNum = pathNum;
        imagePath.ImagePath = ImagePath;

        Receiver.infoPub(imagePath);

        while(!Receiver.flagStatus());

        imageMsg = Receiver.infoGet();
        //imageMsg = Receiver.infoGet();
        //imageMsg = Receiver.infoGet();
        //vector<int> Imagemsg;

       // Imagemsg = imageMsg.ImageMsg;
        //trafficlight.DetectionLightShow(Image, Imagemsg);

        end = clock();
        float TimeSpend = (double)(end - start) / CLOCKS_PER_SEC;
        float frequency = 1/TimeSpend;
        printf("FrameNum: %d, HZ is %f\n", frameNum, frequency);

    }
}


void *lcmHandler(void *arg){
    while(1){
        Receiver.receiverRun();
        usleep(10*1000);
    }
}
