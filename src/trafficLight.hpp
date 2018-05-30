#ifndef TRAFFICLIGHT_HPP
#define TRAFFICLIGHT_HPP

#include "iostream"
#include "limits.h"
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define WIDTH_DRAW 50
#define HEIGHT_DRAW 50

class TrafficLight{
public:
	TrafficLight();
	~TrafficLight() {}
	void LightDetection(const Mat &originImage, Mat &redImage, Mat &greenImage);
	void LightDetection(const Mat &originImage, Mat &LightImage);
	void ClosingOperation(Mat &LightImage);
	vector<Point> LightBoundingBox(const Mat &LightImage);
	void BoundingBoxShow(const Mat &LightImage, vector<Point> circlePoint);
	vector<string> LightExtract(const Mat &LightImage, vector<Point> circlePoint, int *ID);
    vector<int> DetectionMsgAnlysis(const vector<Point> circlePoint, const vector<int> msg);
    void DetectionLightShow(const Mat &LightImage, const vector<int> msg);
private:
    Mat RedLight, GreenLight, NullLight;
    Mat RedMask, GreenMask, NullMask;

    int LightHeight, LightWidth;
    const int thresh_red_min = 145;
	const int thresh_red_max = 256;
	const int thresh_green_min = 90;
	const int thresh_green_max = 110;

	const float gain_green_a = 0.25;
	const float gain_red_a = 0.45;

	const int thresh_light_min = 80;	// green
	const int thresh_light_max = 190;	// red
};

#endif
