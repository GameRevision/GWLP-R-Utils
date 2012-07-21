#include "stdafx.h"

void DisplayError( const char* fmt,... )
{
	static char Msg[250000];
	va_list args;
	va_start(args,fmt);
	vsprintf_s(Msg,fmt,args);
	MessageBoxA(NULL,Msg,"Error",MB_OK|MB_ICONERROR);
	va_end(args);
}