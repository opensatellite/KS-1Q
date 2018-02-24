/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"

#include <stdio.h>

#include "task.h"

/* Utility functions to implement run time stats on Cortex-M CPUs.  The collected
run time data can be viewed through the CLI interface.  See the following URL for
more information on run time stats:
http://www.freertos.org/rtos-run-time-stats.html */

/* Addresses of registers in the Cortex-M debug hardware. */
#define rtsDWT_CYCCNT 			( *( ( volatile unsigned long * ) 0xE0001004 ) )
#define rtsDWT_CONTROL 			( *( ( volatile unsigned long * ) 0xE0001000 ) )
#define rtsSCB_DEMCR 			( *( ( volatile unsigned long * ) 0xE000EDFC ) )
#define rtsTRCENA_BIT			( 0x01000000UL )
#define rtsCOUNTER_ENABLE_BIT	( 0x01UL )

/* Simple shift divide for scaling to avoid an overflow occurring too soon.  The
number of bits to shift depends on the clock speed. */
#define runtimeSLOWER_CLOCK_SPEEDS	( 70000000UL )
#define runtimeSHIFT_13				13
#define runtimeOVERFLOW_BIT_13		( 1UL << ( 32UL - runtimeSHIFT_13 ) )
#define runtimeSHIFT_14				14
#define runtimeOVERFLOW_BIT_14		( 1UL << ( 32UL - runtimeSHIFT_14 ) )

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
	/* Enable TRCENA. */
	rtsSCB_DEMCR = rtsSCB_DEMCR | rtsTRCENA_BIT;

	/* Reset counter. */
	rtsDWT_CYCCNT = 0;

	/* Enable counter. */
	rtsDWT_CONTROL = rtsDWT_CONTROL | rtsCOUNTER_ENABLE_BIT;
}
/*-----------------------------------------------------------*/

uint32_t ulGetRunTimeCounterValue( void )
{
static unsigned long ulLastCounterValue = 0UL, ulOverflows = 0;
unsigned long ulValueNow;

	ulValueNow = rtsDWT_CYCCNT;

	/* Has the value overflowed since it was last read. */
	if( ulValueNow < ulLastCounterValue )
	{
		ulOverflows++;
	}
	ulLastCounterValue = ulValueNow;

	/* Cannot use configCPU_CLOCK_HZ directly as it may itself not be a constant
	but instead map to a variable that holds the clock speed. */

	/* There is no prescale on the counter, so simulate in software. */
	if( configCPU_CLOCK_HZ < runtimeSLOWER_CLOCK_SPEEDS )
	{
		ulValueNow >>= runtimeSHIFT_13;
		ulValueNow += ( runtimeOVERFLOW_BIT_13 * ulOverflows );
	}
	else
	{
		ulValueNow >>= runtimeSHIFT_14;
		ulValueNow += ( runtimeOVERFLOW_BIT_14 * ulOverflows );
	}

	return ulValueNow;
}


/*-----------------------------------------------------------*/
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */

	#if configCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1
	{
		/* If the file system is only going to be accessed from one task then
		F_FS_THREAD_AWARE can be set to 0 and the set of example files is created
		before the RTOS scheduler is started.  If the file system is going to be
		access from more than one task then F_FS_THREAD_AWARE must be set to 1 and
		the	set of sample files are created from the idle task hook function. */
		#if F_FS_THREAD_AWARE == 1
		{
			static portBASE_TYPE xCreatedSampleFiles = pdFALSE;

			/* Initialise the drive and file system, then create a few example
			files.  The output from this function just goes to the stdout window,
			allowing the output to be viewed when the UDP command console is not
			connected. */
			if( xCreatedSampleFiles == pdFALSE )
			{
				vCreateAndVerifySampleFiles();
				xCreatedSampleFiles = pdTRUE;
			}
		}
		#endif
	}
	#endif
}

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
