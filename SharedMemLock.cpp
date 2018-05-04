//==============================================================================
//                  SharedMemLock.cpp
// 
// begin    : 2018-01-27
// describe : 共享内存锁
//==============================================================================

#include "SharedMemLock.h"
#include "SharedMemDef.h"
#include <fcntl.h>
#include <cassert>

//==============================================================================
//
//                   CSharedMemLock
//                   共享内存Lock
//
//==============================================================================

CSharedMemLock::CSharedMemLock()
	: m_nWaitMs(SHM_LOCK_WAIT_INFI)
{

}

CSharedMemLock::CSharedMemLock(const string& strName)
	: m_nWaitMs(SHM_LOCK_WAIT_INFI)
{
	SetLockName(strName);
}

void CSharedMemLock::SetLockName(const string& strName)
{
	assert(!strName.empty());
	m_strLockName = strName;

#if defined(OS_UNIX)
	//创建并初始化有名信号量
	m_pSem = sem_open(m_strLockName.c_str(), O_RDWR | O_CREAT, 0644, 1);
	if (SEM_FAILED == m_pSem)
	{
		// ...
	}
#elif defined(OS_WIN)
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, m_strLockName.c_str());
	if (NULL == m_hMutex)
	{
		m_hMutex = CreateMutex(NULL, FALSE, m_strLockName.c_str());
	}
#endif
}

void CSharedMemLock::SetWaitTime(size_t nWaitMs)
{
	m_nWaitMs = nWaitMs;
}
	
void CSharedMemLock::Lock()
{
#if defined(OS_UNIX)
	if (m_nWaitMs != SHM_LOCK_WAIT_INFI)
	{
		timespec ts;
		clock_gettime( CLOCK_REALTIME, &ts ); //获取当前时间
		ts.tv_sec += ( m_nWaitMs / 1000 ); //加上等待时间的秒数
		ts.tv_nsec += ( m_nWaitMs % 1000 ) * 1000; //加上等待时间纳秒数
		int nRet = sem_timedwait(m_pSem, &ts );
		while( (nRet != 0) && (errno ==  EINTR) ) //等待信号量，errno==EINTR屏蔽其他信号事件引起的等待中断  
		{
			nRet = sem_timedwait(m_pSem, &ts);
		}
	}
	else
	{
		int nRet = sem_wait(m_pSem);
		while ( nRet != 0 && errno == EINTR)
		{
			nRet = sem_wait(m_pSem);
		}
	}

#elif defined(OS_WIN)
	DWORD nRet = WaitForSingleObject(m_hMutex, (DWORD)m_nWaitMs);
#endif
}

void CSharedMemLock::Unlock()
{
	//释放共享资源
#if defined(OS_UNIX)
	sem_post(m_pSem);
#elif defined(OS_WIN)
	ReleaseMutex(m_hMutex);
#endif
}


//==============================================================================
//
//                   CShmAutoLock
//                   共享内存自动锁（自动加锁解锁）
//
//==============================================================================
CShmAutoLock::CShmAutoLock(CSharedMemLock* pLock)
	: m_pLock(pLock)
{
	assert(m_pLock);
	m_pLock->Lock();
}

CShmAutoLock::~CShmAutoLock()
{
	assert(m_pLock);
	m_pLock->Unlock();
}


