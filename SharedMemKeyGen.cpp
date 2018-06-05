//==============================================================================
//                  SharedMemKeyGen.cpp
// 
// begin    : 2018-01-27
// describe : 共享内存用到的Key，用于唯一标识一块共享内存
//==============================================================================

#include "SharedMemKeyGen.h"
#include "SharedMemDef.h"

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
	FILE* pFile = fopen(m_strKeyFile.c_str(), "a");
	if(pFile == NULL)
	{
		return false;
	}
	fclose(pFile);
	pFile = NULL;

#if defined(OS_UNIX)
	m_objShmKey = ftok(m_strKeyFile.c_str(), m_nId);
#elif defined(OS_WIN)
	BY_HANDLE_FILE_INFORMATION objFileInfo;
	HANDLE hFile = CreateFile(m_strKeyFile.c_str(), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	GetFileInformationByHandle(hFile, &objFileInfo);
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

