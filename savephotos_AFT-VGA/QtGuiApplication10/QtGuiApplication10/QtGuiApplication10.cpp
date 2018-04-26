/***************************************************
//本程序主要是利用FILR公司提供的摄像头库对双系统进行采集
 //并显示在窗口上，其中还用到了opencv的库，因为我们的位姿
 //估计中的图像处理主要是利用opencv完成的，所以我们需要将
 //图像转换为Mat格式来进行处理。
 ***************************************************/



#include "QtGuiApplication10.h"




QtGuiApplication10::QtGuiApplication10(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
	//设置定时器与相应的槽函数
	saveflag1 = false;//初始化保存图像标志位
	saveflag2 = false;
	Initialize();

	connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));

	
	
}
void QtGuiApplication10::Initialize()
{
	system = System::GetInstance();
	camList = system->GetCameras();
	unsigned int numCameras = camList.GetSize();

	// Finish if there are no cameras
	if (numCameras == 0)
	{
		// Clear camera list before releasing system
		camList.Clear();

		// Release system
		system->ReleaseInstance();

	}

	result = RunMultipleCameras(camList);
}

QtGuiApplication10::~QtGuiApplication10()
{

	timer->stop();
	camList.Clear();
	system->ReleaseInstance();
	//pCam->EndAcquisition();
	// Deinitialize camera
	//pCam->DeInit();

}
void QtGuiApplication10::on_pushButton_2_clicked()//close all按钮
{
	timer->stop();//关闭定时器
	QApplication *app;//关闭窗口

	//
	//for (int i = 0; i < camList.GetSize(); i++)
	//{
	//	// Select camera
	//	CameraPtr pCam;
	//	pCam = camList.GetByIndex(i);

	//	// Deinitialize camera
	//	pCam->DeInit();
	//}
	camList.Clear();
	system->ReleaseInstance();
	app->quit();
}

void QtGuiApplication10::timerUpDate()
{
	
	QImage CamImage;
	QImage CamImage1;
	CamImage = GetImage(camList.GetByIndex(0));
	CamImage1 = GetImage(camList.GetByIndex(1));
	ui.label->setPixmap(QPixmap::fromImage(CamImage));
	ui.label_2->setPixmap(QPixmap::fromImage(CamImage1));;
	if (saveflag1)//如果saveflag为true，则保存
	{
	/*	1 QString str;
		2 str = QString("%1 was born in %2 .").arg("Rimond").arg(1990);*/
		//QString::sprintf(fileName1, "left_%d.jpg", i)
		//sprintf(fileName1, "left_%d.jpg", i); i++;//保存名为left_i，i从0开始计数
		//在qt中使用类似sprintf的函数
		//参考：http://www.cnblogs.com/jingliming/p/4965199.html
		fileName1=QString("left_%1.jpg").arg(i);
		i++;
		CamImage=CamImage.scaled(1280, 1040);
		CamImage.save(fileName1);
		//imwrite(fileName1, frame1); 
		saveflag1 = false;
	}
	if (saveflag2)//如果saveflag为true，则保存
	{
		/*	1 QString str;
		2 str = QString("%1 was born in %2 .").arg("Rimond").arg(1990);*/
		//QString::sprintf(fileName1, "left_%d.jpg", i)
		//sprintf(fileName1, "left_%d.jpg", i); i++;//保存名为left_i，i从0开始计数
		fileName2 = QString("right_%1.jpg").arg(j);
		j++;
		CamImage1=CamImage1.scaled(1280, 1040);
		CamImage1.save(fileName2);
		//imwrite(fileName1, frame1); 
		saveflag2 = false;
	}
	
}

void QtGuiApplication10::slot1()
{
	timer->start(5);
	
}
QImage QtGuiApplication10::ConvertToCVmat(ImagePtr pImage)
{
	int result = 0;
	ImagePtr convertedImage = pImage->Convert(PixelFormat_BGR8, NEAREST_NEIGHBOR);

	unsigned int XPadding = convertedImage->GetXPadding();
	unsigned int YPadding = convertedImage->GetYPadding();
	unsigned int rowsize = convertedImage->GetWidth();
	unsigned int colsize = convertedImage->GetHeight();

	//image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding. 
	cvimg = cv::Mat(colsize + YPadding, rowsize + XPadding, CV_8UC3, convertedImage->GetData(), convertedImage->GetStride());
	Mat resizeImage;
	cv::resize(cvimg, resizeImage, Size(320, 240), 0, 0, INTER_LINEAR);
	QImage CamImage;
	CamImage = cvMat2QImage1(resizeImage);
	/*namedWindow("current Image", CV_WINDOW_AUTOSIZE);
	imshow("current Image", cvimg);
	resizeWindow("current Image", rowsize / 2, colsize / 2);*/
	//waitKey(1);//otherwise the image will not display...

	return CamImage;
}
void QtGuiApplication10::AcquireImage(CameraPtr pCam, INodeMap & nodeMap)
{
	// Set acquisition mode to continuous
	CEnumerationPtr ptrAcquisitionMode = nodeMap.GetNode("AcquisitionMode");
	if (!IsAvailable(ptrAcquisitionMode) || !IsWritable(ptrAcquisitionMode))
	{
		cout << "Unable to set acquisition mode to continuous (node retrieval). Aborting..." << endl << endl;
		//return -1;
	}

	CEnumEntryPtr ptrAcquisitionModeContinuous = ptrAcquisitionMode->GetEntryByName("Continuous");
	if (!IsAvailable(ptrAcquisitionModeContinuous) || !IsReadable(ptrAcquisitionModeContinuous))
	{
		//cout << "Unable to set acquisition mode to continuous (entry 'continuous' retrieval). Aborting..." << endl << endl;
		//return -1;
	}

	int64_t acquisitionModeContinuous = ptrAcquisitionModeContinuous->GetValue();

	ptrAcquisitionMode->SetIntValue(acquisitionModeContinuous);


	// Begin acquiring images
	pCam->BeginAcquisition();


	// Execute software trigger
	//CCommandPtr ptrSoftwareTriggerCommand = nodeMap.GetNode("TriggerSoftware");
	//if (!IsAvailable(ptrSoftwareTriggerCommand) || !IsWritable(ptrSoftwareTriggerCommand))
	//{
	//	//cout << "Unable to execute trigger. Aborting..." << endl;
	//	return -1;
	//}

	//ptrSoftwareTriggerCommand->Execute();

	// Retrieve the next received image

}

