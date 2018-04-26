#pragma once
#include "NonlinerUKF8.h"
#include "Eigen\Dense"
#include <Eigen\Eigen>
#include "qmath.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include "CamProcess.h"

class CallUKF
{
public:
	CallUKF();
	~CallUKF();
	double dbx[12];
	double dbP[144];
	double dbz[16];
	double dbfusionz[32];
	bool xFlag;
	void UKFProcess();
	double dbflag[1];
private:
	


};

