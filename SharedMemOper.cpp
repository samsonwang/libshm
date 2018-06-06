//==============================================================================
//                      SharedMemOper.cpp
//
//begin   : 2016-09-11
//describe: 定义共享内存操作类
//==============================================================================

#include "SharedMemOper.h"
#include <errno.h>
#include <cstring>
#include "SharedMemDef.h"
#include "Log.h"

//==============================================================================
//
//                   CSharedMemOper
//                   共享内存操作类
//
//==============================================================================

CSharedMemOper::CSharedMemOper()
	: m_objShmId(SHM_ID_INVALID),
	  m_objShmKey(SHM_KEY_INVALID),
	  m_nShmSize(0),
	  m_pShmAddr(NULL)
{

}

CSharedMemOper::~CSharedMemOper()
{
	DetachSharedMem();
}

void CSharedMemOper::SetSharedMemKey(shmkey_t objShmKey)
{
	m_objShmKey = objShmKey;
}

bool CSharedMemOper::CreateSharedMem(size_t nSize)
{
    Log("CSharedMemOper::CreateSharedMem, start to create sharedmemory，shmkey="
        SHM_KEY_FMT ", size=%u\n",
        SHM_KEY_VAL(m_objShmKey), nSize);

    if (m_objShmKey == SHM_KEY_INVALID)
	{
        Log("[ERROR] CSharedMemOper::CreateSharedMem, invalid key!\n");
        return false;
	}
	
	// 要创建共享内存
	// 可能出现程序异常退出，共享内存残留的情况
#if defined(OS_UNIX)
	m_objShmId = shmget(m_objShmKey, 0, 0);
#elif defined(OS_WIN)
    m_objShmId = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_objShmKey.c_str());
#endif
    if (m_objShmId != SHM_ID_INVALID)
    {
        // 共享内存已经存在
        Log("[WARN] CSharedMemOper::CreateSharedMem, 创建共享内存前删除已有的(shmkey=" SHM_KEY_FMT ")\n",
            SHM_KEY_VAL(m_objShmKey));

        ReleaseSharedMem();
    }

    // 创建：在Linux环境中，对开始申请的共享内存空间进行了初始化，初始值为0x00
    // IPC_CREAT：如果不存在与key相等的共享内存，则新建；如果存在，返回此共享内存的标识符(但是如果新size比原有的大会出错)
#if defined(OS_UNIX)
	m_objShmId = shmget(m_objShmKey, nSize, IPC_CREAT|0664);
#elif defined(OS_WIN)
	m_objShmId = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                                   (DWORD)nSize, m_objShmKey.c_str());
#endif
	if (m_objShmId == SHM_ID_INVALID)
	{
        Log("[ERROR] CSharedMemOper::CreateSharedMem, allocate shared mem fail, shmkey="
            SHM_KEY_FMT ", errno=%d, strerror(errno)=%s\n",
            SHM_KEY_VAL(m_objShmKey), nSize, errno, strerror(errno));

		return false;
	}
	
    Log("CSharedMemOper::CreateSharedMem, allocate shm succeed, shmid=%d\n", m_objShmId);

#if defined(OS_UNIX)
	m_pShmAddr = shmat(m_objShmId, NULL, 0);
#elif defined(OS_WIN)
	m_pShmAddr = MapViewOfFile(m_objShmId, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif
	if (((void*)-1)==m_pShmAddr || NULL==m_pShmAddr)
	{
        Log("[ERROR] CSharedMemOper::CreateSharedMem, attach shm fail, shmid=%d, errno=%d, info=%s\n",
            m_objShmId, errno, strerror(errno));
        
        ReleaseSharedMem();
		return false;
	}

	// 映射成功后，记录共享内存大小
	m_nShmSize = nSize;
    
    Log("CSharedMemOper::CreateSharedMem, create succeed, m_nShmId=%d, m_pShmAddr=0x%x, m_nShmSize=%u",
        m_objShmId, m_pShmAddr, m_nShmSize);

	return true;
}

