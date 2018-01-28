#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>  
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

const int kvalue = 15;//˫���˲������С  

int iLowH1 = 90;
int iHighH1 = 110;

int iLowS1 = 70;
int iHighS1 = 255;

int iLowV1 = 40;
int iHighV1 = 255;

//double alpha = 0.5;
//double beta;
//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-------------------------------------------------------------------------------------------------
int main()
{
	bool have = false;
	Mat srcImageorigin, srcImage;//����һ��srcImage���������ڴ洢ÿһ֡��ͼ��
	srcImage = imread("Y1.bmp");
	//resize(srcImageorigin, srcImage, Size(600, 800)); //ͼ���Сת��
	Mat Image_hsv;//��ʱ������Ŀ��ͼ�Ķ���
	Mat dstImage(srcImage.size(), srcImage.type());
	dstImage = Scalar::all(0);
	//��2����ʾԭʼͼ
	//imshow("��ԭʼͼ��", srcImage);

	//��3��תΪHSVͼ������ͼ��ƽ��
	cvtColor(srcImage, Image_hsv, COLOR_BGR2HSV);//ת����Ե�����ͼΪHSVͼ
	GaussianBlur(Image_hsv, Image_hsv, Size(3, 3), 2, 2);
	//imshow("��HSVͼ��", Image_hsv);

	vector<Mat> hsvSplit;
	split(Image_hsv, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);//ֱ��ͼ��ֵ�������ϲ�һЩ�Ҷ��񻯲�����Աȶ�
	merge(hsvSplit, Image_hsv);
	imshow("����ֵ����1", Image_hsv);
	split(Image_hsv, hsvSplit);


	Mat imgThresholded1;
	Mat combination;
	inRange(Image_hsv, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1), imgThresholded1); //Threshold the image  
	//addWeighted(combination, 1, imgThresholded3, 1, 0.0, combination); //���������addWeighted�������õ��Ľ���洢��dst��
	imshow("��1��", imgThresholded1);

	Mat bf;//�ԻҶ�ͼ�����˫���˲�  
	bilateralFilter(imgThresholded1, bf, kvalue, kvalue * 2, kvalue / 2);
	imshow("�Ҷ�˫���˲�����", bf);

	vector<vector<Point>> contours;
	findContours(bf, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//5.��������Ӿ���  
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].capacity()>100) {
			boundRect[i] = boundingRect(contours[i]);
			//6.��������Ӿ��β��������  
			if (boundRect[i].width > 300 && boundRect[i].height > 300) {
				rectangle(srcImage, boundRect[i], Scalar(0, 255, 0), 1, 8);
				cout <<"�� "<< boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " " << boundRect[i].height << endl;
				have = true;
			}
		}
	}
	if (!have) {
		cout << "�� ";
	}
	//7.��ʾÿһ֡������ԭFPS��ʾ  
	imshow("ans", srcImage);

	waitKey(0);
	return 0;
}
