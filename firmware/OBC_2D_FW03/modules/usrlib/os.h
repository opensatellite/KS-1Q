#ifndef __OS_H__
#define __OS_H__

//------------------------------------system headers----------------------------
#include <intrinsics.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "arm_math.h"

//----------------------------------common headers------------------------------
#include "config.h"
#include "datatypes.h"
//#include "delay.h"
//#include "systick.h"
//#include "vtimer.h"
#include "debug.h"
#include "dm.h"

//----------------------------------periphials----------------------------------
#include "pin.h"
#include "spi.h"
#include "uart.h"
#include "stm32f4_pwmicp.h"

//----------------------------------OS API--------------------------------------
#include "cmsis_os.h"

//----------------------------------filesystem----------------------------------
#include "ff.h"

//----------------------------------filesystem----------------------------------
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/interfaces/csp_if_can.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUMBER_OF(X)    (sizeof(X)/sizeof((X)[0]))    
    
//----------------------------------cpu feature alias---------------------------
#define sys_irq()       __enable_irq()
#define sys_noirq()     __disable_irq()
#define nop()           asm("nop")
#define reboot()        NVIC_SystemReset()
    
//----------------------------------bit operations------------------------------
#define setb(ADDR,BIT) ((ADDR)|= (1<<(BIT)))
#define clr(ADDR,BIT)  ((ADDR)&=~(1<<(BIT)))
#define get(ADDR,BIT)  ((ADDR)&  (1<<(BIT)))

//----------------------------------for convenience-----------------------------
#define max(A,B) ((A)>(B)?(A):(B))
#define min(A,B) ((A)<(B)?(A):(B))
    
//----------------------------------STM32 bit-banding,not tested----------------
#define IS_BIT_VAILD(BIT)        (((BIT)&~0x000000ff)==0)
#define IS_BITBAND_AREA(ADDR)    (((ADDR)&~0x200fffff)==0 || ((ADDR)&~0x400fffff)==0)
#define BITBAND(addr, bitnum)    ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)           *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

	
#define osEnterCritical			 portTICK_TYPE_ENTER_CRITICAL
#define osExtiCritical			 portTICK_TYPE_EXIT_CRITICAL
	
#define osEndSwithchISR    portEND_SWITCHING_ISR
	
#ifdef __cplusplus
}
#endif

#endif
