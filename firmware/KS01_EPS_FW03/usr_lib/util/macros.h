#ifndef __MACROS_H__
#define __MACROS_H__

#ifdef __cplusplus
extern "C" {
#endif

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
#define NUMBER_OF(X)	(sizeof(X)/sizeof((X)[0]))
    
//----------------------------------STM32 bit-banding,not tested----------------
#define IS_BIT_VAILD(BIT)        (((BIT)&~0x000000ff)==0)
#define IS_BITBAND_AREA(ADDR)    (((ADDR)&~0x200fffff)==0 || ((ADDR)&~0x400fffff)==0)
#define BITBAND(addr, bitnum)    ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)           *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

#ifdef __cplusplus
}
#endif

#endif
