#include "CallUKF.h"

double dbfocal[8] = { 836.5512, 831.9177, 313.9417, 266.0186, 819.7424, 815.8510, 367.4169, 233.321 };
double dbRelateCoor[24] = { -35, 35, 35, -35, -20, 20, 20, -20, -80, -80, -10, -10, -65, -65, -25, -25, 0, 0, 0, 0, 0, 0, 0, 0 };
double dbt[1] = { 0.05 };
double dbEuler[3] = { 0.0365, -0.4284, -0.0457 };
double dbT[12] = { 0.9169, -0.0493, 0.3961, 0.0337, 0.9984, 0.0460, -0.3977, -0.0288, 0.9170, 166.0290, 18.5801, 36.5276 };


CallUKF::CallUKF()
{
	xFlag = true;
	for (int i = 0; i < 144; i++)
		dbP[i] = 0;
	for (int i = 0; i < 12; i++)
	{
		dbx[i] = 0;
		dbP[12*i+i] = 1;
	}
	for (int i = 0; i < 16; i++)
	{
		dbz[i] = 0;
	}
	for (int i = 0; i < 32; i++)
	{
		dbfusionz[i] = 0;
	}
	if (!NonlinerUKF8Initialize())
	{
		
	}
	Mat R3x1 = Mat(3, 1, CV_64FC1, Scalar::all(0));
	Mat R3x3 = Mat(3, 3, CV_64FC1, Scalar::all(0));
	for (int i = 0; i < 3; i++)
		R3x1.ptr<double>(i)[0] = dbEuler[i];
	Rodrigues(R3x1, R3x3);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			dbT[3 * i + j] = R3x3.ptr<double>(j)[i];
}


CallUKF::~CallUKF()
{
}

void CallUKF::UKFProcess()
{
	
	mwArray focalIndex(4, 2, mxDOUBLE_CLASS, mxREAL);
	focalIndex.SetData(dbfocal,8);
	mwArray t(1, 1, mxDOUBLE_CLASS, mxREAL);
	t.SetData(dbt, 1);
	mwArray RelatObjCoor(8, 3, mxDOUBLE_CLASS, mxREAL);
	RelatObjCoor.SetData(dbRelateCoor, 24);
	mwArray euler(1, 3, mxDOUBLE_CLASS, mxREAL);
	euler.SetData(dbEuler, 3);
	mwArray T(3, 4, mxDOUBLE_CLASS, mxREAL);
	T.SetData(dbT, 12);
	mwArray flag(1, 1, mxDOUBLE_CLASS, mxREAL);
	flag.SetData(dbflag, 1);
	mwArray x(12, 1, mxDOUBLE_CLASS, mxREAL);
	x.SetData(dbx, 12);
	mwArray P(12, 12, mxDOUBLE_CLASS, mxREAL);
 	P.SetData(dbP, 144);
	mwArray z(16, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray fusionz(16, 2, mxDOUBLE_CLASS, mxREAL);
	if (dbflag[0] < 3)
	{
		z.SetData(dbz, 16);
	}
	else
	{
		fusionz.SetData(dbfusionz, 32);
	}
	mwArray xout(12, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray Pout(12, 12, mxDOUBLE_CLASS, mxREAL);
	if (dbflag[0] < 3)
		NonlinerUKF8(2, xout, Pout, z, x, P, focalIndex, t, RelatObjCoor, euler, T, flag);
	else
		NonlinerUKF8(2, xout, Pout, fusionz, x, P, focalIndex, t, RelatObjCoor, euler, T, flag);
	for (int j = 0; j < 12; j++)
	{

		dbP[j] = Pout(j + 1, 1);
		dbP[j + 12] = Pout(j + 1, 2);
		dbP[j + 24] = Pout(j + 1, 3);
		dbP[j + 36] = Pout(j + 1, 4);
		dbP[j + 48] = Pout(j + 1, 5);
		dbP[j + 60] = Pout(j + 1, 6);
		dbP[j + 72] = Pout(j + 1, 7);
		dbP[j + 84] = Pout(j + 1, 8);
		dbP[j + 96] = Pout(j + 1, 9);
		dbP[j + 108] = Pout(j + 1, 10);
		dbP[j + 120] = Pout(j + 1, 11);
		dbP[j + 132] = Pout(j + 1, 12);
	}

	for (int j = 0; j < 12; j++)
	{
		dbx[j] = xout(j + 1, 1);

	}


}