//
// Created by peng on 18-4-8.
//

#include "lineTest.h"

using namespace std;
using namespace cv;

/*
寻找光线色差，分离通道
*/
bool LineTest::is_nan(double dVal) {
    if (dVal == dVal) {
        return false;
    }
    return true;
}

void LineTest::GetDiffImage(Mat src1, Mat dst) {

    for (int i = 0; i < HEIGHT; i++) {
        uchar *pData1 = src1.ptr<uchar>(i);
        uchar *pData3 = dst.ptr<uchar>(i);
        for (int j = 0; j < WIDTH; j++) {

            pData3[j] = pData1[j];

        }
    }
}

bool LineTest::comp(const Vec4i &a, const Vec4i &b) {
    return a[0] < b[0];

}

float LineTest::RadianCalculate(float rleft, float rright) {
    float radian = asinf((float) (rleft - rright) / (float) SREAL_WIDTH);
    return radian;
}

float LineTest::rad(float ang) {
    return (float) ang * M_PI / (float) 180;
}

float LineTest::angleProcessor(float newNum) {
    float ca = 0;
    int zeroNum = 0;
    int otherNum = 0;
    if (angleRecord.size() < 10) {
        angleRecord.push_back(newNum);
    } else {
        num = num % 10;
        angleRecord[num] = newNum;
        num++;
    }
    for (int o = 0; o < angleRecord.size(); o++) {
        if (angleRecord[o] == 0) {
            zeroNum++;
        } else {
            otherNum++;
        }
    }
    if (zeroNum > otherNum) {
        return 0;
    } else {
        for (int o = 0; o < angleRecord.size(); o++) {
            ca = angleRecord[o] + ca;
        }
        ca = ca / angleRecord.size();
        return ca;
    }
}

float LineTest::xProcessor(float newNum) {
    float ca = 0;
    int zeroNum = 0;
    int otherNum = 0;
    if (xRecord.size() < 10) {
        xRecord.push_back(newNum);
    } else {
        num = num % 10;
        xRecord[num] = newNum;
        num++;
    }
    for (int o = 0; o < xRecord.size(); o++) {
        if (xRecord[o] == 0) {
            zeroNum++;
        } else {
            otherNum++;
        }
    }
    if (zeroNum > otherNum) {
        return 0;
    } else {
        for (int o = 0; o < xRecord.size(); o++) {
            ca = xRecord[o] + ca;
        }
        ca = ca / xRecord.size();
        return ca;
    }
}

float LineTest::yProcessor(float newNum) {
    float ca = 0;
    int zeroNum = 0;
    int otherNum = 0;
    if (yRecord.size() < 10) {
        yRecord.push_back(newNum);
    } else {
        num = num % 10;
        yRecord[num] = newNum;
        num++;
    }
    for (int o = 0; o < yRecord.size(); o++) {
        if (yRecord[o] == 0) {
            zeroNum++;
        } else {
            otherNum++;
        }
    }
    if (zeroNum > otherNum) {
        return 0;
    } else {
        for (int o = 0; o < yRecord.size(); o++) {
            ca = yRecord[o] + ca;
        }
        ca = ca / yRecord.size();
        return ca;
    }
}

float LineTest::AngleCalculate(float rleft, float rright) {
    float radian = asinf((float) (rleft - rright) / (float) SREAL_WIDTH);
    float angle = (float) radian * 180 / (float) M_PI;
    return angle;
}

