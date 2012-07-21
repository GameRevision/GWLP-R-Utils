#include "stdafx.h"
#include "Scanner.h"

using namespace std;

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

BYTE* GetLoadStoCTemplateAddr()
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

BYTE* GetStoCPacketStreamHandlerAddr()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00900000;

	BYTE StoCPacketStreamHandlerCode[] = { 0x89, 0x46, 0x44, 0x03, 0xC7, 0x8B, 0xCE };
	while(start != end)
	{
		if(!memcmp(start,StoCPacketStreamHandlerCode,sizeof(StoCPacketStreamHandlerCode)))
			return start + 0x5;
		start++;
	}

	return NULL;
}

BYTE* GetCtoSPacketCrypterAddr()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00900000;

	BYTE CtoSPacketCrypterCode[] = { 0x8B, 0xDC, 0x8D, 0x87, 0xA4, 0x02, 0x00, 0x00 };
	while(start != end)
	{
		if(!memcmp(start,CtoSPacketCrypterCode,sizeof(CtoSPacketCrypterCode)))
			return start + 0x8;
		start++;
	}

	return NULL;

}