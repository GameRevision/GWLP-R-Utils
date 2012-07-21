#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "Scanner.h"

using namespace std;

BYTE* StoCStreamHandlerStart = NULL;
BYTE* StoCStreamHandlerEnd = NULL;
BYTE* StoCLoadPacketTemplateStart = NULL;
BYTE* StoCLoadPacketTemplateEnd = NULL;
BYTE* StoCPacketHandlerStart = NULL;
BYTE* StoCPacketHandlerEnd = NULL;
BYTE* StoCPacketStreamEndStart = NULL;
BYTE* StoCPacketStreamEndEnd = NULL;

BYTE* StoCCurPacket = NULL;
DWORD StoCStreamSize = 0;
DWORD TempStoCStreamLength = 0;
BYTE TempStoCStream[0x1000];

char* CurrentFolder = NULL;

void DumpPacket(BYTE* pack,int length)
{
	static char FileName[MAX_PATH];

	short Header = *(short*)pack;

	sprintf_s(FileName,"%s\\ByteArrayDump.txt",CurrentFolder);

	FILE* f = fopen(FileName,"a");

	fprintf(f,"var packet%i = new NetworkMessage(netID);\n",Header);
	fprintf(f,"rawData = {\n"
		"        ");

	if(TempStoCStreamLength)
	{
		for(int i=0;i<TempStoCStreamLength;i++)
		{
			fprintf(f,"0x%02X, ",TempStoCStream[i]);
			if(i && !((i+1)%12))
				fprintf(f,"\n        ");
		}
		fprintf(f,"\n        ");
		TempStoCStreamLength = 0;
	}	

	for(int i=0;i<length;i++)
	{
		fprintf(f,"0x%02X, ",pack[i]);
			if(i && !((i+1)%12))
				fprintf(f,"\n        ");
	}

	fprintf(f,"};\n"
				"packet%i.PacketData = new MemoryStream(rawData);\n"
				"QueuingService.NetOutQueue.Enqueue(packet%i);\n\n\n",Header,Header);

	fclose(f);
}

void __stdcall StoCStreamStart(BYTE* stream,DWORD size)
{
	StoCCurPacket = stream;
	StoCStreamSize = size;
}

void __stdcall StoCLoadPacketTemplate(DWORD size)
{
	if(size < 100)
		CurrentFolder = ".\\Dump\\StoC\\LS";
	else
		CurrentFolder = ".\\Dump\\StoC\\GS";
}

void __stdcall StoCPacketHandler(BYTE* new_stream_start)
{
	if(!StoCCurPacket)
		return;
	StoCStreamSize -= (DWORD)new_stream_start - (DWORD)StoCCurPacket;
	DumpPacket(StoCCurPacket,(DWORD)new_stream_start - (DWORD)StoCCurPacket);
	StoCCurPacket = new_stream_start;
}

void __stdcall StoCPacketStreamEnd()
{
	if(StoCStreamSize)
	{
		memcpy(TempStoCStream,StoCCurPacket,StoCStreamSize);
		TempStoCStreamLength = StoCStreamSize;
	}
}

#pragma region Hooks

void __declspec(naked) StoCStreamStartHook()
{
	__asm
	{
		PUSHAD
		PUSH EDI//length
		PUSH DWORD PTR DS:[ESI + 0x44]//start
		CALL StoCStreamStart
		POPAD
		JMP StoCStreamHandlerEnd
	}
}

void __declspec(naked) StoCLoadPacketTemplateHook()
{
	__asm
	{
		PUSHAD
		PUSH DWORD PTR DS:[ECX + 0x34]//length
		CALL StoCLoadPacketTemplate
		POPAD
		JMP StoCLoadPacketTemplateEnd
	}
}

void __declspec(naked) StoCPacketHandlerHook()
{
	__asm
	{
		PUSHAD
		PUSH DWORD PTR DS:[ESI + 0x44]//new stream start
		CALL StoCPacketHandler
		POPAD
		JMP StoCPacketHandlerEnd
	}
}

void __declspec(naked) StoCPacketStreamEndHook()
{
	__asm
	{
		PUSHAD
		CALL StoCPacketStreamEnd
		POPAD
		JMP StoCPacketStreamEndEnd
	}
}

#pragma endregion 
//Hooks

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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		CreateDirectory(TEXT(".\\Dump"),NULL);
		CreateDirectory(TEXT(".\\Dump\\StoC"),NULL);
		CreateDirectory(TEXT(".\\Dump\\StoC\\LS"),NULL);
		CreateDirectory(TEXT(".\\Dump\\StoC\\GS"),NULL);

		StoCStreamHandlerStart = GetStoCPacketStreamHandlerAddr();
		if(!StoCStreamHandlerStart)
		{
			MessageBoxA(0,"Scan Error","Unable to locate StoCStreamHandlerStart!",MB_OK|MB_ICONERROR);
			return FALSE;
		}
		StoCStreamHandlerEnd = (BYTE*)DetourFunc(StoCStreamHandlerStart,(BYTE*)StoCStreamStartHook,5);

		StoCLoadPacketTemplateStart = GetLoadStoCTemplateAddr();
		if(!StoCLoadPacketTemplateStart)
		{
			MessageBoxA(0,"Scan Error","Unable to locate StoCLoadPacketTemplateStar!",MB_OK|MB_ICONERROR);
			return FALSE;
		}
		StoCLoadPacketTemplateEnd = (BYTE*)DetourFunc(StoCLoadPacketTemplateStart,(BYTE*)StoCLoadPacketTemplateHook,6);

		StoCPacketHandlerStart = GetStoCPacketHandlerAddr();
		if(!StoCPacketHandlerStart)
		{
			MessageBoxA(0,"Scan Error","Unable to locate StoCPacketHandler!",MB_OK|MB_ICONERROR);
			return FALSE;
		}
		StoCPacketHandlerEnd = (BYTE*)DetourFunc(StoCPacketHandlerStart,(BYTE*)StoCPacketHandlerHook,6);

		StoCPacketStreamEndStart = StoCPacketHandlerStart + 0x2F;
		StoCPacketStreamEndEnd = (BYTE*)DetourFunc(StoCPacketStreamEndStart,(BYTE*)StoCPacketStreamEndHook,5);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

