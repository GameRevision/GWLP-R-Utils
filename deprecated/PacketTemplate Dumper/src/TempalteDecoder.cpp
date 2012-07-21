#include "TemplateDecoder.h"
#include <string>
#include <sstream>

using namespace std;

string DecodeTemplate(DWORD* Template,int TemplateSize)
{
	if(!Template)
		return string("");

	int Type;
	bool Loop = false;

	int BitsToRead;
	DWORD Mask;

	int VariableSize,MaxVariableSize;
	int StaticSize;

	stringstream packet_structure;
	packet_structure << "\tstruct Packet_0x" << hex << Template[0] << "\n" 
		<< "\t{\n"
		<< "\t\tconst short Header = 0x" << hex << Template[0] << ";" << "\t\t//Size in Packet: 4 bytes"<< "\n";
	for(int i=1;i<TemplateSize;i++)
	{
		Type = Template[i] & 0x0F;

		switch(Type)
		{
		case 3:
			packet_structure << "\t\t" << "float" << "\t\t//Size in Packet: 4 bytes" << "\n";
		case 2:
			packet_structure << "\t\t" << "float" << "\t\t//Size in Packet: 4 bytes" << "\n";
		case 1:
			packet_structure << "\t\t" << "float" << "\t\t//Size in Packet: 4 bytes" << "\n";
			break;

		case 0:
			packet_structure << "\t\t" << "ID" << "\t\t//Size in Packet: 4 bytes" << "\n";
			break;

		case 4:
		case 8:
			BitsToRead = (Template[i] >> 8) << 3;
			Mask = 0xFFFFFFFF >> (32 - BitsToRead);

			switch(BitsToRead)
			{
			case 8:
				if(Loop)
				{
					packet_structure << "\t\t" << "BYTE[ArraySize]" << "\t//Mask: 0x000000" << hex << Mask << "\t\t//Size in Packet: 4 bytes / element" << "\n";
					Loop = false;
				}
				else
					packet_structure << "\t\t" << "BYTE" << "\t//Mask: 0x000000" << hex << Mask << "\t\t//Size in Packet: 4 bytes" << "\n";
				break;
			case 16:
				if(Loop)
				{
					packet_structure << "\t\t" << "WORD[ArraySize]" << "\t//Mask: 0x0000" << hex << Mask << "\t\t//Size in Packet: 4 bytes / element" << "\n";
					Loop = false;
				}
				else
					packet_structure << "\t\t" << "WORD" << "\t//Mask: 0x0000" << hex << Mask << "\t\t//Size in Packet: 4 bytes" << "\n";
				break;
			case 32:
				if(Loop)
				{
					packet_structure << "\t\t" << "DWORD[ArraySize]" << "\t//Mask: 0x" << hex << Mask << "\t\t//Size in Packet: 4 bytes / element" << "\n";
					Loop = false;
				}
				else
					packet_structure << "\t\t" << "DWORD" << "\t//Mask: 0x" << hex << Mask << "\t\t//Size in Packet: 4 bytes" << "\n";
				break;
			default:
				packet_structure << "\t\t" << dec << BitsToRead << " bits" << "\n";
				break;
			}

			break;

		case 5:
		case 9:
			StaticSize = Template[i] >> 8;
			packet_structure << "\t\t" << "BYTE[" << dec << StaticSize << "]" << "\t\t//Size in Packet: " << StaticSize << " bytes" << "\n";
			break;

		case 11:
			MaxVariableSize = Template[i] >> 8;
			packet_structure << "\t\t" << "short length;" << "\t//MaxLength: " << dec << MaxVariableSize << "\t\t//Size in Packet: 4 bytes" << "\n";

			if( ((Template[i] >> 4) & 0x0F) == 0)
				packet_structure << "\t\t" << "BYTE[" << "length" << "]" << "\t//MaxLength: " << dec << MaxVariableSize;
			else if( ((Template[i] >> 4) & 0x0F) == 1)
				packet_structure << "\t\t" << "WORD[" << "length" << "]" << "\t//MaxLength: " << dec << MaxVariableSize;
			else if( ((Template[i] >> 4) & 0x0F) == 2)
				packet_structure << "\t\t" << "DWORD[" << "length" << "]" << "\t//MaxLength: " << dec << MaxVariableSize;

			MaxVariableSize <<= ((Template[i] >> 4) & 0x0F);
			VariableSize = 1;
			VariableSize <<= ((Template[i] >> 4) & 0x0F);
			break;

		case 7:
			MaxVariableSize = Template[i] >> 8;
			packet_structure << "\t\t" << "//short string_length;" << "\t//MaxLength: " << dec << MaxVariableSize << "\t\t//Size in Packet: 4 bytes"<< "\n";
			packet_structure << "\t\t" << "wchar_t[" << dec << MaxVariableSize << "]" << "\t\t//Size in Packet: 2 * string_length bytes"<< "\n";
			break;

		case 6:
		case 10:
			packet_structure << "\t\t//Size in Packet: " << dec << VariableSize << " * length bytes"  << " MaxSize in Packet: " << dec << MaxVariableSize << "bytes" << "\n";
			break;

		case 12:
			MaxVariableSize = Template[i] >> 8;
			packet_structure << "\t\t" << "BYTE ArraySize;" <<"\t//MaxSize: " << dec << MaxVariableSize << "\t\t//Size in Packet: 4 bytes" << "\n";
			Loop = true;
			break;
		}
	}
	packet_structure << "\t}" << "\n";
	return packet_structure.str();
}

