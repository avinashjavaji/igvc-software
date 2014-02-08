#include "cameraadaptertester.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>

using namespace cv;

CameraAdapterTester::CameraAdapterTester()
{
    _iothread = boost::thread(boost::bind(&CameraAdapterTester::threadRun, this));
}

void CameraAdapterTester::threadRun()
{
    cv::VideoCapture cap0("/media/StorageDrive/Downloads/drop.avi");
    cv::VideoCapture cap1("/media/StorageDrive/Downloads/bird.avi");
    cv::VideoCapture cap2("/media/StorageDrive/Downloads/flame.avi");
    cv::VideoCapture cap3("/media/StorageDrive/Downloads/cbw3.avi");

    while(1){
        if(cap0.isOpened()){
            cap0 >> data.leftFeed;

        }
        if(cap1.isOpened()){
            cap1 >> data.rightFeed;
        }
        if(cap2.isOpened()){
            cap2 >> data.depthMap;
        }
        if(cap3.isOpened()){
            cap3 >> data.pointCloud;
        }    

        onNewData(data);
        cv::waitKey(30);
    }
}
