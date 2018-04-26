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
	double ChangeData[6];//保存Pnp运算结果（最初的）
	//相机内参
	
	Mat camera_matrix1;
	Mat distortion_coefficients1;
	Mat rvec1;
	Mat tvec1;
	double fx, u0, fy, v0;
	int filterNum; //滤去前filterNum 帧图像信息
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
	double m_observe_z[16];//将特征点图像坐标传到主程序
	

	Mat frame;

	vector<Point3f> ObjCoor;

	
	ImgAndData ProcessImage(const Mat &frame);//处理所得图像，得到特征点在图像坐标系下坐标
	QImage cvMat2QImage1(const cv::Mat& mat); // 转化为qt下能显示的图像
};

