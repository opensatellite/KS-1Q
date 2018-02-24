/*******************************************************************************
 * (c) Copyright 2010-2015 Microsemi SoC Products Group.  All rights reserved.
 * 
 * SmartFusion2 MSS HPDMA bare metal driver implementation.
 *
 * SVN $Revision: 7146 $
 * SVN $Date: 2015-01-30 15:37:25 +0530 (Fri, 30 Jan 2015) $
 */
#include "../../CMSIS/mss_assert.h"
#include "mss_hpdma.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*-------------------------------------------------------------------------*//**
 * Constants
 */
#define HPDMA_ENABLE                    1u
#define HPDMA_DISABLE                   0u
#define NULL_HANDLER                    ((mss_hpdma_handler_t)0)

/* 64 K DMA Transfer */
#define MAX_SIZE_PER_DMA_XFR            0x10000u
/* No of Descriptors*/
#define NO_OF_HPDMA_DESCRIPTORS         0x4u

/*-------------------------------------------------------------------------*//**
 * Mask Constants
 */
#define HPDMA_SOFT_RESET                0x00020000u
#define HPDMA_XFR_SIZE_MASK             0xFFFF0000u

#define HPDMACR_XFR_CMP_INT_MASK        0x00100000u
#define HPDMACR_XFR_ERR_INT_MASK        0x00200000u
#define HPDMACR_NON_WORD_INT_MASK       0x00400000u

#define HPDMACR_ALL_IRQ_ENABLE_MASK     (HPDMACR_XFR_CMP_INT_MASK | HPDMACR_XFR_ERR_INT_MASK | HPDMACR_NON_WORD_INT_MASK)

#define HPDMAICR_CLEAR_ALL_IRQ          0x000000FFu

#define HPDMAEDR_DCP_ERR_MASK           0x00000F00u

#define HPDMAEDR_NON_WORD_ERR_MASK      0x0000F000u

#define ADDRESS_NON_WORD_ALIGNED_MASK   0x00000003u

/*-------------------------------------------------------------------------*//**
  Current transfer state information.
 */
typedef struct hpdma_xfer_info
{
    volatile hpdma_status_t     state;
    mss_hpdma_handler_t         completion_handler;
    uint32_t                    src_addr;
    uint32_t                    des_addr;
    uint32_t                    xfr_size;
    uint8_t                     xfr_dir;
} hpdma_xfer_info_t;

/*-------------------------------------------------------------------------*//**
  Current transfer state information.
 */
static hpdma_xfer_info_t g_transfer;
/*-------------------------------------------------------------------------*//**
HPDMA channel end descriptor(no of descriptor used in transfer operation)
*/
static uint32_t g_hpdma_end_desc = 0u;
/*-------------------------------------------------------------------------*//**
HPDMA channel  current descriptor(transfer operation started on current descriptor)
*/
static uint32_t g_hpdma_current_desc = 0u;

/*-------------------------------------------------------------------------*//**
  Interrupt service routines function prototypes.
  The name of these interrupt service routines must match the name of the ISRs
  defined in startup_m2sxxx.s distributed as part of the SmartFusion2 CMSIS.
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void HPDMA_Complete_IRQHandler(void);
#else
void HPDMA_Complete_IRQHandler(void);
#endif 

#if defined(__GNUC__)
__attribute__((__interrupt__)) void HPDMA_Error_IRQHandler(void);
#else
void HPDMA_Error_IRQHandler(void);
#endif 

/*-------------------------------------------------------------------------*//**
 * Declare local functions:
 */
static void start_transfer(uint32_t src_addr, 
                           uint32_t dest_addr, 
                           uint32_t transfer_size, 
                           uint8_t transfer_dir); 
 
