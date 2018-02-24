//通用数据类型定义 STM32版本 
#ifndef _DATATYPES_H__
#define _DATATYPES_H__

#include <stdint.h>

#ifndef __cplusplus
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#define true TRUE
#define false FALSE
#endif

#include <stdint.h>
#include <ysizet.h>

#ifdef __cplusplus
extern "C" {
#endif
 
  //    typedef unsigned char        bool        ;
  typedef float    single;
  typedef float    real32;
  typedef double   real64;
  typedef char     TCHAR ;
  
  //typedef uint32_t size_t;
  typedef int32_t ssize_t;
  
#ifdef __cplusplus
}
#endif

#endif
