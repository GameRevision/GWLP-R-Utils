#pragma once
#include "stdafx.h"

struct PacketStructure
{
	std::vector<DWORD> Template;
	std::vector<std::tstring> VarNames;
};

/*
File Format:
template arg count
binary template data
string count
var names (delemiter |)
*/

class PacketTemplateParser
{
public:
	PacketTemplateParser(std::tstring templateFile);
	~PacketTemplateParser(void);

	bool ParseFile();

	PacketStructure GetPacketStructure(int Head){if(Head < m_PacketStructs.size())return m_PacketStructs[Head];throw 1;}
private:
	template<typename T> T GetData(BYTE** r);
private:
	std::vector<PacketStructure> m_PacketStructs;
	std::tstring m_TemplateFile;
};


