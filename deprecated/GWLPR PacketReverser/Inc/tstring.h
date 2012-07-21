#ifndef tstring_h__
#define tstring_h__
/*
 © 2011 ACB
*/
#include <tchar.h>
#include <string>
#include <sstream>
namespace std
{
	typedef basic_string<TCHAR, char_traits<TCHAR>,
		allocator<TCHAR> > tstring;
	typedef basic_stringstream<TCHAR> tstringstream;
}
#endif // tstring_h__

