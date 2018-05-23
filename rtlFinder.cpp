//
// Created by peng on 18-5-9.
//

#include "rtlFinder.h"

using namespace cv;
using namespace std;

Mat RtlFinder::getThreshold(Mat tinput) {
    Mat toutput;
    Mat Gray;
    Mat binary;
    Mat opened;
    Mat closed;
    GaussianBlur(tinput, Gray, Size(5, 5), 2, 2);
    cvtColor(Gray, binary, CV_BGR2GRAY);
    //GaussianBlur(Gray, Gray, Size(3, 3), 2, 2)

    //透视变换
    float rotate[9] = {1.56581223e+00, 8.83653523e+00, -1.38263906e+03,
                       -2.75104595e+00, 5.91734853e+00, 5.58120106e+02,
                       1.88488768e-04, 1.00633412e-02, 1.00000000e+00};
    Mat transform = Mat(3, 3, CV_32FC1, rotate);
    Mat perspectiveImage;
    warpPerspective(binary, perspectiveImage, transform, Size(binary.cols * 3, binary.rows * 3),
                    INTER_LINEAR | WARP_INVERSE_MAP);
    //截取能处理部分

    Mat small = perspectiveImage(Rect(200, 200, 400, 400));
    Scalar tempVal = cv::mean(small);
    float matMean = tempVal.val[0];
    threshold(small, closed, matMean + 15, 255, CV_THRESH_BINARY);//179
    const int elesize = 15;
    Mat element = getStructuringElement(MORPH_RECT, Size(elesize, elesize));
    morphologyEx(closed, opened, MORPH_CLOSE, element);
    morphologyEx(opened, toutput, MORPH_OPEN, element);
    return toutput;

    //Mat toutput;
    //Mat gray;
    //cvtColor(tinput, gray, CV_BGR2GRAY);
    //GaussianBlur(gray, gray, Size(5, 5), 2, 2);
    //
    //const int elesize = 25;
    //Mat element = getStructuringElement(MORPH_RECT, Size(elesize, elesize));
    //morphologyEx(gray, gray, MORPH_CLOSE, element);
    //morphologyEx(gray, gray, MORPH_OPEN, element);
    //cv:Scalar tempVal = cv::mean(gray);
    //float matMean = tempVal.val[0];
    //threshold(gray, toutput, matMean + 20, 255, CV_THRESH_BINARY);//179
    //return toutput;
}

bool RtlFinder::IsBoundary(Mat img, Point P) {
    int sum = 0;
    if (P.x <= 0 || P.x >= (img.cols - 1) || P.y <= 0 || P.y >= (img.rows - 1))
        return false;
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            sum += img.at<uchar>(P.y + i, P.x + j);//越界
    if (sum > 255 && sum < 255 * 7)
        return true;
    else
        return false;
}

Point RtlFinder::FindFirstMidPointBottom(Mat img) {
    int row = img.rows;
    int col = img.cols;
    int left = 0, right = 0;
    int i = 0;
    Point FirstPoint;
    for (i = 1; i < col; i++)
        if (img.at<uchar>(row - 1, i) == img.at<uchar>(row - 1, i - 1))
            continue;
        else {
            left = i;
            break;
        }
    for (i = col - 2; i > 0; i--)
        if (img.at<uchar>(row - 1, i) == img.at<uchar>(row - 1, i + 1))
            continue;
        else {
            right = i;
            break;
        }
    if (left == 0 || right == 0) {
        FirstPoint.x = 0;
        FirstPoint.y = 0;
    } else {
        FirstPoint.x = int(round((left + right) / 2));
        FirstPoint.y = row - 1;
    }
    return FirstPoint;
}

Point RtlFinder::FindFirstMidPointLeft(Mat img) {
    int row = img.rows;
    int col = img.cols;
    int left = 0, right = 0;
    int i = 0;
    Point FirstPoint;
    for (i = 1; i < row; i++)
        if (img.at<uchar>(i, col - 1) == img.at<uchar>(i - 1, col - 1))
            continue;
        else {
            left = i;
            break;
        }
    for (i = row - 2; i > 0; i--)
        if (img.at<uchar>(i, col - 1) == img.at<uchar>(i + 1, col - 1))
            continue;
        else {
            right = i;
            break;
        }
    if (left == 0 || right == 0) {
        FirstPoint.x = 0;
        FirstPoint.y = 0;
    } else {
        FirstPoint.y = int(round((left + right) / 2));
        FirstPoint.x = col - 1;
    }
    return FirstPoint;
}

int RtlFinder::FindNextMiddlePointBottom(Mat Img, Point *NextPoint, Point NowPoint, int intervel) {
    (*NextPoint).y = NowPoint.y - intervel;
    (*NextPoint).x = NowPoint.x;
    int NextMidPointL = 0, NextMidPointR = 0;
    int i = 0;
    for (i = 0; i < 60; i++) {
        (*NextPoint).x = NowPoint.x - i;
        if (IsBoundary(Img, *NextPoint) == true) {
            NextMidPointL = NowPoint.x - i;
            break;
        } else
            NextMidPointL = 0;
    }
    for (i = 0; i < 40; i++) {
        (*NextPoint).x = NowPoint.x + i;
        if (IsBoundary(Img, *NextPoint) == true) {
            NextMidPointR = NowPoint.x + i;
            break;
        } else
            NextMidPointR = 0;
    }
    (*NextPoint).x = int(round((NextMidPointL + NextMidPointR) / 2));
    if (NextMidPointL == 0 || NextMidPointR == 0)
        return 0;
    else
        return 1;
}

