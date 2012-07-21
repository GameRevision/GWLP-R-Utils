#include "Scanner.h"
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

BYTE* GetStoCPacketHandlerAddr()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00900000;

	BYTE StoCPacketHandlerCode[] = { 0x8B, 0x46, 0x18, 0x8B, 0x55, 0xF4, 0x8D, 0x8E, 0x58, 0x08, 0x00, 0x00 };
	while(start != end)
	{
		if(!memcmp(start,StoCPacketHandlerCode,sizeof(StoCPacketHandlerCode)))
			return start + 6;
		start++;
	}

	return NULL;
}
