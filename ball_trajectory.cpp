//------------------------------------------------------------------------------
// <copyright file="ColorBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "resource.h"
#include "ColorBasics.h"
#include <fstream> 
#define ringx 0
#define ringy 0
#define distance 0
#define ringRangex 0
#define ringRangey 0

vector<RotatedRect> CColorBasics::get_foreground_object(
	Ptr<BackgroundSubtractorMOG2> pBackgroundKnn,
	Mat scene, double scale, bool isFlag) {
	if (!isFlag) {
		std::vector<RotatedRect> one_rect;
		Rect whole;
		whole.x = whole.y = 0;
		whole.height = scene.rows;
		whole.width = scene.cols;


		one_rect.emplace_back(whole.br(), whole.tl(),
			Point2f(whole.tl().x, whole.tl().y + whole.height));
		return one_rect;
	}
	Mat img;
	resize(scene, img, Size(0, 0), scale, scale);


	Mat fgmask, fgimg, bgimg;
	pBackgroundKnn->apply(img, fgmask);


	medianBlur(fgmask, fgmask, 5);


	morphologyEx(fgmask, fgmask, MORPH_CLOSE, Mat::ones(15, 3, CV_8UC1));
	//imshow("MORPH_CLOSE", fgmask);
	std::vector<std::vector<Point>> region_contours;
	findContours(fgmask, region_contours, CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	std::vector<RotatedRect> objects;
	for (size_t i = 0; i != region_contours.size(); ++i) {
		RotatedRect rect = minAreaRect(region_contours[i]);
		objects.push_back(rect);
	}
	return objects;
}

// 转换color图像到cv::Mat  
Mat CColorBasics::ConvertMat(const RGBQUAD* pBuffer, int nWidth, int nHeight)
{
	Mat img(nHeight, nWidth, CV_8UC3);
	uchar* p_mat = img.data;

	const RGBQUAD* pBufferEnd = pBuffer + (nWidth * nHeight);

	while (pBuffer < pBufferEnd)
	{
		*p_mat = pBuffer->rgbBlue;
		p_mat++;
		*p_mat = pBuffer->rgbGreen;
		p_mat++;
		*p_mat = pBuffer->rgbRed;
		p_mat++;

		++pBuffer;
	}
	return img;
}

/*
下面是关于精度的改进函数
*/
cv::Mat CColorBasics::ConvertMat(const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{
	cv::Mat img(nHeight, nWidth, CV_8UC1);
	depthMat = Mat(nHeight, nWidth, CV_16UC1);//记录深度数据
	BYTE* p_mat = img.data;
	ushort * p_dmat = (ushort *)depthMat.data;

	const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);

	while (pBuffer < pBufferEnd)
	{
		USHORT depth = *pBuffer;
		BYTE level = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth / 16) : 0);
		//BYTE level = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth / 16) : 0);
		//BYTE intensity = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth % 256) : 0);
		*p_mat = level;
		p_mat++;
		ushort dlevel = static_cast<ushort>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth * 8) : 0);
		*p_dmat = dlevel;
		p_dmat++;
		//*p_mat = intensity;
		//p_mat++;

		++pBuffer;
	}
	return img;
}

/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CColorBasics application;
	application.Run(hInstance, nShowCmd);
}

/// <summary>
/// Constructor
/// </summary>
CColorBasics::CColorBasics() :
	m_hWnd(NULL),
	m_nLastCounter(0),
	m_nFramesSinceUpdate(0),
	m_nNextStatusTime(0LL),
	m_pKinectSensor(NULL),
	m_pColorFrameReader(NULL),
	m_pD2DFactory(NULL),
	m_pColorRGBX(NULL),
	m_pDepthFrameReader(NULL),
	m_pDepthRGBX(NULL)
{
	//write1.open(outFlie, -1, 30, Size(1920, 1080), true);
	//waitKey(50);
	//write2.open(outFlie2, -1, 30, Size(512, 424), true);
	//waitKey(50);
	//write3.open(outFlie3, -1, 30, Size(512, 424), true);
	// create heap storage for color pixel data in RGBX format
	pBackgroundKnn = createBackgroundSubtractorMOG2();
	
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];
	outfile.open("fish.txt");
	outfile2.open("ball.txt");
}


