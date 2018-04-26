
#include "MatlabCameratest.h"
QFile cam1File("cam1.txt");
QFile realPointFile("ralPointFile.txt");
QFile cam2File("cam2.txt");
QFile pnp1File("pnp1.txt");

MatlabCameratest::MatlabCameratest(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//设置定时器
	timer = new QTimer(this);
	//设置定时器与相应的槽函数
	connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
	//初始化坐标轴
	InitCustomplot();
	mtime = 0;

	IsSixAxisOpen = false;
	SixAxis1 = new SixAxis;

	mcam1 = new CamProcess;
	mcam2 = new CamProcess;
	mukf1 = new CallUKF;
	mukf2 = new CallUKF;
	mukfF1 = new CallUKF;
	mukf1->dbflag[0] = 1;
	mukf2->dbflag[0] = 2;
	mukfF1->dbflag[0] = 5;
	double fx1, fy1, u01, v01, coefficient1, coefficient2;
	double fx2, fy2, u02, v02, coefficient3, coefficient4;
	fx1 = 836.5512; fy1 = 831.9177; u01 = 313.9417; v01 = 266.0186;
	fx2 = 819.7424; fy2 = 815.8510; u02 = 367.4169; v02 = 233.321;
	double index1[6], index2[6];
	index1[0] = fx1; index1[1] = fy1; index1[2] = u01; index1[3] = v01; 
	index1[4] = coefficient1; index1[5] = coefficient2;
	index2[0] = fx2; index2[1] = fy2; index2[2] = u01; index2[3] = v01; 
	index2[4] = coefficient3; index2[5] = coefficient4;
	InitCameraIndex(mcam1, index1);
	InitCameraIndex(mcam2, index2);

	int ii_2 = 0;
	int ii_3 = 0;
	Coordinate_correction = true;
	Coordinate_correction2 = true;
	realDataCheck = true;
	cam1PnpCheck = true;
	cam1UkfCheck = false;
	real2DataCheck = false;
	cam2PnpCheck = false;
	cam2UkfCheck = true;

	cam1TxtRemoveFlag = true;
	realPointTextRemoveFlag = true;

}

