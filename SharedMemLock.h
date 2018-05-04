//==============================================================================
//                  SharedMemLock.h
// 
// begin    : 2018-01-27
// describe : 共享内存锁
//==============================================================================

#ifndef SHAREDMEMLOCK_H
#define SHAREDMEMLOCK_H

#include <string>
#include "OperatingSystemDef.h"
#include "TypeDef.h"

#if defined(OS_UNIX)
#include <semaphore.h>
#elif defined(OS_WIN)
#include <windows.h>
#endif

using namespace std;

//==============================================================================
//
//                   CSharedMemLock
//                   共享内存锁
//
//==============================================================================

class MY_EXT_CLASS CSharedMemLock
{
public:
	CSharedMemLock();
	CSharedMemLock(const string& strName);
	
public:
	// 设置锁的名称
	void SetLockName(const string& strName);

	// 设置等待的时间
	void SetWaitTime(size_t nWaitMs);
	
	void Lock();
	void Unlock();

private:
	string m_strLockName;
	size_t m_nWaitMs;

#if defined(OS_UNIX)
	sem_t* m_pSem;
#elif defined(OS_WIN)
	HANDLE m_hMutex;
#endif
};

//==============================================================================
//
//                   CShmAutoLock
//                   共享内存自动锁（自动加锁解锁）
//
//==============================================================================
class MY_EXT_CLASS CShmAutoLock
{
public:
	CShmAutoLock(CSharedMemLock* pLock);
	~CShmAutoLock();
	
private:
	CShmAutoLock(const CShmAutoLock& objLock);
	CShmAutoLock& operator=(const CShmAutoLock& objLock);
	
private:
	CSharedMemLock* m_pLock;
};

#endif /* SHAREDMEMLOCK_H */


