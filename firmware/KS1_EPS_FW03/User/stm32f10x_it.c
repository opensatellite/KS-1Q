/**
******************************************************************************
* @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
* @author  MCD Application Team
* @version V3.5.0
* @date    08-April-2011
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and
*          peripherals interrupt service routine.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "debug.h"
#include "macros.h"
#include <stdlib.h>

void HardFault_Handler()
{
   sys_reboot();


    __asm volatile
    (//".extern hard_fault_handler_c      \n"
     "HardFault_Handler:                \n"
     "TST LR, #4                        \n"
     "ITE EQ                            \n"
     "MRSEQ R0, MSP                     \n"
     "MRSNE R0, PSP                     \n"
     "B hard_fault_handler_c            \n"
     );
}

// From Joseph Yiu, minor edits by FVH
// hard fault handler in C,
// with stack frame location as input parameter
// called from HardFault_Handler in file xxx.s
void hard_fault_handler_c (unsigned int * hardfault_args)
{
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;
 
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
 
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
 
  //vTraceStop();
  
  kprintf ("\n\n[Hard fault handler]\n");
  kprintf ("R0 = 0x%08x\n", stacked_r0);
  kprintf ("R1 = 0x%08x\n", stacked_r1);
  kprintf ("R2 = 0x%08x\n", stacked_r2);
  kprintf ("R3 = 0x%08x\n", stacked_r3);
  kprintf ("R12 = 0x%08x\n", stacked_r12);
  kprintf ("LR [R14] = 0x%08x  subroutine call return address\n", stacked_lr);
  kprintf ("PC [R15] = 0x%08x  program counter\n", stacked_pc);
  kprintf ("PSR = 0x%08x\n", stacked_psr);
  kprintf ("BFAR = 0x%08x\n", (*((volatile unsigned long *)(0xE000ED38))));
  kprintf ("CFSR = 0x%08x\n", (*((volatile unsigned long *)(0xE000ED28))));
  kprintf ("HFSR = 0x%08x\n", (*((volatile unsigned long *)(0xE000ED2C))));
  kprintf ("DFSR = 0x%08x\n", (*((volatile unsigned long *)(0xE000ED30))));
  kprintf ("AFSR = 0x%08x\n", (*((volatile unsigned long *)(0xE000ED3C))));
  kprintf ("SCB_SHCSR = 0x%08x\n", SCB->SHCSR);
 
#ifdef REBOOT_ON_ERROR
    reboot();
#else
    exit(stacked_lr);
#endif

}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
