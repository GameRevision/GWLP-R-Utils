#include "StdAfx.h"
#include "PacketTemplateParser.h"


PacketTemplateParser::~PacketTemplateParser(void)
{
}

PacketTemplateParser::PacketTemplateParser( std::tstring templateFile )
	:m_TemplateFile(templateFile)
{

}

bool PacketTemplateParser::ParseFile()
{
	HANDLE hFile = CreateFile(m_TemplateFile.c_str(),FILE_READ_ACCESS,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD FileSize = GetFileSize(hFile,NULL);
	if(!FileSize)
		return false;

	BYTE* Buffer = new BYTE[FileSize];

	DWORD dwRead;
	if(ReadFile(hFile,Buffer,FileSize,&dwRead,NULL) == FALSE)
		return false;

	BYTE* ReadPtr = Buffer;
	BYTE* BufferEnd = Buffer + FileSize;
	while(ReadPtr != BufferEnd)
	{
		PacketStructure PS;
		DWORD TemplateArgCount = GetData<DWORD>(&ReadPtr);
		for(int i=0;i<TemplateArgCount;i++)
			PS.Template.push_back(GetData<DWORD>(&ReadPtr));

		DWORD StringCount = GetData<DWORD>(&ReadPtr);
		for(int i=0;i<StringCount;i++)
		{
			std::tstring varname;
			TCHAR c;
			while((c = GetData<TCHAR>(&ReadPtr)) != '|')
				varname.push_back(c);
			PS.VarNames.push_back(varname);
		}
		m_PacketStructs.push_back(PS);
	}

	delete[] Buffer;
	return true;
}

template<typename T> T PacketTemplateParser::GetData( BYTE** r )
{
	T ret = *(reinterpret_cast<T*>(*r));
	*r += sizeof(T);
	return ret;
}
