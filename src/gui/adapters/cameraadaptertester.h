#ifndef CAMERAADAPTERTESTER_H
#define CAMERAADAPTERTESTER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "common/events/Event.hpp"
#include <boost/thread.hpp>

class CameraData {
    public:
        cv::Mat leftFeed;
        cv::Mat rightFeed;
        cv::Mat depthMap;
        cv::Mat pointCloud;
};

class CameraAdapterTester
{
public:
    CameraAdapterTester();
    Event<CameraData> onNewData;

private:
    boost::thread _iothread;
    void threadRun();
    CameraData data;
};

#endif // CAMERAADAPTERTESTER_H
