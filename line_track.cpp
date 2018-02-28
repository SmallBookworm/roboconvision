#include "line_track.h"

using namespace std;
using namespace cv;

// Set the resolution of the accumulator
void LineFinder::setAccResolution(double dRho, double dTheta) {
    deltaRho = dRho;
    deltaTheta = dTheta;
}

// Set the minimum number of votes
void LineFinder::setMinVote(int minv) {
    minVote = minv;
}

// Set line length and gap
void LineFinder::setLineLengthAndGap(double length, double gap) {
    minLength = length;
    maxGap = gap;
}

// Apply probabilistic Hough Transform
std::vector<cv::Vec4i> LineFinder::findLines(cv::Mat &binary) {
    lines.clear();
    cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, 5, minLength, maxGap);
    return lines;
}

// Draw the detected lines on an image
void LineFinder::drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255)) {
    // Draw the lines
    std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
    while (it2 != lines.end()) {
        cv::Point pt1((*it2)[0], (*it2)[1]);
        cv::Point pt2((*it2)[2], (*it2)[3]);
        cv::line(image, pt1, pt2, color, 2);
        ++it2;
    }
}

bool LineTracker::my_cmp(Vec4f a, Vec4f b) {
    return abs(a[3]) < abs(b[3]);
}

float LineTracker::CauculateAngle(int a, int b) {
    if (b == 0) {
        return 90;
    } else {
        return atan((float) a / (float) b) * 180 / PI;
    }
}

Vec2i LineTracker::GetPosition(std::vector<cv::Vec4f> lines) {
    Vec2i point;
    double k1, k2, b1, b2;

    if (lines[0][1] == 0)//如果第一条直线斜率不存在
    {
        point[0] = (float) -1 * lines[0][2] / lines[0][0];
        if (lines[1][1] == 0) {
            return NULL;
        }
        point[1] = -1 * ((float) lines[1][2] + (float) lines[1][0] * (float) point[0]) / (float) lines[1][1];
    } else if (lines[1][1] == 0)//如果第二条直线斜率不存在
    {
        point[0] = (float) -1 * (float) lines[1][2] / (float) lines[1][0];
        if (lines[0][1] == 0) {
            return NULL;
        }
        point[1] = -1 * ((float) lines[0][2] + (float) lines[0][0] * (float) point[0]) / (float) lines[0][1];
    } else {
        point[1] = ((float) lines[0][2] * (float) lines[1][0] + (float) lines[1][2] * (float) lines[0][0])
                   / (float) (lines[0][0] * (float) lines[1][1] - (float) lines[0][1] * (float) lines[1][0]);
        //point[0] = (-1) * ((float)lines[0][2] - (float)lines[0][1] * (float)point[1]) / (float)lines[0][0];
        point[0] = (-1) * ((float) lines[1][2] - (float) lines[1][1] * (float) point[1]) / (float) lines[1][0];
    }

    return point;
}

