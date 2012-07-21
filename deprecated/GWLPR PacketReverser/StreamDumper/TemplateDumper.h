#pragma once
#include "stdafx.h"

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

void DumpStoCTemplates(std::tstring FileName,StoCPacket* pack,int templateCount);
void DumpCtoSTemplates(std::tstring FileName,CtoSPacket* pack,int templateCount);