//
// Created by peng on 18-1-28.
//

#ifndef ROBOCONVISION_LINE_TRACK_H
#define ROBOCONVISION_LINE_TRACK_H

#define PI 3.14159265358979323846
#define POINT_X 0
#define POINT_Y 0
#define THETA_A 0
#define THETA_B 90

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/ml/ml.hpp"
#include <opencv2/videoio.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class LineFinder {
private:
    cv::Mat img; // original image
    std::vector<cv::Vec4i> lines;
    double deltaRho;
    double deltaTheta;
    int minVote;

    double minLength; // min length for a line
    double maxGap; // max allowed gap along the line
public:
    // Default accumulator resolution is 1 pixel by 1 degree
    // no gap, no mimimum length
    LineFinder() : deltaRho(1),
                   deltaTheta(PI / 180),
                   minVote(10),
                   minLength(0.),
                   maxGap(0.) {}

    // Set the resolution of the accumulator
    void setAccResolution(double dRho, double dTheta);

    // Set the minimum number of votes
    void setMinVote(int minv);

    // Set line length and gap
    void setLineLengthAndGap(double length, double gap);

    // Apply probabilistic Hough Transform
    std::vector<cv::Vec4i> findLines(cv::Mat &binary);

    // Draw the detected lines on an image
    void drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255));
};

class LineTracker {
private:
    cv::VideoCapture capture;
    LineFinder finder;
public:
    explicit LineTracker(cv::VideoCapture capture) {
        this->capture = capture;
        // Set probabilistic Hough parameters
        this->finder.setLineLengthAndGap(100, 20);
        this->finder.setMinVote(10);
    }

public:
    bool my_cmp(cv::Vec4f a, cv::Vec4f b);

    float CauculateAngle(int a, int b);

    cv::Vec2i GetPosition(std::vector<cv::Vec4f> lines);

    cv::Point2f watch();
};

#endif //ROBOCONVISION_LINE_TRACK_H
