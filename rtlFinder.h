//
// Created by peng on 18-5-9.
//

#ifndef ROBOCONVISION_RTLFINDER_H
#define ROBOCONVISION_RTLFINDER_H

#include <opencv2/opencv.hpp>
#include <zconf.h>
#include "rtlInfo.h"

class RtlFinder {
public:
    cv::Mat getThreshold(cv::Mat tinput);

    bool IsBoundary(cv::Mat img, cv::Point P);

    cv::Point FindFirstMidPointBottom(cv::Mat img);

    cv::Point FindFirstMidPointLeft(cv::Mat img);

    int FindNextMiddlePointBottom(cv::Mat Img, cv::Point *NextPoint, cv::Point NowPoint, int intervel);

    int FindNextMiddlePointLeft(cv::Mat Img, cv::Point *NextPoint, cv::Point NowPoint, int intervel);

    std::vector<cv::Point> FindAllMidPointsBottom(cv::Mat Img);

    std::vector<cv::Point> FindAllMidPointsLeft(cv::Mat Img);

    bool polynomial_curve_fit(std::vector<cv::Point> &key_point, int n, cv::Mat &A);

    int operator()(RtlInfo &info);

};


#endif //ROBOCONVISION_RTLFINDER_H
