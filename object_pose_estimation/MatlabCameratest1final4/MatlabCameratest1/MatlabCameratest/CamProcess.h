#pragma once
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <Eigen\Dense>
#include <Eigen\Eigen>
#include <QImage>
using namespace cv;
class CamProcess
{
public:
	CamProcess();
	~CamProcess();

	void ImageProcess(Mat &frame);
	double measureData[16];
	double ChangeData[6];//����Pnp������������ģ�
	//����ڲ�
	
	Mat camera_matrix1;
	Mat distortion_coefficients1;
	Mat rvec1;
	Mat tvec1;
	double fx, u0, fy, v0;
	int filterNum; //��ȥǰfilterNum ֡ͼ����Ϣ
	QImage mImg;
	QImage CamImage;

private:
	struct ImgAndData
	{
		QImage mQImg;
		double* mData;
	};
	struct Coor
	{
		double x;
		double y;
	};
	double m_observe_z[16];//��������ͼ�����괫��������
	

	Mat frame;

	vector<Point3f> ObjCoor;

	
	ImgAndData ProcessImage(const Mat &frame);//��������ͼ�񣬵õ���������ͼ������ϵ������
	QImage cvMat2QImage1(const cv::Mat& mat); // ת��Ϊqt������ʾ��ͼ��
};

