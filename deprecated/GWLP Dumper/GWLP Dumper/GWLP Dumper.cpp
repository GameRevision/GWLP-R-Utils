#include "GWLP Dumper.h"
#include "tinyxml.h"

using namespace std;
queue<PacketHandler*> PacketDumper::m_PacketQueue;
HANDLE PacketDumper::m_PacketQueueMutex;
MySQLDatabase* PacketDumper::m_Database = NULL;
handler* PacketDumper::m_OrigLSHandler = NULL;
handler* PacketDumper::m_OrigGSHandler = NULL;

bool PacketDumper::InitDumper()
{
	TiXmlDocument config(CONFIG_NAME);
	if(!config.LoadFile())
	{
		DisplayError("Config Error: %s",config.ErrorDesc());
		return false;
	}
	TiXmlNode* root = config.RootElement();
	if(!root)
	{
		DisplayError("Config Error: Config file empty!");
		return false;
	}
	
	const char *host,*db,*user,*pw,*tmpport;
	int port;
	TiXmlHandle handle(root);
	TiXmlElement* element;
	/**
	* <DumperConfig>
	*	<Hostname>localhost</Hostname>
	*	<Database>gwlp</Database>
	*	<User>root</User>
	*	<Password></Password> //optional
	*	<Port></Port>		  //optional
	* </DumperConfig>
	**/
	element = handle.FirstChild("Hostname").ToElement();
	host = element->FirstChild()->Value();
	element = handle.FirstChild("Database").ToElement();
	db = element->FirstChild()->Value();
	element = handle.FirstChild("User").ToElement();
	user = element->FirstChild()->Value();
	element = handle.FirstChild("Password").ToElement();
	if(element)
	{
		if(element->FirstChild())
		{
			pw = element->FirstChild()->Value();
		}
		else
		{
			pw = "";
		}	
	}
	else
	{
		pw = "";
	}
	element = handle.FirstChild("Port").ToElement();
	if(element)
	{
		if(element->FirstChild())
		{
			tmpport = element->FirstChild()->Value();
			port = atoi(tmpport);
		}
		else
		{
			port = 0;
		}
	}
	else
	{
		port = 0;
	}

	try
	{
		m_Database = new MySQLDatabase(host,db,user,pw,port);;
	}
	catch (const char* error)
	{
		DisplayError("MYSQL Error: %s",error);
		return false;
	}

	m_PacketQueueMutex = CreateMutex(NULL,FALSE,NULL);
	if(!m_PacketQueueMutex)
	{
		DisplayError("Unable to create mutex!");
		return false;
	}
	CreateThread(0,0,ProcessPacketThread,0,0,0);
	
	return true;
}

DWORD WINAPI PacketDumper::ProcessPacketThread( LPVOID )
{
	while(true)
	{
		AutoMutex mutex(m_PacketQueueMutex);
		if(m_PacketQueue.empty())
			continue;
		PacketHandler* pack = m_PacketQueue.front();
		m_PacketQueue.pop();
		if(!pack->HandlePacket(m_Database))
		{
			DisplayError("Unable to handle packet 0x%04X",pack->GetHeader());
			break;
		}
	}
	return -1;
}
