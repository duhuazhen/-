#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SavePhotos.h"
#include <QMainWindow>
#include <QTimer>  //qt��ʱ����ͷ�ļ�
#include <cv.h>		//opencvͷ�ļ�
#include <cxcore.h>  //opencvͷ�ļ�
#include <highgui.h>  //opencvͷ�ļ�
#include <QImage>   //qtͼ��
#include<QFile>   //�ļ�����
#include <QPainter>  //��ͼ
#include<vector>  //vector����
#include<math.h>   //��ѧ����
#include<opencv2/opencv.hpp>//opencvͷ�ļ�
#include <iostream>  //c++���������
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
	QTimer *timer; //��ʱ��
	VideoCapture cap1;  //������ͷ��ͷ������ͷ1
	VideoCapture cap2;
	QImage img1;
	QImage img2;
	bool saveflag1; //����ͼ��ı�־λ
	bool saveflag2;

	char fileName1[256];
	int i = 0;
	char fileName2[256];
	int j = 0;
private slots:
	void FcnCallback_timer(); //��ʱ���ص�����
	void on_pushButton_clicked();//������򿪰�ť
	void on_pushButton_2_clicked();//close all��ť
	void on_Btn_Cam1_clicked();//savephoto��ť�����1
	void on_Btn_Cam2_clicked();
	void on_pushButton_3_clicked();
};
