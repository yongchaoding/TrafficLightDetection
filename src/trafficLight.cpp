#include "trafficLight.hpp"

TrafficLight::TrafficLight(){
    RedLight = imread("/home/yongchao/TrafficLightDetection/src/Red.png");
    GreenLight = imread("/home/yongchao/TrafficLightDetection/src/Green.png");
    NullLight = imread("/home/yongchao/TrafficLightDetection/src/NULL.png");


    LightHeight = 50;
    LightWidth = 50;

    resize(RedLight, RedLight, Size(LightHeight,LightWidth));
    resize(GreenLight, GreenLight, Size(LightHeight,LightWidth));
    resize(NullLight, NullLight, Size(LightHeight,LightWidth));

    cvtColor(RedLight, RedMask, CV_BGR2GRAY);
    cvtColor(GreenLight, GreenMask, CV_BGR2GRAY);
    cvtColor(NullLight, NullMask, CV_BGR2GRAY);
    if(0){
        namedWindow("RedLight");
        imshow("RedLight", RedLight);
        namedWindow("GreenLight");
        imshow("GreenLight", GreenLight);
        namedWindow("NullLight");
        imshow("NullLight", NullLight);
        waitKey(100);
    }
}

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
	Mat element_erode = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_dilate = getStructuringElement(MORPH_RECT, Size(20, 20));
	erode(LightImage, LightImage, element_erode);
	dilate(LightImage, LightImage, element_dilate);
}


vector<Point> TrafficLight::LightBoundingBox(const Mat &LightImage){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	// Finds contours in a binary image. (边缘)
	findContours(LightImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Point> circlePoint(contours.size());
	vector<vector<Point> > contours_poly(contours.size());
	for (int i = 0; i < contours.size(); i++){
		// Approximates a polygonal curve(s) with the specified precision.
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		// Calculates a contour area.
		Rect rec = boundingRect(Mat(contours_poly[i]));
		circlePoint[i].x = rec.x + rec.width/2;
		circlePoint[i].y = rec.y + rec.height/2;
	}
	return circlePoint;
}

void TrafficLight::BoundingBoxShow(const Mat &LightImage, vector<Point> circlePoint){
	Mat ImageShow;
	LightImage.copyTo(ImageShow);

	for(int i = 0; i < circlePoint.size(); ++i){
		Point pt1 = Point(circlePoint[i].x - WIDTH_DRAW/2, circlePoint[i].y - HEIGHT_DRAW/2);
		Point pt2 = Point(circlePoint[i].x + WIDTH_DRAW/2, circlePoint[i].y + HEIGHT_DRAW/2);
		rectangle(ImageShow, pt1, pt2, Scalar(255,255,255), 4);
	}

	namedWindow("Bounding Box Show", CV_WINDOW_AUTOSIZE);
	imshow("Bounding Box Show", ImageShow);
	waitKey(50);
}

vector<string> TrafficLight::LightExtract(const Mat &LightImage, vector<Point> circlePoint, int* ID){
	vector<string> ImagePath;
    Mat Image;
	LightImage.copyTo(Image);
	int row = Image.rows;
	int col = Image.cols;
	for(int i = 0; i < circlePoint.size(); ++i){
		if(circlePoint[i].x - WIDTH_DRAW/2 >= 0 && circlePoint[i].y - HEIGHT_DRAW/2 >= 0 && circlePoint[i].x + WIDTH_DRAW/2 < col && circlePoint[i].y + HEIGHT_DRAW/2 < row )
		{
		(*ID) ++;
		//printf("ID:%d, circlePoint[%d].x: %d, circlePoint[%d].y: %d\n",*ID, i, circlePoint[i].x, i, circlePoint[i].y);
		Rect Rec = Rect(circlePoint[i].x - WIDTH_DRAW/2, circlePoint[i].y - HEIGHT_DRAW/2, WIDTH_DRAW, HEIGHT_DRAW);
		Mat Roi = Image(Rec);
		char savepath[128] = "/home/yongchao/TrafficLightDetection/ROI/";
		sprintf(savepath, "%s%08d.jpg", savepath, (*ID));
		//printf("SavePath is %s\n", savepath);
		imwrite(savepath, Roi);
        ImagePath.push_back(savepath);
		}
	}
    return ImagePath;
}

vector<int> TrafficLight::DetectionMsgAnlysis(const vector<Point> circlePoint, const vector<int> msg){

}

void TrafficLight::DetectionLightShow(const Mat &LightImage, const vector<int> msg){
    int msgLength = msg.size();
    Mat Image;
    LightImage.copyTo(Image);
    for(int i=0; i < msgLength; i++){
        if(msg[i] == 0){
            Mat imageROI = Image(Rect(400, 0, LightHeight, LightWidth));
            GreenLight.copyTo(imageROI, GreenMask);
        }
        else if(msg[i] == 1){
            Mat imageROI = Image(Rect(400, 0, LightHeight, LightWidth));
            RedLight.copyTo(imageROI, RedMask);
        }
        else if(msg[i] == 2){
            Mat imageROI = Image(Rect(400, 0, LightHeight, LightWidth));
            GreenLight.copyTo(imageROI, GreenMask);
        }
        else if(msg[i] == 3){
            Mat imageROI = Image(Rect(400, 0, LightHeight, LightWidth));
            RedLight.copyTo(imageROI, RedMask);
        }
        else if(msg[i] == 4){
            Mat imageROI = Image(Rect(0, 0, LightHeight, LightWidth));
            GreenLight.copyTo(imageROI, GreenMask);
        }
        else if(msg[i] == 5){
            Mat imageROI = Image(Rect(0, 0, LightHeight, LightWidth));
            RedLight.copyTo(imageROI, RedMask);
        }
        else if(msg[i] == 7){
            Mat imageROI = Image(Rect(800, 0, LightHeight, LightWidth));
            GreenLight.copyTo(imageROI, GreenMask);
        }
        else if(msg[i] == 8){
            Mat imageROI = Image(Rect(800, 0, LightHeight, LightWidth));
            RedLight.copyTo(imageROI, RedMask);
        }
    }
    namedWindow("Results");
    imshow("Results", Image);
    waitKey(20);

}
