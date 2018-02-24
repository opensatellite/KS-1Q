/*******************************************************************************
 * (c) Copyright 2014 Microsemi SoC Products Group. All rights reserved.
 *
 *  SmartFusion2 vector table for IAR Embedded Workbench.
 *
 * SVN $Revision: 6270 $
 * SVN $Date: 2014-03-28 12:41:48 +0000 (Fri, 28 Mar 2014) $
 */
#pragma language=extended
#pragma segment="CSTACK"

void DummyFlash_Handler(void);

extern void Reset_Handler(void);

/*------------------------------------------------------------------------------
 * Exception handlers.
 */
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void WdogWakeup_IRQHandler(void);
extern void RTC_Wakeup_IRQHandler(void);
extern void SPI0_IRQHandler(void);
extern void SPI1_IRQHandler(void);
extern void I2C0_IRQHandler(void);
extern void I2C0_SMBAlert_IRQHandler(void);
extern void I2C0_SMBus_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void I2C1_SMBAlert_IRQHandler(void);
extern void I2C1_SMBus_IRQHandler(void);
extern void UART0_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void EthernetMAC_IRQHandler(void);
extern void DMA_IRQHandler(void);
extern void Timer1_IRQHandler(void);
extern void Timer2_IRQHandler(void);
extern void CAN_IRQHandler(void);
extern void ENVM0_IRQHandler(void);
extern void ENVM1_IRQHandler(void);
extern void ComBlk_IRQHandler(void);
extern void USB_IRQHandler(void);
extern void USB_DMA_IRQHandler(void);
extern void PLL_Lock_IRQHandler(void);
extern void PLL_LockLost_IRQHandler(void);
extern void CommSwitchError_IRQHandler(void);
extern void CacheError_IRQHandler(void);
extern void DDR_IRQHandler(void);
extern void HPDMA_Complete_IRQHandler(void);
extern void HPDMA_Error_IRQHandler(void);
extern void ECC_Error_IRQHandler(void);
extern void MDDR_IOCalib_IRQHandler(void);
extern void FAB_PLL_Lock_IRQHandler(void);
extern void FAB_PLL_LockLost_IRQHandler(void);
extern void FIC64_IRQHandler(void);
extern void FabricIrq0_IRQHandler(void);
extern void FabricIrq1_IRQHandler(void);
extern void FabricIrq2_IRQHandler(void);
extern void FabricIrq3_IRQHandler(void);
extern void FabricIrq4_IRQHandler(void);
extern void FabricIrq5_IRQHandler(void);
extern void FabricIrq6_IRQHandler(void);
extern void FabricIrq7_IRQHandler(void);
extern void FabricIrq8_IRQHandler(void);
extern void FabricIrq9_IRQHandler(void);
extern void FabricIrq10_IRQHandler(void);
extern void FabricIrq11_IRQHandler(void);
extern void FabricIrq12_IRQHandler(void);
extern void FabricIrq13_IRQHandler(void);
extern void FabricIrq14_IRQHandler(void);
extern void FabricIrq15_IRQHandler(void);
extern void GPIO0_IRQHandler(void);
extern void GPIO1_IRQHandler(void);
extern void GPIO2_IRQHandler(void);
extern void GPIO3_IRQHandler(void);
extern void GPIO4_IRQHandler(void);
extern void GPIO5_IRQHandler(void);
extern void GPIO6_IRQHandler(void);
extern void GPIO7_IRQHandler(void);
extern void GPIO8_IRQHandler(void);
extern void GPIO9_IRQHandler(void);
extern void GPIO10_IRQHandler(void);
extern void GPIO11_IRQHandler(void);
extern void GPIO12_IRQHandler(void);
extern void GPIO13_IRQHandler(void);
extern void GPIO14_IRQHandler(void);
extern void GPIO15_IRQHandler(void);
extern void GPIO16_IRQHandler(void);
extern void GPIO17_IRQHandler(void);
extern void GPIO18_IRQHandler(void);
extern void GPIO19_IRQHandler(void);
extern void GPIO20_IRQHandler(void);
extern void GPIO21_IRQHandler(void);
extern void GPIO22_IRQHandler(void);
extern void GPIO23_IRQHandler(void);
extern void GPIO24_IRQHandler(void);
extern void GPIO25_IRQHandler(void);
extern void GPIO26_IRQHandler(void);
extern void GPIO27_IRQHandler(void);
extern void GPIO28_IRQHandler(void);
extern void GPIO29_IRQHandler(void);
extern void GPIO30_IRQHandler(void);
extern void GPIO31_IRQHandler(void);


