//==============================================================================
//                  SharedMemKeyGen.h
// 
// begin    : 2018-01-27
// describe : 共享内存用到的Key，用于唯一标识一块共享内存
//==============================================================================

#ifndef SHAREDMEMKEYGEN_H
#define SHAREDMEMKEYGEN_H

#include <string>
#include "OperatingSystemDef.h"
#include "SharedMemDef.h"

using namespace std;

//==============================================================================
//
//                   CSharedMemKeyGen
//                   共享内存key生成工具
//
//==============================================================================

class CSharedMemKeyGen
{
public:
	CSharedMemKeyGen();
	CSharedMemKeyGen(const string& strKeyFile, int nId);

public:
	// 设置Key使用的文件和offset
	void SetParam(const string& strKeyFile, int nId);
	// 生成key
	bool GenerateKey();
	// 判断是否有效
	bool IsValid() const;
	// 获取实际的key值
	shmkey_t GetKey() const;

private:
	shmkey_t	m_objShmKey;		// 生成的key
	string 		m_strKeyFile;		// 用于生成key的文件
	int			m_nId;				// 用于生成key的offset
};

#endif /* SHAREDMEMKEYGEN_H */

