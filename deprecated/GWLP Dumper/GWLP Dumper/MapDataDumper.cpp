#include "MapDataDumper.h"

DWORD LastMapID = 0;
BOOL IsOutpost = 0;

bool InstanceLoadDistrictInfo::HandlePacket( MySQLDatabase* database )
{
	LastMapID = m_InstanceLoadDistrictInfo.GameMapID;
	IsOutpost = (m_InstanceLoadDistrictInfo.IsOutpost)?1:0;
	return true;
}

float GetDistance(float x1,float y1,float x2,float y2)
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

bool InstanceLoadSpawnPoint::HandlePacket( MySQLDatabase* database )
{
	printf("Dump MapData for Map: %u\n",LastMapID);
	if(!database->Execute("UPDATE maps_masterdata SET GameMapFileID=%u WHERE GameMapID=%u",m_InstanceLoadSpawnPoint.GameMapFileID,LastMapID))
	{
		DisplayError("MYSQL Error: %s",database->LastError());
		return false;
	}
#ifdef DUMP_SPAWNS
	MySQLQueryResult* result = database->Query("SELECT * FROM maps_spawns WHERE MapID=%u AND IsOutpost=%i",LastMapID,IsOutpost);
	if(result)
	{
		do 
		{
			Field* field = result->Fetch();
			if(GetDistance(m_InstanceLoadSpawnPoint.SpawnX,m_InstanceLoadSpawnPoint.SpawnY,field[5].GetFloat(),field[6].GetFloat()) <= SPAWN_RADIUS * 2)
				return true;
		} while (result->NextRow());
		result->~MySQLQueryResult();
	}

	if(!database->Execute("INSERT INTO maps_spawns (MapID,IsOutpost,IsPvE,SpawnX,SpawnY,SpawnPlane,SpawnRadius) VALUES (%u,%i,1,%f,%f,%i,%f)",LastMapID,IsOutpost,m_InstanceLoadSpawnPoint.SpawnX,m_InstanceLoadSpawnPoint.SpawnY,m_InstanceLoadSpawnPoint.SpawnPlane,SPAWN_RADIUS))
	{
		DisplayError("MYSQL Error: %s",database->LastError());
		return false;
	}
#endif
	return true;
}