/*------------------------------------------------------------------------------
 * Typedefs used to populate the vector table.
 */
typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;


#if !defined(MSCC_RELOCATE_CODE_TO_EXT_RAM)
/*==============================================================================
 * "Normal" vector table.
 * Used for executing directly out of eSRAM through the debugger or out of eNVM
 * in a deployed system.
 */
/*------------------------------------------------------------------------------
 * Place the vector table in the .intvec section.
 */
#pragma location = ".intvec"

const intvec_elem __vector_table[] =
{
    { .__ptr = __sfe( "CSTACK" ) },
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    
    /*
     * Peripherals interrupt handlers:
     */
    WdogWakeup_IRQHandler,
    RTC_Wakeup_IRQHandler,
    SPI0_IRQHandler,
    SPI1_IRQHandler,
    I2C0_IRQHandler,
    I2C0_SMBAlert_IRQHandler,
    I2C0_SMBus_IRQHandler,
    I2C1_IRQHandler,
    I2C1_SMBAlert_IRQHandler,
    I2C1_SMBus_IRQHandler,
    UART0_IRQHandler,
    UART1_IRQHandler,
    EthernetMAC_IRQHandler,
    DMA_IRQHandler,
    Timer1_IRQHandler,
    Timer2_IRQHandler,
    CAN_IRQHandler,
    ENVM0_IRQHandler,
    ENVM1_IRQHandler,
    ComBlk_IRQHandler,
    USB_IRQHandler,
    USB_DMA_IRQHandler,
    PLL_Lock_IRQHandler,
    PLL_LockLost_IRQHandler,
    CommSwitchError_IRQHandler,
    CacheError_IRQHandler,
    DDR_IRQHandler,
    HPDMA_Complete_IRQHandler,
    HPDMA_Error_IRQHandler,
    ECC_Error_IRQHandler,
    MDDR_IOCalib_IRQHandler,
    FAB_PLL_Lock_IRQHandler,
    FAB_PLL_LockLost_IRQHandler,
    FIC64_IRQHandler,
    FabricIrq0_IRQHandler,
    FabricIrq1_IRQHandler,
    FabricIrq2_IRQHandler,
    FabricIrq3_IRQHandler,
    FabricIrq4_IRQHandler,
    FabricIrq5_IRQHandler,
    FabricIrq6_IRQHandler,
    FabricIrq7_IRQHandler,
    FabricIrq8_IRQHandler,
    FabricIrq9_IRQHandler,
    FabricIrq10_IRQHandler,
    FabricIrq11_IRQHandler,
    FabricIrq12_IRQHandler,
    FabricIrq13_IRQHandler,
    FabricIrq14_IRQHandler,
    FabricIrq15_IRQHandler,
    GPIO0_IRQHandler,
    GPIO1_IRQHandler,
    GPIO2_IRQHandler,
    GPIO3_IRQHandler,
    GPIO4_IRQHandler,
    GPIO5_IRQHandler,
    GPIO6_IRQHandler,
    GPIO7_IRQHandler,
    GPIO8_IRQHandler,
    GPIO9_IRQHandler,
    GPIO10_IRQHandler,
    GPIO11_IRQHandler,
    GPIO12_IRQHandler,
    GPIO13_IRQHandler,
    GPIO14_IRQHandler,
    GPIO15_IRQHandler,
    GPIO16_IRQHandler,
    GPIO17_IRQHandler,
    GPIO18_IRQHandler,
    GPIO19_IRQHandler,
    GPIO20_IRQHandler,
    GPIO21_IRQHandler,
    GPIO22_IRQHandler,
    GPIO23_IRQHandler,
    GPIO24_IRQHandler,
    GPIO25_IRQHandler,
    GPIO26_IRQHandler,
    GPIO27_IRQHandler,
    GPIO28_IRQHandler,
    GPIO29_IRQHandler,
    GPIO30_IRQHandler,
    GPIO31_IRQHandler
};

