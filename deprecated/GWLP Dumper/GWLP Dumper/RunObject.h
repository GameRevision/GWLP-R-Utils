#pragma once
class Threadpool;

struct IRunObject
{
	virtual void Run() = 0;
	virtual bool AutoDelete() = 0;
	virtual ~IRunObject(){};
	Threadpool* pThreadPool;
};
