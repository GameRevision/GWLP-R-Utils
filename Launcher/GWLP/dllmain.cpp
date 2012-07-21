/**
* A Launcher to connect to GWLP servers.
* Author: ACB
**/

#include "stdafx.h"

#pragma comment( lib, "ws2_32.lib" )

#include "tinyxml.h"
#include "Crypt.h"
#include "Keys.h"
#include "MC.h"

#define CONFIG_FILE "GWLP.xml" //Name of our Config File

std::map<int,struct Server> GWLPServer;
//std::map<ULONG,USHORT> GSPorts; \\obsolte
std::map<ULONG,int> OriginalServer;

typedef int (PASCAL FAR *ws_connect) (SOCKET,const struct sockaddr*,int);

DWORD JMPAddr = 0;
ws_connect pfnconnect = NULL ;
bool DisableEncryption = false;

struct Server
{
	ULONG IP;
	USHORT Port;
};

/**
* GS Ips to overwrite ports \\obsolete
**/
// void AddGS(const char* hostname,USHORT port)
// {
// 	hostent* host;
// 	in_addr addr;
// 	ULONG IP;
// 
// 
// 	host = gethostbyname(hostname);
// 	if(host == NULL)
// 	{
// 		DisplayError("Unable to retrieve Host: %s",hostname);
// 		ExitProcess(-1);
// 	}
// 
// 	int i=0;
// 	while(host->h_addr_list[i])//In case GS has multiple IPs
// 	{
// 		addr.s_addr = *(u_long *) host->h_addr_list[i];
// 		IP = addr.s_addr;
// 		GSPorts[IP] = port;
// 		i++;
// 	}
// }

/**
* Function to retrieve all information from our GWLP.xml
**/
void ParseIni()
{
	/*
	<EncryptConnection>1/0</EncryptConnection>
	<FileServer Id = X>
		<Hostname></Hostname> or <IP></IP>
		<Port></Port>
	</FileServer>
	<LoginServer>
		<Hostname></Hostname> or <IP></IP>
		<Port></Port>
	</LoginServer>
	*/
	TiXmlDocument Config(CONFIG_FILE);
	if(!Config.LoadFile())
	{
		MessageBoxA(0,"Cannot find Config File!\n Make sure to place it inside your GW folder!","Error",MB_OK);
		ExitProcess(-1);
	}
	TiXmlNode* Root = Config.RootElement();
	while(Root)
	{
		if(!strcmp(Root->Value(),"EncryptConnection"))
		{
			const char* b = Root->FirstChild()->Value();
			DisableEncryption = (b[0] == '0')?true:false;
		}
		else if(!strcmp(Root->Value(),"MultiClient"))
		{
			TiXmlHandle handle(Root);
			TiXmlElement *Enable,*GwDat;
			Enable = handle.FirstChild("Enable").ToElement();
			GwDat = handle.FirstChild("UnlockGwDat").ToElement();

			if(!Enable->FirstChild())
			{
				DisplayError("Config Error!\n Error in Child node <Enable> in Section <MultiClient>");
				continue;
			}
			if(!GwDat->FirstChild())
			{
				DisplayError("Config Error!\n Error in Child node <UnlockGwDat> in Section <MultiClient>");
				continue;
			}

			const char* b = Enable->FirstChild()->Value();
			if(b[0] == '1')
				MulticlientPatch();

			b = GwDat->FirstChild()->Value();
			if(b[0] == '1')
				GWDatFix();
		}
		else if(!strcmp(Root->Value(),"FileServer"))
		{
			TiXmlHandle handle(Root);
			TiXmlElement *Hostname,*IP,*Port;
			int id;
			USHORT port;
			ULONG ip;

			hostent* host;
			in_addr addr;

			handle.ToElement()->QueryIntAttribute("Id",&id);

			Hostname = handle.FirstChild("Hostname").ToElement();
			IP = handle.FirstChild("IP").ToElement();
			Port = handle.FirstChild("Port").ToElement();
			if(Hostname)
			{
				host = gethostbyname(Hostname->FirstChild()->Value());
				if(host == NULL)
				{
					DisplayError("Unable to retrieve Host: %s",Hostname->FirstChild()->Value());
					ExitProcess(-1);
				}
				addr.s_addr = *(u_long *) host->h_addr_list[0];
				ip = addr.s_addr;
			}
			else
			{
				char* temp = (char*)IP->FirstChild()->Value();
				ip = inet_addr(temp);
			}

			char* temp = (char*)Port->FirstChild()->Value();
			port = (USHORT)atoi(temp);

			if(id < 1)
				continue;

			Server s;
			s.IP = ip;
			s.Port = port;

			GWLPServer[id] = s;
		}
		else if(!strcmp(Root->Value(),"LoginServer"))
		{
			TiXmlHandle handle(Root);
			TiXmlElement *Hostname,*IP,*Port;
			USHORT port;
			ULONG ip;

			hostent* host;
			in_addr addr;

			Hostname = handle.FirstChild("Hostname").ToElement();
			IP = handle.FirstChild("IP").ToElement();
			Port = handle.FirstChild("Port").ToElement();

			if(Hostname)
			{
				host = gethostbyname(Hostname->FirstChild()->Value());
				if(host == NULL)
				{
					DisplayError("Unable to retrieve Host: %s",Hostname->FirstChild()->Value());
					ExitProcess(-1);
				}
				addr.s_addr = *(u_long *) host->h_addr_list[0];
				ip = addr.s_addr;
			}
			else
			{
				char* temp = (char*)IP->FirstChild()->Value();
				ip = inet_addr(temp);
			}

			char* temp = (char*)Port->FirstChild()->Value();
			port = (USHORT)atoi(temp);

			Server s;
			s.IP = ip;
			s.Port = port;

			GWLPServer[0] = s;
		}
// 		else if(!strcmp(Root->Value(),"GameServer")) \\ obsolete
// 		{
// 			TiXmlHandle handle(Root);
// 			TiXmlElement *Hostname,*IP,*Port;
// 
// 			Hostname = handle.FirstChild("Hostname").ToElement();
// 			IP = handle.FirstChild("IP").ToElement();
// 			Port = handle.FirstChild("Port").ToElement();
// 			if(Hostname)
// 			{
// 				char* temp = (char*)Port->FirstChild()->Value();
// 				USHORT port = (USHORT)atoi(temp);
// 				AddGS(Hostname->FirstChild()->Value(),port);
// 			}
// 			else
// 			{
// 				char* temp = (char*)Port->FirstChild()->Value();
// 				USHORT port = (USHORT)atoi(temp);
// 				GSPorts[inet_addr((char*)IP->FirstChild()->Value())] = port;
// 			}
// 		}
		Root = Root->NextSibling();
	}
}