int LineTracker::watch(Point2f *point) {
    const char *pstrImageName = "1 0.bmp";//图片路径
    const char *pstrWindowsTitle = "直线检测";

    //!!!
//    VideoCapture cap(0); // open the default camera
//        if(!cap.isOpened())  // check if we succeeded
//            return -1;

//        printf("%f, %f\n",cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT));
//        cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
//        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
//        cap.set(CV_CAP_PROP_FPS, 15);
//        printf("%f, %f\n",cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT));
//        Mat src_gray;
//        cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );

//        while(1)
//        {
//            Mat frame;
//            cvWaitKey(10);
//            cap >> frame;
//            imshow("video", frame);
//        }
//    Mat frame;
//    pCapture >> frame;
//    imshow("show",frame);

    VideoCapture cap(1);
    if (!cap.isOpened()) {
        cerr << "capture is closed\n";
        return -1;
    }
    Mat frame;
    Mat edges;

    cap >> frame;
    if (frame.empty()){
        cerr << "frame is empty\n";
        return -1;
    }
    Mat midImage;
    Mat srcImage = frame;//imread(pstrImageName);
    resize(srcImage, srcImage, Size(srcImage.cols / 2, srcImage.rows / 2), 0, 0, INTER_LINEAR);
    Mat binaryImage;

    cvtColor(srcImage, midImage, CV_BGR2GRAY);
    //imshow("midIm,age", midImage);
    threshold(midImage, binaryImage, 170, 255, CV_THRESH_BINARY);
    //imshow("binaryImage", binaryImage);
    //erode(binaryImage, binaryImage, Mat(5, 5, CV_8U), Point(-1, -1), 4);
    //dilate(binaryImage, binaryImage, Mat(5, 5, CV_8U), Point(-1, -1), 2);
    //GaussianBlur(binaryImage, binaryImage, Size(3, 3), 0, 0);

    //求轮廓并去除噪音
    vector<vector<Point> > contour;
    vector<Vec4i> hierarchy;
    double minarea = 100.0;
    Mat binaryImage_Copy = binaryImage.clone();
    findContours(binaryImage_Copy, contour, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
    Mat imageContours = Mat::zeros(binaryImage_Copy.size(), CV_8UC1);
    for (int i = 0; i < contour.size(); i++) {
        //绘制轮廓
        if (contour[i].size() > minarea) {
            drawContours(imageContours, contour, i, Scalar(255), 2, 8, hierarchy);
            //!!!
            //drawContours(binaryImage, contour, i, Scalar(255), CV_FILLED, 8, hierarchy);
            //drawContours(imageContours, contour, i, Scalar(255), 2, 8, hierarchy);
        } else {
            drawContours(binaryImage, contour, i, Scalar(0), CV_FILLED, 8, hierarchy);
        }
    }

    //imshow("binaryImage_Copy", binaryImage);
    //imshow("Contours Image", imageContours); //轮廓

    //进行角点检测
    vector<Point2f> corners;
    Point2f pointFinal;
    Mat dstImage;
    //cornerHarris(grayImage, dstImage, 2, 3, 0.01);
    GaussianBlur(binaryImage, binaryImage, Size(3, 3), 0, 0);

    Mat mask;
    Rect r1(20, 20, binaryImage.cols - 40, binaryImage.rows - 40);
    Mat img1, img2, img3, img4;
    mask = Mat::zeros(binaryImage.size(), CV_8UC1);
    mask(r1).setTo(255);
    //imshow("mask", mask);


    goodFeaturesToTrack(binaryImage, corners, 4, 0.01, 10, InputArray(mask), 3, false, 0.04);
    float x = 0;
    float y = 0;
    for (int i = 0; i < corners.size(); i++) {
        circle(srcImage, corners[i], 5, Scalar(255), 2, 8, 0);//标出检测到的点
        x += corners[i].x;
        y += corners[i].y;
    }
    x = x / corners.size();
    y = y / corners.size();
    pointFinal.x = x;
    pointFinal.y = y;
    circle(srcImage, pointFinal, 5, Scalar(255), 2, 8, 0);
    //求偏差基准点减去求出的点；因此x偏移正为偏左，y偏移正为偏上
    int offset_x = POINT_X - pointFinal.x;
    int offset_y = POINT_Y - pointFinal.y;


    //求直线的旋转角度
    cv::Mat image = frame;//cv::imread(pstrImageName);
    resize(image, image, Size(image.cols / 2, image.rows / 2), 0, 0, INTER_LINEAR);
    cv::Mat contours;
    cv::cvtColor(image, contours, cv::COLOR_BGR2GRAY);
    cv::bitwise_not(contours, contours);
    //cv::Canny(image, contours, 155, 350);

    //第一种找直线方法
    LineFinder finder = this->finder;

    // Detect lines and draw them
    std::vector<cv::Vec4i> lines = finder.findLines(imageContours);
    std::vector<cv::Vec4f> linesCount;//ax+by+c=0   第一位a，第二位是b，第三位是c，第四位是角度
    std::vector<cv::Vec4f> linesAver;//对相似的直线整合，然后求交点。

    for (size_t i = 0; i < lines.size(); i++) {
        float a, b, c;
        float angle;
        a = lines[i][3] - lines[i][1];
        b = lines[i][2] - lines[i][0];
        c = -1 * (a * lines[i][0] + b * lines[i][1]);
        angle = CauculateAngle(a, b);
        if (angle != 0 && angle != 90) {
            Vec4f temp(a, b, c, angle);
            linesCount.push_back(temp);
        }
    }
    if (linesCount.empty()){
        cerr << "linesCount is empty\n";
        return -1;
    }
    sort(linesCount.begin(), linesCount.end(), my_cmp);//排序

    int cursor = 0;
    for (size_t i = 0; i < linesCount.size() - 1; i++) {
        if (abs(linesCount[i][3] - linesCount[i + 1][3]) > 3)
            break;
        else cursor++;
    }

    float a = 0;
    float b = 0;
    float c = 0;
    float angle = 0;
    for (size_t i = 0; i < linesCount.size(); i++) {
        a += linesCount[i][0];
        b += linesCount[i][1];
        c += linesCount[i][2];
        angle += linesCount[i][3];

        if (i == cursor) {

            a = a / (cursor + 1);
            b = b / (cursor + 1);
            c = c / (cursor + 1);

            Vec4f temp(a, b, c, CauculateAngle(a, b));
            linesAver.push_back(temp);
            a = 0;
            b = 0;
            c = 0;
            angle = 0;
        }
        if (i == linesCount.size() - 1) {

            a = a / (linesCount.size() - cursor - 1);
            b = b / (linesCount.size() - cursor - 1);
            c = c / (linesCount.size() - cursor - 1);

            Vec4f temp(a, b, c, CauculateAngle(a, b));
            linesAver.push_back(temp);
            a = 0;
            b = 0;
            c = 0;
            angle = 0;
        }
    }

    //输出
    cout << "直线角度" << linesAver[0][3] << "\n直线角度" << linesAver[1][3] << endl;
    cout << pointFinal.x << "\t" << pointFinal.y << endl;
    //    message += "偏移" + to_string(offset_x) + "\t" + to_string(offset_y);
    //    message += "角度偏移" + to_string(offset_thetaA) + "\t角点数" + to_string(offset_thetaB);

//    ofstream outFile("line.txt");
//    QString message;
//    message = "直线角度" + QString(linesAver[0][3]) + "\n直线角度" + QString(linesAver[1][3]);


//    message = "";
//    for (size_t i = 0; i < linesCount.size(); i++)
//    {
//        message += to_string(linesCount[i][0]) + "\t" + to_string(linesCount[i][1]) + "\t"+to_string(linesCount[i][2]) + "\t"+to_string(linesCount[i][3]) + "\n";
//    }
//    message += "游标" + to_string(cursor) + "\t角点数"+to_string(corners.size());

//    message += "偏移" + to_string(offset_x) + "\t" + to_string(offset_y);
//    message += "角度偏移" + to_string(offset_thetaA) + "\t角点数" + to_string(offset_thetaB);

    //这个也是基准的偏差角度减去求出来的两条直线的，都是偏平行底边的直线为A；偏差值为正表示视野右边靠前
    int offset_thetaA = THETA_A - linesAver[0][3];
    int offset_thetaB = THETA_B - linesAver[1][3];

    imshow("1", srcImage);
    cap.release();
    point = &pointFinal;
    return 1;
}
