#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "TemplateDecoder.h"

using namespace std;

BYTE* LoadFuncAddrStart = NULL;
BYTE* LoadFuncAddrEnd = NULL;
BYTE* LoadCtoSTemplateStart = NULL;
BYTE* LoadCtoSTemplateEnd = NULL;

struct StoCPacket
{
	DWORD* PacketTemplate;
	int TemplateSize;
	DWORD HandlerFunc;
};

struct CtoSPacket
{
	DWORD* PacketTemplate;
	int TemplateSize;
};

multimap<DWORD,pair<int,StoCPacket>> StoCPackets;
multimap<DWORD,pair<int,CtoSPacket>> CtoSPackets;

void __stdcall CopyStoCTable(StoCPacket* Table,int TableSize)
{
	if(StoCPackets.count((DWORD)Table) >= TableSize)
		return;

	multimap<DWORD,pair<int,StoCPacket>>::iterator it;
	pair<multimap<DWORD,pair<int,StoCPacket>>::iterator,multimap<DWORD,pair<int,StoCPacket>>::iterator> ret;
	ret = StoCPackets.equal_range((DWORD)Table);

	for(it=ret.first; it!=ret.second; ++it )
		StoCPackets.erase(it);

	for(int i=0;i<TableSize;i++)
	{
		StoCPackets.insert(make_pair((DWORD)Table,make_pair(i,Table[i])));
	}
}

void __declspec(naked) LoadFuncAddrHook()
{
	__asm
	{
		PUSHAD
		PUSH DWORD PTR DS:[ECX + 0x34]
		PUSH DWORD PTR DS:[ECX + 0x2C]
		CALL CopyStoCTable
		POPAD
		JMP LoadFuncAddrEnd
	}
}

void __stdcall CopyCtoSTable(CtoSPacket* Table,int TableSize)
{
	if(CtoSPackets.count((DWORD)Table) >= TableSize)
		return;

	multimap<DWORD,pair<int,CtoSPacket>>::iterator it;
	pair<multimap<DWORD,pair<int,CtoSPacket>>::iterator,multimap<DWORD,pair<int,CtoSPacket>>::iterator> ret;
	ret = CtoSPackets.equal_range((DWORD)Table);

	for(it=ret.first; it!=ret.second; ++it )
		CtoSPackets.erase(it);

	for(int i=0;i<TableSize;i++)
	{
		CtoSPackets.insert(make_pair((DWORD)Table,make_pair(i,Table[i])));
	}
}

void __declspec(naked) LoadCtoSTemplateHook()
{
	__asm
	{
		PUSHAD
		PUSH DWORD PTR DS:[ECX + 0x24]
		PUSH DWORD PTR DS:[ECX + 0x1C]
		CALL CopyCtoSTable
		POPAD
		JMP LoadCtoSTemplateEnd
	}
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

BYTE* GetLoadFuncAddr()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00900000;

	BYTE LoadFuncCode[] = { 0x8B, 0x4F, 0x2C, 0x8D, 0x04, 0x76 };
	while(start != end)
	{
		if(!memcmp(start,LoadFuncCode,sizeof(LoadFuncCode)))
			return start - 0x10;

		start++;
	}

	return NULL;
}

BYTE* GetLoadCtoSTemplateAddr()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00900000;

	BYTE LoadCtoSTemplateCode[] = { 0x8B, 0x47, 0x1C, 0x8D, 0x34, 0xF0, 0x8B, 0x46, 0x04 };
	while(start != end)
	{
		if(!memcmp(start,LoadCtoSTemplateCode,sizeof(LoadCtoSTemplateCode)))
			return start - 0x38;

		start++;
	}

	return NULL;
}

