#ifndef TESTSTEREOPROCESSING_HPP
#define TESTSTEREOPROCESSING_HPP

#include <QtTest>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/gpu/gpu.hpp>
//c
#include <stdio.h>
#include <string.h>

//c++
#include <iostream>
#include <string>
#include <utility>


class TestStereoProcessing: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCase()
    {
        using namespace cv;
        using namespace std;
        //Mat leftImage = imread("/media/StorageDrive/DepthMaptest/depth_test_left.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        //Mat rightImage = imread("/media/StorageDrive/DepthMaptest/depth_test_right.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        Mat leftImage = imread("/media/StorageDrive/DepthMaptest/img_left2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        Mat rightImage = imread("/media/StorageDrive/DepthMaptest/img_right2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if(!leftImage.data){
            QFAIL("Could not open or find the left image");
        }
        if(!rightImage.data){
            QFAIL("Could not open or find the right image");
        }
        Mat disparity;

        StereoBM stereo = StereoBM(CV_STEREO_BM_BASIC, 16, 15);

        stereo.state->SADWindowSize = 15;
        stereo.state->numberOfDisparities = 16;
        stereo.state->preFilterSize = 63;
        stereo.state->preFilterCap = 63;
        stereo.state->minDisparity = 0;
        stereo.state->textureThreshold = 30;
        stereo.state->uniquenessRatio = 3;
        stereo.state->speckleWindowSize = 20;
        stereo.state->speckleRange = 32;

        stereo.operator()(leftImage, rightImage, disparity);

        double min;
        double max;
        minMaxIdx(disparity, &min, &max);

        Mat disparity2;

        disparity.convertTo(disparity2, CV_8U,255/(max-min));

        imshow("Disparity",disparity2);
        //imwrite("/media/StorageDrive/DepthMaptest/disparity.jpg", disparity);
        cvWaitKey();
    }
};

#endif // TESTSTEREOPROCESSING_HPP