vector<Vec4i> LineTest::findCorner(Mat dst) {
    vector<vector<Point> > contours;
    vector<Vec4i> lines;

    cv::findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    Mat imageContours = Mat::zeros(dst.size(), CV_8UC1);
    for (int i = 0; i < contours.size(); i++) {
        Point2f P[4];

        if (contours[i].capacity() > 200) //change according to fps
        {
            Vec4i leftLine;
            Vec4i rightLine;
            int averageRightDownY = 0;
            int averageRightDownNumY = 0;

            int averageLeftUpY = 0;
            int averageLeftUpNumY = 0;

            int averageLeftDownY = 0;
            int averageLeftDownNumY = 0;

            int averageRightUpY = 0;
            int averageRightUpNumY = 0;


            int rightDown = 0;
            int rightDownD = 0;
            int leftUp = WIDTH * HEIGHT;
            int leftUpD = 0;

            int leftDown = 0;
            int leftDownD = 0;
            int rightUp = 0;
            int rightUpD = 0;

            for (int j = 0; j < contours[i].size(); j++) {
                if (contours[i][j].x * contours[i][j].y > rightDown) {
                    rightDownD = j;
                    rightDown = contours[i][j].x * contours[i][j].y;
                }
                if (contours[i][j].x * contours[i][j].y < leftUp) {
                    leftUpD = j;
                    leftUp = contours[i][j].x * contours[i][j].y;
                }
                if (contours[i][j].x * (HEIGHT - contours[i][j].y) > rightUp) {
                    rightUpD = j;
                    rightUp = contours[i][j].x * (HEIGHT - contours[i][j].y);
                }
                if ((WIDTH - contours[i][j].x) * contours[i][j].y > leftDown) {
                    leftDownD = j;
                    leftDown = (WIDTH - contours[i][j].x) * contours[i][j].y;
                }

            }
            for (int j = 0; j < contours[i].size(); j++) {
                if (contours[i][j].y > (contours[i][rightDownD].y - AVG) &&
                    contours[i][j].y < (contours[i][rightDownD].y + AVG)) {
                    averageRightDownY += contours[i][j].y;
                    averageRightDownNumY++;
                }
                if (contours[i][j].y > (contours[i][leftUpD].y - AVG) &&
                    contours[i][j].y < (contours[i][leftUpD].y + AVG)) {
                    averageLeftUpY += contours[i][j].y;
                    averageLeftUpNumY++;
                }
                if (contours[i][j].y > (contours[i][leftDownD].y - AVG) &&
                    contours[i][j].y < (contours[i][leftDownD].y + AVG)) {
                    averageLeftDownY += contours[i][j].y;
                    averageLeftDownNumY++;
                }
                if (contours[i][j].y > (contours[i][rightUpD].y - AVG) &&
                    contours[i][j].y < (contours[i][rightUpD].y + AVG)) {
                    averageRightUpY += contours[i][j].y;
                    averageRightUpNumY++;
                }
            }
            averageRightDownY = averageRightDownY / averageRightDownNumY;
            averageLeftUpY = averageLeftUpY / averageLeftUpNumY;
            averageLeftDownY = averageLeftDownY / averageLeftDownNumY;
            averageRightUpY = averageRightUpY / averageRightUpNumY;
            RotatedRect rect = minAreaRect(contours[i]);
            rect.points(P);

            float tempHigh1 = 0, tempLow1 = 0;
            for (int j = 0; j <= 3; j++)//经验值
            {
                //line(src, P[j], P[(j + 1) % 4], Scalar(255), 2);

                if (P[j].y > rect.center.y) {
                    if (tempHigh1 == 0) {
                        tempHigh1 = P[j].x;
                    }
                    if (tempHigh1 > P[j].x) {
                        leftLine[0] = tempHigh1 + 1;
                        leftLine[1] = averageLeftDownY;//左下
                        rightLine[0] = P[j].x + 1;
                        rightLine[1] = averageRightDownY;//右下
                    }
                    if (tempHigh1 < P[j].x) {
                        rightLine[0] = tempHigh1 + 1;
                        rightLine[1] = averageLeftDownY;//左下
                        leftLine[0] = P[j].x - 1;
                        leftLine[1] = averageRightDownY;//右下
                    }
                }
                if (P[j].y < rect.center.y) {
                    if (tempLow1 == 0) {
                        tempLow1 = P[j].x;
                    }
                    if (tempLow1 > P[j].x) {
                        leftLine[2] = tempLow1 + 1;
                        leftLine[3] = averageLeftUpY + 2;//左上
                        rightLine[2] = P[j].x - 1;
                        rightLine[3] = averageRightUpY + 2;//右上
                    }
                    if (tempLow1 < P[j].x) {
                        rightLine[2] = tempLow1 + 1;
                        rightLine[3] = averageLeftUpY + 2;//左上
                        leftLine[2] = P[j].x - 1;
                        leftLine[3] = averageRightUpY + 2;//右上
                    }
                }
            }

            lines.push_back(leftLine);
            lines.push_back(rightLine);
        }
    }
    sort(lines.begin(), lines.end(), comp);
    return lines;
}

