#include "signal_change.h"

using namespace cv;
using namespace std;


//double alpha = 0.5;
//double beta;
//-----------------------------------【main( )函数】--------------------------------------------
//		失败返回-1,
//-------------------------------------------------------------------------------------------------
int SignalWatcher::watch() {

    //【1】读入视频
//    VideoCapture capture("001.avi");

    //【2】循环显示每一帧
    if (this->capture.isOpened()) {
        Mat srcImage;//定义一个srcImage变量，用于存储每一帧的图像
        capture >> srcImage;  //读取当前帧
        if (srcImage.empty())
            throw "frame is empty\n";

        Mat Image_hsv;//临时变量和目标图的定义
        Mat dstImage(srcImage.size(), srcImage.type());
        dstImage = Scalar::all(0);
        //【2】显示原始图
        imshow("【原始图】", srcImage);

        //【3】转为HSV图并进行图像平滑
        cvtColor(srcImage, Image_hsv, COLOR_BGR2HSV);//转化边缘检测后的图为HSV图
        GaussianBlur(Image_hsv, Image_hsv, Size(3, 3), 2, 2);
        //imshow("【HSV图】", Image_hsv);

        vector<Mat> hsvSplit;
        split(Image_hsv, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);//直方图均值化，即合并一些灰度锐化并增大对比度
        merge(hsvSplit, Image_hsv);
        //imshow("【均值化】1", Image_hsv);
        split(Image_hsv, hsvSplit);


        Mat imgThresholded1;
        Mat imgThresholded2;
        Mat imgThresholded3;
        Mat imgThresholded4;
        Mat combination;
        inRange(Image_hsv, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1),
                imgThresholded1); //Threshold the image
        inRange(Image_hsv, Scalar(iLowH2, iLowS1, iLowV1), Scalar(iHighH2, iHighS1, iHighV1),
                imgThresholded2); //Threshold the image
        inRange(Image_hsv, Scalar(iLowH3, iLowS1, iLowV1), Scalar(iHighH3, iHighS1, iHighV1),
                imgThresholded3); //Threshold the image
        inRange(Image_hsv, Scalar(iLowH4, iLowS1, iLowV1), Scalar(iHighH4, iHighS1, iHighV1),
                imgThresholded4); //Threshold the image
        //addWeighted(combination, 1, imgThresholded3, 1, 0.0, combination); //这里调用了addWeighted函数，得到的结果存储在dst中
        //imshow("【1】", imgThresholded1);
        //imshow("【2】", imgThresholded2);
        //imshow("【3】", imgThresholded3);
        //imshow("【4】", imgThresholded4);
        //Mat bf;//对灰度图像进行双边滤波
        //bilateralFilter(imgThresholded1, bf, kvalue, kvalue * 2, kvalue / 2);
        //imshow("灰度双边滤波处理", bf);

        vector<vector<Point>> contours;
        if (color != 0) {
            findContours(imgThresholded1, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.查找正外接矩形
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {

                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SRED;
                        cout << "红" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;

                    }
                }
            }
        }
        if (color != 1) {
            findContours(imgThresholded2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.查找正外接矩形
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {
                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SYELLOW;
                        cout << "黄" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;

                    }
                }
            }
        }
        if (color != 2) {
            findContours(imgThresholded3, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.查找正外接矩形
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {

                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SBLUE;
                        cout << "蓝" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;
                    }
                }
            }
        }
        if (color != 3) {
            findContours(imgThresholded4, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.查找正外接矩形
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {

                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SORANGE;
                        cout << "橙" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;
                    }
                }
            }
        }
    } else {
        throw "capture is closed\n";
    }
    return -1;
}

int SignalWatcher::getColor() const {
    return color;
}

void SignalWatcher::setColor(int color) {
    SignalWatcher::color = color;
}