int RtlFinder::FindNextMiddlePointLeft(Mat Img, Point *NextPoint, Point NowPoint, int intervel) {
    (*NextPoint).x = NowPoint.x - intervel;
    (*NextPoint).y = NowPoint.y;
    int NextMidPointL = 0, NextMidPointR = 0;
    int i = 0;
    for (i = 0; i < 60; i++) {
        (*NextPoint).y = NowPoint.y - i;
        if (IsBoundary(Img, *NextPoint) == true) {
            NextMidPointL = NowPoint.y - i;
            break;
        } else
            NextMidPointL = 0;
    }
    for (i = 0; i < 40; i++) {
        (*NextPoint).y = NowPoint.y + i;
        if (IsBoundary(Img, *NextPoint) == true) {
            NextMidPointR = NowPoint.y + i;
            break;
        } else
            NextMidPointR = 0;
    }
    (*NextPoint).y = int(round((NextMidPointL + NextMidPointR) / 2));
    if (NextMidPointL == 0 || NextMidPointR == 0)
        return 0;
    else
        return 1;
}

vector<Point> RtlFinder::FindAllMidPointsBottom(Mat Img) {
    vector<Point> AllMidPoint;
    Point NowMidPoint;
    Point NextMidPoint = FindFirstMidPointBottom(Img);
    int flag = 1;
    if (NextMidPoint.x == 0 && NextMidPoint.y == 0) {
        flag = 0;
    }
    while (flag) {
        NowMidPoint = NextMidPoint;
        AllMidPoint.push_back(NowMidPoint);
        flag = FindNextMiddlePointBottom(Img, &NextMidPoint, NowMidPoint, 5);
    }
    return AllMidPoint;
}

vector<Point> RtlFinder::FindAllMidPointsLeft(Mat Img) {
    vector<Point> AllMidPoint;
    Point NowMidPoint;
    Point NextMidPoint = FindFirstMidPointLeft(Img);
    int flag = 1;
    if (NextMidPoint.x == 0 && NextMidPoint.y == 0) {
        flag = 0;
    }
    while (flag) {
        NowMidPoint = NextMidPoint;
        AllMidPoint.push_back(NowMidPoint);
        flag = FindNextMiddlePointLeft(Img, &NextMidPoint, NowMidPoint, 5);
    }
    return AllMidPoint;
}

bool RtlFinder::polynomial_curve_fit(std::vector<cv::Point> &key_point, int n, cv::Mat &A) {
    //Number of key points
    int N = key_point.size();

    //构造矩阵X
    cv::Mat X = cv::Mat::zeros(n + 1, n + 1, CV_64FC1);
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            for (int k = 0; k < N; k++) {
                X.at<double>(i, j) = X.at<double>(i, j) +
                                     std::pow(key_point[k].x, i + j);
            }
        }
    }

    //构造矩阵Y
    cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    for (int i = 0; i < n + 1; i++) {
        for (int k = 0; k < N; k++) {
            Y.at<double>(i, 0) = Y.at<double>(i, 0) +
                                 std::pow(key_point[k].x, i) * key_point[k].y;
        }
    }

    A = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    //求解矩阵A
    cv::solve(X, Y, A, cv::DECOMP_LU);
    return true;
}

int RtlFinder::operator()(RtlInfo &info) {
    VideoCapture capture(0);
    //capture.open("123.avi");
    if (!capture.isOpened())
        return 1;
    Mat frame;
    while (1) {
        capture >> frame;
        if (frame.empty())//若视频播放完成，退出循环
        {
            break;
        }
        //imshow("frame", frame);
        Mat small = getThreshold(frame);

        //imshow("small", small);
        //waitKey(10);
        vector<Point> AllMidPointsB = FindAllMidPointsBottom(small);//找到所有底边白色带上的中点
        vector<Point> AllMidPointsL = FindAllMidPointsLeft(small);//找到所有右边白色带上的中点
        int num1 = AllMidPointsB.size();//如果中点数超过5个认为可以进行直线拟合
        int num2 = AllMidPointsL.size();//如果中点数超过5个认为可以进行直线拟合

        double theta2 = 0;
        double error2 = 0;
        double dy = 0;
        double theta1 = 0;
        double error1 = 0;
        double dx = 0;
        unsigned char valueFlag = 0;

        if (num2 > 5) {
            cv::Mat B;
            polynomial_curve_fit(AllMidPointsL, 1, B);//拟合直线得到 k， b
            theta2 = atan(B.at<double>(1, 0)) / M_PI * 180;
            error2 = B.at<double>(0, 0);
            dy = B.at<double>(1, 0) * 399 + B.at<double>(0, 0);
            cout << "x方向 角度：" << theta2 << "    距离：" << dy << endl;
            valueFlag += 1;
        }
        if (num1 > 5) {
            cv::Mat A;
            polynomial_curve_fit(AllMidPointsB, 1, A);//拟合直线得到 k， b
            theta1 = atan(A.at<double>(1, 0)) / M_PI * 180;
            error1 = A.at<double>(0, 0);
            dx = (399 - A.at<double>(0, 0)) / A.at<double>(1, 0);
            cout << "y方向 角度：" << theta1 << "    距离：" << dx << endl;
            valueFlag += 2;
        }

        if (valueFlag == 0) {
            //cout << " 未出现正确图像" << endl;
            continue;
        } else
            info.set(theta2, dy, theta1, dx, valueFlag);
    }
    //destroyAllWindows();
    return 0;
}