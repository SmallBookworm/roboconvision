#include "signal_change.h"

using namespace cv;
using namespace std;


//double alpha = 0.5;
//double beta;
//-----------------------------------��main( )������--------------------------------------------
//		ʧ�ܷ���-1,
//-------------------------------------------------------------------------------------------------
int SignalWatcher::watch() {

    //��1��������Ƶ
//    VideoCapture capture("001.avi");

    //��2��ѭ����ʾÿһ֡
    if (this->capture.isOpened()) {
        Mat srcImage;//����һ��srcImage���������ڴ洢ÿһ֡��ͼ��
        capture >> srcImage;  //��ȡ��ǰ֡
        if (srcImage.empty())
            throw "frame is empty\n";

        Mat Image_hsv;//��ʱ������Ŀ��ͼ�Ķ���
        Mat dstImage(srcImage.size(), srcImage.type());
        dstImage = Scalar::all(0);
        //��2����ʾԭʼͼ
        imshow("��ԭʼͼ��", srcImage);

        //��3��תΪHSVͼ������ͼ��ƽ��
        cvtColor(srcImage, Image_hsv, COLOR_BGR2HSV);//ת����Ե�����ͼΪHSVͼ
        GaussianBlur(Image_hsv, Image_hsv, Size(3, 3), 2, 2);
        //imshow("��HSVͼ��", Image_hsv);

        vector<Mat> hsvSplit;
        split(Image_hsv, hsvSplit);
        equalizeHist(hsvSplit[2], hsvSplit[2]);//ֱ��ͼ��ֵ�������ϲ�һЩ�Ҷ��񻯲�����Աȶ�
        merge(hsvSplit, Image_hsv);
        //imshow("����ֵ����1", Image_hsv);
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
        //addWeighted(combination, 1, imgThresholded3, 1, 0.0, combination); //���������addWeighted�������õ��Ľ���洢��dst��
        //imshow("��1��", imgThresholded1);
        //imshow("��2��", imgThresholded2);
        //imshow("��3��", imgThresholded3);
        //imshow("��4��", imgThresholded4);
        //Mat bf;//�ԻҶ�ͼ�����˫���˲�
        //bilateralFilter(imgThresholded1, bf, kvalue, kvalue * 2, kvalue / 2);
        //imshow("�Ҷ�˫���˲�����", bf);

        vector<vector<Point>> contours;
        if (color != 0) {
            findContours(imgThresholded1, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.��������Ӿ���
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {

                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SRED;
                        cout << "��" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;

                    }
                }
            }
        }
        if (color != 1) {
            findContours(imgThresholded2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.��������Ӿ���
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {
                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SYELLOW;
                        cout << "��" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;

                    }
                }
            }
        }
        if (color != 2) {
            findContours(imgThresholded3, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.��������Ӿ���
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {

                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SBLUE;
                        cout << "��" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
                             << boundRect[i].height << endl;
                        return color;
                    }
                }
            }
        }
        if (color != 3) {
            findContours(imgThresholded4, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            //5.��������Ӿ���
            vector<Rect> boundRect(contours.size());
            for (int i = 0; i < contours.size(); i++) {

                if (contours[i].capacity() > 100) {
                    boundRect[i] = boundingRect(contours[i]);
                    if (boundRect[i].height > 50 && boundRect[i].width > 50) {
                        color = SORANGE;
                        cout << "��" << "" << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " "
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
