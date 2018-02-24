#ifndef _SYSTICK_H__
#define _SYSTICK_H__

#include "datatypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define SYSTICK_FREQ    1000    
//systick is in 1kHz, cannot change
    
void systick_init();
void systick_deinit();
void systick_attach(void (*job)(void));
void systick_detach(void (*job)(void));

//time since power on
uint32_t micros();
uint32_t millis();
uint64_t seconds();
uint32_t ticks();

uint16_t ticks_per_us();

#ifdef __cplusplus
}
#endif

#endif