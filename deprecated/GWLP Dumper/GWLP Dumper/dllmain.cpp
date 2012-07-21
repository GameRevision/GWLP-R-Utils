#include "stdafx.h"
#include "MySQLDatabase.h"
#include "GWLP Dumper.h"
#include "detours.h"
#include "NPCDumper.h"
#include "ItemDumper.h"
#include "MapDataDumper.h"

BYTE* StringLogStart = NULL;
BYTE* StringLogEnd = NULL;
BYTE* HashLogStart = NULL;
BYTE* HashLogEnd = NULL;
DWORD GSPacketMetadataBase = NULL;
DWORD LSPacketMetadataBase = NULL;

DWORD CurrentHash = NULL;

void _declspec(naked) HashLogHook()
{
	__asm
	{
		PUSHAD
		MOV EDX,DWORD PTR SS:[EBP+8]
		MOV CurrentHash,EDX
		POPAD
		JMP HashLogEnd
	}
}


void _declspec(naked) StringLogHook()
{
	__asm
	{
		PUSHAD
		PUSH ECX
		PUSH CurrentHash
		CALL ItemArray::CompleteItem
		TEST EAX,EAX
		JNZ StringLogFinish
		//NPC
StringLogFinish:
		MOV CurrentHash,NULL
		POPAD
		JMP StringLogEnd
	}	
}

void scan()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00800000;

	BYTE LSPacketMetadataCode[] = { 0x33, 0xF6, 0x8B, 0x55, 0xF8, 0x8B, 0x45, 0x08, 0x8D, 0x4E, 0x20 };
	BYTE GSPacketMetadataCode[] = { 0x83, 0xEC, 0x08, 0x83, 0xF9, 0x04, 0x8B, 0xC2 };
	BYTE StringLogCode[] = { 0x89, 0x3E, 0x8B, 0x7D, 0x10, 0x89, 0x5E, 0x04, 0x39, 0x7E, 0x08 };
	BYTE HashLogCode[] = { 0x8B, 0x90, 0xCC, 0x01, 0x00, 0x00, 0x8B, 0x88, 0xD0, 0x01, 0x00, 0x00 };

	while(start != end)
	{
		if(!memcmp(LSPacketMetadataCode,start,sizeof(LSPacketMetadataCode)))
		{
			LSPacketMetadataBase = *(DWORD*)(start - 0x38);
		}
		if(!memcmp(GSPacketMetadataCode,start,sizeof(GSPacketMetadataCode)))
		{
			GSPacketMetadataBase = *(DWORD*)(start + 0x4E);
		}
		if(!memcmp(HashLogCode,start,sizeof(HashLogCode)))
		{
			HashLogStart = start;
		}
		if(!memcmp(StringLogCode,start,sizeof(StringLogCode)))
		{
			StringLogStart = start + 0x22;
		}
		start++;
	}
}
// TODO: hook by replacing handlerfunc
BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	PacketDumper* p;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		scan();
		if(!StringLogStart)
		{
			DisplayError("Unable to find StringLogStart!");
			return FALSE;
		}
		if(!HashLogStart)
		{
			DisplayError("Unable to find HashLog!");
			return FALSE;
		}
		if(!GSPacketMetadataBase)
		{
			DisplayError("Unable to find GSPacketMetadataBase!");
			return FALSE;
		}
		if(!LSPacketMetadataBase)
		{
			DisplayError("Unable to find LSPacketMetadataBase!");
			return FALSE;
		}
		
		p  = new PacketDumper(
			ReadPtrChain<StoCPacketMetadata*>(*(DWORD*)LSPacketMetadataBase,0x8,0x20,0x14,0x8,0x2C),ReadPtrChain<int>(*(DWORD*)LSPacketMetadataBase,0x8,0x20,0x14,0x8,0x34),
			ReadPtrChain<StoCPacketMetadata*>(*(DWORD*)GSPacketMetadataBase,0x8,0x2C),ReadPtrChain<int>(*(DWORD*)GSPacketMetadataBase,0x8,0x34)
			);

		new ItemArray();

		
		if(!p->InitDumper())
		{
			return FALSE;
		}

 		p->AddHandler<ItemGeneral>(343,true);
 		p->AddHandler<ItemProfession>(336,true);
 		p->AddHandler<InstanceLoadSpawnPoint>(391,true);
 		p->AddHandler<InstanceLoadDistrictInfo>(395,true);

		HashLogEnd = (BYTE*)DetourFunc(HashLogStart,(BYTE*)HashLogHook,6);
		StringLogEnd = (BYTE*)DetourFunc(StringLogStart,(BYTE*)StringLogHook,6);

		try
		{
			new Threadpool();
		}
		catch (int)
		{
			DisplayError("Unable to create Threadpool!");
			return FALSE;
		}
		

		AllocConsole();
		SetConsoleTitleA("GWLP-R Dumper by ACB");
		FILE *fh;
		freopen_s(&fh, "CONOUT$", "wb", stdout);
		
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