#else   /* MSCC_RELOCATE_CODE_TO_EXT_RAM */

/*==============================================================================
 * Vector table located in eNVM.
 */
/*------------------------------------------------------------------------------
 * Place the vector table in the .intvec section.
 */
#pragma location = ".intvec"

const intvec_elem __vector_table[] =
{
    { .__ptr = __sfe( "CSTACK" ) },
    Reset_Handler,
    DummyFlash_Handler,
    DummyFlash_Handler
};

void DummyFlash_Handler(void)
{
    for(;;)
    {
        ;
    }
}

/*------------------------------------------------------------------------------
 * Place the vector table that will be copied to RAM in the .intvec_RAM section.
 */
#pragma location = ".intvec_RAM"

__root const intvec_elem __vector_RAM_table[] =
{
    { .__ptr = __sfe( "CSTACK" ) },
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    
    /*
     * Peripherals interrupt handlers:
     */
    WdogWakeup_IRQHandler,
    RTC_Wakeup_IRQHandler,
    SPI0_IRQHandler,
    SPI1_IRQHandler,
    I2C0_IRQHandler,
    I2C0_SMBAlert_IRQHandler,
    I2C0_SMBus_IRQHandler,
    I2C1_IRQHandler,
    I2C1_SMBAlert_IRQHandler,
    I2C1_SMBus_IRQHandler,
    UART0_IRQHandler,
    UART1_IRQHandler,
    EthernetMAC_IRQHandler,
    DMA_IRQHandler,
    Timer1_IRQHandler,
    Timer2_IRQHandler,
    CAN_IRQHandler,
    ENVM0_IRQHandler,
    ENVM1_IRQHandler,
    ComBlk_IRQHandler,
    USB_IRQHandler,
    USB_DMA_IRQHandler,
    PLL_Lock_IRQHandler,
    PLL_LockLost_IRQHandler,
    CommSwitchError_IRQHandler,
    CacheError_IRQHandler,
    DDR_IRQHandler,
    HPDMA_Complete_IRQHandler,
    HPDMA_Error_IRQHandler,
    ECC_Error_IRQHandler,
    MDDR_IOCalib_IRQHandler,
    FAB_PLL_Lock_IRQHandler,
    FAB_PLL_LockLost_IRQHandler,
    FIC64_IRQHandler,
    FabricIrq0_IRQHandler,
    FabricIrq1_IRQHandler,
    FabricIrq2_IRQHandler,
    FabricIrq3_IRQHandler,
    FabricIrq4_IRQHandler,
    FabricIrq5_IRQHandler,
    FabricIrq6_IRQHandler,
    FabricIrq7_IRQHandler,
    FabricIrq8_IRQHandler,
    FabricIrq9_IRQHandler,
    FabricIrq10_IRQHandler,
    FabricIrq11_IRQHandler,
    FabricIrq12_IRQHandler,
    FabricIrq13_IRQHandler,
    FabricIrq14_IRQHandler,
    FabricIrq15_IRQHandler,
    GPIO0_IRQHandler,
    GPIO1_IRQHandler,
    GPIO2_IRQHandler,
    GPIO3_IRQHandler,
    GPIO4_IRQHandler,
    GPIO5_IRQHandler,
    GPIO6_IRQHandler,
    GPIO7_IRQHandler,
    GPIO8_IRQHandler,
    GPIO9_IRQHandler,
    GPIO10_IRQHandler,
    GPIO11_IRQHandler,
    GPIO12_IRQHandler,
    GPIO13_IRQHandler,
    GPIO14_IRQHandler,
    GPIO15_IRQHandler,
    GPIO16_IRQHandler,
    GPIO17_IRQHandler,
    GPIO18_IRQHandler,
    GPIO19_IRQHandler,
    GPIO20_IRQHandler,
    GPIO21_IRQHandler,
    GPIO22_IRQHandler,
    GPIO23_IRQHandler,
    GPIO24_IRQHandler,
    GPIO25_IRQHandler,
    GPIO26_IRQHandler,
    GPIO27_IRQHandler,
    GPIO28_IRQHandler,
    GPIO29_IRQHandler,
    GPIO30_IRQHandler,
    GPIO31_IRQHandler
};

#endif  /* MSCC_RELOCATE_CODE_TO_EXT_RAM */