void DumpStoC()
{
	map<DWORD,FILE*> files;
	multimap<DWORD,pair<int,StoCPacket>>::iterator it;
	for(it = StoCPackets.begin();it != StoCPackets.end();it++)
	{
		if(files.find(it->first) == files.end())
		{
			char* name;
			if(StoCPackets.count(it->first) < 100)
				name = ".\\Packet Templates\\[StoC] Login server Table.txt";
			else
				name = ".\\Packet Templates\\[StoC] Game server Table.txt";

			files[it->first] = fopen(name,"w");
			fprintf_s(files[it->first],"Table has %i entries\n\n",StoCPackets.count(it->first));
		}
		fprintf(files[it->first],"Packet 0x%04X\n",it->second.first);
		fprintf(files[it->first],"\tTemplateSize: %i\n\n",it->second.second.TemplateSize);
		fprintf(files[it->first],"\tTemplate: ");
		for(int i=0;i<it->second.second.TemplateSize;i++)
		{	
			fprintf(files[it->first],"0x%08X ",it->second.second.PacketTemplate[i]);
			if((i == 3 || (i > 4 && i%4 == 0)) && i+1 < it->second.second.TemplateSize)
				fprintf(files[it->first],"\n\t\t");
		}
		fprintf(files[it->first],"\n\n\tDecodedTemplate: \n%s",DecodeTemplate(it->second.second.PacketTemplate,it->second.second.TemplateSize).c_str());
		fprintf(files[it->first],"\n\n\tHandler: 0x%08X\n\n\n",it->second.second.HandlerFunc);
	}

	map<DWORD,FILE*>::iterator it2;
	for(it2 = files.begin();it2 != files.end();it2++)
		fclose(it2->second);
}

void DumpStoCCode()
{
	multimap<DWORD,pair<int,StoCPacket>>::iterator it;
	for(it = StoCPackets.begin();it != StoCPackets.end();it++)
	{
		char handler_name[MAX_PATH];
		if(StoCPackets.count(it->first) < 100)
			sprintf_s(handler_name,".\\Packet Templates\\Code\\[StoC]Login server\\Packet%i.cs",it->second.first);
		else
			sprintf_s(handler_name,".\\Packet Templates\\Code\\[StoC]Game server\\Packet%i.cs",it->second.first);

		if(it->second.second.PacketTemplate == 0)
			continue;

		FILE* handler_file = fopen(handler_name,"w");

		fprintf_s(handler_file,"using System;\n");
		fprintf_s(handler_file,"using ServerEngine.ProcessorQueues;\n");
		fprintf_s(handler_file,"using ServerEngine.PacketManagement.CustomAttributes;\n");
		fprintf_s(handler_file,"using ServerEngine.PacketManagement.Definitions;\n\n");

		if(StoCPackets.count(it->first) < 100)
			fprintf_s(handler_file,"namespace LoginServer.Packets.ToClient\n");
		else
			fprintf_s(handler_file,"namespace GameServer.Packets.ToClient\n");
		
		fprintf_s(handler_file,"{\n");

		fprintf_s(handler_file,TAB "[PacketAttributes(IsIncoming = false, Header = %i)]\n",it->second.first);
		fprintf_s(handler_file,TAB "public class Packet%i : IPacket\n",it->second.first);
		fprintf_s(handler_file,TAB "{\n");

		fprintf_s(handler_file,"%s",DecodeTemplateCSharp(it->second.second.PacketTemplate,it->second.second.TemplateSize).c_str());

		fprintf_s(handler_file,TAB TAB "public void InitPacket(object parser)\n");
		fprintf_s(handler_file,TAB TAB "{\n");
		fprintf_s(handler_file,TAB TAB TAB "pParser = (PacketParser<PacketSt%i>)parser;\n",it->second.first);
		fprintf_s(handler_file,TAB TAB TAB "IsInitialized = true;\n");
		fprintf_s(handler_file,TAB TAB TAB "IsInUse = false;\n");
		fprintf_s(handler_file,TAB TAB "}\n\n");

		fprintf_s(handler_file,TAB TAB "public bool Handler(ref NetworkMessage message)\n");
		fprintf_s(handler_file,TAB TAB "{\n");
		fprintf_s(handler_file,TAB TAB TAB "pParser((PacketSt%i)message.PacketTemplate, message.PacketData);\n",it->second.first);
		fprintf_s(handler_file,TAB TAB TAB "QueuingService.NetOutQueue.Enqueue(message);\n");
		fprintf_s(handler_file,TAB TAB TAB "return true;\n");
		fprintf_s(handler_file,TAB TAB "}\n\n");

		fprintf_s(handler_file,TAB TAB "public bool IsInitialized { get; set; }\n\n");

		fprintf_s(handler_file,TAB TAB "public bool IsInUse { get; set; }\n\n");

		fprintf_s(handler_file,TAB TAB "private PacketParser<PacketSt%i> pParser;\n\n",it->second.first);

		fprintf_s(handler_file,TAB "}\n");//public class PacketX : IPacket

		fprintf_s(handler_file,"}\n");//namespace ScratchGWLPR.Packets.ToClient
		fclose(handler_file);
	}
}

