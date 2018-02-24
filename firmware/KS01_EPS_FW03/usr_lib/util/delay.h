#ifndef _DELAY_H__
#define _DELAY_H__

#include "datatypes.h"
#include "systick.h"

#ifdef __cplusplus
extern "C" {
#endif

void delay_cycles(uint32_t x);//delay x cpu instruction cycles
void delay_ticks(uint32_t x);
void delay_nus(uint32_t x);
void delay_nms(uint32_t x);
#define delay(X) delay_nms(X)
#define delayMicroseconds(X) delay_nus(X)
#define delay_1ms() delay_nms(1)
#define delay_1us() delay_nus(1)

#ifdef __cplusplus
}
#endif

#endif
