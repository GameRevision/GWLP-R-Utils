#include <windows.h>
#include "detours.h"

void *DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwBack;

	VirtualProtect(src, len, PAGE_READWRITE, &dwBack);

	memcpy(jmp, src, len);	
	jmp += len;
	
	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;

	for( int i=5; i < len; i++ )
		src[i] = 0x90;

	VirtualProtect(src, len, dwBack, &dwBack);

	return (jmp-len);
}

void RetourFunc(BYTE *src, BYTE *restore, const int len)
{
	DWORD dwBack;
		
	VirtualProtect(src, len, PAGE_READWRITE, &dwBack);
	memcpy(src, restore, len);

	restore[0] = 0xE9;
	*(DWORD*)(restore+1) = (DWORD)(src - restore) - 5;

	VirtualProtect(src, len, dwBack, &dwBack);
}