/*******************************************************************************
 * (c) Copyright 2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Redirection of the standard library input from one of the SmartFusion2
 * MMUART.
 *
 * This file was created based on the IAR Systems __read template implementation
 * revision 38614.
 *
 * SVN $Revision: 7375 $
 * SVN $Date: 2015-05-01 14:57:40 +0100 (Fri, 01 May 2015) $
 */

/*==============================================================================
 * The content of this source file will only be compiled if either one of the
 * following two defined symbols are defined in the project settings:
 *  - MICROSEMI_STDIO_THRU_MMUART0
 *  - MICROSEMI_STDIO_THRU_MMUART1
 *
 */
#ifdef MICROSEMI_STDIO_THRU_MMUART0
#ifndef MICROSEMI_STDIO_THRU_UART
#define MICROSEMI_STDIO_THRU_UART
#endif
#endif  /* MICROSEMI_STDIO_THRU_MMUART0 */

#ifdef MICROSEMI_STDIO_THRU_MMUART1
#ifndef MICROSEMI_STDIO_THRU_UART
#define MICROSEMI_STDIO_THRU_UART
#endif
#endif  /* MICROSEMI_STDIO_THRU_MMUART1 */

/*==============================================================================
 * Actual implementation.
 */
#ifdef MICROSEMI_STDIO_THRU_UART

#include <yfuns.h>
#include <stdio.h>
#include <string.h>
#ifdef MSCC_NO_RELATIVE_PATHS
#include "mss_uart_regs.h"
#include "mss_uart.h"
#else
#include "../../drivers/mss_uart/mss_uart_regs.h"
#include "../../drivers/mss_uart/mss_uart.h"
#endif

_STD_BEGIN

#pragma module_name = "?__read"

/*
 * The baud rate will default to 57600 baud if no baud rate is specified though the
 * MICROSEMI_STDIO_BAUD_RATE define.
 */
#ifndef MICROSEMI_STDIO_BAUD_RATE
#define MICROSEMI_STDIO_BAUD_RATE  MSS_UART_115200_BAUD
#endif

/*
 * Select the SmartFusion2 MMUART that will be used.
 */
#ifdef MICROSEMI_STDIO_THRU_MMUART0
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart0;
#else
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;
#endif

/*
 * Flag used to indicate if the UART driver needs to be initialized.
 */
static int g_stdio_uart_init_done = 0;

/*==============================================================================
 * Implementation of the __read function used by the standard library.
 *
 * The "__read" function reads a number of bytes, at most "size" into the memory
 * area pointed to by "buffer".  It returns the number of bytes read, 0 at the
 * end of the file, or _LLIO_ERROR if failure occurs.
 */
size_t __read(int handle, unsigned char * buffer, size_t size)
{
    int nChars = 0;

    /*-------------------------------------------------------------------------- 
     * This template only reads from "standard in", for all other file
     * handles it returns failure. */
    if (handle != _LLIO_STDIN)
    {
        return _LLIO_ERROR;
    }

    /*--------------------------------------------------------------------------
     * Initialize the UART driver if it is the first time this function is
     * called.
     */
    if(!g_stdio_uart_init_done)
    {
        MSS_UART_init(gp_my_uart,
                      MICROSEMI_STDIO_BAUD_RATE,
                      MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY);
                      
        g_stdio_uart_init_done = 1;
    }

    /*--------------------------------------------------------------------------
     * Read the requested number of characters from the UART.
     */
    for (/* Empty */; size > 0; --size)
    {
        uint8_t rx_size;
        
        do {
            rx_size = MSS_UART_get_rx(gp_my_uart, buffer, 1);
        }while(0 == rx_size);

        ++buffer;
        ++nChars;
    }
    
    return nChars;
}

_STD_END

#endif  /* MICROSEMI_STDIO_THRU_UART */
