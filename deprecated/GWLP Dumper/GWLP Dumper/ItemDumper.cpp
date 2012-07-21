#include "ItemDumper.h"

bool ItemGeneral::HandlePacket( MySQLDatabase* database )
{
	Item item;
	item.ItemID = m_ItemGeneral.ItemID;
	item.FileID = m_ItemGeneral.FileID;
	item.ItemType = m_ItemGeneral.ItemType;
	item.DyeColor = m_ItemGeneral.DyeColor;
	item.CanBeDyed = m_ItemGeneral.CanBeDyed;
	item.Flags = m_ItemGeneral.Flags;
	item.MerchantPrice = m_ItemGeneral.MerchantPrice;
	item.NumStats = m_ItemGeneral.NumStats;

 	for(int i=0;i<m_ItemGeneral.NumStats;i++)
 		item.Stats[i] = m_ItemGeneral.Stats[i];

	wcscpy(item.NameHash,m_ItemGeneral.NameHash);

	ItemArray::GetInstance()->InsertItem(m_ItemGeneral.LocalID,item);

	return true;
}


bool ItemProfession::HandlePacket( MySQLDatabase* database )
{
 	ItemArray::GetInstance()->GetItem(m_ItemProfession.ItemLocalID)->Profession = m_ItemProfession.Profession;
	return true;
}

DWORD WINAPI ItemArray::DumpItem( int id )
{
	MySQLDatabase* database = PacketDumper::GetInstance()->GetDatabase();
	ItemArray* pThis = ItemArray::GetInstance();
	if(pThis->m_Items.find(id) == pThis->m_Items.end())
		return false;

	Item item = pThis->m_Items[id];

	static char tmpname[250];
	wcstombs(tmpname,item.Name,250);

	char* name;
	for(int i=0;i<strlen(tmpname);i++)
	{
		if(tmpname[i] == '<')
		{
			for(;i<strlen(tmpname);i++)
				if(tmpname[i] == '>')
					break;
		}
		else
		{
			name = strtok(&tmpname[i],"<");
			break;
		}
	}

	MySQLQueryResult* result = database->Query("SELECT * FROM items_masterdata WHERE Name='%s'",database->EscapeString(name,strlen(name)).c_str());
	if(result)
	{
		result->~MySQLQueryResult();
		return true;
	}

	printf("Dump Item: %s\n",name);

	if(!database->Execute("INSERT INTO items_masterdata(ItemID,GameItemID,GameItemFileID,NameHash,Name,ItemType,Profession) VALUES (0,%u,%u,'%s','%s',%u,%i)",item.ItemID,item.FileID,database->EscapeString((char*)item.NameHash,wcslen(item.NameHash) * 2).c_str(),database->EscapeString(name,strlen(name)).c_str(),item.ItemType,item.Profession))
	{
		DisplayError("MYSQL Error: %s",database->LastError());
		return false;
	}
#ifdef DUMP_PREDEF
 	if(!database->Execute("INSERT INTO items_predefineddata(ItemID,DyeColor,Flags,Stats) VALUES (%i,%u,%u,'%s')",database->GetInsertId(),item.DyeColor,item.Flags,(item.NumStats)?database->EscapeString((char*)item.Stats,item.NumStats * sizeof(DWORD)).c_str():"NULL"))
 	{
 		DisplayError("MYSQL Error: %s",database->LastError());
 		return false;
 	}
#endif

	pThis->RemoveItem(id);

	return true;
}

bool ItemArray::CompleteItem( wchar_t* hash,wchar_t* name )
{
	if(!hash || wcslen(hash) < 4)
		return false;

	AutoMutex mutex(ItemArray::GetInstance()->m_Mutex);
	std::map<DWORD,Item>::iterator it;
	for(it = ItemArray::GetInstance()->m_Items.begin();it != ItemArray::GetInstance()->m_Items.end();++it)
	{
		bool match = true;
		for(int i=0;i<wcslen(it->second.NameHash) && match;i++)
			if(hash[2+i] != it->second.NameHash[i] && hash[2+i])
				match = false;

		if(match)
		{
			it->second.Name = name;
			Threadpool::GetInstance()->Run((LPTHREAD_START_ROUTINE)ItemArray::DumpItem,(LPVOID)it->first);
			return true;
		}
	}
	return false;
}
