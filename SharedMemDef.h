//==============================================================================
//                  SharedMemDef.h
// 
// begin    : 2018-01-27
// describe : 共享内存用到的定义
//==============================================================================

#ifndef SHAREDMEMDEF_H
#define SHAREDMEMDEF_H

#include "TypeDef.h"
#include "OSDef.h"

#if defined(OS_UNIX)
	#include <sys/ipc.h>
	#include <sys/shm.h>
	typedef int					shmid_t;
	typedef key_t				shmkey_t;
	#define SHM_ID_INVALID 		-1
	#define SHM_KEY_INVALID 	-1
	#define SHM_KEY_FMT			"0x%08x"
	#define SHM_KEY_VAL(key)	(key)
	#define SHM_LOCK_WAIT_INFI	0
#elif defined(OS_WIN)
	#include <string>
	#include <windows.h>
	#include <esent.h>
	typedef HANDLE 				shmid_t;
	typedef std::string			shmkey_t;
	#define SHM_ID_INVALID 		INVALID_HANDLE_VALUE
	#define SHM_KEY_INVALID		""
	#define SHM_KEY_FMT			"%s"
	#define SHM_KEY_VAL(key)	((key).c_str())
	#define SHM_LOCK_WAIT_INFI	INFINITE
#endif

#endif // SHAREDMEMDEF_H

