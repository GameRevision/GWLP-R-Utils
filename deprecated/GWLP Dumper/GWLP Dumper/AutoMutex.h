#pragma once
#include <windows.h>
class AutoMutex
{
public:
	AutoMutex(HANDLE mutex)
	{
		m_Mutex = mutex;
		if(WaitForSingleObject(m_Mutex,INFINITE) != WAIT_OBJECT_0)
		{
			MessageBox(NULL,"Unable to retrieve mutex!","Auto Mutex",MB_ICONERROR|MB_OK);
		}
	}
	~AutoMutex()
	{
		ReleaseMutex(m_Mutex);
	}
private:
	HANDLE m_Mutex;
};