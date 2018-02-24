/*******************************************************************************
 * (c) Copyright 2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Redirection of the standard library output to one of the SmartFusion2
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
#include "mss_uart.h"
#else
#include "../../drivers/mss_uart/mss_uart.h"
#endif

_STD_BEGIN

#pragma module_name = "?__write"

/*
 * The baud rate will default to 57600 baud if no baud rate is specified though the
 * MICROSEMI_STDIO_BAUD_RATE define.
 */
#ifndef MICROSEMI_STDIO_BAUD_RATE
#define MICROSEMI_STDIO_BAUD_RATE  MSS_UART_115200_BAUD
#endif

#ifdef MICROSEMI_STDIO_THRU_MMUART0
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart0;
#else
static mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;
#endif

/*==============================================================================
 * Flag used to indicate if the UART driver needs to be initialized.
 */
static int g_stdio_uart_init_done = 0;

/*==============================================================================
 * Implementation of the __write function used by the standard library.
 *
 * The "__write" function should output "size" number of bytes from "buffer" in
 * some application-specific way.  It should return the number of characters
 * written, or _LLIO_ERROR on failure.
 *
 * If "buffer" is zero then __write should perform flushing of internal buffers,
 * if any.  In this case "handle" can be -1 to indicate that all handles should
 * be flushed.
 */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
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

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }

    /* This template only writes to "standard out" and "standard err",
     * for all other file handles it returns failure. */
    if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
    {
        return _LLIO_ERROR;
    }

    MSS_UART_polled_tx(gp_my_uart, buffer, size);

    return size;
}

_STD_END

#endif  /* MICROSEMI_STDIO_THRU_UART */
