#pragma once
#include "stdafx.h"
#include "PacketTemplateParser.h"

							//Type		//Name		//Data
typedef std::tr1::tuple<std::tstring,std::tstring,std::tstring> DataTuple;
struct Packet
{
	short Header;					
	std::vector<DataTuple> PacketData;
};

class PacketStreamParser
{
	union Data
	{
		BYTE b;
		WORD w;
		DWORD dw;
		float f;
		DWORD id;
		short s;
		wchar_t wc;
	};
public:
	PacketStreamParser(std::tstring filename);
	~PacketStreamParser(void);

	bool Parse();

	int GetPacketCount(){return m_Packets.size();}
	Packet* GetPacket(int i){return &m_Packets[i];}
	Packet* GetNextPacket(){return &m_Packets[m_CurPacket++];}
private:
	template<typename packet_type,typename result_type> bool GetDataFromPacketQueue( BYTE** readptr,BYTE* buffend,result_type* res );
private:
	std::tstring m_PacketQueueFile;
	PacketTemplateParser* m_Template;
	std::vector<Packet> m_Packets;
	int m_CurPacket;
};