string DecodeTemplateCSharp(DWORD* Template,int TemplateSize)
{

	if(!Template)
		return string("");

	int Data = 1;
	int ID = 1;
	int ArraySize = 1;

	int Type;
	bool Loop = false;

	int BitsToRead;
	DWORD Mask;

	int VariableSize,MaxVariableSize;
	int StaticSize;

	stringstream packet_structure;
	packet_structure << TAB TAB << "public class PacketSt" << dec << Template[0] << " : IPacketTemplate" << "\n" 
		<< TAB TAB << "{\n"
		<< TAB TAB TAB << "public UInt16 Header { get { return " << dec << Template[0] << "; } }" << "\n";

	for(int i=1;i<TemplateSize;i++)
	{
		Type = Template[i] & 0x0F;

		switch(Type)
		{
		case 3:
			packet_structure << TAB TAB TAB << "public Single" << " Data" << dec << Data << ";\n";Data++;
		case 2:
			packet_structure << TAB TAB TAB << "public Single" << " Data" << dec << Data << ";\n";Data++;
		case 1:
			packet_structure << TAB TAB TAB << "public Single" << " Data" << dec << Data << ";\n";Data++;
			break;

		case 0:
			packet_structure << TAB TAB TAB << "public UInt32" << " ID" << dec << ID << ";\n";ID++;
			break;

		case 4:
		case 8:
			BitsToRead = (Template[i] >> 8) << 3;
			Mask = 0xFFFFFFFF >> (32 - BitsToRead);

			switch(BitsToRead)
			{
			case 8:
				if(Loop)
				{
					packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << MaxVariableSize << ")]\n"*/
						<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize << ")]\n"
						<< TAB TAB TAB << "public byte[]" << " Data" << dec << Data << ";\n";Data++;
					Loop = false;
				}
				else
					packet_structure << TAB TAB TAB << "public byte" << " Data" << dec << Data << ";\n";Data++;
				break;
			case 16:
				if(Loop)
				{
					packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << MaxVariableSize << ")]\n"*/
						<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize * 2 << ")]\n"
						<< TAB TAB << "public UInt16[]" << " Data" << dec << Data << ";\n";Data++;
					Loop = false;
				}
				else
					packet_structure << TAB TAB TAB << "public UInt16" << " Data" << dec << Data << ";\n";Data++;
				break;
			case 32:
				if(Loop)
				{
					packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << MaxVariableSize << ")]\n"*/ 
						<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize * 4 << ")]\n"
						<< TAB TAB TAB << "public UInt32[]" << " Data" << dec << Data << ";\n";Data++;
					Loop = false;
				}
				else
					packet_structure << TAB TAB TAB << "public UInt32" << " Data" << dec << Data << ";\n";Data++;
				break;
				break;
			}

			break;

		case 5:
		case 9:
			StaticSize = Template[i] >> 8;
			packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << StaticSize <<")]\n"*/ 
				<< "[PacketFieldType(ConstSize = true, MaxSize = " << dec << StaticSize << ")]\n"
				<< TAB TAB TAB << "public byte[]" << " Data" << dec << Data << ";\n";Data++;
			break;

		case 11:
			MaxVariableSize = Template[i] >> 8;
			packet_structure << TAB TAB TAB << "public UInt16" << " ArraySize" << dec << ArraySize << ";\n";ArraySize++;

			if( ((Template[i] >> 4) & 0x0F) == 0)
			{
				packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << MaxVariableSize << ")]\n"*/ 
					<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize << ")]\n"
					<< TAB TAB TAB << "public byte[]" << " Data" << dec << Data << ";\n";Data++;
			}
			else if( ((Template[i] >> 4) & 0x0F) == 1)
			{
				packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << MaxVariableSize << ")]\n"*/ 
					<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize * 2 << ")]\n"
					<< TAB TAB TAB << "public UInt16[]" << " Data" << dec << Data << ";\n";Data++;
			}
			else if( ((Template[i] >> 4) & 0x0F) == 2)
			{
				packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValArray, SizeConst=" << dec << MaxVariableSize << ")]\n"*/ 
					<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize * 4 << ")]\n"
					<< TAB TAB TAB << "public UInt32[]" << " Data" << dec << Data << ";\n";Data++;
			}
			break;

		case 7:
			MaxVariableSize = Template[i] >> 8;
			packet_structure << TAB TAB TAB /*<< "[MarshalAs(UnmanagedType.ByValStr, SizeConst=" << dec << MaxVariableSize / *<< "*2"* / <<")]\n"*/ 
				<< "[PacketFieldType(ConstSize = false, MaxSize = " << dec << MaxVariableSize << ")]\n"
				<< TAB TAB TAB << "public string" << " Data" << dec << Data << ";\n";Data++;
			break;

		case 6:
		case 10:
			break;

		case 12:
			MaxVariableSize = Template[i] >> 8;
			packet_structure << TAB TAB TAB << "public byte" << " ArraySize" << dec << ArraySize << ";\n";ArraySize++;
			Loop = true;
			break;
		}
	}
	packet_structure << TAB TAB << "}" << "\n\n";
	return packet_structure.str();
}

