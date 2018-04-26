#include "SavePhotos.h"

SavePhotos::SavePhotos(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);//�½�һ����ʱ��
	connect(timer, SIGNAL(timeout()), this, SLOT(FcnCallback_timer()));
	//���Ӷ�ʱ���Ͷ�ʱ���ص�����
	saveflag1 = false;//��ʼ������ͼ���־λ
	saveflag2 = false;
}


void SavePhotos::FcnCallback_timer()//��ʱ���ص�����
{
	Mat frame1;//opencv3��ͼ���ʽ
	Mat frame2;
	cap1 >> frame1;//ȡ��ǰһ֡ͼƬ��frame1��
	cap2 >> frame2;

	if (saveflag1)//���saveflagΪtrue���򱣴�
	{
		sprintf(fileName1, "left_%d.jpg", i); i++;//������Ϊleft_i��i��0��ʼ����
		imwrite(fileName1, frame1); saveflag1 = false;
	}
	if (saveflag2)
	{
		sprintf(fileName2, "right_%d.jpg", j); j++;
		imwrite(fileName2, frame2); saveflag2 = false;
	}
	Mat frame11, frame22;
	//cv::resize(frame1, frame11, Size(320, 240), 0, 0, INTER_LINEAR);
	//Ϊ�˱��ⴰ�ڹ��󣬽���ʾ�ڶԻ����е�ͼ���С��СΪԭͼһ��
	//cv::resize(frame2, frame22, Size(320, 240), 0, 0, INTER_LINEAR);
	//    resize(frame1,frame1,Size(320,240),(0,0),(0,0),INTER_LINEAR);
	//    resize(frame2,frame2,Size(320,240),(0,0),(0,0),INTER_LINEAR);
	img1 = cvMat2QImage(frame1);//��Mat��ʽͼ��ת��������qt����ʾ��Qimageͼ��
	img2 = cvMat2QImage(frame2);
	ui.C1->setPixmap(QPixmap::fromImage(img1));//����Ҫ��ͼ��ӳ����ui�Ŀؼ���
	ui.C2->setPixmap(QPixmap::fromImage(img2));
}

void SavePhotos::on_pushButton_clicked()//������򿪰�ť
{
	cap1.open(0);//�������
	cap2.open(1);
	timer->start(50);//���򿪶�ʱ������
}

void SavePhotos::on_pushButton_2_clicked()//close all��ť
{
	timer->stop();//�رն�ʱ��
	QApplication *app;//�رմ���
	app->quit();
}

void SavePhotos::on_Btn_Cam1_clicked()//savephoto��ť�����1
{
	saveflag1 = true;//��־λ����Ϊtrue
}

void SavePhotos::on_Btn_Cam2_clicked()//savephoto��ť�����2
{
	saveflag2 = true;
}

void SavePhotos::on_pushButton_3_clicked()//save all ��ť
{
	saveflag1 = true;
	saveflag2 = true;//��־λ����Ϊtrue
}


QImage SavePhotos::cvMat2QImage(const cv::Mat& mat)//ͼ���ʽת������
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