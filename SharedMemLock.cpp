//==============================================================================
//                  SharedMemLock.cpp
// 
// begin    : 2018-01-27
// describe : 共享内存锁
//==============================================================================

#include "SharedMemLock.h"
#include <fcntl.h>
#include <cassert>

//==============================================================================
//
//                   CSharedMemKey
//                   共享内存key
//
//==============================================================================

CSharedMemLock::CSharedMemLock()
	: m_nWaitMs(0)
{
	
}

CSharedMemLock::CSharedMemLock(const string& strName)
	: m_strLockName(strName),
	  m_nWaitMs(0)
{

}

void CSharedMemLock::SetLockName(const string& strName)
{
	assert(!strName.empty());
	m_strLockName = strName;

	//创建并初始化有名信号量
	m_pSem = sem_open(m_strLockName.c_str(), O_RDWR | O_CREAT, 0644, 1);
	if (m_pSem)
	{
		
	}
}

void CSharedMemLock::SetWaitTime(size_t nWaitMs)
{
	m_nWaitMs = nWaitMs;
}
	
void CSharedMemLock::Lock() const
{
	timespec ts;
  	clock_gettime(CLOCK_REALTIME, &ts );     //获取当前时间
  	ts.tv_sec += ( m_nWaitMs / 1000 );        	 //加上等待时间的秒数
  	ts.tv_nsec += ( m_nWaitMs % 1000 ) * 1000; //加上等待时间纳秒数
  	
 	int nRet = sem_timedwait(m_pSem, &ts );                            
 	while( (nRet != 0) && (errno ==  EINTR) )   //等待信号量，errno==EINTR屏蔽其他信号事件引起的等待中断  
  	{
  		nRet = sem_timedwait(m_pSem, &ts );
  	}

	
	
}

void CSharedMemLock::UnLock() const
{
	//释放共享资源
	sem_post(m_pSem);
}


