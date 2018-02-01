//==============================================================================
//                      SharedMemOper.cpp
//
//begin   : 2016-09-11
//describe: 定义共享内存操作类
//==============================================================================

#include "SharedMemOper.h"
#include "SharedMemDef.h"

#include <sys/shm.h>
#include <errno.h>
#include <cstring>

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
	LogImportant("CSharedMemOper::CreateSharedMem, 开始创建共享内存，shmkey=0x%x, size=%u",
				 m_objShmKey, nSize);


	if (m_objShmKey == SHM_KEY_INVALID)
	{
		LogImportant("[ERROR] CSharedMemOper::CreateShareMem, 指定的Key无效!");
		return false;
	}
	
	// 要创建共享内存
    // 如果已经存在则删掉
	m_objShmId = shmget(m_objShmKey, 0, 0);	// 映射
	if (m_objShmId != SHM_ID_INVALID)
	{
		LogImportant("[WARN] CSharedMemOper::CreateShareMem, 创建共享内存前删除已有的(shmkey=0x%x)！",
					 m_objShmKey);
		
		ReleaseSharedMem();
	}

	// 创建：在Linux环境中，对开始申请的共享内存空间进行了初始化，初始值为0x00
	// IPC_CREAT：如果不存在与key相等的共享内存，则新建；如果存在，返回此共享内存的标识符（但是如果新size比原有的大会出错）
	m_objShmId = shmget(m_objShmKey, nSize, IPC_CREAT|0664);
	if (m_objShmId == SHM_ID_INVALID)
	{
		LogImportant("[ERROR] CSharedMemOper::CreateSharedMem, shmget fail, shmkey=0x%08x, errno=%d, strerror(errno)=%s",
					 m_objShmKey, nSize, errno, strerror(errno));
		return false;
	}

	LogImportant("CSharedMemOper::CreateShareMem, allocate shm succeed, shmid=%d", m_objShmId);

	// 映射共享内存
	m_pShmAddr = shmat(m_objShmId, NULL, 0);
	if (((void*)-1)==m_pShmAddr || NULL==m_pShmAddr)
	{
		LogImportant("[ERROR] CSharedMemOper::CreateShareMem, attach shm fail, shmid=%d, errno=%d, info=%s",
					 m_objShmId, errno, strerror(errno));
		ReleaseSharedMem();
		return false;
	}

	// 映射成功后，记录共享内存大小
	m_nShmSize = nSize;
	
	LogImportant("CSharedMemOper::CreateSharedMem, create succeed, m_nShmID=%d, m_pShmAddr=0x%x, m_nShmSize=%u",
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
	
	m_objShmId = shmget(m_objShmKey, 0, 0);	// 只映射
	if (m_objShmId == SHM_ID_INVALID)
	{
		LogImportant("CSharedMemOper::AttachShardeMem, shmget fail, key=0x%08x, errno=%d, strerror(errno)=%s",
					 m_objShmKey, errno, strerror(errno));
			return false;
	}

	m_pShmAddr = shmat(m_objShmId, NULL, 0);
	if (((void*)-1)==m_pShmAddr || NULL==m_pShmAddr)
	{
		LogImportant("CSharedMemOper::AttachSharedMem, shmat fail, shmid=%d, errno=%d, info=%s",
					 m_objShmId, errno, strerror(errno));
		DetachSharedMem();
		return false;
	}
	
	// 映射成功后查询大小
	struct shmid_ds objShmStat;
	if (0==shmctl(m_objShmId, IPC_STAT, &objShmStat))
	{
		m_nShmSize = objShmStat.shm_segsz;
		LogImportant("CSharedMemOper::AttachSharedMem, attached shm size=%u", m_nShmSize);
	}
	else
	{
		LogImportant("CSharedMemOper::AttachSharedMem, fail to get shm size after attch succeed");
	}
	
	return true;
}

bool CSharedMemOper::DetachSharedMem()
{
	if (m_pShmAddr == NULL)
	{
		return false;
	}

	LogImportant("CSharedMemOper::DetachShareMem, shmdt shmid=%d", m_objShmId);

	if (shmdt(m_pShmAddr) == 0)
	{
		m_objShmId = SHM_ID_INVALID;
		m_pShmAddr = NULL;
		m_nShmSize = 0;
		return true;
	}

	LogImportant("shmdt fail, errno=%d, %s", errno, strerror(errno));

	return false;
}

bool CSharedMemOper::ReleaseSharedMem()
{
	shmid_t objShmIdTmp = m_objShmId;
	DetachSharedMem();

	if (SHM_ID_INVALID == objShmIdTmp)
	{
		LogImportant("[WARN] CSharedMemOper::ReleaseShareMem, release shmid is invalid (shmid=%d)!", objShmIdTmp);
		return false;
	}

	if (0 == shmctl(objShmIdTmp, IPC_RMID, NULL))
	{
		m_objShmId = SHM_ID_INVALID;
		m_pShmAddr = NULL;
		m_nShmSize = 0;
		return true;
	}
	else
	{
		m_objShmId = objShmIdTmp;
		LogImportant("[ERROR] CSharedMemOper::ReleaseShareMem, shmctl IPC_RMID fail, errno=%d, %s", errno, strerror(errno));
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

