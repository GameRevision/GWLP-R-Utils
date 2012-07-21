#ifndef TemplateDecoder_h__
#define TemplateDecoder_h__
#include <windows.h>
#include <string>
#include <sstream>

#define TAB "        "

std::string DecodeTemplate(DWORD* Template,int TemplateSize);
std::string DecodeTemplateCSharp(DWORD* Template,int TemplateSize);
//std::string ConvertTemplate(DWORD* Template,int TemplateSize);

#endif // TemplateDecoder_h__
