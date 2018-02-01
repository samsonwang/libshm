//==============================================================================
//                      SvrPubDef.h
//
//begin   : 2017-06-14
//describe: 本库用到的宏定义
//==============================================================================
#ifndef RDBPUBDEF_H
#define RDBPUBDEF_H

// =======实时库锁文件名称，“程序运行路径+文件名”是绝对路径===============
#define RDB_SHM_KEY_FILE				"RdbShmKey"


// =======实时库中【参数数据包】共享内存创建时ftok使用的ID======================
#define RDB_SHM_KEY_ID					1
// =======实时库中【状态】共享内存创建时ftok使用的ID============================
#define RDB_SHM_KEY_ID_STAT				2

// 共享内存准备好
#define RDB_SHM_READY					1

// 状态实时库中的状态类型数量
#define RDB_SHM_STAT_TYPE_COUNT			2

// =======实时库锁名称，“共享内存key+锁名称”才是完成的锁名=====================
// 应用服务器上实时库的第一块共享内存访问的互斥锁名称
#define RDB_SHM1_LOCK_ALL					"RdbShm1_All"

// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-共享内存头
#define RDB_SHM_LOCK_HEAD_STAT				"RdbShm2_Head"
// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-遥信
#define RDB_SHM_LOCK_STAT_SIGNAL			"RdbShm2_Signal"
// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-遥测
#define RDB_SHM_LOCK_STAT_MEASURE			"RdbShm2_Measure"

// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-操作设备
#define RDB_SHM2_LOCK_OPERDEV				"RdbShm2_OperDev"
// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-电显设备
#define RDB_SHM2_LOCK_ELECDEV				"RdbShm2_ElecDev"
// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-通信设备
#define RDB_SHM2_LOCK_COMMDEV				"RdbShm2_CommDev"
// 应用服务器上实时库的第二块共享内存中访问状态的互斥锁名称-是否收到全体的状态
#define RDB_SHM2_LOCK_RECALL				"RdbShm2_RecAll"


// 第二块共享内存中的设备类型
enum ENUM_RDBSHM2_DEV_TYPE
{
	ERS2DT_RECALL,					// 接收全体信息
	ERS2DT_SIGNAL,					// 遥信点
	ERS2DT_MEASURE,					// 遥测点
	ERS2DT_OPER_DEV,				// 操控设备
	ERS2DT_DISP_DEV,				// 电显设备
	ERS2DT_COMM_DEV					// 站通信设备
};

#endif /* RDBPUBDEF_H */