// This function prints the device information of the camera from the transport
// layer; please see NodeMapInfo example for more in-depth comments on printing
// device information from the nodemap.
int QtGuiApplication10::PrintDeviceInfo(INodeMap & nodeMap)
{
	int result = 0;

	cout << endl << "*** DEVICE INFORMATION ***" << endl << endl;

	try
	{
		FeatureList_t features;
		CCategoryPtr category = nodeMap.GetNode("DeviceInformation");
		if (IsAvailable(category) && IsReadable(category))
		{
			category->GetFeatures(features);

			FeatureList_t::const_iterator it;
			for (it = features.begin(); it != features.end(); ++it)
			{
				CNodePtr pfeatureNode = *it;
				cout << pfeatureNode->GetName() << " : ";
				CValuePtr pValue = (CValuePtr)pfeatureNode;
				cout << (IsReadable(pValue) ? pValue->ToString() : "Node not readable");
				cout << endl;
			}
		}
		else
		{
			cout << "Device control information not available." << endl;
		}
	}
	catch (Spinnaker::Exception &e)
	{
		cout << "Error: " << e.what() << endl;
		result = -1;
	}

	return result;
}

// This function acts as the body of the example; please see NodeMapInfo example 
// for more in-depth comments on setting up cameras.
int QtGuiApplication10::RunSingleCamera(CameraPtr pCam)
{
	int result = 0;
	int err = 0;
	try
	{
		// Initialize camera
		pCam->Init();

		// Retrieve GenICam nodemap
		INodeMap & nodeMap = pCam->GetNodeMap();
		//set trigger to software trigger
		//result = ConfigureTrigger(nodeMap);

		//ImagePtr pImage;
		//take an image
		//Sleep(1000);
		 AcquireImage(pCam, nodeMap);

		//convert to CVmat format
		//result = ConvertToCVmat(pImage);


		////pImage->Release();
		//pCam->EndAcquisition();
		//// Deinitialize camera
		//pCam->DeInit();
	}

	catch (Spinnaker::Exception &e)
	{
		cout << "Error: " << e.what() << endl;
		result = -1;
	}
	return result;
}

int QtGuiApplication10::RunMultipleCameras(CameraList camList)
{
	int result = 0;
	CameraPtr pCam = NULL;

	try
	{
		for (int i = 0; i < camList.GetSize(); i++)
		{
			// Select camera
			pCam = camList.GetByIndex(i);

			// Initialize camera
			pCam->Init();
		}

		// Acquire images on all cameras
		AcquireImages(camList);


	}
	catch (Spinnaker::Exception &e)
	{
		cout << "Error: " << e.what() << endl;
		result = -1;
	}

	return result;
}

