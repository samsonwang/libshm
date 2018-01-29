//==============================================================================
//                  SharedMemLock.h
// 
// begin    : 2018-01-27
// describe : 共享内存锁
//==============================================================================

#ifndef SHAREDMEMLOCK_H
#define SHAREDMEMLOCK_H

#include "OperatingSystemDef.h"
#include <string>

#if defined(OS_UNIX)
#include <semaphore.h>
#endif

using namespace std;

//==============================================================================
//
//                   CSharedMemKey
//                   共享内存key
//
//==============================================================================

class CSharedMemLock
{
public:
	CSharedMemLock();
	CSharedMemLock(const string& strName);
	
public:
	// 设置锁的名称
	void SetLockName(const string& strName);

	// 设置等待的时间
	void SetWaitTime(size_t nWaitMs);
	
	void Lock() const;
	void UnLock() const;

private:
	string m_strLockName;
	size_t m_nWaitMs;

#if defined(OS_UNIX)
	sem_t* m_pSem;
#elif defined(OS_WIN)
	HANDLE m_hMutex;
#endif
};

#endif /* SHAREDMEMLOCK_H */


