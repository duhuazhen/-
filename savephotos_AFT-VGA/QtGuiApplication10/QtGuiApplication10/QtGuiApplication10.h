#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication10.h"
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include <iostream>
#include <sstream> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <qtimer.h>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;
using namespace cv;

class QtGuiApplication10 : public QMainWindow
{
	Q_OBJECT

public:
	QTimer *timer;
	CameraList camList;
	SystemPtr system;
	int result = 0;
	QtGuiApplication10(QWidget *parent = Q_NULLPTR);
	~QtGuiApplication10();
	void AcquireImage(CameraPtr pCam, INodeMap & nodeMap);
	int AcquireImages(CameraList camList);
	int RunSingleCamera(CameraPtr pCam);
	int RunMultipleCameras(CameraList camList);
	int PrintDeviceInfo(INodeMap & nodeMap);
	int ConfigureTrigger(INodeMap & nodeMap);
	QImage  ConvertToCVmat(ImagePtr pImage);
	QImage GetImage(CameraPtr pCam);
	Mat cvimg;
	QImage cvMat2QImage1(const cv::Mat& mat); // 转化为qt下能显示的图像
	
private:
	Ui::QtGuiApplication10Class ui;
	void Initialize();

	bool saveflag1; //保存图像的标志位
	bool saveflag2;

	QString fileName1;
	int i = 0;
	QString fileName2;
	int j = 0;

private slots:
	void slot1();
	void timerUpDate();
	void on_pushButton_2_clicked();//close all按钮
	void on_Btn_Cam1_clicked();//savephoto按钮摄像机1
	void on_Btn_Cam2_clicked();
	void on_pushButton_3_clicked();
};
