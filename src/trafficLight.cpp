#include "trafficLight.hpp"


void TrafficLight::LightDetection(const Mat &originImage, Mat &redImage, Mat &greenImage){
	Mat imgYCrCb;
	// Convert to YCrCb color space
	cvtColor(originImage, imgYCrCb, CV_BGR2YCrCb);

	vector<Mat> planes;
	split(imgYCrCb, planes);
	// Traversing to split the color of RED and GREEN according to the Cr component
	MatIterator_<uchar> it_Cr = planes[1].begin<uchar>(), it_Cr_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Red = redImage.begin<uchar>();
	MatIterator_<uchar> it_Green = greenImage.begin<uchar>();

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


void TrafficLight::LightDetection(const Mat &originImage, Mat &LightImage){
	Mat imgYCrCb;
	// Convert to YCrCb color space
	cvtColor(originImage, imgYCrCb, CV_BGR2YCrCb);

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


void TrafficLight::ClosingOperation(Mat &LightImage){
	Mat element_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_dilate = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(LightImage, LightImage, element_erode);
	dilate(LightImage, LightImage, element_dilate);
}


vector<Point> TrafficLight::LightBoundingBox(const Mat &LightImage){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	// Finds contours in a binary image. (边缘)
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Point> circlePoint(contours.size());
	vector<vector<Point> > contours_poly(contours.size());
	for (int i = 0; i < contours.size(); i++){
		// Approximates a polygonal curve(s) with the specified precision.
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		// Calculates a contour area.
		Rect rec = boundingRect(Mat(contours_poly[i]));
		circlePoint.x = rec.x + rec.width/2;
		circlePoint.y = rec.y + rec.height/2;
	}
	return circlePoint;
}

void TrafficLight::BoundingBoxShow(const Mat &LightImage, vector<Point> circlePoint){
	Mat ImageShow;
	LightImage.copyTo(ImageShow);
	
	for(int i = 0; i < circlePoint.size(); ++i){
		Point pt1 = Point(circlePoint.x - WIDTH_DRAW/2, circlePoint.y - HEIGHT_DRAW/2);
		Point pt2 = Point(circlePoint.x + WIDTH_DRAW/2, circlePoint.y + HEIGHT_DRAW/2);
		rectangle(ImageShow, pt1, pt2, Scalar(255,255,255), 4); 
	}

	namedWindow("Bounding Box Show", CV_WINDOW_AUTOSIZE);
	imshow("Bounding Box Show", ImageShow);
	waitKey(50);
}

void TrafficLight::LightExtract(){

}
