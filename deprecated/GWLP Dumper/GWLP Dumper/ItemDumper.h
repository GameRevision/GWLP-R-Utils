#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "GWLP Dumper.h"
#include "Threadpool.h"

//#define DUMP_PREDEF

#pragma pack(1)
struct P343_ItemGeneral : public Packet
{
	DWORD 	LocalID; 	
	DWORD 	FileID;
	DWORD 	ItemType;
	DWORD 	Data2;
	DWORD 	DyeColor; 	
	DWORD 	Data4;
	DWORD 	CanBeDyed; 	
	DWORD 	Flags;	
	DWORD 	MerchantPrice; 	
	DWORD 	ItemID;
	DWORD 	Quantity; 	
	wchar_t NameHash[64];
	DWORD 	NumStats; 	
	DWORD 	Stats[64];
};

#pragma pack(1)
struct P336_ItemProfession : public Packet
{
	DWORD 	ItemLocalID;
	DWORD 	Profession;
};
#pragma pack()
struct Item
{
	DWORD ItemID;
	DWORD FileID;
	BYTE ItemType;
	WORD DyeColor;
	BYTE CanBeDyed;
	DWORD Flags;
	DWORD MerchantPrice;
	BYTE Profession;
	wchar_t NameHash[64];
	DWORD NumStats;
	DWORD Stats[64];
	wchar_t* Name;
};

class ItemArray : public Singleton<ItemArray>
{
public:
	ItemArray() 
		:Singleton()
	{
		m_Mutex = CreateMutex(NULL,FALSE,NULL);
	}

	void InsertItem(int id,Item i)
	{
		AutoMutex mutex(m_Mutex);
		m_Items.insert(std::make_pair(id,i));
	}
	void RemoveItem(int id)
	{
		AutoMutex mutex(m_Mutex);
		m_Items.erase(id);
	}

	void Clear()
	{
		AutoMutex mutex(m_Mutex);
		m_Items.clear();
	}

	Item* GetItem(int id)
	{
		AutoMutex mutex(m_Mutex);
		if(m_Items.find(id) != m_Items.end())
			return &m_Items[id];
		return NULL;
	}

	static bool __stdcall CompleteItem(wchar_t* hash,wchar_t* name);
private:
	static DWORD WINAPI DumpItem(int id);
private:
	std::map<DWORD,Item> m_Items;
	HANDLE m_Mutex;
};

class ItemGeneral : public PacketHandler
{
public:
	ItemGeneral(Packet* pack)
	{
		m_ItemGeneral = *(P343_ItemGeneral*)pack;
	}
	virtual bool HandlePacket(MySQLDatabase* database);
	virtual DWORD GetHeader(){return m_ItemGeneral.Header;}
private:
	P343_ItemGeneral m_ItemGeneral;
};

class ItemProfession : public PacketHandler
{
public:
	ItemProfession(Packet* pack){m_ItemProfession = *(P336_ItemProfession*)pack;}
	virtual bool HandlePacket(MySQLDatabase* database);
	virtual DWORD GetHeader(){return m_ItemProfession.Header;}
private:
	P336_ItemProfession m_ItemProfession;
};