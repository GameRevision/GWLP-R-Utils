#pragma once
#include "stdafx.h"
#include "GWLP Dumper.h"
#define SPAWN_RADIUS 1000
#define DUMP_SPAWNS

struct P395_InstanceLoadDistrictInfo : public Packet
{
	DWORD LocalID;
	DWORD GameMapID;
	DWORD IsOutpost;
	DWORD DistrictNumber;
	DWORD DistrictRegion;
	DWORD Data1;
	DWORD ObserverMode;
};

struct P391_InstanceLoadSpawnPoint : public Packet
{
	DWORD GameMapFileID;
	float SpawnX;
	float SpawnY;
	DWORD SpawnPlane;
	DWORD Data5;
	DWORD Data6;
};

class InstanceLoadDistrictInfo : public PacketHandler
{
public:
	InstanceLoadDistrictInfo(Packet* pack)
	{
		m_InstanceLoadDistrictInfo = *(P395_InstanceLoadDistrictInfo*)pack;
	}
	virtual bool HandlePacket(MySQLDatabase* database);
	virtual DWORD GetHeader(){return m_InstanceLoadDistrictInfo.Header;}
private:
	P395_InstanceLoadDistrictInfo m_InstanceLoadDistrictInfo;
};

class InstanceLoadSpawnPoint : public PacketHandler
{
public:
	InstanceLoadSpawnPoint(Packet* pack)
	{
		m_InstanceLoadSpawnPoint = *(P391_InstanceLoadSpawnPoint*)pack;
	}
	virtual bool HandlePacket(MySQLDatabase* database);
	virtual DWORD GetHeader(){return m_InstanceLoadSpawnPoint.Header;}
private:
	P391_InstanceLoadSpawnPoint m_InstanceLoadSpawnPoint;
};