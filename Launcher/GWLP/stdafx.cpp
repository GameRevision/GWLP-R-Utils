#include "stdafx.h"

void DisplayError(char* Format,...)
{
	va_list args;
	va_start(args, Format);
	char error[20000];
	vsprintf(error, Format, args);
	va_end(args);
	MessageBoxA(0,error,"Error",MB_OK);
}