bool CSharedMemOper::AttachSharedMem()
{
	// 已经映射成功后，不会再映射
	if (NULL != m_pShmAddr && ((void*)-1) != m_pShmAddr)
	{
		return true;
	}

	// 只映射
#if defined(OS_UNIX)
	m_objShmId = shmget(m_objShmKey, 0, 0);
#elif defined(OS_WIN)
	m_objShmId = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_objShmKey.c_str());
#endif
	if (m_objShmId == SHM_ID_INVALID)
	{
        Log("CSharedMemOper::AttachShardeMem, shmget fail, key=" SHM_KEY_FMT
            ", errno=%d, strerror(errno)=%s\n",
            SHM_KEY_VAL(m_objShmKey), errno, strerror(errno));
        return false;
	}

#if defined(OS_UNIX)
	m_pShmAddr = shmat(m_objShmId, NULL, 0);
#elif defined(OS_WIN)
	m_pShmAddr = MapViewOfFile(m_objShmId, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif
	if (((void*)-1)==m_pShmAddr || NULL==m_pShmAddr)
	{
        Log("CSharedMemOper::AttachSharedMem, shmat fail, shmid=%d, errno=%d, info=%s\n",
            m_objShmId, errno, strerror(errno));
        
		DetachSharedMem();
		return false;
	}
	
	// 映射成功后查询大小
#if defined(OS_UNIX)
	struct shmid_ds objShmStat;
	if(0 == shmctl(m_objShmId, IPC_STAT, &objShmStat))
	{
		m_nShmSize = objShmStat.shm_segsz;
		Log("CSharedMemOper::AttachSharedMem, attached shm size=%u\n", m_nShmSize);
	}
	else
	{
        Log("CSharedMemOper::AttachSharedMem, fail to get shm size after attch succeed\n");
	}
#endif
	return true;
}

bool CSharedMemOper::DetachSharedMem()
{
	if (m_pShmAddr == NULL)
	{
		return false;
	}

    Log("CSharedMemOper::DetachSharedMem, shmdt shmid=%d\n", m_objShmId);

#if defined(OS_UNIX)
	bool bRet = (shmdt(m_pShmAddr) == 0);
#elif defined(OS_WIN)
	BOOL bRet = UnmapViewOfFile(m_pShmAddr);
#endif
	if (bRet)
	{
		m_objShmId = SHM_ID_INVALID;
		m_pShmAddr = NULL;
		m_nShmSize = 0;
		return true;
	}

 	Log("CSharedMemOper::DetachSharedMem, detach shm fail, errno=%d, %s\n",
        errno, strerror(errno));

	return false;
}

bool CSharedMemOper::ReleaseSharedMem()
{
	shmid_t objShmIdTmp = m_objShmId;
	DetachSharedMem();

	if (SHM_ID_INVALID == objShmIdTmp)
	{
        Log("[WARN] CSharedMemOper::ReleaseSharedMem, release shmid is invalid (shmid=%d)!\n",
            objShmIdTmp);
        return false;
	}

#if defined(OS_UNIX)
	bool bRet = (0 == shmctl(objShmIdTmp, IPC_RMID, NULL));
#elif defined(OS_WIN)
	BOOL bRet = CloseHandle(objShmIdTmp);
#endif
	if (bRet)
	{
		m_objShmId = SHM_ID_INVALID;
		m_pShmAddr = NULL;
		m_nShmSize = 0;
		return true;
	}
	else
	{
		m_objShmId = objShmIdTmp;
        Log("[ERROR] CSharedMemOper::ReleaseSharedMem, release shm fail, errno=%d, %s\n",
            errno, strerror(errno));
	}
	
	return false;
}

size_t CSharedMemOper::GetShmSize() const
{
	return m_nShmSize;
}

void* CSharedMemOper::GetShmAddr() const
{
	return m_pShmAddr;
}

bool CSharedMemOper::IsShmValid() const
{
	return m_objShmId != SHM_ID_INVALID
		&& m_objShmKey != SHM_KEY_INVALID
		&& m_nShmSize != 0
		&& m_pShmAddr != NULL
		&& m_pShmAddr != ((void*)-1);
}

