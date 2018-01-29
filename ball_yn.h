//
// Created by peng on 18-1-29.
//

#ifndef ROBOCONVISION_BALL_YN_H
#define ROBOCONVISION_BALL_YN_H

#include <opencv2/opencv.hpp>

class ClipWatcher {
public:
    const int kvalue = 15;//双边滤波邻域大小

    int iLowH1 = 90;
    int iHighH1 = 110;

    int iLowS1 = 70;
    int iHighS1 = 255;

    int iLowV1 = 40;
    int iHighV1 = 255;
public:
    bool watch(cv::Mat srcImage);
};

#endif //ROBOCONVISION_BALL_YN_H
