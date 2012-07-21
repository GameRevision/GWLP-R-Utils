#ifndef WindowPPException_h__
#define WindowPPException_h__
/*
 © 2011 ACB
*/
#include "stdafx.h"

class WindowPPException
{
public:
	WindowPPException(TCHAR* fmt,...)
	{
		va_list args;
		va_start(args,fmt);
		_vstprintf(m_What,fmt,args);
		va_end(args);
	}

	~WindowPPException(void)
	{}

	virtual TCHAR* what() const throw()
	{
		return (TCHAR*)m_What;
	}
private:
	TCHAR m_What[2048];
};
#endif // WindowPPException_h__