/// <summary>
/// Destructor
/// </summary>
CColorBasics::~CColorBasics()
{
	//write1.release();
	//write2.release();
	//write3.release();

	if (m_pColorRGBX)
	{
		delete[] m_pColorRGBX;
		m_pColorRGBX = NULL;
	}
	if (m_pDepthRGBX)
	{
		delete[] m_pDepthRGBX;
		m_pDepthRGBX = NULL;
	}

	// clean up Direct2D
	SafeRelease(m_pD2DFactory);

	// done with color frame reader
	SafeRelease(m_pColorFrameReader);
	// done with depth frame reader
	SafeRelease(m_pDepthFrameReader);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CColorBasics::Run(HINSTANCE hInstance, int nCmdShow)
{
	MSG       msg = { 0 };
	WNDCLASS  wc;

	// Dialog custom window class
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
	wc.lpfnWndProc = DefDlgProcW;
	wc.lpszClassName = L"ColorBasicsAppDlgWndClass";

	if (!RegisterClassW(&wc))
	{
		return 0;
	}

	// Create main application window
	HWND hWndApp = CreateDialogParamW(
		NULL,
		MAKEINTRESOURCE(IDD_APP),
		NULL,
		(DLGPROC)CColorBasics::MessageRouter,
		reinterpret_cast<LPARAM>(this));
	ShowWindow(hWndApp, nCmdShow);

	// Main message loop
	while (WM_QUIT != msg.message)
	{
		Update();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If a dialog message will be taken care of by the dialog proc
			if (hWndApp && IsDialogMessageW(hWndApp, &msg))
			{
				continue;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

/// <summary>
/// Main processing function
/// </summary>
void CColorBasics::Update()
{
	//if (!m_pColorFrameReader)
	//{
	//    return;
	//}

	//IColorFrame* pColorFrame = NULL;

	//HRESULT hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);
	HRESULT hr;
	//if (SUCCEEDED(hr))
	//{
	//    IFrameDescription* pFrameDescription = NULL;
	//    int nWidth = 0;
	//    int nHeight = 0;
	//    ColorImageFormat imageFormat = ColorImageFormat_None;
	//    UINT nBufferSize = 0;
	//    RGBQUAD *pBuffer = NULL;

	//    if (SUCCEEDED(hr))
	//    {
	//        hr = pColorFrame->get_FrameDescription(&pFrameDescription);
	//    }

	//    if (SUCCEEDED(hr))
	//    {
	//        hr = pFrameDescription->get_Width(&nWidth);
	//    }

	//    if (SUCCEEDED(hr))
	//    {
	//        hr = pFrameDescription->get_Height(&nHeight);
	//    }

	//    if (SUCCEEDED(hr))
	//    {
	//        hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
	//    }

	//    if (SUCCEEDED(hr))
	//    {
	//        if (imageFormat == ColorImageFormat_Bgra)
	//        {
	//            hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
	//        }
	//        else if (m_pColorRGBX)
	//        {
	//            pBuffer = m_pColorRGBX;
	//            nBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
	//            hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);            
	//        }
	//        else
	//        {
	//            hr = E_FAIL;
	//        }
	//    }

	//    if (SUCCEEDED(hr))
	//    {
	//        ProcessColor(pBuffer, nWidth, nHeight);


	//    }

	//    SafeRelease(pFrameDescription);
	//}

	//SafeRelease(pColorFrame);

	if (!m_pDepthFrameReader)
	{
		return;
	}

	IDepthFrame* pDepthFrame = NULL;

	hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

	if (SUCCEEDED(hr))
	{
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxDistance = 0;
		UINT nBufferSize = 0;
		UINT16 *pBuffer = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			// In order to see the full range of depth (including the less reliable far field depth)
			// we are setting nDepthMaxDistance to the extreme potential depth threshold
			nDepthMaxDistance = USHRT_MAX;

			// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
			//// hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		}

		if (SUCCEEDED(hr))
		{
			ProcessDepth(pBuffer, nWidth, nHeight, nDepthMinReliableDistance, nDepthMaxDistance);
		}

		SafeRelease(pFrameDescription);
	}

	SafeRelease(pDepthFrame);
}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CColorBasics::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CColorBasics* pThis = NULL;

	if (WM_INITDIALOG == uMsg)
	{
		pThis = reinterpret_cast<CColorBasics*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<CColorBasics*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CColorBasics::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	string m1;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Bind application window handle
		m_hWnd = hWnd;

		// Init Direct2D
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

		// Create and initialize a new Direct2D image renderer (take a look at ImageRenderer.h)
		// We'll use this to draw the data we receive from the Kinect to the screen

		// Get and initialize the default Kinect sensor
		InitializeDefaultSensor();
	}
	break;

	// If the titlebar X is clicked, destroy app
	case WM_CLOSE:
		//write1.release();
		//write2.release();
		//write3.release();
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		m1 = "过环坐标：" + to_string(passx) + "," + to_string(passy) + "\t" + to_string(mindis + distance);
		outfile2 << m1 << endl;
		outfile.close();
		outfile2.close();
		break;

	case WM_DESTROY:
		//write1.release();
		//write2.release();
		//write3.release();
		DestroyWindow(hWnd);
		// Quit the main message pump
		PostQuitMessage(0);
		m1 = "过环坐标：" + to_string(passx) + "," + to_string(passy) + "\t" + to_string(mindis + distance);
		outfile2 << m1 << endl;
		outfile.close();
		outfile2.close();
		break;
		// Handle button press
	case WM_COMMAND:
		// If it was for the screenshot control and a button clicked event, save a screenshot next frame 
		break;

	}

	return FALSE;
}

/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CColorBasics::InitializeDefaultSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get the color reader
		IColorFrameSource* pColorFrameSource = NULL;
		IDepthFrameSource* pDepthFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}

		SafeRelease(pColorFrameSource);
	}


	return hr;
}

