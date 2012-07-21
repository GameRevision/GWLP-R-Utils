#pragma once
#include <windows.h>
#include <list>
#include <map>
#include <assert.h>
#include "Singleton.h"
#include "RunObject.h"

typedef struct tagFunctionData
{
	LPTHREAD_START_ROUTINE StartAddress;
	union
	{
		LPVOID UserData;
		IRunObject* runObject;
	};
	
}_FunctionData;

typedef struct tagThreadData
{
	bool Free;
	HANDLE WaitHandle;
	HANDLE Thread;
	DWORD ThreadID;
}_ThreadData;

typedef struct UserPoolData
{
	LPVOID Data;
	Threadpool* Pool;
}_tagUSerPoolData;


typedef std::map<DWORD,_ThreadData,std::less<DWORD>,std::allocator<_ThreadData>> ThreadMap;
typedef std::list<_FunctionData,std::allocator<_FunctionData>> FunctionList;

typedef enum ThreadPriority : BYTE
{
	THREAD_PRIORITY_HIGH,
	THREAD_PRIORITY_LOW
};

typedef enum PoolState : BYTE
{
	POOL_READY,
	POOL_DESTROYING,
	POOL_DESTROYED
};

class Threadpool : public Singleton<Threadpool>
{
public:
	Threadpool(int nPoolSize = 10,int nWaitTimeForThreadsToComplete = 5000,bool bCreateNow = true);
	~Threadpool(void);

	bool Create();
	void Destroy();


	int GetPoolSize(){return m_PoolSize;}
	void SetPoolSize(int size);

	bool Run(LPTHREAD_START_ROUTINE pFunc,LPVOID pData,ThreadPriority priority = THREAD_PRIORITY_LOW);
	bool Run(IRunObject*,ThreadPriority priority = THREAD_PRIORITY_LOW);

	bool CheckThreadStop(){return m_State== POOL_DESTROYING||m_State==POOL_DESTROYED;}
	int GetWorkingThreadCount();

	PoolState GetState(){return m_State;}
private:
	static DWORD WINAPI ThreadProc(LPVOID);

	void FinishNotify(DWORD dwThreadId);
	void BusyNotify(DWORD dwThreadId);
	void ReleaseMemory();

	HANDLE GetWaitHandle(DWORD dwThreadId);
	HANDLE GetShutdownHandle(){return m_NotifyShutdown;}

	bool GetThreadProc(DWORD dwThreadId,LPTHREAD_START_ROUTINE&,LPVOID*,IRunObject**);
private:
	FunctionList m_FunctionList;
	ThreadMap m_Threads;

	int m_PoolSize;
	int m_WaitForThreadsToDieMS;
	TCHAR m_PoolName[256];
	HANDLE m_NotifyShutdown;

	volatile PoolState m_State;
	volatile static long m_lInstance;

	CRITICAL_SECTION m_csFuncList;
	CRITICAL_SECTION m_csThreads;
};

