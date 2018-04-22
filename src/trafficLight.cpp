#include "trafficLight.hpp"


void trafficLight.LightDetection(Mat originImage, Mat &redImage, Mat &greenImage){
	Mat imgYCrCb;
	// Convert to YCrCb color space
	cvtColor(img, imgYCrCb, CV_BGR2YCrCb);

	vector<Mat> planes;
	split(imgYCrCb, planes);
	// Traversing to split the color of RED and GREEN according to the Cr component
	MatIterator_<uchar> it_Cr = planes[1].begin<uchar>(), it_Cr_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Red = imgRed.begin<uchar>();
	MatIterator_<uchar> it_Green = imgGreen.begin<uchar>();

	// Parameters of brightness
	float gain_red_b = (1 - gain_red_a) * 125;
	float gain_green_b = (1 - gain_green_a) * 125;

	for (; it_Cr != it_Cr_end; ++it_Cr, ++it_Red, ++it_Green)
	{
		float cr = float(*it_Cr) * gain_red_a + gain_red_b;
		// RED, 145<Cr<470 
		if (cr > thresh_red_min && cr < thresh_red_max)
			*it_Red = 255;
		else
			*it_Red = 0;

		float cg = float(*it_Cr) * gain_green_a + gain_green_b;
		// GREEN£¬95<Cr<110
		if (cg > thresh_green_min && cg < thresh_green_max)
			*it_Green = 255;
		else
			*it_Green = 0;
	}
}


void trafficLight.LightDetection(Mat originImage, Mat &LightImage){
	Mat imgYCrCb;
	// Convert to YCrCb color space
	cvtColor(img, imgYCrCb, CV_BGR2YCrCb);

	vector<Mat> planes;
	split(imgYCrCb, planes);
	// Traversing to split the color of RED and GREEN according to the Cr component
	MatIterator_<uchar> it_Cr = planes[1].begin<uchar>(), it_Cr_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Light = LightImage.begin<uchar>();

	for (; it_Cr != it_Cr_end; ++it_Cr, ++it_Light){
		if( float(*it_Cr) < thresh_light_min || float(*it_Cr) > thresh_light_max){
			*it_Light = 255;
		}
		else{
			*it_Light = 0;
		}
	}
}


void trafficLight.LightBoundingBox(){

}

void trafficLight.LightExtract(){

}