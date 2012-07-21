#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstdio>
#include <math.h>
#include <string>

#include "AutoMutex.h"

void DisplayError(const char* Format,...);
const char* ConvertToHex(DWORD d);

template <typename T> T ReadPtrChain(DWORD pBase, long pOffset1=0, long pOffset2=0, long pOffset3=0, long pOffset4=0,long pOffset5=0)
{
	DWORD pRead = pBase;
	if(pRead == NULL){return 0;}

	if(pOffset1){pRead = *(DWORD*)(pRead + pOffset1);}
	if(pRead == NULL){return 0;}

	if(pOffset2){pRead = *(DWORD*)(pRead + pOffset2);}
	if(pRead == NULL){return 0;}

	if(pOffset3){pRead = *(DWORD*)(pRead + pOffset3);}
	if(pRead == NULL){return 0;}

	if(pOffset4){pRead = *(DWORD*)(pRead + pOffset4);}
	if(pRead == NULL){return 0;}

	if(pOffset5){pRead = *(DWORD*)(pRead + pOffset5);}
	if(pRead == NULL){return 0;}

	return (T)(pRead);
}