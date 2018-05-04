//==============================================================================
//                  SharedMemKeyGen.cpp
// 
// begin    : 2018-01-27
// describe : 共享内存用到的Key，用于唯一标识一块共享内存
//==============================================================================

#include "SharedMemKeyGen.h"
#include "SharedMemDef.h"
#include "AppPath.h"
#include "Utility.h"

//==============================================================================
//
//                   CSharedMemKeyGen
//                   共享内存key生成工具
//
//==============================================================================

CSharedMemKeyGen::CSharedMemKeyGen()
	: m_objShmKey(SHM_KEY_INVALID),
	  m_nId(-1)
{

}

CSharedMemKeyGen::CSharedMemKeyGen(const string& strKeyFile, int nId)
	: m_objShmKey(SHM_KEY_INVALID),
	  m_strKeyFile(strKeyFile),
	  m_nId(nId)
{

}

void CSharedMemKeyGen::SetParam(const string& strKeyFile, int nId)
{
	m_strKeyFile = strKeyFile;
	m_nId = nId;
}

bool CSharedMemKeyGen::GenerateKey()
{
	// 获取文件全路径
	string strKeyFileFullPath = CAppPath::GetShmKeyPath() + m_strKeyFile;
	
	// 创建路径
	if (!CUtility::MakeDir(strKeyFileFullPath.c_str()))
	{
//		LogImportant("【CSharedMemOper::CreateFtokShmFile】调用CUtility::MakeDir失败，要创建的文件路径：%s",
//					 strKeyFileFullPath.c_str());
		return false;
	}

	// 不存在就创建文件
	FILE* pFile = fopen(strKeyFileFullPath.c_str(), "a");
	if(pFile == NULL)
	{
//		LogImportant("【CSharedMemOper::CreateFtokShmFile】调用fopen失败，要创建的文件：%s", strKeyFileFullPath.c_str());
		return false;
	}
	fclose(pFile);
	pFile = NULL;

	// 生成key
#if defined(OS_UNIX)
	m_objShmKey = ftok(strKeyFileFullPath.c_str(), m_nId);
#elif defined(OS_WIN)
	BY_HANDLE_FILE_INFORMATION objFileInfo;
	HANDLE hFile = CreateFile(strKeyFileFullPath.c_str(), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	GetFileInformationByHandle(hFile, &objFileInfo);
	objFileInfo;
	char szBuffer[1024];
	snprintf(szBuffer, sizeof(szBuffer), "%d-%u-%u%u",
		m_nId,
		objFileInfo.dwVolumeSerialNumber,
		objFileInfo.nFileIndexHigh,
		objFileInfo.nFileIndexLow);
	m_objShmKey = szBuffer;
#endif

	if(m_objShmKey == SHM_KEY_INVALID)
	{
//		LogImportant("【CStatusMemOper::InitShmInfo】ftok() fail! fname=%s, id=%d. errno=%d, info=%s",
//			strKeyFileFullPath.c_str(), RDB_SHM2_FTOK_ID, errno, strerror(errno));
		return false;
	}

	return true;
}

bool CSharedMemKeyGen::IsValid() const
{
	return m_objShmKey == SHM_KEY_INVALID;
}

shmkey_t CSharedMemKeyGen::GetKey() const
{
	return m_objShmKey;
}


