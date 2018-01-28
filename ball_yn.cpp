#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>  
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

const int kvalue = 15;//双边滤波邻域大小  

int iLowH1 = 90;
int iHighH1 = 110;

int iLowS1 = 70;
int iHighS1 = 255;

int iLowV1 = 40;
int iHighV1 = 255;

//double alpha = 0.5;
//double beta;
//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main()
{
	bool have = false;
	Mat srcImageorigin, srcImage;//定义一个srcImage变量，用于存储每一帧的图像
	srcImage = imread("Y1.bmp");
	//resize(srcImageorigin, srcImage, Size(600, 800)); //图像大小转换
	Mat Image_hsv;//临时变量和目标图的定义
	Mat dstImage(srcImage.size(), srcImage.type());
	dstImage = Scalar::all(0);
	//【2】显示原始图
	//imshow("【原始图】", srcImage);

	//【3】转为HSV图并进行图像平滑
	cvtColor(srcImage, Image_hsv, COLOR_BGR2HSV);//转化边缘检测后的图为HSV图
	GaussianBlur(Image_hsv, Image_hsv, Size(3, 3), 2, 2);
	//imshow("【HSV图】", Image_hsv);

	vector<Mat> hsvSplit;
	split(Image_hsv, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);//直方图均值化，即合并一些灰度锐化并增大对比度
	merge(hsvSplit, Image_hsv);
	imshow("【均值化】1", Image_hsv);
	split(Image_hsv, hsvSplit);


	Mat imgThresholded1;
	Mat combination;
	inRange(Image_hsv, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1), imgThresholded1); //Threshold the image  
	//addWeighted(combination, 1, imgThresholded3, 1, 0.0, combination); //这里调用了addWeighted函数，得到的结果存储在dst中
	imshow("【1】", imgThresholded1);

	Mat bf;//对灰度图像进行双边滤波  
	bilateralFilter(imgThresholded1, bf, kvalue, kvalue * 2, kvalue / 2);
	imshow("灰度双边滤波处理", bf);

	vector<vector<Point>> contours;
	findContours(bf, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//5.查找正外接矩形  
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].capacity()>100) {
			boundRect[i] = boundingRect(contours[i]);
			//6.绘制正外接矩形并添加文字  
			if (boundRect[i].width > 300 && boundRect[i].height > 300) {
				rectangle(srcImage, boundRect[i], Scalar(0, 255, 0), 1, 8);
				cout <<"有 "<< boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " " << boundRect[i].height << endl;
				have = true;
			}
		}
	}
	if (!have) {
		cout << "无 ";
	}
	//7.显示每一帧，并按原FPS显示  
	imshow("ans", srcImage);

	waitKey(0);
	return 0;
}
