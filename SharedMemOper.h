//==============================================================================
//                      SharedMemOper.h
//
//begin   : 2016-09-09
//describe: 共享内存操作类头文件
//==============================================================================

#ifndef SHAREDMEMOPER_H
#define SHAREDMEMOPER_H

#include "SharedMemDef.h"

//==============================================================================
//
//                   CSharedMemOper
//                  共享内存操作类
//
//==============================================================================

class EXT_CLASS CSharedMemOper
{
public:
	CSharedMemOper();
	virtual ~CSharedMemOper();

public:
	// 设定共享内存的key
	void SetSharedMemKey(shmkey_t objShmKey);

	// 创建并映射一个指定“Key”的共享内存。
	virtual bool CreateSharedMem(size_t nSize);
	// 映射共享内存
	virtual bool AttachSharedMem();
	// 取消共享内存的映射
	bool DetachSharedMem();
	// 释放共享内存资源（从系统中删除该共享内存）
	virtual bool ReleaseSharedMem();

	// 获取实际共享内存的大小
	size_t GetShmSize() const;
	// 获取共享内存的首地址
	void* GetShmAddr() const;

protected:
	// 当前的共享内存是否有效
	bool IsShmValid() const;

private:
	CSharedMemOper(const CSharedMemOper& other);
	CSharedMemOper& operator=(const CSharedMemOper& other);
	
protected:
	shmid_t m_objShmId;			// 标识共享内存的id
	shmkey_t m_objShmKey;		// 创建共享内存使用额key
	size_t m_nShmSize;			// 实际的共享内存大小，包含头信息（用于快速获取长度）
	void* m_pShmAddr;			// 共享内存的首地址
};

#endif /* SHAREDMEMOPER_H */

