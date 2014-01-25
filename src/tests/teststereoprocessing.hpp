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
        Mat leftImage = imread("/media/StorageDrive/DepthMaptest/depth_test_left.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        Mat rightImage = imread("/media/StorageDrive/DepthMaptest/depth_test_right.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if(!leftImage.data){
            QFAIL("Could not open or find the left image");
        }
        if(!rightImage.data){
            QFAIL("Could not open or find the right image");
        }
        /*Mat disparity;

        StereoBM stereo = StereoBM(CV_STEREO_BM_BASIC, 16, 15);
        stereo.operator()(leftImage, rightImage, disparity);

        double min;
        double max;
        cv::minMaxIdx(disparity, &min, &max);
        cv::Mat adjMap;
        // expand your range to 0..255. Similar to histEq();
        map.convertTo(adjMap,CV_8UC1, 255 / (max-min), -min);

        // this is great. It converts your grayscale image into a tone-mapped one,
        // much more pleasing for the eye
        // function is found in contrib module, so include contrib.hpp
        // and link accordingly
        cv::Mat falseColorsMap;
        applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_AUTUMN);

        imshow("left", leftImage);
        imshow("right",rightImage);
        imshow("Disparity",falseColorsMap);
        cvWaitKey();*/
    }
};

#endif // TESTSTEREOPROCESSING_HPP
