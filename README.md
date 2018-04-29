  
# Required Packages
  
* GCC 5.4.x or later
* Cmake 3.5 or higher
* [OpenCV 3.0.0 or higher](https://docs.opencv.org/3.4.1/d7/d9f/tutorial_linux_install.html )
* [Intel® RealSense™ SDK 2.0](https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md )
  
# Program Construct(undone)
  

![](assets/ad.png?0.24737685638665918)  
# 程序简介
  
此程序用于参加2018robocon比赛的我校机器人中，是机器人的视觉程序，作为电控的辅助系统，进行手动和自动机器人对接，定位和抛球结果确定。
整个系统采用多线程设计。在主线程中进行通信，通过串口，主线程每隔固定时间发送信息，接收信息则是实时的，每次从串口读一个字节（API最小单位），同时对信息是否符合通信协议进行判断，包括数据头和求和位，当接收到满足一个协议包的数据集时，程序根据数据启动或关闭相应子线程，并通过下位机传来的数据改变状态。
  