MatlabCameratest::~MatlabCameratest()
{
	//timer->stop();
	//delete timer;
	//delete mukf1;
	//delete mukf2;
	//delete mcam1;
	//delete mcam2;

}
void MatlabCameratest::InitCustomplot()
{
	//建立6个坐标轴控件
	mcustomPlotx = new QCustomPlot;
	mcustomPlotx = ui.widget;
	mcustomPlotx->xAxis->setLabel("time");
	mcustomPlotx->yAxis->setLabel("x");
	//在坐标轴上面添加曲线
	mcustomPlotx->addGraph(); // blue line
	//mcustomPlotx->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,1));
	//mcustomPlotx->graph(0)->setLineStyle(QCPGraph::lsStepLeft);
	//mcustomPlotx->graph(0)->setPen(QPen(QColor(120, 120, 120), 2));
	mcustomPlotx->graph(0)->setPen(QPen(Qt::blue));
	mcustomPlotx->addGraph(); // blue line
	mcustomPlotx->graph(1)->setPen(QPen(Qt::red));
	mcustomPlotx->addGraph(); // blue line
	mcustomPlotx->graph(2)->setPen(QPen(Qt::black));
	mcustomPlotx->addGraph(); // blue line
	mcustomPlotx->graph(3)->setPen(QPen(Qt::darkRed));
	mcustomPlotx->addGraph(); // blue line
	mcustomPlotx->graph(4)->setPen(QPen(Qt::darkYellow));
	mcustomPlotx->addGraph(); // blue line
	mcustomPlotx->graph(5)->setPen(QPen(Qt::darkBlue));

	mcustomPloty = new QCustomPlot;
	mcustomPloty = ui.widget2;
	mcustomPloty->xAxis->setLabel("time");
	mcustomPloty->yAxis->setLabel("y");
	mcustomPloty->addGraph(); // blue line
	mcustomPloty->graph(0)->setPen(QPen(Qt::blue));
	mcustomPloty->addGraph(); // blue line
	mcustomPloty->graph(1)->setPen(QPen(Qt::red));
	mcustomPloty->addGraph(); // blue line
	mcustomPloty->graph(2)->setPen(QPen(Qt::black));
	mcustomPloty->addGraph(); // blue line
	mcustomPloty->graph(3)->setPen(QPen(Qt::darkRed));
	mcustomPloty->addGraph(); // blue line
	mcustomPloty->graph(4)->setPen(QPen(Qt::darkYellow));
	mcustomPloty->addGraph(); // blue line
	mcustomPloty->graph(5)->setPen(QPen(Qt::darkBlue));
	mcustomPlotz = new QCustomPlot;
	mcustomPlotz = ui.widget3;
	mcustomPlotz->xAxis->setLabel("time");
	mcustomPlotz->yAxis->setLabel("z");
	mcustomPlotz->addGraph(); // blue line
	mcustomPlotz->graph(0)->setPen(QPen(Qt::blue));
	mcustomPlotz->addGraph(); // blue line
	mcustomPlotz->graph(1)->setPen(QPen(Qt::red));
	mcustomPlotz->addGraph(); // blue line
	mcustomPlotz->graph(2)->setPen(QPen(Qt::black));
	mcustomPlotz->addGraph(); // blue line
	mcustomPlotz->graph(3)->setPen(QPen(Qt::darkRed));
	mcustomPlotz->addGraph(); // blue line
	mcustomPlotz->graph(4)->setPen(QPen(Qt::darkYellow));
	mcustomPlotz->addGraph(); // blue line
	mcustomPlotz->graph(5)->setPen(QPen(Qt::darkBlue));
	mcustomPlota = new QCustomPlot;
	mcustomPlota = ui.widget4;
	mcustomPlota->xAxis->setLabel("time");
	mcustomPlota->yAxis->setLabel("u");
	mcustomPlota->addGraph(); // blue line
	mcustomPlota->graph(0)->setPen(QPen(Qt::blue));
	mcustomPlota->addGraph(); // blue line
	mcustomPlota->graph(1)->setPen(QPen(Qt::red));
	mcustomPlota->addGraph(); // blue line
	mcustomPlota->graph(2)->setPen(QPen(Qt::black));
	mcustomPlota->addGraph(); // blue line
	mcustomPlota->graph(3)->setPen(QPen(Qt::darkRed));
	mcustomPlota->addGraph(); // blue line
	mcustomPlota->graph(4)->setPen(QPen(Qt::darkYellow));
	mcustomPlota->addGraph(); // blue line
	mcustomPlota->graph(5)->setPen(QPen(Qt::darkBlue));
	mcustomPlotb = new QCustomPlot;
	mcustomPlotb = ui.widget5;
	mcustomPlotb->xAxis->setLabel("time");
	mcustomPlotb->yAxis->setLabel("v");
	mcustomPlotb->addGraph(); // blue line
	mcustomPlotb->graph(0)->setPen(QPen(Qt::blue));
	mcustomPlotb->addGraph(); // blue line
	mcustomPlotb->graph(1)->setPen(QPen(Qt::red));
	mcustomPlotb->addGraph(); // blue line
	mcustomPlotb->graph(2)->setPen(QPen(Qt::black));
	mcustomPlotb->addGraph(); // blue line
	mcustomPlotb->graph(3)->setPen(QPen(Qt::darkRed));
	mcustomPlotb->addGraph(); // blue line
	mcustomPlotb->graph(4)->setPen(QPen(Qt::darkYellow));
	mcustomPlotb->addGraph(); // blue line
	mcustomPlotb->graph(5)->setPen(QPen(Qt::darkBlue));
	mcustomPlotc = new QCustomPlot;
	mcustomPlotc = ui.widget6;
	mcustomPlotc->xAxis->setLabel("time");
	mcustomPlotc->yAxis->setLabel("w");
	mcustomPlotc->addGraph(); // blue line
	mcustomPlotc->graph(0)->setPen(QPen(Qt::blue));
	mcustomPlotc->addGraph(); // blue line
	mcustomPlotc->graph(1)->setPen(QPen(Qt::red));
	mcustomPlotc->addGraph(); // blue line
	mcustomPlotc->graph(2)->setPen(QPen(Qt::black));
	mcustomPlotc->addGraph(); // blue line
	mcustomPlotc->graph(3)->setPen(QPen(Qt::darkRed));
	mcustomPlotc->addGraph(); // blue line
	mcustomPlotc->graph(4)->setPen(QPen(Qt::darkYellow));
	mcustomPlotc->addGraph(); // blue line
	mcustomPlotc->graph(5)->setPen(QPen(Qt::darkBlue));
}
void MatlabCameratest::InitCameraIndex(CamProcess *mcam,double *index)
{
	mcam->camera_matrix1.ptr<double>(0)[0] = index[0];
	mcam->camera_matrix1.ptr<double>(0)[2] = index[2];
	mcam->camera_matrix1.ptr<double>(1)[1] = index[1];
	mcam->camera_matrix1.ptr<double>(1)[2] = index[3];
	mcam->camera_matrix1.ptr<double>(2)[2] = 1.0;
	mcam->distortion_coefficients1.ptr<double>(0)[0] = index[4];
	mcam->distortion_coefficients1.ptr<double>(0)[1] = index[5];
	mcam->distortion_coefficients1.ptr<double>(0)[2] = 0;
	mcam->distortion_coefficients1.ptr<double>(0)[3] = 0;
	mcam->distortion_coefficients1.ptr<double>(0)[4] = 0;

}
void MatlabCameratest::Exit()
{
		timer->stop();
		SixAxis1->terminate();
		delete mukf1;
	    delete mukf2;
		delete mcam1;
		delete mcam2;
		delete SixAxis1;
		delete timer;
	//	SixAxis1->stop();
		QApplication *app;
		app->quit();
}
//点击open按钮，打开摄像头，进入定时器程序
void MatlabCameratest::btn_open()
{
	cap1.open(0);
	cap2.open(1);
	timer->start(50);
}

