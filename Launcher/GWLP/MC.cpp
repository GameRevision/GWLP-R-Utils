#include "stdafx.h"
#include "MC.h"

#define SCAN_START (BYTE*)0x00401000
#define SCAN_END (BYTE*)0x00900000

bool DataCompare(BYTE* CodeMask,char* StringMask,BYTE* pos)
{
	for(;*StringMask; pos++,CodeMask++,StringMask++)
	{
		if(*StringMask == 'x' && *CodeMask != *pos)
			return false;
	}
	return true;
}

BYTE* FindPattern(BYTE* CodeMask,char* StringMask)
{
	BYTE* start = SCAN_START;
	BYTE* end = SCAN_END;
	while(start++ != end)
	{
		if(DataCompare(CodeMask,StringMask,start))
			return start;
	}
	return 0;
}

void MulticlientPatch()
{
	DWORD dwBack;
	BYTE* Patch = NULL;
	BYTE PatchCode[] = {0x75,0x07,0xC7,0x45,0xFC,0xFF,0xFF,0xFF,0xFF,0x85,0xF6,0x75,0xFF,0xFF,0xD7,0x50,0x68};
	Patch = FindPattern( PatchCode, "xxxxx????xxx?xxxx" );
	if(!Patch)
	{
		DisplayError("Unable to retrieve Multi client Patch location!");
		ExitProcess(-1);
	}

	VirtualProtect(Patch,1,PAGE_READWRITE,&dwBack);
	*Patch = (BYTE)0xEB; //JMP
	VirtualProtect(Patch,1,dwBack,&dwBack);
}

void GWDatFix()
{
	DWORD dwBack;
	BYTE GWDat[] = {0x6A, 0x00, 0xBA, 0x00, 0x00, 0x00, 0xC0};
	BYTE* Patch = FindPattern( GWDat, "xxxxxxx" ) + 1;
	if(!Patch)
	{
		DisplayError("Unable to retrieve GWDat fix location!");
		ExitProcess(-1);
	}

	VirtualProtect(Patch,1,PAGE_READWRITE,&dwBack);
	*Patch = (BYTE)0x03;//Share Read/Write
	VirtualProtect(Patch,1,dwBack,&dwBack);

	BYTE WriteCode[] = {0x8D, 0x55, 0x0C, 0x6A, 0x00, 0x52, 0x57, 0x50, 0x51};

	BYTE* Patch2 = FindPattern( WriteCode, "xxxxxxxxx" ) + 3;
	if(!Patch2)
	{
		if(!Patch)
		{
			DisplayError("Unable to retrieve Write Fix location!");
			ExitProcess(-1);
		}
	}
	VirtualProtect(Patch2,12,PAGE_READWRITE,&dwBack);
	for(int i=0;i<12;i++)
		Patch2[i] = (BYTE)0x90;//POP Registers and RETN 10
	VirtualProtect(Patch,12,dwBack,&dwBack);


}