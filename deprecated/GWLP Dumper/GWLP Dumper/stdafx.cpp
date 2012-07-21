#include "stdafx.h"


void DisplayError(const char* Format,...)
{
	va_list args;
	va_start (args, Format);
	char* buffer = new char[_vscprintf(Format,args)];
	vsprintf(buffer,Format,args);
	va_end(args);
	MessageBoxA(0,buffer,"ERROR",MB_ICONERROR|MB_OK);
	delete[] buffer;
}

union DwordToHex
{
	DWORD d;
	unsigned char str[3];
};

const char* ConvertToHex(DWORD d)
{
	if(d == 0)
		return "00000000";

	DwordToHex h;
	h.d = d;

	char* hex = new char[20];

	sprintf_s(hex,20,"%02X%02X%02X%02X\0",h.str[0],h.str[1],h.str[2],h.str[3]);

	return hex;
}