void MatlabCameratest::timerUpDate()
{
	//实时反馈六轴平台的位姿
	//在SixAxis类中设置六轴开启标志位，当标志位为true时才可以进入下面反馈位姿的程序
	if (IsSixAxisOpen)
	{
		//PI_qPOS用于获取轴的信息，具体信息可参考六轴函数手册，获取到的信息存放到dPos[6]中
		if (!PI_qPOS(SixAxis1->ID, "x y z u v w", dPos))
		{
			//如果获取失败的话，程序退出，这里以后应该加上try catch语句块，否则无法知道哪里出错
			QApplication *app;
			app->quit();
		}
		//将实时获取的六轴平台位姿显示在界面上的相应label控件中
		//qt的label控件上的文本是Qstring类型，QString QString::number ( double n, char f = 'g', int prec = 6 ) 
		//一个把数字n转换为字符串表示的方便制造函数,g为默认情况,小数点之后数字的个数等于prec指定的精度
		ui.position_x->setText(QString::number(dPos[0], 'g', 6));
		ui.position_y->setText(QString::number(dPos[1], 'g', 6));
		ui.position_z->setText(QString::number(dPos[2], 'g', 6));
		ui.position_u->setText(QString::number(dPos[3], 'g', 6));
		ui.position_v->setText(QString::number(dPos[4], 'g', 6));
		ui.position_w->setText(QString::number(dPos[5], 'g', 6));
	}
	//获取摄像头1的图像
	cap1 >> frame1;
	//获取摄像头2的图像
	cap2 >> frame2;
	//图像处理过程
	mcam1->ImageProcess(frame1);
	mcam2->ImageProcess(frame2);
	if (mcam1->filterNum == -2)
	{
		//获取通过摄像头1得到的图片的位姿，将欧拉角存放到cam1Angle中，将平移位置存放到cam1Trans中
		Mat cam1Angle = mcam1->rvec1;
		Mat cam1Trans = mcam1->tvec1;
		if (cam1TxtRemoveFlag)
		{
			QFile::remove("./pnp1.txt");
			cam1TxtRemoveFlag = false;
		}
		
		pnp1File.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream writetxt1(&pnp1File);
		writetxt1 << QString::number(mcam1->tvec1.ptr<double>(0)[0]) + "  " + QString::number(mcam1->tvec1.ptr<double>(0)[1]) + "  " +
			QString::number(mcam1->tvec1.ptr<double>(0)[2]) + "  " + QString::number((mcam1->rvec1.ptr<double>(0)[0]) * 180 / M_PI) + "  " +
			QString::number((mcam1->rvec1.ptr<double>(1)[0]) * 180 / M_PI) + "  " + QString::number((mcam1->rvec1.ptr<double>(2)[0]) * 180 / M_PI) + "  " << '\n';
		pnp1File.close();
		Mat cam2Angle = mcam2->rvec1;
		Mat cam2Trans = mcam2->tvec1;
		
		Mat ukfangle1 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		Mat ukftrans1 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		Mat ukfangle2 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		Mat ukftrans2 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		Mat ukfangleF1 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		Mat ukftransF1 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		Matrix6x1 ukf1Result, ukf2Result, cam1realpoint, cam2realpoint, pnp1Result, pnp2Result;
		//使用卡尔曼滤波进行处理，获取物体位姿
		Vector3x1 originalUKFResutl = kalmanProcess(mcam1, mukf1);
		Vector3x1 originalUKFResut2 = kalmanProcess(mcam2, mukf2);
		Vector3x1 originalUKFResutf = kalmanProcess(mcam1, mukfF1);
		ukfangle1 = originalUKFResutl.angles; ukftrans1 = originalUKFResutl.trans;
		ukfangle2 = originalUKFResut2.angles; ukftrans2 = originalUKFResut2.trans;
		ukfangleF1 = originalUKFResutf.angles; ukftransF1 = originalUKFResutf.trans;
		Matrix6x1 ukfF1Result;
		//将获取到的物体的位姿转换到与六轴平台对应的物体位姿上
		//ukf1Result = World2Cam(ukfangle1, ukftrans1);

		ukf1Result = axisInCam(ukfangle1, ukftrans1);
		//ukf2Result = World2Cam(ukfangle2, ukftrans2);
		ukf2Result = axisInCam(ukfangle2, ukftrans2);
		//pnp1Result = World2Cam(cam1Angle, cam1Trans);
		pnp1Result = axisInCam(cam1Angle, cam1Trans);
		pnp2Result = axisInCam(ukfangleF1, ukftransF1);
		//pnp2Result = World2Cam(cam2Angle, cam2Trans);
		//获取六轴平台的真实位姿
		cam1realpoint = Mcam1RealPoints( dPos, mcam1);
		cam2realpoint = Mcam2RealPoints(dPos, mcam2);
		//是否checkbox被选中，如果选中，显示相应的曲线
		if (cam1UkfCheck)
		{ 
			//画图
			showPlot(ukf1Result, 0);
		}
		if (cam2UkfCheck)
		{
			showPlot(ukf2Result, 1);
		}
		if (realDataCheck)
		{
			showPlot(cam1realpoint, 2);
		}
	
		if (cam1PnpCheck)
		{
			showPlot(pnp1Result, 3);
		}
		if (cam2PnpCheck)
		{
			showPlot(pnp2Result, 4);
		}
		if (real2DataCheck)
		{
			showPlot(cam2realpoint, 5);
		}
		//实时更新图像
		trunRangeAndReplot();
		//显示图像或者特征点
		if (ii_2 % 2 == 0)
		{
			ui.Cam1->setPixmap(QPixmap::fromImage(mcam1->CamImage));
			ui.Picture1->setText(tr("Dot"));
		}
		else
		{
			ui.Cam1->setPixmap(QPixmap::fromImage(mcam1->mImg));;
			ui.Picture1->setText(tr("Picture"));
		}

		if (ii_3 % 2 == 0)
		{
			ui.Cam2->setPixmap(QPixmap::fromImage(mcam2->CamImage));
			ui.Picture2->setText(tr("Dot"));
		}
		else
		{
			ui.Cam2->setPixmap(QPixmap::fromImage(mcam2->mImg));
			ui.Picture2->setText(tr("Picture"));
		}

	}

}
MatlabCameratest::Vector3x1 MatlabCameratest::kalmanProcess(CamProcess *mcam, CallUKF *mukf)
{
	//定义3x1的向量
	Vector3x1 mvector;
	//将摄像头获取到的观测信息，也就是特征点的坐标值存放到dbz数组中
	if (mukf->dbflag[0] < 3)
	{
		for (int i = 0; i < 16; i++)
		{
			mukf->dbz[i] = mcam->measureData[i];
		}
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			mukf->dbfusionz[i] = mcam1->measureData[i];
			mukf->dbfusionz[i+16] = mcam2->measureData[i];
		}
	}
	//使用一次使用solvepnp得到的位姿信息，以后通过迭代的方式
	if (mukf->xFlag)
	{
		for (int i = 0; i < 6; i++)
		{
			mukf->dbx[2 * i] = mcam1->ChangeData[i];
		}
		mukf->xFlag = false;
	}
	//进行卡尔曼滤波的处理
	mukf->UKFProcess();
	double ukfResultData[6];
	for (int i = 0; i < 6; i++)
	{
		ukfResultData[i] = mukf->dbx[2 * i];
	}
	Mat ukfangle = Mat(3, 1, CV_64FC1, Scalar::all(0));
	Mat ukftrans = Mat(3, 1, CV_64FC1, Scalar::all(0));

	for (int j = 0; j < 3; j++)
	{
		ukftrans.ptr<double>(j)[0] = ukfResultData[j];
	}
	ukfangle.ptr<double>(0)[0] = ukfResultData[5];
	ukfangle.ptr<double>(1)[0] = ukfResultData[4];
	ukfangle.ptr<double>(2)[0] = ukfResultData[3];
	//获取通过卡尔曼滤波得到的位姿信息
	mvector.angles = ukfangle;
	mvector.trans = ukftrans;
	return mvector;
}