void DumpCtoS()
{
	map<DWORD,FILE*> files;
	multimap<DWORD,pair<int,CtoSPacket>>::iterator it;
	for(it = CtoSPackets.begin();it != CtoSPackets.end();it++)
	{
		if(files.find(it->first) == files.end())
		{
			char* name;
			if(CtoSPackets.count(it->first) < 100)
				name = ".\\Packet Templates\\[CtoS] Login server Table.txt";
			else
				name = ".\\Packet Templates\\[CtoS] Game server Table.txt";

			files[it->first] = fopen(name,"w");
			fprintf_s(files[it->first],"Table has %i entries\n\n",CtoSPackets.count(it->first));
		}
		fprintf(files[it->first],"Packet 0x%04X\n",it->second.first);
		fprintf(files[it->first],"\tTemplateSize: %i\n\n",it->second.second.TemplateSize);
		fprintf(files[it->first],"\tTemplate: ");
		for(int i=0;i<it->second.second.TemplateSize;i++)
		{	
			fprintf(files[it->first],"0x%08X ",it->second.second.PacketTemplate[i]);
			if((i == 3 || (i > 4 && i%4 == 0)) && i+1 < it->second.second.TemplateSize)
				fprintf(files[it->first],"\n\t\t");
		}
		fprintf(files[it->first],"\n\n\tDecodedTemplate: \n%s",DecodeTemplate(it->second.second.PacketTemplate,it->second.second.TemplateSize).c_str());
	}

	map<DWORD,FILE*>::iterator it2;
	for(it2 = files.begin();it2 != files.end();it2++)
		fclose(it2->second);
}

