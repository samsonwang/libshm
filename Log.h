//==============================================================================
//                  Log.h
// 
// begin    : 2018-06-06
// describe : 
//==============================================================================

#ifndef LOG_H
#define LOG_H

#include <cstdio>
#include <cstdarg>

#define Log(format, ...) LogImpl(format, ##__VA_ARGS__)

static void LogImpl(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    vfprintf(stderr, format, ap);

    va_end(ap);
}

#endif /* LOG_H */