void MatlabCameratest::showPlot(const Matrix6x1 data,const int i)
{
	mcustomPlotx->graph(i)->addData(mtime, data(0, 0));
	mcustomPloty->graph(i)->addData(mtime, data(1, 0));
	mcustomPlotz->graph(i)->addData(mtime, data(2, 0));
	mcustomPlota->graph(i)->addData(mtime, data(3, 0) * 180 / M_PI);
	mcustomPlotb->graph(i)->addData(mtime, data(4, 0) * 180 / M_PI);
	mcustomPlotc->graph(i)->addData(mtime, data(5, 0) * 180 / M_PI);

}
void MatlabCameratest::trunRangeAndReplot()
{
	mtime++;
	mcustomPlotx->xAxis->setRange(mtime, 200, Qt::AlignRight);
	mcustomPloty->xAxis->setRange(mtime, 200, Qt::AlignRight);
	mcustomPlotz->xAxis->setRange(mtime, 200, Qt::AlignRight);
	mcustomPlota->xAxis->setRange(mtime, 200, Qt::AlignRight);
	mcustomPlotb->xAxis->setRange(mtime, 200, Qt::AlignRight);
	mcustomPlotc->xAxis->setRange(mtime, 200, Qt::AlignRight);
	mcustomPlotx->yAxis->setRange(-250, 250);
	mcustomPloty->yAxis->setRange(-150, 150);
	mcustomPlotz->yAxis->setRange(200, 500);
	mcustomPlota->yAxis->setRange(-20, 20);
	mcustomPlotb->yAxis->setRange(-30, 30);
	mcustomPlotc->yAxis->setRange(-30, 30);
	mcustomPlotx->replot(); mcustomPloty->replot(); mcustomPlotz->replot();
	mcustomPlota->replot(); mcustomPlotb->replot(); mcustomPlotc->replot();
}
Matrix6x1 MatlabCameratest::World2Cam(Mat angle, Mat translation)
{
	Mat R_1 = Mat(3, 3, CV_64FC1, Scalar::all(0));
	//double a;
	//a = angle.ptr<double>(0)[0];
	//angle.ptr<double>(0)[0] = angle.ptr<double>(2)[0];
	//angle.ptr<double>(2)[0] = a;
	Rodrigues(angle, R_1);
	//cv::cv2eigen(R, r);
	Matrix4x4 T_1;
	T_1 << R_1.ptr<double>(0)[0], R_1.ptr<double>(0)[1], R_1.ptr<double>(0)[2], translation.ptr<double>(0)[0],
		R_1.ptr<double>(1)[0], R_1.ptr<double>(1)[1], R_1.ptr<double>(1)[2], translation.ptr<double>(1)[0],
		R_1.ptr<double>(2)[0], R_1.ptr<double>(2)[1], R_1.ptr<double>(2)[2], translation.ptr<double>(2)[0],
		0, 0, 0, 1;

	Mat recv_2 = Mat(3, 1, CV_64FC1, Scalar::all(0));
	recv_2.ptr<double>(0)[0] = 0.0467;
	recv_2.ptr<double>(1)[0] = 0.3884;
	recv_2.ptr<double>(2)[0] = -0.0375;
	Mat R_3 = Mat(3, 3, CV_64FC1, Scalar::all(0));
	Rodrigues(recv_2, R_3);
	Matrix3x3 R_4;
	R_4(0, 0) = R_3.ptr<double>(0)[0];
	R_4(0, 1) = R_3.ptr<double>(0)[1];
	R_4(0, 2) = R_3.ptr<double>(0)[2];
	R_4(1, 0) = R_3.ptr<double>(1)[0];
	R_4(1, 1) = R_3.ptr<double>(1)[1];
	R_4(1, 2) = R_3.ptr<double>(1)[2];
	R_4(2, 0) = R_3.ptr<double>(2)[0];
	R_4(2, 1) = R_3.ptr<double>(2)[1];
	R_4(2, 2) = R_3.ptr<double>(2)[2];

	Matrix4x4 T_2;
	T_2 << R_4(0, 0), R_4(0, 1), R_4(0, 2), -176,
		R_4(1, 0), R_4(1, 1), R_4(1, 2), 26,
		R_4(2, 0), R_4(2, 1), R_4(2, 2), 39,
		0, 0, 0, 1;

	Matrix4x4 T_3;
	T_3 = T_2*T_1;
	Mat R = Mat(3, 3, CV_64FC1, Scalar::all(0));
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			R.ptr<double>(i)[j] = T_3(i, j);
		}
	Mat Rvector = Mat(3, 1, CV_64FC1, Scalar::all(0));;
	Rodrigues(R, Rvector);
	Matrix6x1 vectorResult;
	vectorResult(0,0) = T_3(0, 3);
	vectorResult(1, 0) = T_3(1, 3);
	vectorResult(2, 0) = T_3(2, 3);
	vectorResult(3, 0) = Rvector.ptr<double>(0)[0];
	vectorResult(4, 0) = Rvector.ptr<double>(1)[0];
	vectorResult(5, 0) = Rvector.ptr<double>(2)[0];

	return vectorResult;
}

