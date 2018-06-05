//==============================================================================
//                  OperatingSystemDef.h
// 
// begin    : 2018-01-26
// describe : 使用系统宏定义检测操作系统
//==============================================================================

#ifndef OPERATINGSYSTEMDEF_H
#define OPERATINGSYSTEMDEF_H

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#define OS_WIN
#endif

#if defined(__unix__) || defined(__unix)
#define OS_UNIX
#endif

#if defined(_AIX) || defined(__TOS_AIX__)
#define OS_AIX
#endif

#if defined(_hpux) || defined(hpux) || defined(__hpux)
#define OS_HP_UX
#endif

#endif /* OPERATINGSYSTEMDEF_H */