// string ConvertTemplate(DWORD* Template,int TemplateSize)
// {
// 	enum GWLPRTemplateTypes : int
// 	{
// 		PACKET_STATIC_SIZED_ARRAY,
// 		PACKET_VARIABLE_SIZED_ARRAY,
// 		PACKET_VARIABLE_SIZED_ARRAY_ELEMENT_SIZE,
// 		PACKET_TYPE_NONE,
// 	};
// 	if(!Template || TemplateSize <= 1)
// 		return string(TAB "new int[0,0],\n");
// 
// 	int GWLPRTemplateType = PACKET_TYPE_NONE;
// 	int ArraySize = 0;
// 
// 	stringstream result;
// 	result << TAB << "new int[,] {";
// 	for(int i=1;i<TemplateSize;i++)
// 	{
// 		int Type = Template[i] & 0x0F;
// 		switch(Type)
// 		{
// 			//Static sized
// 		case 3:
// 		case 2:
// 		case 1:
// 		case 0:
// 			switch(GWLPRTemplateType)
// 			{
// 			case PACKET_TYPE_NONE:
// 				GWLPRTemplateType = PACKET_STATIC_SIZED_ARRAY;
// 				ArraySize = 0;
// 			case PACKET_STATIC_SIZED_ARRAY:
// 				ArraySize += 4;
// 				break;
// 			}
// 			break;
// 
// 		case 4:
// 		case 5:
// 		case 8:
// 		case 9:
// 			switch(GWLPRTemplateType)
// 			{
// 			case PACKET_TYPE_NONE:
// 				GWLPRTemplateType = PACKET_STATIC_SIZED_ARRAY;
// 				ArraySize = 0;
// 			case PACKET_STATIC_SIZED_ARRAY:
// 				ArraySize += Template[i] >> 8;
// 				break;
// 			}
// 			break;
// 
// 			//Variable sized
// 		case 11:
// 			if(GWLPRTemplateType == PACKET_STATIC_SIZED_ARRAY)
// 				result << "{" << dec << PACKET_STATIC_SIZED_ARRAY << ", " << dec << ArraySize << "}" << ", ";
// 
// 			result << "{" << dec << PACKET_VARIABLE_SIZED_ARRAY << ", " << dec << (Template[i] >> 8) << "}" << ", ";
// 			result << "{" << dec << PACKET_VARIABLE_SIZED_ARRAY_ELEMENT_SIZE << ", " << dec << (1 << ((Template[i] >> 4) & 0x0F)) << "}";
// 
// 			i++;//Skip 6/10
// 			if(i+1 < TemplateSize)
// 				result << ",";
// 
// 			GWLPRTemplateType = PACKET_TYPE_NONE;
// 			break;
// 
// 		case 7:
// 			if(GWLPRTemplateType == PACKET_STATIC_SIZED_ARRAY)
// 				result << "{" << dec << PACKET_STATIC_SIZED_ARRAY << ", " << dec << ArraySize << "}" << ", ";
// 
// 			result << "{" << dec << PACKET_VARIABLE_SIZED_ARRAY << ", " << dec << (Template[i] >> 8) << "}" << ", ";
// 			result << "{" << dec << PACKET_VARIABLE_SIZED_ARRAY_ELEMENT_SIZE << ", " << dec << 2 << "}";
// 
// 			if(i+1 < TemplateSize)
// 				result << ",";
// 
// 			GWLPRTemplateType = PACKET_TYPE_NONE;
// 			break;
// 
// 		case 12:
// 			if(GWLPRTemplateType == PACKET_STATIC_SIZED_ARRAY)
// 				result << "{" << dec << PACKET_STATIC_SIZED_ARRAY << ", " << dec << ArraySize << "}" << ", ";
// 
// 			result << "{" << dec << PACKET_VARIABLE_SIZED_ARRAY << ", " << dec << (Template[i] >> 8) << "}" << ", ";
// 			result << "{" << dec << PACKET_VARIABLE_SIZED_ARRAY_ELEMENT_SIZE << ", " << dec << (1 << ((Template[i+1] >> 4) & 0x0F)) << "}";
// 
// 			i++;//skip 4/8
// 			if(i+1 < TemplateSize)
// 				result << ",";
// 
// 			GWLPRTemplateType = PACKET_TYPE_NONE;
// 			break;
// 		}
// 	}
// 
// 	if(GWLPRTemplateType == PACKET_STATIC_SIZED_ARRAY)
// 		result << "{" << dec << PACKET_STATIC_SIZED_ARRAY << ", " << dec << ArraySize << "}";
// 
// 	result << "}" << "\n";
// 	return result.str();
// }