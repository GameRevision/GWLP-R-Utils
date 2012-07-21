#include "stdafx.h"
#include "Scanner.h"
#include "TemplateDumper.h"
using namespace std;

BYTE* LoadStoCTemplateStart = NULL;
BYTE* LoadStoCTemplateEnd = NULL;
BYTE* LoadCtoSTemplateStart = NULL;
BYTE* LoadCtoSTemplateEnd = NULL;
BYTE* StoCStreamHandlerStart = NULL;
BYTE* StoCStreamHandlerEnd = NULL;
BYTE* CtoSPacketCrypterStart = NULL;
BYTE* CtoSPacketCrypterEnd = NULL;


pair<StoCPacket*,int> StoCLoginServerTemplates;
pair<CtoSPacket*,int> CtoSLoginServerTemplates;
pair<StoCPacket*,int> StoCGameServerTemplates;
pair<CtoSPacket*,int> CtoSGameServerTemplates;

pair<HANDLE,HANDLE> CtoSFiles;
pair<HANDLE,HANDLE> StoCFiles;

enum
{
	LOGINSERVER,
	GAMESERVER
};

int CurrentCtoSServer = LOGINSERVER;
int CurrentStoCServer = LOGINSERVER;

void __stdcall SaveStoCTable(StoCPacket* Table,int TableSize)
{
	if(TableSize < 100)
	{
		StoCLoginServerTemplates = make_pair(Table,TableSize);
		CurrentStoCServer = LOGINSERVER;
	}
	else
	{
		StoCGameServerTemplates = make_pair(Table,TableSize);
		CurrentStoCServer = GAMESERVER;
	}
}

void __declspec(naked) LoadStoCTemplateHook()
{
	__asm
	{
		PUSHAD
		PUSH DWORD PTR DS:[ECX + 0x34]
		PUSH DWORD PTR DS:[ECX + 0x2C]
		CALL SaveStoCTable
		POPAD
		JMP LoadStoCTemplateEnd 
	}
}

void __stdcall SaveCtoSTable(CtoSPacket* Table,int TableSize)
{
	if(TableSize < 100)
	{
		CtoSLoginServerTemplates = make_pair(Table,TableSize);
		CurrentCtoSServer = LOGINSERVER;
	}
	else
	{
		CtoSGameServerTemplates = make_pair(Table,TableSize);
		CurrentCtoSServer = GAMESERVER;
	}
}

void __declspec(naked) LoadCtoSTemplateHook()
{
	__asm
	{
		PUSHAD
		PUSH DWORD PTR DS:[ECX + 0x24]
		PUSH DWORD PTR DS:[ECX + 0x1C]
		CALL SaveCtoSTable
		POPAD
		JMP LoadCtoSTemplateEnd
	}
}

void __stdcall DumpStoCStream(BYTE* StreamStart,int length)
{
	DWORD dwWritten;
	if(CurrentStoCServer == LOGINSERVER)
		WriteFile(StoCFiles.first,StreamStart,length,&dwWritten,NULL);
	else
		WriteFile(StoCFiles.second,StreamStart,length,&dwWritten,NULL);
}

void __declspec(naked) StoCStreamHandlerHook()
{
	__asm
	{
		PUSHAD
		PUSH EDI//end
		PUSH DWORD PTR DS:[ESI+0x44]//start
		CALL DumpStoCStream
		POPAD
		JMP StoCStreamHandlerEnd
	}
}

void __stdcall DumpCtoSPacket(BYTE* packet,int length)
{
	DWORD dwWritten;
	if(CurrentCtoSServer == LOGINSERVER)
		WriteFile(CtoSFiles.first,packet,length,&dwWritten,NULL);
	else
		WriteFile(CtoSFiles.second,packet,length,&dwWritten,NULL);
}

void __declspec(naked) CtoSPacketCrypterHook()
{
	__asm
	{
		PUSHAD
		PUSH ESI//size
		PUSH EAX//packet
		CALL DumpCtoSPacket
		POPAD
		JMP CtoSPacketCrypterEnd
	}
}