vector<float> LineTest::analyse(Mat paint, LinesOption left_line, LinesOption right_line,
                                vector<Vec4i> lines) {

    vector<float> all_data;
    float pix_left_height = left_line.pixheight(0, lines);
    float pix_right_height = right_line.pixheight(3, lines);
    float real_left_D = left_line.realdist(0, lines);//左灯条左边的真实距离
    float real_right_D = right_line.realdist(3, lines);//右灯条右边的真实距离

    //求角度
    float pic_angle = AngleCalculate(real_left_D, real_right_D);
    float angle = pic_angle - SINIT_ANGLE;
    float radian = rad(angle);
    float leftToCenter = left_line.centerPoint(0, lines).x - WIDTH / 2;//pix
    float rightToCenter = right_line.centerPoint(3, lines).x - WIDTH / 2;//pix
    int r = 2;
    if (pix_left_height - pix_right_height > 2) {
        float real_delta_x = (float) SREAL_HEIGHT * ((float) leftToCenter / (float) left_line.pixheight(0, lines) -
                                                     (float) SLEFTTOCENTER / (float) SPIX_LIGHT_HEIGHT);//#########改过
        float real_delta_d = real_left_D - SD;
        float a[2] = {real_delta_x, real_delta_d};
        Mat av = Mat(2, 1, CV_32FC1, a);
        float b[2] = {SLEFTTOCENTER * (float) SREAL_WIDTH / (float) SPIX_LIGHT_WIDTH, SD};
        Mat bv = Mat(2, 1, CV_32FC1, b);
        float rotate[4] = {cosf(-radian), -sinf(-radian), sinf(-radian), cosf(-radian)};
        Mat rotatev = Mat(2, 2, CV_32FC1, rotate);
        Mat cv = rotatev * bv;
        Mat dv = bv + av - cv;
        float lc[2] = {0, 259};//*****************************
        Mat locateToCamera1 = Mat(2, 1, CV_32FC1, lc);
        Mat locateToCamera2 = rotatev * locateToCamera1;
        Mat locateToLocate = locateToCamera1 + dv - locateToCamera2;
        float *data1 = locateToLocate.ptr<float>(0);
        float x = data1[0];
        float *data2 = locateToLocate.ptr<float>(0);
        float y = data2[1];
        all_data.push_back(angle);
        all_data.push_back(x);
        all_data.push_back(y);
    }
    if (pix_right_height - pix_left_height > 2) {
        float real_delta_x = (float) SREAL_HEIGHT * ((float) rightToCenter / (float) right_line.pixheight(3, lines) -
                                                     (float) SRIGHTTOCENTER / (float) SPIX_LIGHT_HEIGHT);
        float real_delta_d = real_right_D - SD;
        float a[2] = {real_delta_x, real_delta_d};
        Mat av = Mat(2, 1, CV_32FC1, a);
        float b[2] = {SRIGHTTOCENTER * (float) SREAL_WIDTH / (float) SPIX_LIGHT_WIDTH, SD};
        Mat bv = Mat(2, 1, CV_32FC1, b);
        float rotate[4] = {cosf(-radian), -sinf(-radian), sinf(-radian), cosf(-radian)};
        Mat rotatev = Mat(2, 2, CV_32FC1, rotate);
        Mat cv = rotatev * bv;
        Mat dv = bv + av - cv;
        float lc[2] = {0, 259};//*****************************
        Mat locateToCamera1 = Mat(2, 1, CV_32FC1, lc);
        Mat locateToCamera2 = rotatev * locateToCamera1;
        Mat locateToLocate = locateToCamera1 + dv - locateToCamera2;
        float *data1 = locateToLocate.ptr<float>(0);
        float x = data1[0];
        float *data2 = locateToLocate.ptr<float>(0);
        float y = data2[1];
        all_data.push_back(angle);
        all_data.push_back(x);
        all_data.push_back(y);
    }
    if (abs(pix_left_height - pix_right_height) <= 2) {
        radian = 0;
        angle = 0;
        float real_delta_x = (float) SREAL_HEIGHT * ((float) leftToCenter / (((float) left_line.pixheight(0, lines) +
                                                                              (float) right_line.pixheight(3, lines)) /
                                                                             2) -
                                                     (float) SLEFTTOCENTER / (float) SPIX_LIGHT_HEIGHT);//#########改过
        float real_delta_d = real_left_D - SD;
        float a[2] = {real_delta_x, real_delta_d};
        Mat av = Mat(2, 1, CV_32FC1, a);
        float b[2] = {SLEFTTOCENTER * (float) SREAL_WIDTH / (float) SPIX_LIGHT_WIDTH, SD};
        Mat bv = Mat(2, 1, CV_32FC1, b);
        float rotate[4] = {cosf(-radian), -sinf(-radian), sinf(-radian), cosf(-radian)};
        Mat rotatev = Mat(2, 2, CV_32FC1, rotate);
        Mat cv = rotatev * bv;
        Mat dv = bv + av - cv;
        float lc[2] = {0, 259};//*****************************
        Mat locateToCamera1 = Mat(2, 1, CV_32FC1, lc);
        Mat locateToCamera2 = rotatev * locateToCamera1;
        Mat locateToLocate = locateToCamera1 + dv - locateToCamera2;
        float *data1 = locateToLocate.ptr<float>(0);
        float x = data1[0];
        float *data2 = locateToLocate.ptr<float>(0);
        float y = data2[1];
        all_data.push_back(angle);
        all_data.push_back(x);
        all_data.push_back(y);
    }
    /*调参数用*/
//    all_data.push_back(pix_left_height);
//    all_data.push_back(pix_right_height);
//    all_data.push_back(leftToCenter);
//    all_data.push_back(rightToCenter);
    //namedWindow("final", 0);
    imshow("final", paint);
    waitKey(0);
    return all_data;
}

