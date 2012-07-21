#pragma once
#include "stdafx.h"
#include <map>
#include <queue>
#include "Singleton.h"
#include "Packet.h"
#include "MySQLDatabase.h"

#define CONFIG_NAME "dumper.xml"

typedef bool (__fastcall *handler)(Packet*,DWORD);

struct StoCPacketMetadata
{
	DWORD* PacketTemplate;
	int TemplateSize;
	handler HandlerFunc;
};

class PacketHandler
{
public:
	virtual bool HandlePacket(MySQLDatabase* database){return false;}
	virtual DWORD GetHeader(){return -1;}
};

class PacketDumper : public Singleton<PacketDumper>
{
public:

	PacketDumper(StoCPacketMetadata* ls,int lscount,StoCPacketMetadata* gs,int gscount)
		:Singleton(),m_LSPacketMetadata(ls),m_GSPacketMetadata(gs)
	{
		m_OrigLSHandler = new handler[lscount];
		m_OrigGSHandler = new handler[gscount];
	}

	~PacketDumper()
	{
		if(m_Database)
			m_Database->~MySQLDatabase();
	}

	bool InitDumper();

	template<class T> void AddHandler(DWORD Header,bool Gameserver)
	{
		if(Gameserver)
		{
			m_OrigGSHandler[Header] = m_GSPacketMetadata[Header].HandlerFunc;
			m_GSPacketMetadata[Header].HandlerFunc = GSEnqueuePacket<T>;
		}
		else
		{
			m_OrigLSHandler[Header] = m_LSPacketMetadata[Header].HandlerFunc;
			m_LSPacketMetadata[Header].HandlerFunc = LSEnqueuePacket<T>;
		}
	}

	MySQLDatabase* GetDatabase(){return m_Database;}
private:
	template<class T> static bool __fastcall LSEnqueuePacket(Packet* pack,DWORD unk)
	{
		AutoMutex mutex(m_PacketQueueMutex);
		m_PacketQueue.push(new T(pack));
		return m_OrigLSHandler[pack->Header](pack,unk);
	}
	template<class T> static bool __fastcall GSEnqueuePacket(Packet* pack,DWORD unk)
	{
		AutoMutex mutex(m_PacketQueueMutex);
		m_PacketQueue.push(new T(pack));
		return m_OrigGSHandler[pack->Header](pack,unk);
	}
	static DWORD WINAPI ProcessPacketThread(LPVOID);
private:
	static std::queue<PacketHandler*> m_PacketQueue;
	static HANDLE m_PacketQueueMutex;
	static MySQLDatabase* m_Database;
	static handler* m_OrigLSHandler;
	static handler* m_OrigGSHandler;
	StoCPacketMetadata* m_LSPacketMetadata;
	StoCPacketMetadata* m_GSPacketMetadata;
};