DWORD WINAPI InputThread(LPVOID lpParam)
{
	CreateDirectory(TEXT(".\\PacketExplorer"),NULL);
	CreateDirectory(TEXT(".\\PacketExplorer\\Dump"),NULL);
	HANDLE login;
	HANDLE game;
	login = CreateFile(TEXT(".\\PacketExplorer\\Dump\\[CtoS]Login Server.dump"),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	game = CreateFile(TEXT(".\\PacketExplorer\\Dump\\[CtoS]Game Server.dump"),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	CtoSFiles = make_pair(login,game);

	login = CreateFile(TEXT(".\\PacketExplorer\\Dump\\[StoC]Login Server.dump"),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	game = CreateFile(TEXT(".\\PacketExplorer\\Dump\\[StoC]Game Server.dump"),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	StoCFiles = make_pair(login,game);

	while(true)
	{
		Sleep(10);
		if(GetAsyncKeyState(VK_F1) & 1)
		{
			DumpCtoSTemplates(TEXT(".\\PacketExplorer\\Dump\\[CtoS]Login Server.template"),CtoSLoginServerTemplates.first,CtoSLoginServerTemplates.second);
			DumpStoCTemplates(TEXT(".\\PacketExplorer\\Dump\\[StoC]Login Server.template"),StoCLoginServerTemplates.first,StoCLoginServerTemplates.second);
			DumpCtoSTemplates(TEXT(".\\PacketExplorer\\Dump\\[CtoS]Game Server.template"),CtoSGameServerTemplates.first,CtoSGameServerTemplates.second);
			DumpStoCTemplates(TEXT(".\\PacketExplorer\\Dump\\[StoC]Game Server.template"),StoCGameServerTemplates.first,StoCGameServerTemplates.second);
			Sleep(5000);
		}
		if(GetAsyncKeyState(VK_F12) & 1)
		{
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)FreeLibrary,lpParam,0,0);
			return 0;
		}
	}
	return 0;
}

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

BOOL APIENTRY DllMain( HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		LoadStoCTemplateStart = GetLoadStoCTemplateAddr();
		if(!LoadStoCTemplateStart)
		{
			MessageBoxA(0,"Unable to retrieve LoadFuncAddr addr","Error",MB_OK|MB_ICONERROR);
			return false;
		}
		LoadStoCTemplateEnd = (BYTE*)DetourFunc(LoadStoCTemplateStart,(BYTE*)LoadStoCTemplateHook,6);

		LoadCtoSTemplateStart = GetLoadCtoSTemplateAddr();
		if(!LoadCtoSTemplateStart)
		{
			MessageBoxA(0,"Unable to retrieve LoadCtoSTemplate addr","Error",MB_OK|MB_ICONERROR);
			return false;
		}
		LoadCtoSTemplateEnd = (BYTE*)DetourFunc(LoadCtoSTemplateStart,(BYTE*)LoadCtoSTemplateHook,6);

		StoCStreamHandlerStart = GetStoCPacketStreamHandlerAddr();
		if(!StoCStreamHandlerStart)
		{
			MessageBoxA(0,"Unable to retrieve StoCStreamHandler addr","Error",MB_OK|MB_ICONERROR);
			return false;
		}
		StoCStreamHandlerEnd = (BYTE*)DetourFunc(StoCStreamHandlerStart,(BYTE*)StoCStreamHandlerHook,5);

		CtoSPacketCrypterStart = GetCtoSPacketCrypterAddr();
		if(!CtoSPacketCrypterStart)
		{
			MessageBoxA(0,"Unable to retrieve CtoSPacketCrypter addr","Error",MB_OK|MB_ICONERROR);
			return false;
		}
		CtoSPacketCrypterEnd = (BYTE*)DetourFunc(CtoSPacketCrypterStart,(BYTE*)CtoSPacketCrypterHook,9);

		CreateThread(NULL,0,InputThread,NULL,NULL,NULL);
		break;
	case DLL_PROCESS_DETACH:
		RetourFunc(LoadStoCTemplateStart,LoadStoCTemplateEnd,6);
		RetourFunc(LoadCtoSTemplateStart,LoadCtoSTemplateEnd,6);

		RetourFunc(StoCStreamHandlerStart,StoCStreamHandlerEnd,5);
		RetourFunc(CtoSPacketCrypterEnd,CtoSPacketCrypterStart,9);
		break;
	}
	return TRUE;
}

