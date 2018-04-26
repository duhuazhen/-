#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MatlabCameratest.h"
#include <qtimer.h>
#include <QtCore>
#include "qcustomplot.h"
#include "CamProcess.h"
#include <QImage>
#include <iostream>
#include "CallUKF.h"

#include "SixAxis.h"
using namespace std;
using namespace cv;
using namespace Eigen;

typedef Matrix<double, 3, 1> Matrix3x1;
typedef Matrix<double, 4, 4> Matrix4x4;
typedef Matrix<double, 3, 3> Matrix3x3;
typedef Matrix<double, 6, 1> Matrix6x1;
typedef Matrix<double, 1, 3> Matrix1x3;
class MatlabCameratest : public QMainWindow
{
	Q_OBJECT

public:
	MatlabCameratest(QWidget *parent = Q_NULLPTR);
	~MatlabCameratest();

protected slots:
	void btn_open();
	void timerUpDate();
	void ConnectSixAxis();
	void ReferenceSixAxis();
	void MoveToTarget();
	void Exit();
	void Track1();
	void picture1Clicked();
	void picture2Clicked();
	void Cam1RPoint(bool checked);
	void Cam1Pnp(bool checked);
	void Cam1Ukf(bool checked);
	void Cam2RPoint(bool checked);
	void Cam2Pnp(bool checked);
	void Cam2Ukf(bool checked);
private:
	Ui::MatlabCameratestClass ui;

	struct Vector3x1
	{
		Mat angles;
		Mat trans;
	};

	bool realDataCheck;
	bool cam1PnpCheck;
	bool cam1UkfCheck;
	bool real2DataCheck;
	bool cam2PnpCheck;
	bool cam2UkfCheck;


	QTimer *timer;
	VideoCapture cap1;
	VideoCapture cap2;
	Mat frame1;
	Mat frame2;
	bool Coordinate_correction;
	bool Coordinate_correction2;
	Matrix6x1 World2Cam(Mat angle, Mat translation);
	Matrix6x1 axisInCam(Mat angle, Mat translation);
	
	double ChangeData[6];

	QCustomPlot *mcustomPlotx;
	QCustomPlot *mcustomPloty;
	QCustomPlot *mcustomPlotz;
	QCustomPlot *mcustomPlota;
	QCustomPlot *mcustomPlotb;
	QCustomPlot *mcustomPlotc;
	double mtime;
	bool IsSixAxisOpen;
	int ii_2;
	int ii_3;
	SixAxis *SixAxis1;

	double dPos[6];

	Matrix4x4 T_3;
	Matrix4x4 T_3_1;
	CallUKF *mukf1;
	CallUKF *mukf2;
	CallUKF *mukfF1;
	CamProcess *mcam1;
	CamProcess *mcam2;
	Matrix6x1 Mcam1RealPoints(double* dPos, CamProcess *mcam);
	Matrix6x1 Mcam2RealPoints(double* dPos, CamProcess *mcam);
	void showPlot(const Matrix6x1 data,const int i);
	void trunRangeAndReplot();

	void InitCustomplot();
	void InitCameraIndex(CamProcess *mcam,double *index);
	Vector3x1 kalmanProcess(CamProcess *mcam, CallUKF *mukf);
	Matrix4x4 pose2Matrix(Matrix1x3 angle, Matrix3x1 translation);

	bool cam1TxtRemoveFlag;
	bool realPointTextRemoveFlag;
};