/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
void
MSS_HPDMA_init
(
    void
)
{
    uint8_t inc;
    
    /* Reset HPDMA block. */
    SYSREG->SOFT_RST_CR |= HPDMA_SOFT_RESET;

    /* Take HPDMA controller out of reset. */
    SYSREG->SOFT_RST_CR &= ~HPDMA_SOFT_RESET;

    /* Initialize data structures. */
    g_transfer.xfr_size = 0u;
    g_transfer.completion_handler = NULL_HANDLER;
    g_transfer.state = HPDMA_COMPLETED;
    
    for(inc = 0u; inc < NO_OF_HPDMA_DESCRIPTORS; inc++)
    {
        /* Pause transfer. */
        HPDMA_BITBAND->Descriptor[inc].HPDMACR_DCP_PAUSE = HPDMA_ENABLE;
        /* Disable Interrupt. */
        HPDMA->Descriptor[inc].HPDMACR_REG &= ~HPDMACR_ALL_IRQ_ENABLE_MASK;
    }     

    NVIC_DisableIRQ(HPDMA_Complete_IRQn);
    NVIC_DisableIRQ(HPDMA_Error_IRQn);
    /* Clear any previously pending MSS HPDMA interrupt. */
    NVIC_ClearPendingIRQ(HPDMA_Error_IRQn);
    NVIC_ClearPendingIRQ(HPDMA_Complete_IRQn);
    
    /* Clear all interrupts. */
    HPDMA->HPDMAICR_REG = HPDMAICR_CLEAR_ALL_IRQ;
}
/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
void
MSS_HPDMA_start
(
    uint32_t src_addr,
    uint32_t dest_addr,
    uint32_t transfer_size,
    uint8_t transfer_dir
)
{
    uint32_t hpdma_total_desc = 0u;

    ASSERT((src_addr & ADDRESS_NON_WORD_ALIGNED_MASK) == 0u); 
    ASSERT((dest_addr & ADDRESS_NON_WORD_ALIGNED_MASK) == 0u);      
    ASSERT((g_transfer.state != HPDMA_IN_PROGRESS) && (transfer_size > 0u));
    
    /* Check transfer_size is not zero and that a transfer is not in progress */   
    if((g_transfer.state != HPDMA_IN_PROGRESS) && (transfer_size > 0u))
    {        
        NVIC_EnableIRQ(HPDMA_Complete_IRQn);
        NVIC_EnableIRQ(HPDMA_Error_IRQn);
   
        g_transfer.xfr_size = transfer_size;
        g_transfer.des_addr = dest_addr;
        g_transfer.src_addr = src_addr; 
        g_transfer.xfr_dir = transfer_dir;
        /* Total no of descriptors used for transfer */
        hpdma_total_desc = ((transfer_size - 1u) / MAX_SIZE_PER_DMA_XFR) + 1u;
        /* Current HPDMA descriptor */
        g_hpdma_current_desc = 0u;         
        g_hpdma_end_desc = 0u; 
        
        /* Start transfer */
        g_transfer.state = HPDMA_IN_PROGRESS; 
        
        while((g_hpdma_end_desc < hpdma_total_desc) && (g_hpdma_end_desc < NO_OF_HPDMA_DESCRIPTORS))  
        {          
            start_transfer(g_transfer.src_addr,
                            g_transfer.des_addr,
                            g_transfer.xfr_size,
                            g_transfer.xfr_dir);
                        
            ++g_hpdma_end_desc;            
        }
   }
}
/*------------------------------------------------------------------------------

 */
static void
start_transfer
(
    uint32_t src_addr,
    uint32_t dest_addr,
    uint32_t transfer_size,
    uint8_t transfer_dir
)
{
    /* Set descriptor transfer direction */
    HPDMA_BITBAND->Descriptor[g_hpdma_end_desc].HPDMACR_DCP_XFR_DIR = transfer_dir;
    
    /* Store Source and destination Address */
    HPDMA->Descriptor[g_hpdma_end_desc].HPDMASAR_REG = src_addr;
    HPDMA->Descriptor[g_hpdma_end_desc].HPDMADAR_REG = dest_addr;
    
    /* Set the transfer size to 64K */
    HPDMA->Descriptor[g_hpdma_end_desc].HPDMACR_REG &= HPDMA_XFR_SIZE_MASK;
    
    if(transfer_size > MAX_SIZE_PER_DMA_XFR)
    {
        g_transfer.xfr_size = transfer_size - MAX_SIZE_PER_DMA_XFR;
        g_transfer.des_addr = dest_addr + MAX_SIZE_PER_DMA_XFR;
        g_transfer.src_addr = src_addr + MAX_SIZE_PER_DMA_XFR; 
    }
    else
    {
        /* Set descriptor transfer size */
        HPDMA->Descriptor[g_hpdma_end_desc].HPDMACR_REG |= (uint16_t)transfer_size;
        g_transfer.xfr_size = 0u;     
    }    
    /* Enable interrupts for the requested descriptor. */
    HPDMA->Descriptor[g_hpdma_end_desc].HPDMACR_REG |= HPDMACR_ALL_IRQ_ENABLE_MASK;       
    /* Set valid descriptor to start transfer */
    HPDMA_BITBAND->Descriptor[g_hpdma_end_desc].HPDMACR_DCP_VALID = HPDMA_ENABLE;  
    /* Start transfer */
    HPDMA_BITBAND->Descriptor[g_hpdma_end_desc].HPDMACR_DCP_PAUSE = HPDMA_DISABLE;        
}

/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
void
MSS_HPDMA_pause
(
    void
)
{
    /* Pause transfer */
    HPDMA_BITBAND->Descriptor[g_hpdma_current_desc].HPDMACR_DCP_PAUSE = HPDMA_ENABLE;
}

/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
void
MSS_HPDMA_resume
(
    void
)
{
    /* Resume transfer */
    HPDMA_BITBAND->Descriptor[g_hpdma_current_desc].HPDMACR_DCP_PAUSE = HPDMA_DISABLE;
}

/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
void
MSS_HPDMA_abort
(
    void
)
{
    uint32_t inc;
    
    for(inc = 0u; inc < NO_OF_HPDMA_DESCRIPTORS; inc++)
    {
        /* Abort transfer. */
        HPDMA_BITBAND->Descriptor[inc].HPDMACR_DCP_CLR = HPDMA_ENABLE;
    }
    g_transfer.state = HPDMA_COMPLETED;
}

