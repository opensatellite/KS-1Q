/*******************************************************************************
 * (c) Copyright 2014 Microsemi SoC Products Group. All rights reserved.
 *
 *  IAR Embedded Workbench specific system initialization.
 *
 * SVN $Revision: 7375 $
 * SVN $Date: 2015-05-01 14:57:40 +0100 (Fri, 01 May 2015) $
 */
#ifdef MSCC_NO_RELATIVE_PATHS
#include "m2sxxx.h"
#else
#include "..\m2sxxx.h"
#endif


#pragma section = ".intvec"
#pragma section = ".intvec_RAM"
#pragma section = "HEAP"
#pragma section = ".data"

#define DISABLE_NON_BUFFERABLE_REGION       0u
#define ENABLE_WRITE_COMBINING              0xFFu

#define ESRAM0_EDAC_EN_MASK                 0x00000001u
#define ESRAM1_EDAC_EN_MASK                 0x00000002u

#define MDDR_MODE_MASK                      0x0000001Cu
#define MDDR_SECDED_MODE                    0x00000014u

#define MDDRC_CLEAR_ECC_INTERRUPT           0u
#define SYSREG_MDDR_ECC_INT_MASK            0x00000020u

#define MDDR_BASE_ADDRESS                   0xA0000000U

/*==============================================================================
 * The __low_level_init() function is called after SystemInit. Therefore, the
 * external RAM should be configured at this stage if it is used.
 */
int __low_level_init(void)
{
    uint32_t edac_enabled;
    uint32_t mddr_mode;
    uint32_t readonly_region_base;
    uint32_t data_region;
    
    /*--------------------------------------------------------------------------
     * Set the Cortex-M3 Vector Table Offset Register to point to the vector
     * table.
     */
#ifdef MSCC_RELOCATE_CODE_TO_EXT_RAM
    readonly_region_base = (uint32_t)__section_begin(".intvec_RAM");
    SCB->VTOR = readonly_region_base;
#else
    readonly_region_base = (uint32_t)__section_begin(".intvec");
    SCB->VTOR = readonly_region_base;
#endif
    
    /*--------------------------------------------------------------------------
     * Remap MDDR to address 0x00000000.
     */
    data_region = (uint32_t)__section_begin(".data");
    if(data_region > MDDR_BASE_ADDRESS)
    {
        SYSREG->ESRAM_CR = 0u;
        SYSREG->ENVM_REMAP_BASE_CR = 0u;
        SYSREG->DDR_CR = 1u;
    }
    
    /*--------------------------------------------------------------------------
     * Modify MDDR configuration if ECC/SECDED is enabled fo MDDR.
     * Enable write combining on MDDR bridge, disable non-bufferable regions.
     */
    SYSREG->DDRB_NB_SIZE_CR = DISABLE_NON_BUFFERABLE_REGION;
    SYSREG->DDRB_CR = ENABLE_WRITE_COMBINING;
    
    /*--------------------------------------------------------------------------
     * Initialize heap RAM content to initialize the error detection and
     * correction. This takes place if either of the eSRAM EDAC or MDDR ECC is
     * enabled as we do know in which physical memory the heap is located in.
     */
    edac_enabled = SYSREG->EDAC_CR & (ESRAM0_EDAC_EN_MASK | ESRAM1_EDAC_EN_MASK);
    mddr_mode = MDDR->core.ddrc.MODE_CR & MDDR_MODE_MASK;
    if((edac_enabled != 0) || (MDDR_SECDED_MODE == mddr_mode))
    {
        uint32_t * heap_start;
        uint32_t * heap_end;
        
        heap_start = (uint32_t *)__section_begin("HEAP");
        heap_end = (uint32_t *)__section_end("HEAP");
        
        while(heap_start < heap_end)
        {
            *heap_start++ = 0xCDCDCDCDu;
        }
    }
    
    /*--------------------------------------------------------------------------
     * Return 1 to indicate that the .code section can be copied.
     */
    return 1;
}

/*==============================================================================
 *
 */
void __iar_argc_argv(void)
{
    /*--------------------------------------------------------------------------
     * Clear any EDAC/ECC/SECDED errors that may have occured during section
     * copy.
     */
    MDDR->core.ddrc.ECC_INT_CLR_REG = MDDRC_CLEAR_ECC_INTERRUPT;
    SYSREG->MSS_INTERNAL_SR |= SYSREG_MDDR_ECC_INT_MASK;
}
