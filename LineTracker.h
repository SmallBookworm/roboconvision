//
// Created by peng on 18-3-14.
//

#ifndef LINETRACKER_H
#define LINETRACKER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "LineFinder.h"

#define PI 3.14159265358979323846
#define mindiffer 15
#define MaxtransLinesgap 130
#define Maxdist 150
#define x_move 500
#define y_move 80
#define paper_height 148.5
#define paper_weight 210
#define standard_picture_x 800
#define standard_picture_y 176
#define standard_real_x -1070   //第一次交接位置
#define standard_real_y 6380    //第一次交接位置
#define standard_angle 0.65

class LineTracker {
private:
    LineFinder finder;
public:
    explicit LineTracker() {
        // Set probabilistic Hough parameters
        this->finder.setLineLengthAndGap(100, 20);
        this->finder.setMinVote(10);
    }

    static bool x_cmp(cv::Vec6f a, cv::Vec6f b) {
        return -1 * (a[2] / a[0]) < -1 * (b[2] / b[0]);
    }

    static bool y_cmp(cv::Vec6f a, cv::Vec6f b) {
        return -1 * (a[2] / a[1]) < -1 * (b[2] / b[1]);
    }

    static bool theta_cmp(cv::Vec6f a, cv::Vec6f b) {
        return a[3] < b[3];
    }

    static bool size_cmp(std::vector<cv::Vec6f> a, std::vector<cv::Vec6f> b) {
        return a.size() < b.size();
    }

    static bool px_cmp(cv::Vec2f a, cv::Vec2f b) {
        return a[0] < b[0];
    }

    float distance(cv::Vec2f pa, cv::Vec2f pb) {
        return sqrt((pa[0] - pb[0]) * (pa[0] - pb[0]) + (pa[1] - pb[1]) * (pa[1] - pb[1]));
    }

    cv::Vec6f averLines(std::vector<cv::Vec6f> oneLine);

    float CalculateAngle(float a, float b);

    void drawLine(cv::Vec6f oLine, cv::Mat &singleLine);

    std::vector<std::vector<cv::Vec6f>> divideAngleLines(std::vector<cv::Vec6f> linesCount, float DValue, float cValue);

    std::vector<std::vector<cv::Vec6f>> divideLines(std::vector<cv::Vec6f> linesCount, int dataNmber, float DValue);

    int watch(cv::Mat &computerImage, cv::Point2f *point);
};


#endif //CROSS_LINETRACKER_H
