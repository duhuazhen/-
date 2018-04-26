#include "SixAxis.h"


SixAxis::SixAxis()
{
	SixAxisConnectFlag = false;
	SixAxisFlag = false;
}

SixAxis::~SixAxis()
{
}
void SixAxis::Connect()
{
	ID = ConnectFirstFoundHexapodViaTCPIP();
	if (ID>-1)
	{
		SixAxisConnectFlag = true;
	}
	else
	{
		throw(ID);
	}

}
long SixAxis::ConnectFirstFoundHexapodViaTCPIP()
{
	char szFoundDevices[10000];
	//printf("searching TCPIP devices...\n");
	if (PI_EnumerateTCPIPDevices(szFoundDevices, 9999, "") <= 0)
	{
		return -1;
	}
	char* szAddressToConnect = NULL;
	int port = 0;
	char * pch = strtok(szFoundDevices, "\n");
	while (pch != NULL)
	{
		_strupr(pch);
		if (
			((strstr(pch, "F-HEX") != NULL) && (strstr(pch, "LISTENING") != NULL))
			|| ((strstr(pch, "HEXAPOD") != NULL) && (strstr(pch, "LISTENING") != NULL))
			|| ((strstr(pch, "F-206") != NULL) && (strstr(pch, "LISTENING") != NULL))
			|| ((strstr(pch, "M-8") != NULL) && (strstr(pch, "LISTENING") != NULL))
			|| ((strstr(pch, "C-887") != NULL) && (strstr(pch, "LISTENING") != NULL))
			)
		{
			char* colon = strstr(pch, ":");
			if (colon == NULL)
			{
				continue;
			}
			*colon = '\0';
			char* bracket = strstr(pch, "(");
			if (bracket == NULL)
			{
				continue;
			}
			szAddressToConnect = new char[strlen(bracket + 1) + 1];
			strcpy(szAddressToConnect, bracket + 1);
			bracket = strstr(colon + 1, ")");
			if (bracket == NULL)
			{
				continue;
			}
			*bracket = '\0';
			port = atoi(colon + 1);
			break;

		}
		pch = strtok(NULL, "\n");
	}

	if (szAddressToConnect != NULL)
	{
		//printf("trying to connect with %s, port %d\n", szAddressToConnect, port);
		int iD = PI_ConnectTCPIP(szAddressToConnect, port);
		delete[]szAddressToConnect;
		return iD;
	}
	return -1;

}
bool SixAxis::ReferenceIfNeeded(int ID, char* axis)
{
	BOOL bReferenced;
	BOOL bFlag;
	//PI_qFRF检查相应的坐标是否已经到达参考点
	//if (!PI_qFRF(ID, axis, &bReferenced))
	//	return false;
	//如果没有到达参考点就用PI_FRF命令使相应的坐标轴到达参考点
	//if (!bReferenced)
	//{
	if (!PI_FRF(ID, axis))
		return false;
	bFlag = false;
	while (bFlag != TRUE)
	{
		//使用命令PI_IsControllerReady检查参考点标定是否已经完成，如果没有完成的话不能退出while循环。
		if (!PI_IsControllerReady(ID, &bFlag))
			return false;
		//	}
	}
	return true;
}
bool SixAxis::MoveTo(int ID, char* axis, double* dVal)
{

	if (!PI_MOV(ID, axis, dVal))
		return FALSE;
	// Wait until the closed loop move is done.
	return true;
}
void SixAxis::Track1()
{
	double position[6] = { 5, 2, 3, 0, 0, 0 };
	double position1[6] = { 8, 9, 6, 3, 5, 2 };
	double position2[6] = { 12, 7, 5, 9, 3, 5 };
	double position3[6] = { 9, 8, 8, 10, 5, 5 };
	double position4[6] = { 10, 10, 10, 8, 4, 3 };
	double position5[6] = { 14, 7, -6, -5, 5, 5 };
	double position6[6] = { 8, 5, 3, 8, 2, 3 };
	double position7[6] = { 0, 2, 5, 5, -2, 6 };
	double position8[6] = { 0, 0, 0, 0, 0, 0 };
	BOOL bIsMoving = TRUE;
	PI_MOV(ID, "x y z u v w", position);
	while (bIsMoving == TRUE)
	{ 	
		PI_IsMoving(ID, "x", &bIsMoving);
	}
	BOOL bIsMoving1 = TRUE;
	PI_MOV(ID, "x y z u v w", position1);
	while (bIsMoving1 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving1);
	}
	BOOL bIsMoving2 = TRUE;
	PI_MOV(ID, "x y z u v w", position2);
	while (bIsMoving2 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving2);
	}
	BOOL bIsMoving3 = TRUE;
	PI_MOV(ID, "x y z u v w", position3);
	while (bIsMoving3 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving3);
	}
	BOOL bIsMoving4 = TRUE;
	PI_MOV(ID, "x y z u v w", position4);
	while (bIsMoving4 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving4);
	}
	BOOL bIsMoving5 = TRUE;
	PI_MOV(ID, "x y z u v w", position5);
	while (bIsMoving5 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving5);
	}
	BOOL bIsMoving6 = TRUE;
	PI_MOV(ID, "x y z u v w", position6);
	while (bIsMoving6 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving6);
	}
	BOOL bIsMoving7 = TRUE;
	PI_MOV(ID, "x y z u v w", position7);
	while (bIsMoving7 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving7);
	}
	BOOL bIsMoving8 = TRUE;
	PI_MOV(ID, "x y z u v w", position8);
	while (bIsMoving8 == TRUE)
	{
		PI_IsMoving(ID, "x", &bIsMoving8);
	}
}

void SixAxis::run()
{
	if (SixAxisFlag)
	{

		Track1();
		SixAxisFlag = false;
	}

}