/**
* Connect to GWLP servers
**/
int PASCAL FAR  MyConnect(SOCKET s,const struct sockaddr* name,int namelen)
{
	SOCKADDR_IN* addr = (SOCKADDR_IN*)name;

	if(OriginalServer.find(addr->sin_addr.s_addr) == OriginalServer.end())
	{
		//Should be a GS
// 		if(GSPorts.find(addr->sin_addr.s_addr) == GSPorts.end())
// 		{
// 			DisplayError("Unknown host %s",inet_ntoa(addr->sin_addr));//should never happen
// 			ExitProcess(-1);
// 		}
// 		SOCKADDR_IN gs;
// 		gs.sin_family = addr->sin_family;
// 		gs.sin_port = htons(GSPorts[addr->sin_addr.s_addr]);
// 		gs.sin_addr.s_addr = addr->sin_addr.s_addr;

		//return pfnconnect(s,(LPSOCKADDR)&gs,sizeof(struct sockaddr));
		return connect(s,name/*(LPSOCKADDR)&gs*/,sizeof(struct sockaddr));//port patch removed :S
	}

	int id = OriginalServer[addr->sin_addr.s_addr];

	if(id > GWLPServer.size())
	{
		DisplayError("You need to provide IPs for all 12 FileServers!");
		ExitProcess(-1);
	}

 	if(GWLPServer.find(id) == GWLPServer.end())
 	{
 		DisplayError("Unknown host %s\n Please provide IPs for all 12 FileServers!",inet_ntoa(addr->sin_addr));//should never happen
 		return WSAEADDRNOTAVAIL;
 	}

	SOCKADDR_IN gwlp;
	gwlp.sin_family = addr->sin_family;
	gwlp.sin_port = htons(GWLPServer[id].Port);
	gwlp.sin_addr.s_addr = GWLPServer[id].IP;
	
// 	if(!pfnconnect)
// 	{
// 		DWORD WSConnectAddr = *(DWORD*)JMPAddr;
// 		pfnconnect = (ws_connect)WSConnectAddr;
// 	}

	//return pfnconnect(s,(LPSOCKADDR)&gwlp,sizeof(struct sockaddr));
	return connect(s,(LPSOCKADDR)&gwlp,sizeof(struct sockaddr));
}

/**
* Init Winsock to resolve hostnames
**/
bool initWinsock()
{
	WORD sockVersion;
	WSADATA wsaData;
	sockVersion = MAKEWORD(2, 2);
	return (WSAStartup(sockVersion, &wsaData) == 0);
}

