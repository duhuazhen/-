#include "SavePhotos.h"

SavePhotos::SavePhotos(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);//新建一个定时器
	connect(timer, SIGNAL(timeout()), this, SLOT(FcnCallback_timer()));
	//连接定时器和定时器回调函数
	saveflag1 = false;//初始化保存图像标志位
	saveflag2 = false;
}


void SavePhotos::FcnCallback_timer()//定时器回调函数
{
	Mat frame1;//opencv3中图像格式
	Mat frame2;
	cap1 >> frame1;//取当前一帧图片到frame1中
	cap2 >> frame2;

	if (saveflag1)//如果saveflag为true，则保存
	{
		sprintf(fileName1, "left_%d.jpg", i); i++;//保存名为left_i，i从0开始计数
		imwrite(fileName1, frame1); saveflag1 = false;
	}
	if (saveflag2)
	{
		sprintf(fileName2, "right_%d.jpg", j); j++;
		imwrite(fileName2, frame2); saveflag2 = false;
	}
	Mat frame11, frame22;
	//cv::resize(frame1, frame11, Size(320, 240), 0, 0, INTER_LINEAR);
	//为了避免窗口过大，将显示在对话框中的图像大小缩小为原图一半
	//cv::resize(frame2, frame22, Size(320, 240), 0, 0, INTER_LINEAR);
	//    resize(frame1,frame1,Size(320,240),(0,0),(0,0),INTER_LINEAR);
	//    resize(frame2,frame2,Size(320,240),(0,0),(0,0),INTER_LINEAR);
	img1 = cvMat2QImage(frame1);//将Mat格式图像转换成能在qt中显示的Qimage图像
	img2 = cvMat2QImage(frame2);
	ui.C1->setPixmap(QPixmap::fromImage(img1));//将将要的图像映射在ui的控件上
	ui.C2->setPixmap(QPixmap::fromImage(img2));
}

void SavePhotos::on_pushButton_clicked()//摄像机打开按钮
{
	cap1.open(0);//打开摄像机
	cap2.open(1);
	timer->start(50);//并打开定时器开关
}

void SavePhotos::on_pushButton_2_clicked()//close all按钮
{
	timer->stop();//关闭定时器
	QApplication *app;//关闭窗口
	app->quit();
}

void SavePhotos::on_Btn_Cam1_clicked()//savephoto按钮摄像机1
{
	saveflag1 = true;//标志位设置为true
}

void SavePhotos::on_Btn_Cam2_clicked()//savephoto按钮摄像机2
{
	saveflag2 = true;
}

void SavePhotos::on_pushButton_3_clicked()//save all 按钮
{
	saveflag1 = true;
	saveflag2 = true;//标志位设置为true
}


QImage SavePhotos::cvMat2QImage(const cv::Mat& mat)//图像格式转换函数
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		std::cout << "CV_8UC4";
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		std::cout << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}