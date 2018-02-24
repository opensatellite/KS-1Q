/*------------------------------------------------------------------------------
 * (c) Copyright 2010-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * Interrupt disabling/restoration for critical section protection.
 *
 * SVN $Revision: 5258 $
 * SVN $Date: 2013-03-21 12:41:02 +0000 (Thu, 21 Mar 2013) $
 */
    MODULE hal
    PUBLIC HAL_disable_interrupts
    PUBLIC HAL_restore_interrupts
    SECTION `.text`:CODE:NOROOT(2)
    THUMB

/*------------------------------------------------------------------------------
 *
 */
HAL_disable_interrupts:
    mrs r0, PRIMASK
    cpsid I
    bx lr

/*------------------------------------------------------------------------------
 *
 */
HAL_restore_interrupts:
    msr PRIMASK, r0
    bx lr

    END