Matrix6x1 MatlabCameratest::axisInCam(Mat angle, Mat translation)
{

	Matrix6x1 vectorResult;
	vectorResult(0, 0) = translation.ptr<double>(0)[0];
	vectorResult(1, 0) = translation.ptr<double>(1)[0];
	vectorResult(2, 0) = translation.ptr<double>(2)[0];
	vectorResult(3, 0) = angle.ptr<double>(0)[0];
	vectorResult(4, 0) = angle.ptr<double>(1)[0];
	vectorResult(5, 0) = angle.ptr<double>(2)[0];
	return vectorResult;
}

void MatlabCameratest::ConnectSixAxis()
{

	try
	{
		SixAxis1->Connect();
		IsSixAxisOpen = true;
		ui.Reference->setEnabled(true); 
		ui.MoveToTarget->setEnabled(true);
		ui.Track1->setEnabled(true);
	}
	catch (int)                                    //捕获并处理异常
		  {
		       cerr << "error of dividing zero.\n";
				 QApplication *app;
				 app->quit();                                   //异常退出程序
		   }
	if (SixAxis1->SixAxisConnectFlag)
	{
		ui.label->setText("yes");
	}

	

}

void MatlabCameratest::ReferenceSixAxis()
{

	if (SixAxis1->ReferenceIfNeeded(SixAxis1->ID, "x") == false)
	{
		//printf("Not referenced, Referencing failed.\n");
		QApplication *app;
		app->quit();
	}
	
}

void MatlabCameratest::MoveToTarget()
{
	QString str = ui.zposition_x->text();
	QByteArray str_1 = str.toLatin1();
	char *str_1_1 = str_1.data();
	double zpositionx = atoi(str_1_1);

	QString str2 = ui.zposition_y->text();
	QByteArray str_2 = str2.toLatin1();
	char *str_2_2 = str_2.data();
	double zpositiony = atoi(str_2_2);

	QString str3 = ui.zposition_z->text();
	QByteArray str_3 = str3.toLatin1();
	char *str_3_3 = str_3.data();
	double zpositionz = atoi(str_3_3);

	QString str4 = ui.zposition_u->text();
	QByteArray str_4 = str4.toLatin1();
	char *str_4_4 = str_4.data();
	double zpositionu = atoi(str_4_4);

	QString str5 = ui.zposition_v->text();
	QByteArray str_5 = str5.toLatin1();
	char *str_5_5 = str_5.data();
	double zpositionv = atoi(str_5_5);

	QString str6 = ui.zposition_w->text();
	QByteArray str_6 = str6.toLatin1();
	char *str_6_6 = str_6.data();
	double zpositionw = atoi(str_6_6);

	double xyz[6] = { zpositionx, zpositiony, zpositionz, zpositionu, zpositionv, zpositionw };

	if (!SixAxis1->MoveTo(SixAxis1->ID,"x y z u v w", xyz))
	{
		QApplication *app;
		app->quit();
	}
}
void MatlabCameratest::Track1()
{
	SixAxis1->SixAxisFlag = true;
	SixAxis1->start();

}

