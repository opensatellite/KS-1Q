//虚拟定时器
#ifndef _VTIMER_H__
#define _VTIMER_H__

#include "datatypes.h"
#include "systick.h"

#ifdef __cplusplus
extern "C" {
#endif  

#define VTIMERS_NUM       32             //最多256个,必须是8的整数倍
#define VTIMER_NULL       255           //无效定时器号 

bool vtimer_ovf(uint8_t n);               //是否时间到 
void vtimer_service(void);              //服务进程函数,每个系统节拍中调用一次
bool vtimer_init(void);                 //初始化虚拟定时器
bool vtimer_deinit(void);
void vtimer_set(uint8_t n,uint32_t t);    //n号定时器定时长度设定为t个系统节拍
uint8_t vtimer_alloc(void);               //申请一个定时器,返回255表示没有多余定时器了
uint32_t vtimer_timeleft(uint8_t n);      //剩余多少时间
void vtimer_free(uint8_t n);              //释放一个不需要用的定时器

#ifdef __cplusplus
}
#endif

#endif