/**
* Fetch all official Ips to overwrite
**/
void PopulateAnetIps()
{
	static const char Auth[] = "auth1.arenanetworks.com";
	static const char File[] = "file%u.arenanetworks.com";

	hostent* host;
	in_addr addr;
	ULONG IP;


	host = gethostbyname(Auth);
	if(host == NULL)
	{
		DisplayError("Unable to retrieve Host: %s",Auth);
		ExitProcess(-1);
	}

	int i=0;
	while(host->h_addr_list[i])//Auth server has multiple IPs
	{
		addr.s_addr = *(u_long *) host->h_addr_list[i];
		IP = addr.s_addr;
		OriginalServer[IP] = 0;
		i++;
	}
	
	for(int i=1;i <= 12;i++)
	{
		char file[30];
		sprintf_s(file,30,File,i);

		host = gethostbyname(file);
		if(host == NULL)
		{
			DisplayError("Unable to retrieve Host: %s",file);
			ExitProcess(-1);
		}
		int j = 0;
		while(host->h_addr_list[j])//In case File Server has multiple IPs
		{
			addr.s_addr = *(u_long *) host->h_addr_list[j];
			IP = addr.s_addr;
			OriginalServer[IP] = i;
			j++;
		}
	}
}

void InjectKeys(BYTE* loc)
{
	DWORD oldProtect;
	VirtualProtect(loc,sizeof(G) + sizeof(P) + sizeof(PublicServerKey),PAGE_READWRITE,&oldProtect);

	memcpy(loc,G,sizeof(G));
	loc += sizeof(G);;
	memcpy(loc,P,sizeof(P));
	loc += sizeof(P);
	memcpy(loc,PublicServerKey,sizeof(PublicServerKey));
}

void WriteJMP(BYTE* location, BYTE* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	location[0] = 0xE9;
	*((DWORD*)(location + 1)) = (DWORD)(newFunction - location) - 5;
	VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}

void InstallHooks()
{
	BYTE* start = (BYTE*)0x00401000;
	BYTE* end = (BYTE*)0x00800000;

	BYTE ConnectCode[] = {0x8D, 0x55, 0xE8, 0x6A, 0x10, 0x52, 0x56};
	BYTE KeyCode[] = {0xBA, 0x02, 0x00, 0x00, 0x00, 0xB9, 0x30, 0x00, 0x00, 0x00, 0x68, 0x1B, 0x05, 0x00, 0x00};
	BYTE PassInjectCode[] = {0x89, 0x55, 0x10, 0x8B, 0x55, 0x08, 0x89, 0x55, 0x08, 0x46, 0x81, 0xE6, 0xFF, 0x00, 0x00, 0x00};
	BYTE PassGrabCode[] = {0x8B, 0xEC, 0x81, 0xEC, 0xA0, 0x00, 0x00, 0x00, 0x53, 0x56, 0x8B, 0xD9, 0x57, 0x6A, 0x10};
	BYTE PortFixCode[] = {0x8D, 0x53, 0x3C, 0x89, 0x7E, 0x08};

	while(start != end)
	{
		if(!memcmp(start, ConnectCode, sizeof(ConnectCode)) && !JMPAddr)
		{
			start+= 8;
			DWORD ConnectJMP = (DWORD)start + (*(DWORD*)start) + 4 ;
			JMPAddr = *(DWORD*)(ConnectJMP + 2);
			WriteJMP((BYTE*)ConnectJMP,(BYTE*)MyConnect);
			start-= 7;
		}
		if(!memcmp(start,KeyCode,sizeof(KeyCode)))
		{
			start -= 4;
			BYTE* KeyLoc = (BYTE*)(start + (*(DWORD*)start) + 4);
			KeyLoc = KeyLoc + 7;
			KeyLoc = (BYTE*)(*(DWORD*)KeyLoc);
			InjectKeys(KeyLoc + 4);
			start += 5;
		}
		if(!memcmp(start,PassInjectCode,sizeof(PassInjectCode)))
		{
			InjectPWHook(start,DisableEncryption);
		}
		if(!memcmp(start,PassGrabCode,sizeof(PassGrabCode)))
		{
			GrabPWHook(start);
		}
		if(!memcmp(start,PortFixCode,sizeof(PortFixCode)))
		{
			start += 7;
			BYTE* PortFixLoc = start + (*(DWORD*)start) + 4;
			PortFixLoc += 10;
			DWORD dwBack;
			VirtualProtect(PortFixLoc,1,PAGE_READWRITE,&dwBack);
			*PortFixLoc = (BYTE)0x8B;
			VirtualProtect(PortFixLoc,1,dwBack,&dwBack);
			start -= 6;
		}
		start++;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if(!initWinsock())
		{	
			DisplayError("Unable to init Winsock!");
			return FALSE;
		}
		PopulateAnetIps();
		ParseIni();
		InstallHooks();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