void MatlabCameratest::picture1Clicked()
{
	ii_2++;
}
void MatlabCameratest::picture2Clicked()
{

	ii_3++;
}

Matrix6x1 MatlabCameratest::Mcam1RealPoints(double* dPos,CamProcess *mcam)
{
	Mat cam1Angle_1;
	Mat cam1Trans_1;
	if (Coordinate_correction)
	{
		 cam1Angle_1 = mcam->rvec1;
		 cam1Trans_1 = mcam->tvec1;
		 Mat R_1_1 = Mat(3, 3, CV_64FC1, Scalar::all(0));
		 Rodrigues(cam1Angle_1, R_1_1);
		 Matrix4x4 T_1_1;
		 T_1_1 << R_1_1.ptr<double>(0)[0], R_1_1.ptr<double>(0)[1], R_1_1.ptr<double>(0)[2], cam1Trans_1.ptr<double>(0)[0],
			 R_1_1.ptr<double>(1)[0], R_1_1.ptr<double>(1)[1], R_1_1.ptr<double>(1)[2], cam1Trans_1.ptr<double>(1)[0],
			 R_1_1.ptr<double>(2)[0], R_1_1.ptr<double>(2)[1], R_1_1.ptr<double>(2)[2], cam1Trans_1.ptr<double>(2)[0],
			 0, 0, 0, 1;

		 Mat recv_2 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		 recv_2.ptr<double>(0)[0] = M_PI / 2;
		 recv_2.ptr<double>(1)[0] = 0;
		 recv_2.ptr<double>(2)[0] = 0;
		 Mat R_3 = Mat(3, 3, CV_64FC1, Scalar::all(0));
		 Rodrigues(recv_2, R_3);
		 Matrix3x3 R_4;
		 R_4(0, 0) = R_3.ptr<double>(0)[0];
		 R_4(0, 1) = R_3.ptr<double>(0)[1];
		 R_4(0, 2) = R_3.ptr<double>(0)[2];
		 R_4(1, 0) = R_3.ptr<double>(1)[0];
		 R_4(1, 1) = R_3.ptr<double>(1)[1];
		 R_4(1, 2) = R_3.ptr<double>(1)[2];
		 R_4(2, 0) = R_3.ptr<double>(2)[0];
		 R_4(2, 1) = R_3.ptr<double>(2)[1];
		 R_4(2, 2) = R_3.ptr<double>(2)[2];

		 Matrix4x4 T_2;
		 T_2 << R_4(0, 0), R_4(0, 1), R_4(0, 2), 0.001,
			 R_4(1, 0), R_4(1, 1), R_4(1, 2), 76.35,
			 R_4(2, 0), R_4(2, 1), R_4(2, 2), 42,
			 0, 0, 0, 1;

		 
		 T_3 = T_1_1*T_2;
		 Coordinate_correction = false;
	}


	//获取六轴控制在世界坐标系下的位姿，并装换为4x4矩阵
	Matrix3x1 positionObject;
	//a << -0.40072, 0.31619, -2.15179;
	Matrix1x3 angleObject;
	positionObject << dPos[0], 
		dPos[1], 
		dPos[2];
	angleObject << dPos[3], dPos[4], dPos[5];
	Matrix4x4 object2World;
	object2World = pose2Matrix(angleObject, positionObject);
	//获取物体在相机坐标系下的位姿，以4x4矩阵来表示
	Matrix4x4 objectInCamera;
	objectInCamera = T_3*object2World;
	//将物体在相机坐标系下的矩阵形式转换为6个自由度的形式

	Mat recv_2_2 = Mat(3, 1, CV_64FC1, Scalar::all(0));
	recv_2_2.ptr<double>(0)[0] = -M_PI / 2;
	recv_2_2.ptr<double>(1)[0] = 0;
	recv_2_2.ptr<double>(2)[0] = 0;
	Mat R_3_3 = Mat(3, 3, CV_64FC1, Scalar::all(0));
	Rodrigues(recv_2_2, R_3_3);
	Matrix3x3 R_4_4;
	R_4_4(0, 0) = R_3_3.ptr<double>(0)[0];
	R_4_4(0, 1) = R_3_3.ptr<double>(0)[1];
	R_4_4(0, 2) = R_3_3.ptr<double>(0)[2];
	R_4_4(1, 0) = R_3_3.ptr<double>(1)[0];
	R_4_4(1, 1) = R_3_3.ptr<double>(1)[1];
	R_4_4(1, 2) = R_3_3.ptr<double>(1)[2];
	R_4_4(2, 0) = R_3_3.ptr<double>(2)[0];
	R_4_4(2, 1) = R_3_3.ptr<double>(2)[1];
	R_4_4(2, 2) = R_3_3.ptr<double>(2)[2];

	Matrix4x4 T_2_2;
	T_2_2 << R_4_4(0, 0), R_4_4(0, 1), R_4_4(0, 2), 0,
		R_4_4(1, 0), R_4_4(1, 1), R_4_4(1, 2), -42,
		R_4_4(2, 0), R_4_4(2, 1), R_4_4(2, 2), 76.35,
		0, 0, 0, 1;
	Matrix4x4 object2Camera_1;
	object2Camera_1 = objectInCamera*T_2_2;
	//将物体在相机坐标系下的矩阵形式转换为6个自由度的形式
	Mat objectRotationMatrix = Mat(3, 3, CV_64FC1, Scalar::all(0));
	objectRotationMatrix.ptr<double>(0)[0] = object2Camera_1(0, 0);
	objectRotationMatrix.ptr<double>(0)[1] = object2Camera_1(0, 1);
	objectRotationMatrix.ptr<double>(0)[2] = object2Camera_1(0, 2);
	objectRotationMatrix.ptr<double>(1)[0] = object2Camera_1(1, 0);
	objectRotationMatrix.ptr<double>(1)[1] = object2Camera_1(1, 1);
	objectRotationMatrix.ptr<double>(1)[2] = object2Camera_1(1, 2);
	objectRotationMatrix.ptr<double>(2)[0] = object2Camera_1(2, 0);
	objectRotationMatrix.ptr<double>(2)[1] = object2Camera_1(2, 1);
	objectRotationMatrix.ptr<double>(2)[2] = object2Camera_1(2, 2);
	Mat theta_3 = Mat(1, 3, CV_64FC1, Scalar::all(0));
	Rodrigues(objectRotationMatrix, theta_3);
	Matrix6x1 vectorResult;
	vectorResult(0, 0) = object2Camera_1(0, 3);
	vectorResult(1, 0) = object2Camera_1(1, 3);
	vectorResult(2, 0) = object2Camera_1(2, 3);
	vectorResult(3, 0) = theta_3.ptr<double>(0)[0];
	vectorResult(4, 0) = theta_3.ptr<double>(0)[1];
	vectorResult(5, 0) = theta_3.ptr<double>(0)[2];

	if (realPointTextRemoveFlag)
	{
		QFile::remove("./ralPointFile.txt");
		realPointTextRemoveFlag = false;
	}
	realPointFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream writetxt12(&realPointFile);
	writetxt12 << QString::number(vectorResult(0, 0)) + "  " + QString::number(vectorResult(1, 0)) + "  " +
		QString::number(vectorResult(2, 0)) + "  " + QString::number(vectorResult(3, 0)*180/M_PI) + "  " +
		QString::number(vectorResult(4, 0) * 180 / M_PI) + "  " + QString::number(vectorResult(5, 0) * 180 / M_PI) + "  " << '\n';
	realPointFile.close();


	return  vectorResult;
}