/// <summary>
/// Handle new color data
/// <param name="pBuffer">pointer to frame data</param>
/// <param name="nWidth">width (in pixels) of input image data</param>
/// <param name="nHeight">height (in pixels) of input image data</param>
/// </summary>
void CColorBasics::ProcessColor(RGBQUAD* pBuffer, int nWidth, int nHeight)
{

	// Make sure we've received valid data
	if (pBuffer && (nWidth == cColorWidth) && (nHeight == cColorHeight))
	{
		// Draw the data with Direct2D
		Mat srcImage = ConvertMat(pBuffer, cColorWidth, cColorHeight);
		imshow("A", srcImage);
		//write1.write(srcImage);


	}
}
void CColorBasics::testDist(Mat srcImage) {
	Mat dstImage(srcImage.size(), srcImage.type());
	dstImage = Scalar::all(0);
	//【2】显示原始图
	/*imshow("【源视频】", srcImage);*/
	point = srcImage.clone();
	//膨胀操作，让断的圆接起来
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat out;
	dilate(srcImage, out, element);
	imshow("【out】", out);

	//【3】转为灰度图并进行图像平滑
	Mat Image_gray;//临时变量和目标图的定义
				   /*waitKey(50);*/
				   //cvtColor(out, Image_gray, COLOR_BGR2GRAY);//转化边缘检测后的图为灰度图
	GaussianBlur(out, out, Size(9, 9), 2, 2);
	imshow("2", out);
	Mat bf;//对灰度图像进行双边滤波  
	bilateralFilter(out, bf, kvalue, kvalue * 2, kvalue / 2);
	//imshow("灰度双边滤波处理", bf);
	//imwrite("src_bf.png", bf);//attention

	//【4】进行霍夫圆变换
	vector<Vec3f> circles;
	HoughCircles(bf, circles, HOUGH_GRADIENT, 1.5, 20, 130, 38, 25, 30);//源图；输出矢量；使用的检测方法（此处仅HOUGH_GRADIENT一种）；分辨率，dp=1相同分辨率，dp=2一半；mindist能明显检测到两个圆的最小距离；对应3，传递给canny边缘检测算子的高阈值，默认100；越小可以检测到不存在的圆，越大可以更接近圆形；minradius；maxradius


																		//【5】依次在图中绘制出圆	
	for (size_t i = 0; i < circles.size(); i++)
	{
		//参数定义
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//绘制圆心
		circle(srcImage, center, 1, Scalar(255, 0, 0), -1, 8, 0);//center后面数字代表圆心大小，Scalar(0,255,0) 第一个多少没有变化，第二个255变小圆心变暗
																 //绘制圆轮廓
		circle(srcImage, center, radius, Scalar(255, 0, 0), 1, 8, 0);//1代表圆周的线宽，8变小以后圆开始走形，0不可以改成负数，改成正数以后十字叉的圆周和圆心分离且缩小为原来的1/x+1
		circle(point, center, 1, Scalar(255, 0, 0), -1); //第五个参数我设为-1，表明这是个实点。

														 //cout << cvRound(circles[i][0]) << "\t" << cvRound(circles[i][1]) << "\t"
														 //	<< cvRound(circles[i][2]) << endl;//在控制台输出圆心坐标和半径 
		if (cvRound(circles[i][0]) != NULL)
		{
			cnt++;
		}
		xSum += cvRound(circles[i][0]);
		ySum += cvRound(circles[i][1]);
		rSum += cvRound(circles[i][2]);


		/*int centerpoints_sum_x(int x, size_t n);*/
		//int x = centerpoints_sum_x(cvRound(circles[i][0]),i);
		//int centerpoints_sum_x(int x, size_t n)
		//{
		//	int centerpoint_x = 0;			
		//	centerpoint_x = centerpoint_x + cvRound(circles[i][0]);
		//	return centerpoint_x/n;
		//}
	}
	imshow("【源视频】", srcImage);
	//waitKey(30);
	int xAver;
	int yAver;
	int rAver;
	if (cnt != 0) {
		xAver = xSum / cnt;
		yAver = ySum / cnt;
		rAver = rSum / cnt;
	}
	else {
		xAver = 0;
		yAver = 0;
		rAver = 0;
	}

	/*验证平均值点*/
	Point finalcenter;
	finalcenter.x = xAver;
	finalcenter.y = yAver;

	Point up;
	up.x = xAver;
	up.y = yAver - rAver;

	Point down;
	down.x = xAver;
	down.y = yAver + rAver;

	Point left;
	left.x = xAver - rAver;
	left.y = yAver;

	Point right;
	right.x = xAver + rAver - 4;
	right.y = yAver + 6;

	circle(point, finalcenter, 1, Scalar(255, 0, 0), -1); //圆心
	circle(point, up, 1, Scalar(255, 0, 0), -1); //上顶点
	circle(point, down, 1, Scalar(255, 0, 0), -1); //下顶点
	circle(point, left, 1, Scalar(255, 0, 0), -1); //左顶点
	circle(point, right, 1, Scalar(255, 0, 0), -1); //右顶点




	imshow("666", point);
	ushort * p_dmat = (ushort *)depthMat.data;
	
	int ans, ans2, ans3, ans4 = 1;

	ushort* data;
	data = depthMat.ptr<ushort>(up.y);
	ans = (int)data[up.x];

	data = depthMat.ptr<ushort>(down.y);
	ans2 = (int)data[down.x];

	data = depthMat.ptr<ushort>(left.y);
	ans3 = (int)data[left.x];

	data = depthMat.ptr<ushort>(right.y);
	ans4 = (int)data[right.x];


	string a = "环心坐标是：" + to_string(xAver) + "\t" + to_string(yAver) + "\t" + "半径是：" + to_string(rAver) + "\t" + to_string(ans) + "\t" + to_string(ans2) + "\t" + to_string(ans3) + "\t" + to_string(ans4);
	//OutputDebugString(L(a) );


	/*cout << "环心坐标是：" << xAver << "\t" << yAver << endl;
	cout << "半径是：" << rAver << endl;*/
	outfile << a << endl;
	//imshow("B", srcImage);
	//write2.write(srcImage);
	//BYTE* p_mat = srcImage.data;
}
void CColorBasics::ProcessDepth(const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{

	// Make sure we've received valid data
	if (m_pDepthRGBX && pBuffer && (nWidth == cDepthWidth) && (nHeight == cDepthHeight))
	{

		// Draw the data with Direct2D
		Mat srcImage = ConvertMat(pBuffer, nWidth, nHeight, nMinDepth, nMaxDepth);
		//【1】载入原始图、Mat变量定义   
		//imshow("ww", srcImage);
		//testDist(srcImage);
		catchBall(srcImage);
		


	}
}
void CColorBasics::catchBall(Mat srcImage) {
	Mat resultImage = srcImage.clone();

	std::vector<RotatedRect> regions = get_foreground_object(pBackgroundKnn, srcImage, 1, true);
	for (int i = 0; i < regions.size(); ++i) {
		if (regions[i].size.area() < 8 || regions[i].size.area() > 200)
			continue;
		if (regions[i].center.x > ringx+ringRangex || regions[i].center.y < ringy- ringRangey || regions[i].center.x < ringx - ringRangex || regions[i].center.y > ringy + ringRangey)
			continue;
		//if (nCount > 150 && (regions[i].center.y > 170 || regions[i].center.y < 120))
		//continue;
		/*if (regions[i].center.x > 350|| regions[i].center.x <150)
			continue;*/
		/*Point2f points[4];
		regions[i].points(points);
		Point2f maxP[2] = { Point2f(0, 0), Point2f(0, 0) };
		for (int j = 0; j < 4; ++j) {
			line(resultImage, points[j], points[(j + 1) % 4], Scalar(0, 255, 0), 3, 8);
			if (points[j].x > maxP[0].x) {
				maxP[1] = maxP[0];
				maxP[0] = points[j];
			}
			else if (points[j].x > maxP[1].x)
				maxP[1] = points[j];
		}*/
		ushort * p_dmat = (ushort *)depthMat.data;

		int ans = 1;

		ushort* data;
		data = depthMat.ptr<ushort>(regions[i].center.y);
		ans = (int)data[(int)regions[i].center.x]/8;

		if (mindis > abs(ans - distance)) {
			mindis = abs(ans - distance);
			passx = regions[i].center.x;
			passy = regions[i].center.y;
		}
		//std::cout << regions[i].center << std::endl;
		string a = "球心坐标：" + to_string(regions[i].center.x) + "," + to_string(regions[i].center.y) + "\t" +to_string(ans);
		outfile2 << a << endl;
	}
	//std::cout << nCount << std::endl;
	imshow("resultImage", resultImage);
	//write3.write(resultImage);
	//waitKey(40);
}