void LineTest::drawDetectLines(Mat &image, const vector<Vec4i> &lines, Scalar &color) {
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it = lines.begin();
    while (it != lines.end()) {
        Point pt1((*it)[0], (*it)[1]);
        Point pt2((*it)[2], (*it)[3]);
        line(image, pt1, pt2, color, 2); //  线条宽度设置为2
        ++it;
    }
}

int LineTest::watch(cv::Mat src) {
    Mat pBinary, record, dst;
    dst = Mat::zeros(Size(WIDTH, HEIGHT), CV_8UC1);
    vector<Mat> mv;
    vector<Vec4i> lines;
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    Mat elementE = getStructuringElement(MORPH_RECT, Size(9, 9));
    Mat elementC = getStructuringElement(MORPH_RECT, Size(3, 3));
    vector<vector<float>> dateRecord;

    split(src, mv);
    GetDiffImage(mv[1], dst);

    //先膨胀，后腐蚀（联通区域）
    cv::erode(dst, pBinary, elementC);
    cv::dilate(pBinary, dst, elementC);
    threshold(dst, dst, 30, 255, THRESH_BINARY);

    GaussianBlur(dst, dst, Size(3, 3), 0, 0);
    //imshow("eee",dst);
    //得到角点
    lines = findCorner(dst);
    vector<float> data;
    vector<float> data_filter1;
    vector<float> data_final;
    if (lines.size() == 4) {
        Scalar sca = Scalar(0, 0, 255);
        drawDetectLines(src, lines, sca);
        data = analyse(src, left_line, right_line, lines);
        if (abs(data[0]) < angleThreshold) {
            data[0] = 0;
        }
        if (abs(data[1]) < xThreshold) {
            data[1] = 0;
        }
        if (abs(data[2]) < yThreshold) {
            data[2] = 0;
        }
        data_filter1.push_back(angleProcessor(data[0]));
        data_filter1.push_back(xProcessor(data[1]));
        data_filter1.push_back(yProcessor(data[2]));
        float vectRadian = atan2f(data_filter1[2], data_filter1[1]);
        float vectAngle = 180 * vectRadian / (float) M_PI;
        float vectLength = sqrtf(powf(data_filter1[1], 2) + powf(data_filter1[2], 2));
        data_final.push_back(data_filter1[0]);
        data_final.push_back(vectAngle);
        data_final.push_back(vectLength);

        cout << "angle: " << data_final[0] << endl;
        cout << "vectAngle: " << data_final[1] << endl;
        cout << "vectLength: " << data_final[2] << endl;
        info_value[0] = data_final[2];
        info_value[1] = data_final[1];
        info_value[2] = data_final[0];
    } else if (lines.size() < 4) {
        cout << "invalid " << lines.size() << endl;
    } else if (lines.size() > 4) {
        //imshow("eeee",src);
        //waitKey(0);
        cout << "invalid " << lines.size() << endl;
    }
    //waitKey(0);
    return static_cast<int>(lines.size());
}

int LineTest::operator()(LineInfo &info) {
    //system("v4l2-ctl --set-ctrl=exposure_auto=1 -d /dev/video1");

    VideoCapture capture(1);
    //capture.open("/home/peng/下载/realse/1.avi");
    capture.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);

//    int fd = open("/dev/video1", O_RDWR);
//    if (fd >= 0) {
//        struct v4l2_control ctrl;
//        ctrl.id = V4L2_CID_EXPOSURE_AUTO;
//        ctrl.value = V4L2_EXPOSURE_MANUAL;
//        int ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
//
//        struct v4l2_control ctrl1;
//        ctrl1.id = V4L2_CID_EXPOSURE_ABSOLUTE;
//        ctrl1.value=1;
//        ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl1);
//        if (ret < 0) {
//            printf("Get exposure failed (%d)\n", ret);
//        } else
//            printf("\nGet Exposure :[%d]\n", ctrl1.value);
//    }

    Mat srcImage;
    if (!capture.isOpened()) {
        std::cout << "fail to open video!" << std::endl;
        return -1;
    }

    bool status = info.getStop();
    while (!status) {

        capture >> srcImage;
        if (!capture.isOpened() || srcImage.empty())
            break;
        int size = watch(srcImage);
        //test
        //cout << size << endl;
        if (size == 4) {
            info.set(info_value);
        }
        //test
        //imshow("show", srcImage);
        //if (waitKey(1) == 27) {
        //    break;
        //}
        status = info.getStop();
    };
    return 0;
}
