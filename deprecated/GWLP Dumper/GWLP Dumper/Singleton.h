#pragma once
#include <assert.h>
#include <windows.h>

/**
*	Singelton Template class
*/
template <class type> class Singleton
{
public:
	/**
	*	Constructor
	*/
	Singleton(){
		assert(this->m_Instance == NULL);
		this->m_Instance = static_cast<type*>(this);
	}
	/**
	*	virtual Destructor
	*/
	virtual ~Singleton(){
		this->m_Instance = NULL;
	}
	/**
	*	Function to retrieve Singelton Instance
	*/
	static inline type* GetInstance()
	{
		assert(m_Instance);
		return m_Instance;
	}
private:
	Singleton(const Singleton& source){}
protected:
	static type* m_Instance;
};

template <class type> type* Singleton<type>::m_Instance = NULL;