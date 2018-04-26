#pragma once
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <qthread.h>
#include <time.h>
#include <qstring.h>
#include "iostream"
#include "PI_GCS2_DLL.h"

class SixAxis: public QThread
{
public:
	SixAxis();
	~SixAxis();
	void Connect();
	long ConnectFirstFoundHexapodViaTCPIP();
	void CompleteConnectionProces();
	bool SixAxisConnectFlag;
	bool ReferenceIfNeeded(int ID, char* axis);
	bool MoveTo(int ID, char* axis, double* dVal);
	void Track1();
	int ID;
	bool SixAxisFlag;

private:

protected:
	void run();
	
};

