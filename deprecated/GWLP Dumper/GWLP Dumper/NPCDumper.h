#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "GWLP Dumper.h"

struct Agent
{

};

class AgentArray : public Singleton<AgentArray>
{
public:
	AgentArray()
		:Singleton()
	{}
	~AgentArray(){}

	Agent* operator[] (int id)
	{
		if(m_AgentArray.find(id) != m_AgentArray.end())
			return &m_AgentArray[id];
	}
protected:
	std::map<int,Agent> m_AgentArray;
};