void DumpCtoSCode()
{
	multimap<DWORD,pair<int,CtoSPacket>>::iterator it;
	for(it = CtoSPackets.begin();it != CtoSPackets.end();it++)
	{
		char handler_name[MAX_PATH];
		if(CtoSPackets.count(it->first) < 100)
			sprintf_s(handler_name,".\\Packet Templates\\Code\\[CtoS]Login server\\Packet%i.cs",it->second.first);
		else
			sprintf_s(handler_name,".\\Packet Templates\\Code\\[CtoS]Game server\\Packet%i.cs",it->second.first);

		if(it->second.second.PacketTemplate == 0)
			continue;

		FILE* handler_file = fopen(handler_name,"w");

		fprintf_s(handler_file,"using System;\n");
		fprintf_s(handler_file,"using ServerEngine.ProcessorQueues;\n");
		fprintf_s(handler_file,"using ServerEngine.PacketManagement.CustomAttributes;\n");
		fprintf_s(handler_file,"using ServerEngine.PacketManagement.Definitions;\n\n");

		if(CtoSPackets.count(it->first) < 100)
			fprintf_s(handler_file,"namespace LoginServer.Packets.FromClient\n");
		else
			fprintf_s(handler_file,"namespace GameServer.Packets.FromClient\n");
		
		fprintf_s(handler_file,"{\n");

		fprintf_s(handler_file,TAB "[PacketAttributes(IsIncoming = true, Header = %i)]\n",it->second.first);
		fprintf_s(handler_file,TAB "public class Packet%i : IPacket\n",it->second.first);
		fprintf_s(handler_file,TAB "{\n");

		fprintf_s(handler_file,"%s",DecodeTemplateCSharp(it->second.second.PacketTemplate,it->second.second.TemplateSize).c_str());

		fprintf_s(handler_file,TAB TAB "public void InitPacket(object parser)\n");
		fprintf_s(handler_file,TAB TAB "{\n");
		fprintf_s(handler_file,TAB TAB TAB "pParser = (PacketParser<PacketSt%i>)parser;\n",it->second.first);
		fprintf_s(handler_file,TAB TAB TAB "IsInitialized = true;\n");
		fprintf_s(handler_file,TAB TAB TAB "IsInUse = false;\n");
		fprintf_s(handler_file,TAB TAB "}\n\n");

		fprintf_s(handler_file,TAB TAB "public bool Handler(ref NetworkMessage message)\n");
		fprintf_s(handler_file,TAB TAB "{\n");
		fprintf_s(handler_file,TAB TAB TAB "throw new NotImplementedException();\n");
		fprintf_s(handler_file,TAB TAB "}\n\n");

		fprintf_s(handler_file,TAB TAB "public bool IsInitialized { get; set; }\n\n");

		fprintf_s(handler_file,TAB TAB "public bool IsInUse { get; set; }\n\n");

		fprintf_s(handler_file,TAB TAB "private PacketParser<PacketSt%i> pParser;\n",it->second.first);

		fprintf_s(handler_file,TAB "}\n");//public class PacketX : IPacket

		fprintf_s(handler_file,"}\n");//namespace ScratchGWLPR.Packets.FromClient
		fclose(handler_file);
	}
}


DWORD WINAPI InputThread(LPVOID lpParam)
{
	while(true)
	{
		Sleep(0);
		if(GetAsyncKeyState(VK_INSERT) & 1)
		{
			CreateDirectoryA(".\\Packet Templates",NULL);
			CreateDirectoryA(".\\Packet Templates\\Code",NULL);
			CreateDirectoryA(".\\Packet Templates\\Code\\[CtoS]Login server",NULL);
			CreateDirectoryA(".\\Packet Templates\\Code\\[CtoS]Game server",NULL);
			CreateDirectoryA(".\\Packet Templates\\Code\\[StoC]Login server",NULL);
			CreateDirectoryA(".\\Packet Templates\\Code\\[StoC]Game server",NULL);
			DumpStoC();
			DumpStoCCode();
			DumpCtoS();
			DumpCtoSCode();
			Sleep(5000);
		}
		if(GetAsyncKeyState(VK_DELETE) & 1)
		{
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)FreeLibrary,lpParam,0,0);
			return 0;
		}
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		LoadFuncAddrStart = GetLoadFuncAddr();
		if(!LoadFuncAddrStart)
		{
			MessageBoxA(0,"Unable to retrieve LoadFuncAddr addr","Error",MB_OK|MB_ICONERROR);
			return false;
		}
		LoadFuncAddrEnd = (BYTE*)DetourFunc(LoadFuncAddrStart,(BYTE*)LoadFuncAddrHook,6);

		LoadCtoSTemplateStart = GetLoadCtoSTemplateAddr();
		if(!LoadCtoSTemplateStart)
		{
			MessageBoxA(0,"Unable to retrieve LoadCtoSTemplate addr","Error",MB_OK|MB_ICONERROR);
			return false;
		}
		LoadCtoSTemplateEnd = (BYTE*)DetourFunc(LoadCtoSTemplateStart,(BYTE*)LoadCtoSTemplateHook,6);

		CreateThread(NULL,0,InputThread,hModule,0,0);
		break;
	case DLL_PROCESS_DETACH:
		RetourFunc(LoadFuncAddrStart,LoadFuncAddrEnd,6);
		RetourFunc(LoadCtoSTemplateStart,LoadCtoSTemplateEnd,6);
		break;
	}
	return true;
}
