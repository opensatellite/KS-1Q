/*******************************************************************************
 * (c) Copyright 2010-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * @file  startup_m2sxxx.s
 * @brief SmartFusion2 startup code for IAR EWARM.
 *
 * SVN $Revision: 7683 $
 * SVN $Date: 2015-08-21 17:06:30 +0100 (Fri, 21 Aug 2015) $
 */

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        EXTERN  __iar_program_start
        EXTERN  SystemInit

        SECTION .text:CODE:ROOT(3)
        THUMB

;===============================================================================
; Reset_Handler
;
        PUBLIC Reset_Handler
Reset_Handler
        ;-----------------------------------------------------------------------
        ; Check if EDAC is enabled on the internal eSRAM. Must initialize the stack
        ; content if EDAC is enabled. 
        ;
        LDR R0, SF2_EDAC_CR
        LDR R0, [R0]
        AND R0, R0, #3
        CMP R0, #0
        BEQ call_system_init
clear_stack:
        LDR R0, CSTACK_START
        LDR R1, CSTACK_END
        LDR R2, RAM_INIT_PATTERN
        BL  fill_memory /* ; fill_memory takes r0 - r2 as arguments uses r4, r5, r6, r7, r8, r9, and does not preserve contents */
        
        ;-----------------------------------------------------------------------
        ; Call SystemInit() to perform Libero specified configuration.
        ;
call_system_init
        LDR     R0, =SystemInit
        BLX     R0
        
        ;-----------------------------------------------------------------------
        ; Call __iar_program_start() to perform the standard IAR program startup
        ; including copy of .data and .text sections.
        ;
        LDR     R0, =__iar_program_start
        BX      R0
      
/*;------------------------------------------------------------------------------
; * fill_memory.
; * @brief Fills memory with Pattern contained in r2
; * This routine uses the stmne instruction to copy 4 words at a time which is very efficient
; * The instruction can only write to word aligned memory, hence the code at the start and end of this routine
; * to handle possible unaligned bytes at start and end.
; *
; * @param param1 r0: start address
; * @param param2 r1: end address
; * @param param3 r2: FILL PATTETN
; *
; * @note note: Most efficient if memory aligned. Linker ALIGN(4) command
; * should be used as per example linker scripts on 16 byte boundery.
; * Stack is not used in this routine
; * register contents r4, r5, r6, r7, r8, r9, will are used and will be returned undefined 
; * @return none - Used Registers are not preserved
; */

fill_memory:
    /* ;push {r4, r5, r6, r7, r8, r9, lr}    We will not use stack as may be not available */
    cmp r0, r1
    beq fill_memory_exit        /* ; Exit early if source and destination the same */
/* ; copy non-aligned bytes at the start */
    and.w  r6, r0, #3           /* ; see if non-alaigned bytes at the start     */
    cmp r6, #0
    beq fill_memory_end_start   /* ; no spare bytes at start, continue  */
    mov    r5, #4
    sub.w  r4, r5, r6           /* ; now have number of non-aligned bytes in r4 */
    mov  r7, #8
    mul  r8, r7, r6             /* ; calculate number of shifts required to initalise pattern for non-aligned bytes */
    mov  r9, r2                 /* ; copy pattern */
    ror  r9, r9, r8                 /* ; Rotate right to keep pattern consistent */
fill_memory_spare_bytes_start:  /* ; From above, R0 contains source address, R1 contains destination address */
    cmp r4, #0                  /* ; no spare bytes at end- end now     */
    beq fill_memory_end_start
    strb r9, [r0]               /* ; fill byte */
    ror.w  r9, r9, r7               /* ; Rotate right by one byte for the next time, to keep pattern consistent */
    add r0, r0, #1              /* ; add one to address */
    subs r4, r4, #1             /* ; subtract one from byte count 1 */
    b fill_memory_spare_bytes_start
fill_memory_end_start:
    mov  r6, #0
    mov  r7, r1                  /* ; save end address */
    subs r1, r1, r0              /* ; Calculate number of bytes to fill */
    mov  r8,r1                   /* ; Save copy of byte count */
    asrs r1,r1, #4               /* ; Div by 16 to get number of chunks to move */
    mov  r9, r2                  /* ; copy pattern */
    mov  r4, r2                  /* ; copy pattern */
    mov  r5, r2                  /* ; copy pattern */
    cmp r1, r6                   /* ; compare to see if all chunks copied */
    beq fill_memory_spare_bytes_end
fill_memory_loop:
    it ne
    stmne r0!, {r2, r4, r5, r9}  /* ; copy pattern- note: stmne instruction must me word aligned (address in r0) */
    add.w r6, r6, #1             /* ; use Thumb2- make sure condition code reg. not updated */
    cmp r1, r6                   /* ; compare to see if all chunks copied */
    bne fill_memory_loop
fill_memory_spare_bytes_end:     /* ; copy spare bytes at the end if any */
    and.w r8, r8, #15                    /* ; get spare bytes  --check can you do an ands?  */
fill_memory_spare_end_loop:      /* ; From above, R0 contains source address, R1 contains destination address */
    cmp r8, #0                   /* ; no spare bytes at end- end now    */
    beq fill_memory_exit
    strb r2, [r0]
    ror.w  r2, r2, #8                /* ; Rotate right by one byte for the next time, to keep pattern consistent */
    add r0, r0, #1               /* ; add one to address */
    subs r8, r8, #1              /* ; subtract one from byte count 1    */
    b fill_memory_spare_end_loop
fill_memory_exit:
    bx  lr              /* ; We will not use pop as stack may be not available */
        
        
        
        

        DATA

CSTACK_START        DCD     sfb(CSTACK)
CSTACK_END          DCD     sfe(CSTACK)

SF2_EDAC_CR         DC32    0x40038038
RAM_INIT_PATTERN    DC32    0x00000000

        END