int QtGuiApplication10::ConfigureTrigger(INodeMap & nodeMap)
{
	int result = 0;
	cout << endl << endl << "*** CONFIGURING TRIGGER ***" << endl << endl;
	try
	{
		// Ensure trigger mode off
		//
		// *** NOTES ***
		// The trigger must be disabled in order to configure whether the source
		// is software or hardware.
		//
		CEnumerationPtr ptrTriggerMode = nodeMap.GetNode("TriggerMode");
		if (!IsAvailable(ptrTriggerMode) || !IsReadable(ptrTriggerMode))
		{
			cout << "Unable to disable trigger mode (node retrieval). Aborting..." << endl;
			return -1;
		}

		CEnumEntryPtr ptrTriggerModeOff = ptrTriggerMode->GetEntryByName("Off");
		if (!IsAvailable(ptrTriggerModeOff) || !IsReadable(ptrTriggerModeOff))
		{
			cout << "Unable to disable trigger mode (enum entry retrieval). Aborting..." << endl;
			return -1;
		}

		ptrTriggerMode->SetIntValue(ptrTriggerModeOff->GetValue());

		cout << "Trigger mode disabled..." << endl;

		CEnumerationPtr ptrTriggerSource = nodeMap.GetNode("TriggerSource");
		if (!IsAvailable(ptrTriggerSource) || !IsWritable(ptrTriggerSource))
		{
			cout << "Unable to set trigger mode (node retrieval). Aborting..." << endl;
			return -1;
		}

		CEnumEntryPtr ptrTriggerSourceSoftware = ptrTriggerSource->GetEntryByName("Software");
		if (!IsAvailable(ptrTriggerSourceSoftware) || !IsReadable(ptrTriggerSourceSoftware))
		{
			cout << "Unable to set trigger mode (enum entry retrieval). Aborting..." << endl;
			return -1;
		}

		ptrTriggerSource->SetIntValue(ptrTriggerSourceSoftware->GetValue());

		cout << "Trigger source set to software..." << endl;

		// Turn trigger mode on
		//
		// *** LATER ***
		// Once the appropriate trigger source has been set, turn trigger mode 
		// on in order to retrieve images using the trigger.
		//
		CEnumEntryPtr ptrTriggerModeOn = ptrTriggerMode->GetEntryByName("On");
		if (!IsAvailable(ptrTriggerModeOn) || !IsReadable(ptrTriggerModeOn))
		{
			cout << "Unable to enable trigger mode (enum entry retrieval). Aborting..." << endl;
			return -1;
		}

		ptrTriggerMode->SetIntValue(ptrTriggerModeOn->GetValue());

		cout << "Trigger mode turned back on..." << endl << endl;
	}
	catch (Spinnaker::Exception &e)
	{
		cout << "Error: " << e.what() << endl;
		result = -1;
	}
	return result;
}
QImage QtGuiApplication10::cvMat2QImage1(const cv::Mat& mat)
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

QImage QtGuiApplication10::GetImage(CameraPtr pCam)
{
	// Retrieve the next received image
	//CameraPtr pCam = NULL;
	//pCam = camList.GetByIndex(0);
	ImagePtr pResultImage = pCam->GetNextImage(10);
	if (pResultImage->IsIncomplete())
	{
		//cout << "Image incomplete with image status " << pResultImage->GetImageStatus() << "..." << endl << endl;
	}
	QImage PictureImage;
	PictureImage = ConvertToCVmat(pResultImage);
	return PictureImage;
}
int QtGuiApplication10::AcquireImages(CameraList camList)
{
	int result = 0;
	CameraPtr pCam = NULL;

	cout << endl << "*** IMAGE ACQUISITION ***" << endl << endl;

	try
	{
		//
		// Prepare each camera to acquire images
		// 
		// *** NOTES ***
		// For pseudo-simultaneous streaming, each camera is prepared as if it 
		// were just one, but in a loop. Notice that cameras are selected with 
		// an index. We demonstrate pseduo-simultaneous streaming because true 
		// simultaneous streaming would require multiple process or threads,
		// which is too complex for an example. 
		// 
		// Serial numbers are the only persistent objects we gather in this
		// example, which is why a vector is created.
		//
		vector<gcstring> strSerialNumbers(camList.GetSize());

		for (int i = 0; i < camList.GetSize(); i++)
		{
			// Select camera
			pCam = camList.GetByIndex(i);

			// Set acquisition mode to continuous
			CEnumerationPtr ptrAcquisitionMode = pCam->GetNodeMap().GetNode("AcquisitionMode");
			if (!IsAvailable(ptrAcquisitionMode) || !IsWritable(ptrAcquisitionMode))
			{
				cout << "Unable to set acquisition mode to continuous (node retrieval; camera " << i << "). Aborting..." << endl << endl;
				return -1;
			}

			CEnumEntryPtr ptrAcquisitionModeContinuous = ptrAcquisitionMode->GetEntryByName("Continuous");
			if (!IsAvailable(ptrAcquisitionModeContinuous) || !IsReadable(ptrAcquisitionModeContinuous))
			{
				cout << "Unable to set acquisition mode to continuous (entry 'continuous' retrieval " << i << "). Aborting..." << endl << endl;
				return -1;
			}

			int64_t acquisitionModeContinuous = ptrAcquisitionModeContinuous->GetValue();

			ptrAcquisitionMode->SetIntValue(acquisitionModeContinuous);

			cout << "Camera " << i << " acquisition mode set to continuous..." << endl;

			// Begin acquiring images
			pCam->BeginAcquisition();

		}
		
	}
	catch (Spinnaker::Exception &e)
	{
		cout << "Error: " << e.what() << endl;
		result = -1;
	}

	return result;
}

void QtGuiApplication10::on_Btn_Cam1_clicked()//savephoto按钮摄像机1
{
	saveflag1 = true;//标志位设置为true
}

void QtGuiApplication10::on_Btn_Cam2_clicked()//savephoto按钮摄像机2
{
	saveflag2 = true;
}

void QtGuiApplication10::on_pushButton_3_clicked()//save all 按钮
{
	saveflag1 = true;
	saveflag2 = true;//标志位设置为true
}
