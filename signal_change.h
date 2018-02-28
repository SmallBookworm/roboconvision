//
// Created by peng on 18-1-28.
//

#ifndef ROBOCONVISION_SIGNAL_CHANGE_H
#define ROBOCONVISION_SIGNAL_CHANGE_H

#define SRED 0
#define SYELLOW 1
#define SBLUE 2
#define SORANGE 3

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

class SignalWatcher {
private:
    cv::VideoCapture capture;
    int color = 0;
public:
    int getColor() const;

    void setColor(int color);

public:
    int iLowH1 = 165;
    int iHighH1 = 175;

    int iLowH2 = 30;
    int iHighH2 = 36;

    int iLowH3 = 93;
    int iHighH3 = 94;

    int iLowH4 = 11;
    int iHighH4 = 16;

    int iLowS1 = 0;
    int iHighS1 = 255;

    int iLowV1 = 60;
    int iHighV1 = 255;

    explicit SignalWatcher(const cv::VideoCapture &capture) {
        this->capture = capture;
    };
public:

    int watch();
};

#endif //ROBOCONVISION_SIGNAL_CHANGE_H