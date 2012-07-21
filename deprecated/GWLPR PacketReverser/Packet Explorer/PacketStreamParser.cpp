#include "StdAfx.h"
#include "PacketStreamParser.h"


PacketStreamParser::PacketStreamParser(std::tstring filename)
	:m_PacketQueueFile(filename),m_Template(NULL),m_CurPacket(0)
{
}


PacketStreamParser::~PacketStreamParser(void)
{
	m_Template->~PacketTemplateParser();
}

bool PacketStreamParser::Parse()
{
	std::tstring templateFileName = m_PacketQueueFile.substr(0,m_PacketQueueFile.length() - 4/*std::tstring("dump").length()*/);
	templateFileName.append(_T("template"));

	m_Template = new PacketTemplateParser(templateFileName);
	m_Template->ParseFile();

	HANDLE hFile = CreateFile(m_PacketQueueFile.c_str(),FILE_READ_ACCESS,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD FileSize = GetFileSize(hFile,NULL);
	if(!FileSize)
		return false;

	BYTE* Buffer = new BYTE[FileSize];

	DWORD dwRead;
	if(ReadFile(hFile,Buffer,FileSize,&dwRead,NULL) == FALSE)
		return false;

	BYTE* BufferReadPtr = Buffer;
	BYTE* BufferEndPtr = Buffer + FileSize;
	while (BufferReadPtr != BufferEndPtr)
	{
		Packet pack;
		short Header;
		if(!GetDataFromPacketQueue<short>(&BufferReadPtr,BufferEndPtr,&Header))
			return true;

		pack.Header = Header;
		PacketStructure structure;
		try
		{
			structure = m_Template->GetPacketStructure(Header);
		}
		catch (...)
		{
			return true;
		}
		

		int VarName = 1;
		int TemplateArg = 1;
		short VarSize,MaxVarSize,LoopCounter = 0;
		while(TemplateArg < structure.Template.size())
		{
			int Type = structure.Template[TemplateArg] & 0x0F;
			Data pd;
			std::tstringstream converter;
			std::tstringstream array_name;
			converter.fill('0');
			switch(Type)
			{
			case 3:
				if(!GetDataFromPacketQueue<float>(&BufferReadPtr,BufferEndPtr,&pd.f))
					return true;
				converter << pd.f;
				pack.PacketData.push_back(DataTuple(_T("Single"),structure.VarNames[VarName],converter.str()));VarName++;
			case 2:
				if(!GetDataFromPacketQueue<float>(&BufferReadPtr,BufferEndPtr,&pd.f))
					return true;
				converter << pd.f;
				pack.PacketData.push_back(DataTuple(_T("Single"),structure.VarNames[VarName],converter.str()));VarName++;
			case 1:
				if(!GetDataFromPacketQueue<float>(&BufferReadPtr,BufferEndPtr,&pd.f))
					return true;
				converter << pd.f;
				pack.PacketData.push_back(DataTuple(_T("Single"),structure.VarNames[VarName],converter.str()));VarName++;
				break;

			case 0:
				if(!GetDataFromPacketQueue<DWORD>(&BufferReadPtr,BufferEndPtr,&pd.id))
					return true;
				converter << pd.id;
				pack.PacketData.push_back(DataTuple(_T("UInt32"),structure.VarNames[VarName],converter.str()));VarName++;
				break;

			case 4:
			case 8:
				if(LoopCounter)
				{
					switch(structure.Template[TemplateArg] >> 8)
					{
					case 4:
						for(;LoopCounter;LoopCounter--)
						{
							if(!GetDataFromPacketQueue<DWORD>(&BufferReadPtr,BufferEndPtr,&pd.dw))
								return true;
							converter << "0x" << std::hex << std::setw(8) << std::right << pd.dw << " ";
						}
						array_name << structure.VarNames[TemplateArg] << "[" << std::dec << VarSize << "/" << std::dec << MaxVarSize << "]";
						pack.PacketData.push_back(DataTuple(_T("UInt32"),structure.VarNames[VarName],converter.str()));VarName++;
						break;
					case 2:
						for(;LoopCounter;LoopCounter--)
						{
							if(!GetDataFromPacketQueue<WORD>(&BufferReadPtr,BufferEndPtr,&pd.w))
								return true;
							converter << "0x" << std::hex << std::setw(4) << std::right << pd.w << " ";
						}
						array_name << structure.VarNames[TemplateArg] << "[" << std::dec << VarSize << "/" << std::dec << MaxVarSize << "]";
						pack.PacketData.push_back(DataTuple(_T("UInt16"),structure.VarNames[VarName],converter.str()));VarName++;
						break;;
					case 1:
						for(;LoopCounter;LoopCounter--)
						{
							if(!GetDataFromPacketQueue<BYTE>(&BufferReadPtr,BufferEndPtr,&pd.dw))
								return true;
							converter << "0x" << std::hex << std::setw(2) << std::right << pd.b << " ";
						}
						array_name << structure.VarNames[TemplateArg] << "[" << std::dec << VarSize << "/" << std::dec << MaxVarSize << "]";
						pack.PacketData.push_back(DataTuple(_T("byte"),structure.VarNames[VarName],converter.str()));VarName++;
					}
				}
				else
				{
					switch(structure.Template[TemplateArg] >> 8)
					{
					case 4:
						if(!GetDataFromPacketQueue<DWORD>(&BufferReadPtr,BufferEndPtr,&pd.dw))
							return true;
						converter << pd.dw;
						pack.PacketData.push_back(DataTuple(_T("UInt32"),structure.VarNames[VarName],converter.str()));VarName;
						break;
					case 2:
						if(!GetDataFromPacketQueue<WORD>(&BufferReadPtr,BufferEndPtr,&pd.w))
							return true;
						converter << pd.w;
						pack.PacketData.push_back(DataTuple(_T("UInt16"),structure.VarNames[VarName],converter.str()));VarName;
						break;
					case 1:
						if(!GetDataFromPacketQueue<BYTE>(&BufferReadPtr,BufferEndPtr,&pd.b))
							return true;
						converter << pd.b;
						pack.PacketData.push_back(DataTuple(_T("byte"),structure.VarNames[VarName],converter.str()));VarName;
						break;
					}
				}
				break;

			case 5:
			case 9:
				for(int i=0;i<structure.Template[TemplateArg] >> 8;i++)
				{
					if(!GetDataFromPacketQueue<BYTE>(&BufferReadPtr,BufferEndPtr,&pd.b))
						return true;
					converter << "0x" << std::hex << std::setw(2) << std::right << pd.b << " ";
				}
				array_name << structure.VarNames[VarName] << "[" << std::dec << (structure.Template[TemplateArg] >> 8) << "]";VarName++;
				pack.PacketData.push_back(DataTuple(_T("byte"),array_name.str(),converter.str()));
				break;

			case 11:
				if(!GetDataFromPacketQueue<short>(&BufferReadPtr,BufferEndPtr,&VarSize))
					return true;

				converter << std::dec << VarSize;
				pack.PacketData.push_back(DataTuple(_T("UInt16"),structure.VarNames[VarName],converter.str()));VarName++;

				MaxVarSize = structure.Template[TemplateArg] >> 8;
				VarSize <<= ((structure.Template[TemplateArg] >> 4) & 0x0F);
				break;

			case 7:
				if(!GetDataFromPacketQueue<short>(&BufferReadPtr,BufferEndPtr,&VarSize))
					return true;

				MaxVarSize = structure.Template[TemplateArg] >> 8;
				array_name << structure.VarNames[VarName] << "[" << std::dec << VarSize << "/" << std::dec << MaxVarSize << "]";	VarName++;		

				for(;VarSize;VarSize--)
				{
					if(!GetDataFromPacketQueue<wchar_t>(&BufferReadPtr,BufferEndPtr,&pd.wc))
						return true;
					converter << pd.wc;
				}
				pack.PacketData.push_back(DataTuple(_T("string"),array_name.str(),converter.str()));
				break;

			case 6:
			case 10:
				for(int i=0;i<VarSize;i++)
				{
					if(!GetDataFromPacketQueue<BYTE>(&BufferReadPtr,BufferEndPtr,&pd.b))
						return true;
					converter << "0x" << std::hex << std::setw(2) << std::right << pd.b << " ";
				}

				switch((structure.Template[TemplateArg - 1] >> 4) & 0x0F)
				{
				case 2:
					array_name << structure.VarNames[VarName] << "[" << std::dec << VarSize/4 << "/" << std::dec << MaxVarSize << "]";VarName++;
					pack.PacketData.push_back(DataTuple(_T("UInt32"),array_name.str(),converter.str()));
					break;
				case 1:
					array_name << structure.VarNames[VarName] << "[" << std::dec << VarSize/2 << "/" << std::dec << MaxVarSize << "]";VarName++;
					pack.PacketData.push_back(DataTuple(_T("UInt16"),array_name.str(),converter.str()));
					break;
				case 0:
					array_name << structure.VarNames[VarName] << "[" << std::dec << VarSize << "/" << std::dec << MaxVarSize << "]";VarName++;
					pack.PacketData.push_back(DataTuple(_T("byte"),array_name.str(),converter.str()));
					break;
				}
				break;

			case 12:
				if(!GetDataFromPacketQueue<BYTE>(&BufferReadPtr,BufferEndPtr,&VarSize))
					return true;
				MaxVarSize = structure.Template[TemplateArg] >> 8;
				LoopCounter = VarSize;

				converter << std::dec << VarSize;
				pack.PacketData.push_back(DataTuple(_T("UInt16"),structure.VarNames[VarName],converter.str()));VarName++;
				break;
			}
			TemplateArg++;
		}
		m_Packets.push_back(pack);
	}

	return true;
}

template<typename packet_type,typename result_type> bool PacketStreamParser::GetDataFromPacketQueue( BYTE** readptr,BYTE* buffend,result_type* res )
{
	static_assert( sizeof(packet_type) <= 4 && sizeof(packet_type) > 0 && sizeof(result_type) <= 4 && sizeof(result_type) > 0 );

	if(*readptr + sizeof(packet_type) >= buffend)
		return false;

	*res = (result_type)(*reinterpret_cast<packet_type*>(*readptr));
	*readptr += sizeof(packet_type);

	return true;
}