/**
    wrap.cpp
    Purpose: 
    	wrapped some of the used api for epos2
    	this is not a whole library of epos2 api, only for the convenience to invoke some functions
		this file is used by all the controller cpp and the compile rule was defined in CMakeLists
    @author Amit Ruidas
    @version 0.3 13/09/22 
**/
#include "wrap.h"


void* g_pKeyHandle=0;
unsigned short g_usNodeId;
string g_deviceName;
string g_protocolStackName;
string g_interfaceName;
string g_portName;
int g_baudrate ;


//string g_portName2;

void LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode)
{
	cerr << functionName << " failed (result=" << p_lResult << ", errorCode=0x" << std::hex << p_ulErrorCode << ")"<< endl;
}

void LogInfo(string message)
{
	cout << message << endl;
}

// OPEN THE DEVICE

void* OpenDevice(unsigned int* p_pErrorCode, string g_deviceName, string g_protocolStackName, string g_interfaceName, string g_portName, int g_baudrate, void* g_pKeyHandle)
{
	int lResult = MMC_FAILED;

	char* pDeviceName = new char[255];
	char* pProtocolStackName = new char[255];
	char* pInterfaceName = new char[255];
	char* pPortName = new char[255];

	strcpy(pDeviceName, g_deviceName.c_str());
	strcpy(pProtocolStackName, g_protocolStackName.c_str());
	strcpy(pInterfaceName, g_interfaceName.c_str());
	strcpy(pPortName, g_portName.c_str());

	LogInfo("Open device...");

        cout<<"device name: "<<pDeviceName<<endl;
        cout<<"protocol name: "<<pProtocolStackName<<endl;
        cout<<"interface name: "<<pInterfaceName<<endl;
        cout<<"port name: "<<pPortName<<endl;
	g_pKeyHandle = VCS_OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName, p_pErrorCode);	
	printf("KeyHandle =  %p", g_pKeyHandle);
	if(g_pKeyHandle!=0 && *p_pErrorCode == 0)
	{
		unsigned int lBaudrate = 0;
		unsigned int lTimeout = 0;

		if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
		{
			if(VCS_SetProtocolStackSettings(g_pKeyHandle, g_baudrate, lTimeout, p_pErrorCode)!=0)
			{
				if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
				{
					if(g_baudrate==(int)lBaudrate)
					{
						lResult = MMC_SUCCESS;
                                                LogInfo("Open device successful");
					}
				}
			}
		}
	}
	else
	{
		g_pKeyHandle = 0;
	}

	delete []pDeviceName;
	delete []pProtocolStackName;
	delete []pInterfaceName;
	delete []pPortName;
	return g_pKeyHandle;
}

// SET TO ENABLE STATE

int SetEnableState(void* g_pKeyHandle, unsigned short g_usNodeId, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	BOOL oIsFault = 0;
	
	printf("KeyHandle =  %p", g_pKeyHandle);
	if(VCS_GetFaultState(g_pKeyHandle, g_usNodeId, &oIsFault, p_pErrorCode ) == 0)
	{
		LogError("VCS_GetFaultState", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}

	if(lResult==0)
	{
		if(oIsFault)
		{
			stringstream msg;
			msg << "clear fault, node = '" << g_usNodeId << "'";
			LogInfo(msg.str());

			if(VCS_ClearFault(g_pKeyHandle, g_usNodeId, p_pErrorCode) == 0)
			{
				LogError("VCS_ClearFault", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}
		}

		if(lResult==0)
		{
			BOOL oIsEnabled = 0;

			if(VCS_GetEnableState(g_pKeyHandle, g_usNodeId, &oIsEnabled, p_pErrorCode) == 0)
			{
				LogError("VCS_GetEnableState", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}

			if(lResult==0)
			{
				if(!oIsEnabled)
				{
					if(VCS_SetEnableState(g_pKeyHandle, g_usNodeId, p_pErrorCode) == 0)
					{
						LogError("VCS_SetEnableState", lResult, *p_pErrorCode);
						lResult = MMC_FAILED;
					}
				}
			}
		}
	}
	LogInfo("Enabled EPOS");
	return lResult;
}


// SET TO DISABLE STATE

int SetDisableState(void* g_pKeyHandle, unsigned short g_usNodeId, unsigned int* pErrorCode)
{
	int lResult = MMC_SUCCESS;
	unsigned int lErrorCode = 0;
	if(VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode) == 0)
	{
		LogError("VCS_SetDisableState", lResult, lErrorCode);
		lResult = MMC_FAILED;
	}
	LogInfo("Disable EPOS1");
	return lResult;
}


// CLOSE THE DEVICE

int CloseDevice(unsigned int* p_pErrorCode,void* g_pKeyHandle)
{
	int lResult = MMC_FAILED;

	*p_pErrorCode = 0;

	LogInfo("Close device1");

	if(VCS_CloseDevice(g_pKeyHandle, p_pErrorCode)!=0 && *p_pErrorCode == 0)
	{
		lResult = MMC_SUCCESS;
	}

	return lResult;
}

// ACTIVATE PROFILE POSITION MODE

int ActivateProfilePositionMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int* p_rlErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;

	msg << "set profile position mode, node = " << p_usNodeId;
	LogInfo(msg.str());

	if(VCS_ActivateProfilePositionMode(p_DeviceHandle, p_usNodeId, p_rlErrorCode) == 0)
	{
		LogError("VCS_ActivateProfilePositionMode", lResult, *p_rlErrorCode);
		lResult = MMC_FAILED;
	}
	return  lResult;
}

// ACTIVATE PROFILE VELOCITY MODE

int ActivateProfileVelocityMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int* p_rlErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;

	msg << "set profile velocity mode, node = " << p_usNodeId;
	LogInfo(msg.str());

	if(VCS_ActivateProfileVelocityMode(p_DeviceHandle, p_usNodeId, p_rlErrorCode) == 0)
	{
		LogError("VCS_ActivateProfileVelocityMode", lResult, *p_rlErrorCode);
		lResult = MMC_FAILED;
	}
	return  lResult;
}

