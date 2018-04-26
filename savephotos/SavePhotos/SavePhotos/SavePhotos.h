#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SavePhotos.h"
#include <QMainWindow>
#include <QTimer>  //qt定时器的头文件
#include <cv.h>		//opencv头文件
#include <cxcore.h>  //opencv头文件
#include <highgui.h>  //opencv头文件
#include <QImage>   //qt图像
#include<QFile>   //文件操作
#include <QPainter>  //画图
#include<vector>  //vector容器
#include<math.h>   //数学运算
#include<opencv2/opencv.hpp>//opencv头文件
#include <iostream>  //c++输入输出流
using namespace std;
using namespace cv;

class SavePhotos : public QMainWindow
{
	Q_OBJECT

public:
	SavePhotos(QWidget *parent = Q_NULLPTR);
	
	
	QImage cvMat2QImage(const cv::Mat& mat);

private:
	Ui::SavePhotosClass ui;
	QTimer *timer; //定时器
	VideoCapture cap1;  //打开摄像头的头，摄像头1
	VideoCapture cap2;
	QImage img1;
	QImage img2;
	bool saveflag1; //保存图像的标志位
	bool saveflag2;

	char fileName1[256];
	int i = 0;
	char fileName2[256];
	int j = 0;
private slots:
	void FcnCallback_timer(); //定时器回调函数
	void on_pushButton_clicked();//摄像机打开按钮
	void on_pushButton_2_clicked();//close all按钮
	void on_Btn_Cam1_clicked();//savephoto按钮摄像机1
	void on_Btn_Cam2_clicked();
	void on_pushButton_3_clicked();
};