Matrix6x1 MatlabCameratest::Mcam2RealPoints(double* dPos, CamProcess *mcam)
{

	Mat cam2Angle_1;
	Mat cam2Trans_1;
	if (Coordinate_correction2)
	{
		cam2Angle_1 = mcam->rvec1;
		cam2Trans_1 = mcam->tvec1;
		Mat R_1_1 = Mat(3, 3, CV_64FC1, Scalar::all(0));
		Rodrigues(cam2Angle_1, R_1_1);
		Matrix4x4 T_1_1;
		T_1_1 << R_1_1.ptr<double>(0)[0], R_1_1.ptr<double>(0)[1], R_1_1.ptr<double>(0)[2], cam2Trans_1.ptr<double>(0)[0],
			R_1_1.ptr<double>(1)[0], R_1_1.ptr<double>(1)[1], R_1_1.ptr<double>(1)[2], cam2Trans_1.ptr<double>(1)[0],
			R_1_1.ptr<double>(2)[0], R_1_1.ptr<double>(2)[1], R_1_1.ptr<double>(2)[2], cam2Trans_1.ptr<double>(2)[0],
			0, 0, 0, 1;

		Mat recv_2 = Mat(3, 1, CV_64FC1, Scalar::all(0));
		recv_2.ptr<double>(0)[0] = M_PI / 2;
		recv_2.ptr<double>(1)[0] = 0;
		recv_2.ptr<double>(2)[0] = 0;
		Mat R_3 = Mat(3, 3, CV_64FC1, Scalar::all(0));
		Rodrigues(recv_2, R_3);
		Matrix3x3 R_4;
		R_4(0, 0) = R_3.ptr<double>(0)[0];
		R_4(0, 1) = R_3.ptr<double>(0)[1];
		R_4(0, 2) = R_3.ptr<double>(0)[2];
		R_4(1, 0) = R_3.ptr<double>(1)[0];
		R_4(1, 1) = R_3.ptr<double>(1)[1];
		R_4(1, 2) = R_3.ptr<double>(1)[2];
		R_4(2, 0) = R_3.ptr<double>(2)[0];
		R_4(2, 1) = R_3.ptr<double>(2)[1];
		R_4(2, 2) = R_3.ptr<double>(2)[2];

		Matrix4x4 T_2;
		T_2 << R_4(0, 0), R_4(0, 1), R_4(0, 2), 0.001,
			R_4(1, 0), R_4(1, 1), R_4(1, 2), 42,
			R_4(2, 0), R_4(2, 1), R_4(2, 2), 76.35,
			0, 0, 0, 1;


		T_3_1 = T_1_1*T_2;
		Coordinate_correction2 = false;
	}


	//获取六轴控制在世界坐标系下的位姿，并装换为4x4矩阵
	Matrix3x1 positionObject;
	//a << -0.40072, 0.31619, -2.15179;
	Matrix1x3 angleObject;
	positionObject << dPos[0],
		dPos[1],
		dPos[2];
	angleObject << dPos[3], dPos[4], dPos[5];
	Matrix4x4 object2World;
	object2World = pose2Matrix(angleObject, positionObject);
	//获取物体在相机坐标系下的位姿，以4x4矩阵来表示
	Matrix4x4 objectInCamera;
	objectInCamera = T_3_1*object2World;
	//将物体在相机坐标系下的矩阵形式转换为6个自由度的形式
	Mat objectRotationMatrix = Mat(3, 3, CV_64FC1, Scalar::all(0));
	objectRotationMatrix.ptr<double>(0)[0] = objectInCamera(0, 0);
	objectRotationMatrix.ptr<double>(0)[1] = objectInCamera(0, 1);
	objectRotationMatrix.ptr<double>(0)[2] = objectInCamera(0, 2);
	objectRotationMatrix.ptr<double>(1)[0] = objectInCamera(1, 0);
	objectRotationMatrix.ptr<double>(1)[1] = objectInCamera(1, 1);
	objectRotationMatrix.ptr<double>(1)[2] = objectInCamera(1, 2);
	objectRotationMatrix.ptr<double>(2)[0] = objectInCamera(2, 0);
	objectRotationMatrix.ptr<double>(2)[1] = objectInCamera(2, 1);
	objectRotationMatrix.ptr<double>(2)[2] = objectInCamera(2, 2);
	Mat theta_3 = Mat(1, 3, CV_64FC1, Scalar::all(0));
	Rodrigues(objectRotationMatrix, theta_3);
	Matrix6x1 vectorResult;
	vectorResult(0, 0) = objectInCamera(0, 3);
	vectorResult(1, 0) = objectInCamera(1, 3);
	vectorResult(2, 0) = objectInCamera(2, 3);
	vectorResult(3, 0) = theta_3.ptr<double>(0)[0];
	vectorResult(4, 0) = theta_3.ptr<double>(0)[1];
	vectorResult(5, 0) = theta_3.ptr<double>(0)[2];
	return  vectorResult;
}

