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
	/*
	LogImportant("【CSharedMemOper::CreateShareMem】%s，Key=0x%x, lock=%s, UserSize=%d.",
				 (dwUserSize==0) ? "开始<映射>共享内存" : "开始<创建>共享内存",
				 dwKey, szLockName, dwUserSize);
	*/

	if (m_objShmKey == SHM_KEY_INVALID)
	{
		LogImportant("【CSharedMemOper::CreateShareMem】指定的Key无效!");
		return false;
	}
	
	// 要创建共享内存
    // 如果已经存在则删掉
	m_objShmId = shmget(m_objShmKey, 0, 0);	// 映射
	if (m_objShmId != SHM_ID_INVALID)
	{
		LogImportant("【CSharedMemOper::CreateShareMem】创建共享内存前删除已有的(key=0x%x)！请注意这是不正常的，"
					 "正常情况下上次程序退出应该已经删除了，这种情况一般出现在用kill -9 *杀程序时！！！！！！！！！", m_objShmKey);
		shmctl(m_objShmId, IPC_RMID, NULL);
	}

	// 创建：在Linux环境中，对开始申请的共享内存空间进行了初始化，初始值为0x00
	// IPC_CREAT：如果不存在与key相等的共享内存，则新建；如果存在，返回此共享内存的标识符（但是如果新size比原有的大会出错）
	m_objShmId = shmget(m_objShmKey, nSize, IPC_CREAT|0664);
	if (m_objShmId == SHM_ID_INVALID)
	{
		LogImportant("【CSharedMemOper::CreateShareMem】shmget(0x%08x, %u, %s) fail, errno=%d, strerror(errno)=%s\n"
					 "常见errno大概的中文含义如下（英文解释可以在linux命令行执行 man shmget 查看）:\n"
					 "    %d:EACCES-用户没有权限\n"
					 "    %d:EEXIST-指定了IPC_CREAT|IPC_EXCL选项，但共享内存已存在\n"
					 "    %d:EINVAL-创建了共享内存，但是size小于SHMMIN或大于SHMMAX，或者相同key的共享内存存在，但是新的size比已经存在的大\n"
					 "    %d:ENFILE-打开文件总数已经到达了系统限定的上限\n"
					 "    %d:ENOENT-参数key所指的共享内存不存在，且参数未指定IPC_CREAT选项\n"
					 "    %d:ENOMEM-核心内存不足\n"
					 "    %d:ENOSPC-系统没有可用的共享内存ID，或者如果分配了这个共享内存则会超出系统对共享内存的限制\n"
					 "    %d:EPERM-指定了SHM_HUGETLB标志，但是没有权限\n\n",
					 m_objShmKey, nSize, "IPC_CREAT|0664", errno, strerror(errno),
					 EACCES, EEXIST, EINVAL, ENFILE, ENOENT, ENOMEM, ENOSPC, EPERM);
		return false;
	}

	LogImportant("CSharedMemOper::CreateShareMem, shmget得到的共享内存id=%d.", m_objShmId);

	// 映射共享内存
	m_pShmAddr = shmat(m_objShmId, NULL, 0);
	if (((void*)-1)==m_pShmAddr || NULL==m_pShmAddr)
	{
		LogImportant("CSharedMemOper::CreateShareMem, shmat fail, errno=%d, info=%s", errno, strerror(errno));
		ReleaseSharedMem();
		return false;
	}

	LogImportant("CSharedMemOper::CreateShareMem, create succeed， m_nShmID=%d, m_pShmAddr=0x%x, m_nShmSize=%d",
				 m_objShmId, m_pShmAddr, m_nShmSize);

	return true;
}


bool CSharedMemOper::AttachSharedMem()
{
	m_objShmId = shmget(m_objShmKey, 0, 0);	// 只映射
	if (m_objShmId == SHM_ID_INVALID)
	{
		LogImportant("\n\n【CSharedMemOper::CreateShareMem】shmget(0x%08x, %u, %s) fail, errno=%d, strerror(errno)=%s\n"
					 "常见errno大概的中文含义如下（英文解释可以在linux命令行执行 man shmget 查看）:\n"
					 "    	%d:EACCES-用户没有权限\n"
					 "    %d:EEXIST-指定了IPC_CREAT|IPC_EXCL选项，但共享内存已存在\n"
					 "    %d:EINVAL-创建了共享内存，但是size小于SHMMIN或大于SHMMAX，或者相同key的共享内存存在，但是新的size比已经存在的大\n"
					 "    %d:ENFILE-打开文件总数已经到达了系统限定的上限\n"
					 "    %d:ENOENT-参数key所指的共享内存不存在，且参数未指定IPC_CREAT选项\n"
					 "    %d:ENOMEM-核心内存不足\n"
					 "    %d:ENOSPC-系统没有可用的共享内存ID，或者如果分配了这个共享内存则会超出系统对共享内存的限制\n"
					 "    %d:EPERM-指定了SHM_HUGETLB标志，但是没有权限\n\n",
					 m_objShmKey, 0, 0, errno, strerror(errno),
					 EACCES, EEXIST, EINVAL, ENFILE, ENOENT, ENOMEM, ENOSPC, EPERM);
			return false;
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
		LogImportant("CSharedMemOper::ReleaseShareMem, release shmid is invalid (shmid=%d)!", objShmIdTmp);
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
		LogImportant("CSharedMemOper::ReleaseShareMem, shmctl IPC_RMID fail, errno=%d, %s", errno, strerror(errno));
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
		&& m_pShmAddr != NULL;
}

