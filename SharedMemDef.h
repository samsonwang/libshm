//==============================================================================
//                  SharedMemDef.h
// 
// begin    : 2018-01-27
// describe : 共享内存用到的定义
//==============================================================================

#ifndef SHAREDMEMDEF_H
#define SHAREDMEMDEF_H

#include "OperatingSystemDef.h"

#if defined(OS_UNIX)
	#include <sys/ipc.h>
	typedef int					shmid_t;
	typedef key_t				shmkey_t;
	#define SHM_ID_INVALID 		-1
	#define SHM_KEY_INVALID 	-1

#elif defined(OS_WIN)
	#include <string>
	typedef HANDLE 				shmid_t;
	typedef std::string			shmkey_t;
	#define SHM_ID_INVALID 		INVALID_HANDLE
	#define SHM_KEY_INVALID		""
#endif

#endif /* SHAREDMEMDEF_H */

