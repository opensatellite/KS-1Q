//通用数据类型定义 STM32版本 
#ifndef _DATATYPES_H__
#define _DATATYPES_H__

#ifndef __cplusplus
#ifndef _STDBOOL
#define true TRUE
#define false FALSE
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
  
  #include <stdint.h>
  #include <stddef.h>
 
  typedef float                single     ;
  typedef float                real32     ;
  typedef double               real64     ; 
  typedef char                 TCHAR      ;
  
  //typedef uint32_t size_t;
  typedef int32_t ssize_t;
  
  #define CCM @".ccm" //use cc-memory
  
#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#ifndef _STDBOOL
  typedef enum {false = 0, true = !false} bool;
#endif
#endif

#endif
