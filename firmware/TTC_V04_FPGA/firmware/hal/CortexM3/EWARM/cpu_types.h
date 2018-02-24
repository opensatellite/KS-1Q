/*******************************************************************************
 * (c) Copyright 2007-2013 Microsemi SoC Products Group. All rights reserved.
 * 
 * Base types definitions.
 * 
 * SVN $Revision: 5258 $
 * SVN $Date: 2013-03-21 12:41:02 +0000 (Thu, 21 Mar 2013) $
 */
#ifndef __CPU_TYPES_H
#define __CPU_TYPES_H   1

/*------------------------------------------------------------------------------
 * Use the standard definitions provided with the compiler for uint32_t, int32_t
 * uint16_t, int16_t, uint8_t, int8_t...
 */
#include <stdint.h>
#include <stddef.h>

/*------------------------------------------------------------------------------
 * addr_t: address type.
 * Used to specify the address of peripherals present in the processor's memory
 * map.
 */
typedef uint32_t addr_t;

/*------------------------------------------------------------------------------
 * psr_t: processor state register.
 * Used by HAL_disable_interrupts() and HAL_restore_interrupts() to store the
 * processor's state between disabling and restoring interrupts.
 */
typedef unsigned int psr_t;

#endif  /* __CPU_TYPES_H */
