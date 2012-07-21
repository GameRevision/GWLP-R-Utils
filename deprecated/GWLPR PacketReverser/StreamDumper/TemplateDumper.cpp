#include "stdafx.h"
#include "TemplateDumper.h"

using namespace std;

pair<tstring,int> ConvertTemplate(DWORD* Template,int TemplateSize)
{
	if(!Template)
		return make_pair(_T(""),0);

	int Data = 1;
	int ID = 1;
	int ArraySize = 1;

	int Type;

	int string_count = 0;
	tstringstream packet_structure;
	packet_structure << _T("Header|");
	string_count++;

	for(int i=1;i<TemplateSize;i++)
	{
		Type = Template[i] & 0x0F;

		switch(Type)
		{
		case 3:
			packet_structure << _T("Data") << dec << Data << _T("|");Data++;string_count++;
		case 2:
			packet_structure << _T("Data") << dec << Data << _T("|");Data++;string_count++;
		case 1:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			packet_structure << _T("Data") << dec << Data << _T("|");Data++;string_count++;
			break;

		case 0:
			packet_structure << _T("ID") << dec << ID << _T("|");ID++;string_count++;
			break;

		
		case 11:
			packet_structure << _T("ArraySize") << dec << ArraySize << _T("|");ArraySize++;string_count++;
			packet_structure << _T("Data") << dec << Data << _T("|");Data++;string_count++;
			break;

		case 12:
			packet_structure << _T("ArraySize") << dec << ArraySize << _T("|");ArraySize++;string_count++;
			break;
		}
	}
	return make_pair(packet_structure.str(),string_count);
}

void DumpStoCTemplates( std::tstring FileName,StoCPacket* pack,int templateCount )
{
	HANDLE hFile = CreateFile(FileName.c_str(),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(!hFile)
		return;
	DWORD dwWritten;
	for(int i=0;i<templateCount;i++)
	{
		WriteFile(hFile,&(pack[i].TemplateSize),sizeof(int),&dwWritten,NULL);
		WriteFile(hFile,pack[i].PacketTemplate,sizeof(DWORD) * pack[i].TemplateSize,&dwWritten,NULL);
		pair<tstring,int> ConvertedTemplate = ConvertTemplate(pack[i].PacketTemplate,pack[i].TemplateSize);
		WriteFile(hFile,&ConvertedTemplate.second,sizeof(int),&dwWritten,NULL);
		WriteFile(hFile,ConvertedTemplate.first.c_str(),ConvertedTemplate.first.length() * sizeof(TCHAR),&dwWritten,NULL);
	}
	CloseHandle(hFile);
}

void DumpCtoSTemplates( std::tstring FileName,CtoSPacket* pack,int templateCount )
{
	HANDLE hFile = CreateFile(FileName.c_str(),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(!hFile)
		return;
	DWORD dwWritten;
	for(int i=0;i<templateCount;i++)
	{
		WriteFile(hFile,&(pack[i].TemplateSize),sizeof(int),&dwWritten,NULL);
		WriteFile(hFile,pack[i].PacketTemplate,sizeof(DWORD) * pack[i].TemplateSize,&dwWritten,NULL);
		pair<tstring,int> ConvertedTemplate = ConvertTemplate(pack[i].PacketTemplate,pack[i].TemplateSize);
		WriteFile(hFile,&ConvertedTemplate.second,sizeof(int),&dwWritten,NULL);
		WriteFile(hFile,ConvertedTemplate.first.c_str(),ConvertedTemplate.first.length() * sizeof(TCHAR),&dwWritten,NULL);
	}
	CloseHandle(hFile);
}