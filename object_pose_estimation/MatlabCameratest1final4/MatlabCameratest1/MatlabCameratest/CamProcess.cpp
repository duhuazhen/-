#include "CamProcess.h"


CamProcess::CamProcess()
{
	for (int i = 0; i++; i < 16)
		measureData[i] = 0;
	filterNum = 10;

	//����ڲγ�ʼ��
	ObjCoor.push_back(Point3f(-35, -80, 0));
	ObjCoor.push_back(Point3f(35, -80, 0));
	ObjCoor.push_back(Point3f(35, -10, 0));
	ObjCoor.push_back(Point3f(-35, -10, 0));
	ObjCoor.push_back(Point3f(-20, -65, 0));
	ObjCoor.push_back(Point3f(20, -65, 0));
	ObjCoor.push_back(Point3f(20, -25, 0));
	ObjCoor.push_back(Point3f(-20, -25, 0));

	camera_matrix1 = Mat(3, 3, CV_64FC1, Scalar::all(0));
	distortion_coefficients1 = Mat(1, 5, CV_64FC1, Scalar::all(0));

}


CamProcess::~CamProcess()
{
}

void CamProcess::ImageProcess(Mat &frame)
{
	//�ж�ͼƬ�Ƿ�Ϊ��
	if (!frame.empty())
	{
		//�˳�ǰ5��ͼƬ����ֹǰ�漸֡ͼƬ��ȡ����
		if (filterNum < 0)
		{
			Mat showframe;
			//��ͼƬ��С��������Ϊ240x180��С
			resize(frame, showframe, Size(240, 180), 0, 0, INTER_LINEAR);
			//��mat��ʽ��ͼƬתΪQimage��ʽ
			CamImage = cvMat2QImage1(showframe);
			filterNum = -2;
			//��ͼƬ����ͼ����
			ImgAndData ImgDatatemp = ProcessImage(frame);
			mImg = ImgDatatemp.mQImg;
			vector<Point2f> ImgCoor1;
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[12], ImgDatatemp.mData[13]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[4], ImgDatatemp.mData[5]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[0], ImgDatatemp.mData[1]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[8], ImgDatatemp.mData[9]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[14], ImgDatatemp.mData[15]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[6], ImgDatatemp.mData[7]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[2], ImgDatatemp.mData[3]));
			ImgCoor1.push_back(Point2d(ImgDatatemp.mData[10], ImgDatatemp.mData[11]));
			//����֪����������;���ͼ����õ���ͼ���������solvepnp�����У����Ŀ���λ��
			solvePnP(ObjCoor, ImgCoor1, camera_matrix1, distortion_coefficients1, rvec1, tvec1, false, CV_EPNP);
			/*Mat VectorR = Mat(3, 1, CV_64FC1);
			Rodrigues(rvec1, VectorR);*/
			/*Matrix6x1 lastResult;
			lastResult = World2Cam(rvec1, tvec1);*/
			//����õ�λ�˴�ŵ�ChangeData������
			ChangeData[0] = tvec1.ptr<double>(0)[0];
			ChangeData[1] = tvec1.ptr<double>(0)[1];
			ChangeData[2] = tvec1.ptr<double>(0)[2];
			ChangeData[3] = rvec1.ptr<double>(2)[0];
			ChangeData[4] = rvec1.ptr<double>(1)[0];
			ChangeData[5] = rvec1.ptr<double>(0)[0];
			
			measureData[0] = m_observe_z[12]; measureData[1] = m_observe_z[13];
			measureData[2] = m_observe_z[4]; measureData[3] = m_observe_z[5];
			measureData[4] = m_observe_z[0]; measureData[5] = m_observe_z[1];
			measureData[6] = m_observe_z[8]; measureData[7] = m_observe_z[9];
			measureData[8] = m_observe_z[14]; measureData[9] = m_observe_z[15];
			measureData[10] = m_observe_z[6]; measureData[11] = m_observe_z[7];
			measureData[12] = m_observe_z[2]; measureData[13] = m_observe_z[3];
			measureData[14] = m_observe_z[10]; measureData[15] = m_observe_z[11];
		}
		else
			filterNum--;
	}
}
CamProcess::ImgAndData CamProcess::ProcessImage(const Mat &frame)//��ȡ������ͼ������ϵ������
{
	Mat hsvImage;
	QImage  QImg;
	ImgAndData mreturn;
	Mat frame1;

	cv::cvtColor(frame, hsvImage, CV_BGR2HSV);//��ͼ���ʽת��Ϊhsv
	cv::erode(hsvImage, hsvImage, Mat());//��ʴͼ��
	cv::dilate(hsvImage, hsvImage, Mat());//����ͼ��
	cv::erode(hsvImage, hsvImage, Mat());
	inRange(hsvImage, Scalar(110, 50, 0), Scalar(135, 255, 255), hsvImage);
	//ѡȡ�̶���ɫ���䣬��ֵ��ͼ��
	//Mat resizeImage;
	//resize(hsvImage, resizeImage, Size(320, 240), 0, 0, INTER_LINEAR);
	//QImg = cvMat2QImage1(resizeImage);
	Mat GrayImage;
	GrayImage = hsvImage;
	std::vector<std::vector<Point>> contours;
	//    std::vector<Vec4i> hierarchy;
	findContours(hsvImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	//��ȡ����
	Mat image2;
	image2 = GrayImage;
	Mat ResultImage(hsvImage.size(), CV_8U, Scalar(255));
	std::vector<std::vector<Point>>::iterator itc = contours.begin();
	unsigned int cmin = 70;
	unsigned int cmax = 500;
	//ѡ������������С�����䣬ȥ��������С������
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}
	float radius;
	Point2f center;
	Coor temp;
	Coor temp1;
	Coor temp2;
	Coor temp3;
	Coor temp4;
	//����5���������ڴ�Ž������������Ϣ��ʹ����vector����
	std::vector<Coor> mydata;
	std::vector<Coor> mydata1;
	std::vector<Coor> mydata2;
	std::vector<Coor> mydata3;
	std::vector<Coor> mydata4;
	if (contours.size() > 0)
	{
		minEnclosingCircle(contours[0], center, radius);
		//��Բȥ�ƽ���ɸѡ��������ȷ������Բ
	}

	itc = contours.begin();
	mydata.erase(mydata.begin(), mydata.end());
	while (itc != contours.end())
	{
		Moments mom = cv::moments(Mat(*itc++));
		circle(ResultImage, Point(mom.m10 / mom.m00, mom.m01 / mom.m00), 2, Scalar(0), 2);
		temp.x = mom.m10 / mom.m00;
		temp.y = mom.m01 / mom.m00;
		mydata.push_back(temp);
		//������ȡ��Բ����Բ��������ȡ�����浽mydata��
	}
	Mat resizeImage;
	cv::resize(ResultImage, resizeImage, Size(240, 180), 0, 0, INTER_LINEAR);
	QImg = cvMat2QImage1(resizeImage);
	vector<Coor>::iterator it1;
	double sumx = 0;
	double sumy = 0;
	//������ȡ������ĸ�����8���ж��Ƿ�Ϊ8�������㣬������ʵҲӦ���и��쳣�ж�
	if (contours.size() == 8)
	{
		//��8���������x��y������Ӳ���ȡƽ��ֵ
		for (it1 = mydata.begin(); it1 != mydata.end(); it1++)
		{
			sumx += (*it1).x;
			sumy += (*it1).y;
		}
		sumx = sumx / 8; sumy = sumy / 8;
		//����������������ҵ�4�飬ÿ������������ֵ
		for (it1 = mydata.begin(); it1 != mydata.end(); it1++)
		{
			if ((*it1).x - sumx>0)
			{
				if ((*it1).y - sumy>0)
				{
					temp1.x = (*it1).x;
					temp1.y = (*it1).y;
					mydata1.push_back(temp1);
				}
				else
				{
					temp2.x = (*it1).x;
					temp2.y = (*it1).y;
					mydata2.push_back(temp2);
				}
			}
			else
			{
				if ((*it1).y - sumy>0)
				{
					temp3.x = (*it1).x;
					temp3.y = (*it1).y;
					mydata3.push_back(temp3);
				}
				else
				{
					temp4.x = (*it1).x;
					temp4.y = (*it1).y;
					mydata4.push_back(temp4);
				}
			}

		}
	}
	//���Ƿ����ÿ���������ĸ�����2���ж��Ƿ�Ϊ2�������㣬������ʵҲӦ���и��쳣�ж�
	if (mydata1.size() == 2)
	{
		//�������ǵ�����ԭ������ͼ�����Ͻǣ����Բ��ܽ����ж�����xֵ�Ĵ�С�����������Ӧ���ж�x������x����ƽ��ֵ��Ĵ�С
		//�����ĵ�Ҳ���������ֵ�Ƚ�Զ�ĵ�����Ϊ��һ���㣬����m_observe_z[0]��m_observe_z[0]
		if ((fabs(mydata1.at(0).x - sumx)) > (fabs(mydata1.at(1).x - sumx)))
		{
			m_observe_z[0] = mydata1.at(0).x;
			m_observe_z[1] = mydata1.at(0).y;
			m_observe_z[2] = mydata1.at(1).x;
			m_observe_z[3] = mydata1.at(1).y;
		}
		else
		{
			m_observe_z[0] = mydata1.at(1).x;
			m_observe_z[1] = mydata1.at(1).y;
			m_observe_z[2] = mydata1.at(0).x;
			m_observe_z[3] = mydata1.at(0).y;
		}

	}
	//�����ĵ�Ҳ���������ֵ�Ƚ�Զ�ĵ�����Ϊ��һ���㣬����m_observe_z[4]��m_observe_z[5]
	if (mydata2.size() == 2)
	{
		if ((fabs(mydata2.at(0).x - sumx)) > (fabs(mydata2.at(1).x - sumx)))
		{
			m_observe_z[4] = mydata2.at(0).x;
			m_observe_z[5] = mydata2.at(0).y;
			m_observe_z[6] = mydata2.at(1).x;
			m_observe_z[7] = mydata2.at(1).y;
		}
		else
		{
			m_observe_z[4] = mydata2.at(1).x;
			m_observe_z[5] = mydata2.at(1).y;
			m_observe_z[6] = mydata2.at(0).x;
			m_observe_z[7] = mydata2.at(0).y;

		}

	}
	//�����ĵ�Ҳ���������ֵ�Ƚ�Զ�ĵ�����Ϊ��һ���㣬����m_observe_z[6]��m_observe_z[9]
	if (mydata3.size() == 2)
	{
		if ((fabs(mydata3.at(0).x - sumx)) > (fabs(mydata3.at(1).x - sumx)))
		{
			m_observe_z[8] = mydata3.at(0).x;
			m_observe_z[9] = mydata3.at(0).y;
			m_observe_z[10] = mydata3.at(1).x;
			m_observe_z[11] = mydata3.at(1).y;
		}
		else
		{
			m_observe_z[8] = mydata3.at(1).x;
			m_observe_z[9] = mydata3.at(1).y;
			m_observe_z[10] = mydata3.at(0).x;
			m_observe_z[11] = mydata3.at(0).y;

		}

	}
	//�����ĵ�Ҳ���������ֵ�Ƚ�Զ�ĵ�����Ϊ��һ���㣬����m_observe_z[12]��m_observe_z[13]
	if (mydata4.size() == 2)
	{
		if ((fabs(mydata4.at(0).x - sumx)) > (fabs(mydata4.at(1).x - sumx)))
		{
			m_observe_z[12] = mydata4.at(0).x;
			m_observe_z[13] = mydata4.at(0).y;
			m_observe_z[14] = mydata4.at(1).x;
			m_observe_z[15] = mydata4.at(1).y;
		}
		else
		{
			m_observe_z[12] = mydata4.at(1).x;
			m_observe_z[13] = mydata4.at(1).y;
			m_observe_z[14] = mydata4.at(0).x;
			m_observe_z[15] = mydata4.at(0).y;

		}

	}
	mreturn.mQImg = QImg;
	mreturn.mData = m_observe_z;
	//��������ͼƬ���ͻ�ȡ�����������ݷ���
	return mreturn;
}
QImage CamProcess::cvMat2QImage1(const cv::Mat& mat)
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