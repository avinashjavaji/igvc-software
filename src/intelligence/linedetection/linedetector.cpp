#include "linedetector.h"
#include <common/logger/logger.h>
#include <common/config/configmanager.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <common/utils/ImageUtils.h>


using namespace std;
using namespace cv;

LineDetector::LineDetector()
    : max_elem(2),
      max_kernel_size(2),
      gaussian_size(7)
{
    erosion_elem = 2;
    erosion_size = 3;
    dilation_elem = 2;
    dilation_size = 2;
}

void LineDetector::onImageEvent(ImageData imgd){
    src = imgd.mat();
    dst = src.clone();
    cv::resize(dst, dst, cv::Size(512, 384));
    /** Total Average of the pixels in the screen. Used to account for brightness variability. */
    float totalAvg = getAvg();

    /** Blurs the picture just a little */
    GaussianBlur(dst, dst, Size(gaussian_size,gaussian_size),2,0);
    /** Separates the pixels into black(not lines) and white (lines) */
    blackAndWhite(totalAvg);

    Erosion();
    Dilation();
    cv::Mat transformDst(dst.rows, dst.cols, CV_8UC3);
    transformPoints(dst, transformDst);
    cloud = toPointCloud(transformDst);

   // onNewLines(ImageData(transformDst));
    onNewLinesMat(dst);
    //cout <<"Sending new matrix"<<endl;
    pcl::PointXY offset;
    offset.x = ConfigManager::Instance().getValue("Camera", "OffsetX", 0.0f);
    offset.y = ConfigManager::Instance().getValue("Camera", "OffsetY", 0.0f);

//    int timeElapsed = t.msecsTo(QDateTime::currentDateTime().time());
//    cout << "Time elapsed: " << timeElapsed <<endl;

    onNewCloud(cloud.makeShared(), offset);


//    timeElapsed = t.msecsTo(QDateTime::currentDateTime().time());
//    cout << "Time elapsed: " << timeElapsed <<endl;
}

void LineDetector::Erosion()
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
  // Apply the erosion operation
  erode( dst, dst, element );
}

/** Dilation enhances the white lines */
void LineDetector::Dilation()
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  // Apply the dilation operation
  dilate( dst, dst, element );

}

/**
 *  @brief LineDetector::blackAndWhite converts the image into
 *         black (not lines) and white (lines)
 *  @param totalAvg The average brightness of the picture
 */
void LineDetector::blackAndWhite(float totalAvg){
    Vec3b p;
    int rows = dst.rows;
    int cols = dst.cols;
    uchar* myRow;
    //Turn the top quarter of the screen and bottom sixth of the screen black
    //We can disregard these areas - may extend the bottom of the screen slightly later on
    for (int i = 0; i< rows*4/9; i++){
        myRow = dst.ptr<uchar>(i);
        for(int j=0; j< cols*3; j+=3){
            myRow[j] = 0;
            myRow[j+1]=0;
            myRow[j+2] =0;
        }
    }
    for (int i = rows*5/6; i< rows; i++){
        myRow = dst.ptr<uchar>(i);
        for(int j=0; j< cols*3; j+=3){
             myRow[j] = 0;
             myRow[j+1] =0;
             myRow[j+2]=0;
        }
    }

    //Loops through relevant parts of the image and scans for white lines
    //Also tries to detect obstacles
    int tempAvg;
    float redUp = ConfigManager::Instance().getValue("LineDetector", "RedUp", 1.7);
    float redDown = ConfigManager::Instance().getValue("LineDetector", "RedDown", 1);
    float greenUp = ConfigManager::Instance().getValue("LineDetector", "GreenUp", 1.7);
    float greenDown = ConfigManager::Instance().getValue("LineDetector", "GreenDown", .8);
    float blueUp = ConfigManager::Instance().getValue("LineDetector", "BlueUp", 1.7);
    float blueDown = ConfigManager::Instance().getValue("LineDetector", "BlueDown", 1.1);
    int diff = ConfigManager::Instance().getValue("LineDetector", "diff", 5);
    for (int i = rows*4/9; i< rows*5/6; i++){
        myRow = dst.ptr<uchar>(i);
        for(int j=0; j< cols*3; j+=3){
            tempAvg = totalAvg*(1.1 - i*.1/768);
           // p = dst.at<Vec3b>(i, j); //Current pixel
            uchar blue = myRow[j];
            uchar green = myRow[j+1];
            uchar red = myRow[j+2];

            //If there is a significant amount of red in the pixel, it's most likely an orange cone
            //Get rid of the obstacle
            if (red > totalAvg*2|| red > 253){
                detectObstacle(i, j);
            }

            //Filters out the white and makes it pure white
            if((blue>tempAvg*blueDown)&& (blue < tempAvg*blueUp)&& (green < tempAvg*greenUp)&&(red>tempAvg*redDown) &&
                    (red<tempAvg*redUp)&&(green>tempAvg*greenDown)&&(abs(green - red) <tempAvg/diff)){
                myRow[j] = 255;
                myRow[j+1] = 255;
                myRow[j+2] = 255;
            }

            else { //Otherwise, set pixel to black
                myRow[j] =0;
                myRow[j+1]=0;
                myRow[j+2]=0;
            }
        }
    }
}

/**
 *  \brief LineDetector::detectObstacle detects orange and bright white obstacles
 *  \param col the column of the left of the obstacle
 */
void LineDetector::detectObstacle(int row, int col){
    uchar* myRow = dst.ptr<uchar> (row);
    uchar red = myRow[col+2];

    int row2 = row;
    int col2 = col;
    row++;
    //While the pixel is still orange, turn it black
    //Then on to the next one, by row
    while (red>100){
        myRow[col] =0;
        myRow[col+1]=0;
        myRow[col+2]=0;
        myRow = dst.ptr<uchar>(++row2);
        red = myRow[col+2];
    }
    myRow = dst.ptr<uchar>(row);
    red = myRow[col+2];

    //While the pixel is still orange, turn it black
    //Then on to the next one, by column
    while (red>100){
        myRow[col2] =0;
        myRow[col2+1]=0;
        myRow[col2+2]=0;
        col2+=3;
        red = myRow[col2+2];
    }

    //Turn everything in that block we just found black
    for(int i = row+1; i<row2;i++){
        myRow = dst.ptr<uchar>(i);
        for (int j = col+1; j<col2; j+=3){
            myRow[j] =0;
            myRow[j+1]=0;
            myRow[j+2]=0;
        }
    }
}

/**
 *  \brief LineDetector::getAvg gets the average of the relevant pixels
 *  \return the average as a floating point number
 *  \note This is not really averaging... Not entirely sure what this actually does.
 */
float LineDetector::getAvg(){
    Vec3b p;
        float totalAvg = 0;
        for (int i = dst.rows/3; i< 5*dst.rows/6; i++){
            const uchar* myRow = dst.ptr<uchar>(i);
            for(int j=dst.cols/2; j< 5*dst.cols/2; j+=3){
                totalAvg += (myRow[j]+myRow[j+1]+myRow[j+2])/3;
            }
        }
        totalAvg = (25*totalAvg)/(dst.cols*dst.rows*8);
        //std::cout << "Average: "<< totalAvg <<std::endl;
        return totalAvg;
}



