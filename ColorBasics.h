//------------------------------------------------------------------------------
// <copyright file="ColorBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "resource.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>  
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class CColorBasics
{
    static const int        cColorWidth  = 1920;
    static const int        cColorHeight = 1080;
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    CColorBasics();

    /// <summary>
    /// Destructor
    /// </summary>
    ~CColorBasics();

    /// <summary>
    /// Handles window messages, passes most to the class instance to handle
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Handle windows messages for a class instance
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Creates the main window and begins processing
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
    int                     Run(HINSTANCE hInstance, int nCmdShow);

private:
	//��Ƶд�����  
	//VideoWriter write1;
	//VideoWriter write2;
	VideoWriter write3;
	//д����Ƶ�ļ���  
	Mat point;
	int xSum = 0;//x�����
	int ySum = 0;
	int rSum = 0;
	int cnt = 0;
	int mindis = 100000;
	float passx = 0;
	float passy = 0;
	int kvalue = 15;//˫���˲������С  
	string outFlie = "E://color.avi";
	string outFlie2 = "E://depth.avi";
	string outFlie3 = "E://test.avi";
	Mat ConvertMat(const RGBQUAD* pBuffer, int nWidth, int nHeight);
	Mat ConvertMat(const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth);
    HWND                    m_hWnd;
    INT64                   m_nLastCounter;
    INT64                   m_nNextStatusTime;
    DWORD                   m_nFramesSinceUpdate;
	Mat depthMat;
	Ptr<BackgroundSubtractorMOG2> pBackgroundKnn;
	long nCount;
	void testDist(Mat src);
	void catchBall(Mat src);
	vector<RotatedRect>  get_foreground_object(Ptr<BackgroundSubtractorMOG2> pBackgroundKnn, Mat scene, double scale, bool isFlag);
	ofstream outfile;
	ofstream outfile2;

    // Current Kinect
    IKinectSensor*          m_pKinectSensor;

    // Color reader
    IColorFrameReader*      m_pColorFrameReader;

    // Direct2D
    ID2D1Factory*           m_pD2DFactory;
    RGBQUAD*                m_pColorRGBX;

	// Depth reader
	IDepthFrameReader*      m_pDepthFrameReader;

	// Direct2D
	RGBQUAD*                m_pDepthRGBX;

    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Initializes the default Kinect sensor
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 InitializeDefaultSensor();

    /// <summary>
    /// Handle new color data
    /// <param name="pBuffer">pointer to frame data</param>
    /// <param name="nWidth">width (in pixels) of input image data</param>
    /// <param name="nHeight">height (in pixels) of input image data</param>
    /// </summary>
    void                    ProcessColor( RGBQUAD* pBuffer, int nWidth, int nHeight);
	/// <summary>
	/// Handle new depth data
	/// <param name="pBuffer">pointer to frame data</param>
	/// <param name="nWidth">width (in pixels) of input image data</param>
	/// <param name="nHeight">height (in pixels) of input image data</param>
	/// <param name="nMinDepth">minimum reliable depth</param>
	/// <param name="nMaxDepth">maximum reliable depth</param>
	/// </summary>
	void                    ProcessDepth( const UINT16* pBuffer, int nHeight, int nWidth, USHORT nMinDepth, USHORT nMaxDepth);



};

