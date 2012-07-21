#include "Threadpool.h"

volatile long Threadpool::m_lInstance = 0;

DWORD WINAPI Threadpool::ThreadProc( LPVOID pParam )
{
	Threadpool* pool;
	HANDLE hThread = GetCurrentThread();
	LPTHREAD_START_ROUTINE proc;
	LPVOID data;
	DWORD dwThreadId = GetCurrentThreadId();
	HANDLE hWaits[2];
	IRunObject* runObject;
	bool bAutoDelete;

	pool = static_cast<Threadpool*>(pParam);
	hWaits[0] = pool->GetWaitHandle(dwThreadId);
	hWaits[1] = pool->GetShutdownHandle();


	for(;;)
	{
		if(WaitForMultipleObjects(2,hWaits,FALSE,INFINITE)== WAIT_OBJECT_0)
		{
			if(pool->CheckThreadStop())
			{
				return 0;
			}

			if(pool->GetThreadProc(dwThreadId,proc,&data,&runObject))
			{
				pool->BusyNotify(dwThreadId);

				if(proc == NULL)
				{
					bAutoDelete = runObject->AutoDelete();
					runObject->Run();

					if(bAutoDelete)
					{
						delete runObject;
					}
				}
			}
			else
			{
				proc(data);

				UserPoolData* pPoolData = static_cast<UserPoolData*>(data);
				delete pPoolData;
			}

			pool->FinishNotify(dwThreadId);
		}
		else 
			return 0;
	}

	return 0;
}

Threadpool::Threadpool( int nPoolSize /*= 10*/,int nWaitTimeForThreadsToComplete /*= 5000*/,bool bCreateNow /*= true*/ )
{
	m_State = POOL_DESTROYED;
	m_PoolSize = nPoolSize;
	m_WaitForThreadsToDieMS = nWaitTimeForThreadsToComplete;

	InitializeCriticalSection(&m_csThreads);
	InitializeCriticalSection(&m_csFuncList);

	if(bCreateNow)
	{
		if(!Create())
			throw 1;
		m_State = POOL_READY;
	}
}

bool Threadpool::Create()
{
	if(m_State != POOL_DESTROYED)
		return false;

	InterlockedIncrement(&Threadpool::m_lInstance);
	sprintf(m_PoolName,"Pool%d",m_lInstance);

	HANDLE hThread;
	DWORD dwThreadId;
	_ThreadData ThreadData;
	char EvtName[30];

	m_NotifyShutdown = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_NotifyShutdown = INVALID_HANDLE_VALUE)
		return false;

	for(int index=0;index < m_PoolSize;index++)
	{
		sprintf(EvtName,"PID:%d IID:d TDX:%d",GetCurrentProcessId(),Threadpool::m_lInstance,index);
		hThread = CreateThread(NULL,0,Threadpool::ThreadProc,this,CREATE_SUSPENDED,&dwThreadId);

		if(hThread == INVALID_HANDLE_VALUE)
			return false;

		ThreadData.Free = true;
		ThreadData.WaitHandle = CreateEvent(NULL,TRUE,FALSE,EvtName);
		ThreadData.Thread = hThread;
		ThreadData.ThreadID = dwThreadId;

		m_Threads.insert(ThreadMap::value_type(dwThreadId,ThreadData));
		ResumeThread(hThread);
	}
	m_State = POOL_READY;
	return true;
}

Threadpool::~Threadpool( void )
{
	Destroy();
	ReleaseMemory();
	DeleteCriticalSection(&m_csFuncList);
	DeleteCriticalSection(&m_csThreads);
}

void Threadpool::ReleaseMemory()
{
	FunctionList::iterator funcIter;

	for(funcIter = m_FunctionList.begin();funcIter != m_FunctionList.end();funcIter++)
	{
		if(funcIter->UserData != NULL)
		{
			_FunctionData functionData = (*funcIter);
			UserPoolData* pUserPoolData = static_cast<UserPoolData*>(functionData.UserData);
			if(pUserPoolData != NULL)
			{
				delete pUserPoolData;
				pUserPoolData = NULL;
			}
		}
	}

	m_FunctionList.clear();
	m_Threads.clear();
}

void Threadpool::Destroy()
{
	if(m_State == POOL_DESTROYING || m_State == POOL_DESTROYED)
		return;

	m_State = POOL_DESTROYING;

	SetEvent(m_NotifyShutdown);
	Sleep(2000);

	if(GetWorkingThreadCount() > 0)
	{
		Sleep(m_WaitForThreadsToDieMS);
	}

	ThreadMap::iterator iter;

	for(iter = m_Threads.begin(); iter != m_Threads.end();iter++)
	{
		CloseHandle(iter->second.WaitHandle);
		CloseHandle(iter->second.Thread);
	}

	CloseHandle(m_NotifyShutdown);
	m_NotifyShutdown = INVALID_HANDLE_VALUE;

	ReleaseMemory();

	InterlockedDecrement(&Threadpool::m_lInstance);
	m_State = POOL_DESTROYED;
}

void Threadpool::SetPoolSize( int size )
{
	assert(size > 0);
	if(size <= 0)
		return;
	m_PoolSize = size;
}

