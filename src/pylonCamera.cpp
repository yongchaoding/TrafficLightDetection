#include "pyloncamera.hpp"

pyloncamera::pyloncamera(){
	videoFileName = "openCvVideo.avi";

	std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;
	GenApi::INodeMap& nodemap = camera.GetNodeMap();
    //打开相机
    camera.Open();
    //获取相机成像宽度和高度
    GenApi::CIntegerPtr width = nodemap.GetNode("Width");
    GenApi::CIntegerPtr height = nodemap.GetNode("Height");
    //设置相机最大缓冲区,默认为10
    camera.MaxNumBuffer = 5;
    //确定输出像素格式
    formatConverter.OutputPixelFormat = PixelType_BGR8packed;

    cv::Size frameSize = Size((int)width->GetValue(), (int)height->GetValue());
    //设置视频编码类型和帧率，有三种选择
    // 帧率必须小于等于相机成像帧率
    cvVideoCreator.open(videoFileName, CV_FOURCC('D', 'I', 'V','X'), 10, frameSize, true);
    // 开始抓取c_countOfImagesToGrab images.
    //相机默认设置连续抓取模式
    camera.StartGrabbing(c_countOfImagesToGrab, GrabStrategy_LatestImageOnly);

}
pyloncamera::~pyloncamera(){
	
}
int pyloncamera::cameraInit(){
	
}
Mat pyloncamera::getCameraImg(){
	Mat openCvImage;
	if(camera.IsGrabbing()){
		// 等待接收和恢复图像，超时时间设置为5000 ms.
        camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
		if(ptrGrabResult->GrabSucceeded()){
			// 获取图像数据
            cout <<"SizeX: "<<ptrGrabResult->GetWidth()<<endl;
            cout <<"SizeY: "<<ptrGrabResult->GetHeight()<<endl;
            //将抓取的缓冲数据转化成pylon image.
            formatConverter.Convert(pylonImage, ptrGrabResult);
            // 将 pylon image转成OpenCV image.
            openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());
		}
	}
	return openCvImage;
}