//ACTIVATE PROFILE CURRENT MODE

int ActivateProfileCurrentMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int* p_rlErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;

	msg << "set profile current mode, node = " << p_usNodeId;
	LogInfo(msg.str());

	if(VCS_ActivateCurrentMode(p_DeviceHandle, p_usNodeId, p_rlErrorCode) == 0)
	{
		LogError("VCS_ActivateCurrentMode", lResult, *p_rlErrorCode);
		lResult = MMC_FAILED;
	}
	return  lResult;
}

// GET POSITION

int get_position(HANDLE p_DeviceHandle, unsigned short p_usNodeId, int* pPositionIs, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	if(VCS_GetPositionIs(p_DeviceHandle, p_usNodeId, pPositionIs, p_pErrorCode) == 0)
	{
		LogError("VCS_GetPositionIs", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
}

//MOVE TO TARGET POSITION

int MoveToPosition(HANDLE p_DeviceHandle, unsigned short p_usNodeId, long TargetPosition, int Absolute, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
        int immediate = 1;
        int later = 0; 
	stringstream msg;
	if(VCS_MoveToPosition(p_DeviceHandle, p_usNodeId, TargetPosition, Absolute, immediate, p_pErrorCode) == 0)
	{
		LogError("VCS_MoveToPosition", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
	return lResult;
}

// GET POSITION PROFILE

int get_PositionProfile(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int* p_pErrorCode)
{
	stringstream msg;
	int lResult = MMC_SUCCESS;
	unsigned int pProfileVelocity, pProfileAcceleration, pProfileDeceleration, pErrorCode;
	if(VCS_GetPositionProfile(p_DeviceHandle, p_usNodeId, &pProfileVelocity, &pProfileAcceleration, &pProfileDeceleration, &pErrorCode) == 0)
	{
		LogError("VCS_GetPositionProfile", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
	msg << "\n" 
		<< "pProfileVelocity," << pProfileVelocity 
		<< "pProfileAcceleration," << pProfileAcceleration 
		<< "pProfileDeceleration," << pProfileDeceleration 
		<< "pErrorCode" << pErrorCode;
	LogInfo(msg.str());
}

// GET VELOCITY

int get_velocity(HANDLE p_DeviceHandle, unsigned short p_usNodeId, int* pVelocityIs, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	if(VCS_GetVelocityIs(p_DeviceHandle, p_usNodeId, pVelocityIs, p_pErrorCode) == 0)
	{
		LogError("VCS_GetVelocityIs", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
}

// GET TARGET VELOCITY

int get_TargetVelocity(void* g_pKeyHandle, unsigned short g_usNodeId, unsigned int* ulErrorCode)
{
	int lResult = MMC_SUCCESS;
	long pTargetVelocity;
	stringstream msg;
	if(VCS_GetTargetVelocity(g_pKeyHandle, g_usNodeId, &pTargetVelocity, ulErrorCode) == 0)
	{
		LogError("VCS_GetVelocityIs", lResult, *ulErrorCode);
		lResult = MMC_FAILED;
	}
	msg << "pTargetVelocity," << pTargetVelocity ;
	LogInfo(msg.str());
}

// MOVE WITH TARGET VELOCITY

int MoveWithVelocity(void* p_DeviceHandle, unsigned short p_usNodeId, long TargetVelocity, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;
	if(VCS_MoveWithVelocity(p_DeviceHandle, p_usNodeId, TargetVelocity, p_pErrorCode) == 0)
	{
		LogError("VCS_MoveWithVelocity", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
	msg << "MoveWithVelocity:" << TargetVelocity ;
	LogInfo(msg.str());
	return lResult;
}


// GET CURRENT

int get_current(HANDLE p_DeviceHandle, unsigned short p_usNodeId, short* pCurrentMust, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;
	if(VCS_GetCurrentMust(p_DeviceHandle, p_usNodeId, pCurrentMust, p_pErrorCode) == 0)
	{
		LogError("VCS_GetCurrentMust", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
	msg << "pCurrentMust," << *pCurrentMust ;
}



//SET THE VALUE OF CURRENT

int SetCurrentMust(HANDLE p_DeviceHandle, unsigned short p_usNodeId, short CurrentMust, unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	stringstream msg;
	if(VCS_SetCurrentMust(p_DeviceHandle, p_usNodeId, CurrentMust, p_pErrorCode) == 0)
	{
		LogError("VCS_SetCurrentMust", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}
	msg << "SetCurrentMust:" << CurrentMust;
	return lResult;
}