bool Threadpool::GetThreadProc( DWORD dwThreadId,LPTHREAD_START_ROUTINE& Proc,LPVOID* Data,IRunObject** runObject)
{
	LPTHREAD_START_ROUTINE lpResult = NULL;
	_FunctionData FunctionData;
	FunctionList::iterator iter;

	EnterCriticalSection(&m_csFuncList);

	iter = m_FunctionList.begin();

	if(iter != m_FunctionList.end())
	{
		FunctionData = (*iter);
		Proc = FunctionData.StartAddress;

		if(Proc == NULL)
		{
			*runObject = static_cast<IRunObject*>(FunctionData.UserData);
		}
		else
		{
			*Data = FunctionData.UserData;
			runObject = NULL;
		}

		m_FunctionList.pop_front();
		
		LeaveCriticalSection(&m_csFuncList);
		return true;
	}
	else
	{
		LeaveCriticalSection(&m_csFuncList);
		return false;
	}
}

void Threadpool::FinishNotify( DWORD dwThreadId )
{
	ThreadMap::iterator iter;

	EnterCriticalSection(&m_csThreads);

	iter = m_Threads.find(dwThreadId);

	if(iter == m_Threads.end())
	{
		LeaveCriticalSection(&m_csThreads);
		return;
	}
	else
	{
		iter->second.Free = true;

		if(!m_FunctionList.empty())
		{
			LeaveCriticalSection(&m_csThreads);
			return;
		}
		else
		{
			LeaveCriticalSection(&m_csThreads);
			ResetEvent(iter->second.WaitHandle);
		}
	}
}

void Threadpool::BusyNotify( DWORD dwThreadId )
{
	ThreadMap::iterator iter;

	EnterCriticalSection(&m_csThreads);

	iter = m_Threads.find(dwThreadId);

	if(iter == m_Threads.end())
	{
		LeaveCriticalSection(&m_csThreads);
	}
	else
	{
		m_Threads[dwThreadId].Free = true;
		LeaveCriticalSection(&m_csThreads);
	}
}

bool Threadpool::Run( LPTHREAD_START_ROUTINE pFunc,LPVOID pData,ThreadPriority priority /*= THREAD_PRIORITY_LOW*/ )
{
	if(m_State == POOL_DESTROYING || m_State == POOL_DESTROYED)
		return false;

	_FunctionData funcData;

	UserPoolData* pPoolData = new UserPoolData();

	pPoolData->Data = pData;
	pPoolData->Pool = this;

	funcData.StartAddress = pFunc;
	funcData.UserData = pPoolData;

	EnterCriticalSection(&m_csFuncList);
	if(priority == THREAD_PRIORITY_LOW)
	{
		m_FunctionList.push_back(funcData);
	}
	else
	{
		m_FunctionList.push_front(funcData);
	}
	LeaveCriticalSection(&m_csFuncList);

	ThreadMap::iterator iter;

	EnterCriticalSection(&m_csThreads);
	for(iter = m_Threads.begin(); iter != m_Threads.end();iter++)
	{
		if(iter->second.Free)
		{
			iter->second.Free = false;
			SetEvent(iter->second.WaitHandle);
			break;
		}
	}
	LeaveCriticalSection(&m_csThreads);
	return true;
}

bool Threadpool::Run( IRunObject* runObject,ThreadPriority priority /*= THREAD_PRIORITY_LOW*/ )
{
	if(m_State == POOL_DESTROYING || m_State == POOL_DESTROYED || runObject == NULL)
		return false;

	runObject->pThreadPool = this;

	_FunctionData funcData;

	funcData.StartAddress = NULL;
	funcData.UserData = runObject;

	EnterCriticalSection(&m_csFuncList);
	if(priority == THREAD_PRIORITY_LOW)
		m_FunctionList.push_back(funcData);
	else
		m_FunctionList.push_front(funcData);
	LeaveCriticalSection(&m_csFuncList);

	ThreadMap::iterator iter;
	_ThreadData ThreadData;

	EnterCriticalSection(&m_csThreads);
	for(iter = m_Threads.begin();iter != m_Threads.end(); iter++)
	{
		ThreadData = (*iter).second;

		if(ThreadData.Free)
		{
			iter->second.Free = false;
			SetEvent(ThreadData.WaitHandle);
			break;
		}
	}
	LeaveCriticalSection(&m_csThreads);
	return true;
}

HANDLE Threadpool::GetWaitHandle( DWORD dwThreadId )
{
	HANDLE hWait;
	ThreadMap::iterator iter;

	EnterCriticalSection(&m_csThreads);
	iter = m_Threads.find(dwThreadId);

	if(iter == m_Threads.end())
	{
		LeaveCriticalSection(&m_csThreads);
		return NULL;
	}
	else
	{
		hWait = iter->second.WaitHandle;
		LeaveCriticalSection(&m_csThreads);
	}
	return hWait;
}

int Threadpool::GetWorkingThreadCount()
{
	ThreadMap::iterator iter;
	int nCount = 0;

	for(iter = m_Threads.begin();iter != m_Threads.end();iter++)
	{
		if(!iter->second.Free)
			nCount++;
	}
	return nCount;
}