/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
hpdma_status_t
MSS_HPDMA_get_transfer_status
(
    void
)
{
    return g_transfer.state;
}

/*-------------------------------------------------------------------------*//**
 * See "mss_hpdma.h" for details of how to use this function.
 */
void MSS_HPDMA_set_handler
(
    mss_hpdma_handler_t handler
)
{
    if(handler == NULL_HANDLER)
    {
        g_transfer.completion_handler = NULL_HANDLER;
    }
    else
    {
        g_transfer.completion_handler = handler;
    }
}

/*------------------------------------------------------------------------------
  HPDMA Transfer complete service routine.
  This function will be called as a result of any descriptor transfer
  is completed by HPDMA controller .
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void HPDMA_Complete_IRQHandler(void)
#else
void HPDMA_Complete_IRQHandler(void)
#endif 
{
    /* Clear interrupt */
    HPDMA_BITBAND->HPDMAICR_CLR_XFR_INT[g_hpdma_current_desc] = HPDMA_ENABLE;
    /* Disable interrupts */
    HPDMA->Descriptor[g_hpdma_current_desc].HPDMACR_REG &= ~HPDMACR_ALL_IRQ_ENABLE_MASK;
    
    ++g_hpdma_current_desc;
    
    if(NO_OF_HPDMA_DESCRIPTORS == g_hpdma_current_desc)
    {
        g_hpdma_current_desc = 0u;
    } 
    
    if(g_transfer.xfr_size > 0u)
    {
        if(NO_OF_HPDMA_DESCRIPTORS == g_hpdma_end_desc)
        {
            g_hpdma_end_desc = 0u;
        }
        else
        {
            ++g_hpdma_end_desc;
        }
        start_transfer(g_transfer.src_addr,
                        g_transfer.des_addr,
                        g_transfer.xfr_size,
                        g_transfer.xfr_dir);
    }
    else
    {
        if(((HPDMA->Descriptor[g_hpdma_current_desc].HPDMACR_REG) & HPDMACR_ALL_IRQ_ENABLE_MASK) == 0u)  
        {
            g_hpdma_current_desc = 0u;
            g_hpdma_end_desc = 0u;
        
            g_transfer.state = HPDMA_COMPLETED;
            if(g_transfer.completion_handler != NULL_HANDLER)
            {
                (*(g_transfer.completion_handler))(HPDMA_COMPLETED);
            }  
        }
    }
}

/*------------------------------------------------------------------------------
  HPDMA Transfer Error service routine.
  This function will be called as a result of any descriptor transfer
  has an error condition or it there is non word transfer size error.
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void HPDMA_Error_IRQHandler(void)
#else
void HPDMA_Error_IRQHandler(void)
#endif 
{
    uint32_t error_bits;
    uint32_t inc;

    /*
     * Handle source/destination errors.
     */
    error_bits = HPDMA->HPDMAEDR_REG;
    error_bits = error_bits & HPDMAEDR_DCP_ERR_MASK;
    
    if(error_bits != 0u)
    {
        if(HPDMA_BITBAND->Descriptor[g_hpdma_current_desc].HPDMASR_DCP_SERR != 0u)
        {
            g_transfer.state = HPDMA_SOURCE_ERROR;
            if(g_transfer.completion_handler != NULL_HANDLER)
            {                        
                (*(g_transfer.completion_handler))(HPDMA_SOURCE_ERROR);
            }
        }
        
        if(HPDMA_BITBAND->Descriptor[g_hpdma_current_desc].HPDMASR_DCP_DERR != 0u)
        {
            g_transfer.state = HPDMA_DESTINATION_ERROR;
            if(g_transfer.completion_handler != NULL_HANDLER)
            {                        
                (*(g_transfer.completion_handler))(HPDMA_DESTINATION_ERROR);
            }    
        }
    }
    
    /*
     * Handle non word aligned errors.
     */
    error_bits = HPDMA->HPDMAEDR_REG;
    error_bits = error_bits & HPDMAEDR_NON_WORD_ERR_MASK;
    
    if(error_bits != 0u)
    {
        g_transfer.state = HPDMA_WORD_ALIGN_ERROR;
        /* Call handler. */
        if(g_transfer.completion_handler != NULL_HANDLER)
        {
            (*(g_transfer.completion_handler))(HPDMA_WORD_ALIGN_ERROR);
        }
    }
    
    for(inc = 0u; inc < NO_OF_HPDMA_DESCRIPTORS; inc++)
    {
        /* Abort transfer. */
        HPDMA_BITBAND->Descriptor[inc].HPDMACR_DCP_CLR = HPDMA_ENABLE;
        /* Clear interrupt. */
        HPDMA_BITBAND->HPDMAICR_CLR_XFR_INT[inc] = HPDMA_ENABLE;
    }
}
#ifdef __cplusplus
}
#endif