Matrix4x4 MatlabCameratest::pose2Matrix(Matrix1x3 angle, Matrix3x1 translation)
{

	Matrix1x3 radian1;
	radian1 = angle*M_PI / 180;
	Mat recv_1 = Mat(1, 3, CV_64FC1, Scalar::all(0));
	Mat R_1 = Mat(3, 3, CV_64FC1, Scalar::all(0));
	recv_1.ptr<double>(0)[0] = radian1(0, 0);
	recv_1.ptr<double>(0)[1] = radian1(0, 1);
	recv_1.ptr<double>(0)[2] = radian1(0, 2);
	Rodrigues(recv_1, R_1);


	Matrix3x3 R_2;
	R_2(0, 0) = R_1.ptr<double>(0)[0];
	R_2(0, 1) = R_1.ptr<double>(0)[1];
	R_2(0, 2) = R_1.ptr<double>(0)[2];
	R_2(1, 0) = R_1.ptr<double>(1)[0];
	R_2(1, 1) = R_1.ptr<double>(1)[1];
	R_2(1, 2) = R_1.ptr<double>(1)[2];
	R_2(2, 0) = R_1.ptr<double>(2)[0];
	R_2(2, 1) = R_1.ptr<double>(2)[1];
	R_2(2, 2) = R_1.ptr<double>(2)[2];
	//cv2eigen
	//cv::cv2eigen(R, r);
	Matrix4x4 T_1;
	T_1 << R_2(0, 0), R_2(0, 1), R_2(0, 2), translation(0, 0),
		R_2(1, 0), R_2(1, 1), R_2(1, 2), translation(1, 0),
		R_2(2, 0), R_2(2, 1), R_2(2, 2), translation(2, 0),
		0, 0, 0, 1;
	return T_1;
}

void  MatlabCameratest::Cam1RPoint(bool checked)
{
	 realDataCheck = checked;

}
void  MatlabCameratest::Cam1Pnp(bool checked)
{

	 cam1PnpCheck = checked;
}
void  MatlabCameratest::Cam1Ukf(bool checked)
{

	 cam1UkfCheck = checked;

}
void  MatlabCameratest::Cam2RPoint(bool checked)
{

	 real2DataCheck = checked;

}
void  MatlabCameratest::Cam2Pnp(bool checked)
{

	 cam2PnpCheck = checked;

}
void  MatlabCameratest::Cam2Ukf(bool checked)
{

	 cam2UkfCheck = checked;
}