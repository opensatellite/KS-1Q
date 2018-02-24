`timescale 1 ns/100 ps
// Version: 


module MSS_010(
       CAN_RXBUS_MGPIO3A_H2F_A,
       CAN_RXBUS_MGPIO3A_H2F_B,
       CAN_TX_EBL_MGPIO4A_H2F_A,
       CAN_TX_EBL_MGPIO4A_H2F_B,
       CAN_TXBUS_MGPIO2A_H2F_A,
       CAN_TXBUS_MGPIO2A_H2F_B,
       CLK_CONFIG_APB,
       COMMS_INT,
       CONFIG_PRESET_N,
       EDAC_ERROR,
       F_FM0_RDATA,
       F_FM0_READYOUT,
       F_FM0_RESP,
       F_HM0_ADDR,
       F_HM0_ENABLE,
       F_HM0_SEL,
       F_HM0_SIZE,
       F_HM0_TRANS1,
       F_HM0_WDATA,
       F_HM0_WRITE,
       FAB_CHRGVBUS,
       FAB_DISCHRGVBUS,
       FAB_DMPULLDOWN,
       FAB_DPPULLDOWN,
       FAB_DRVVBUS,
       FAB_IDPULLUP,
       FAB_OPMODE,
       FAB_SUSPENDM,
       FAB_TERMSEL,
       FAB_TXVALID,
       FAB_VCONTROL,
       FAB_VCONTROLLOADM,
       FAB_XCVRSEL,
       FAB_XDATAOUT,
       FACC_GLMUX_SEL,
       FIC32_0_MASTER,
       FIC32_1_MASTER,
       FPGA_RESET_N,
       GTX_CLK,
       H2F_INTERRUPT,
       H2F_NMI,
       H2FCALIB,
       I2C0_SCL_MGPIO31B_H2F_A,
       I2C0_SCL_MGPIO31B_H2F_B,
       I2C0_SDA_MGPIO30B_H2F_A,
       I2C0_SDA_MGPIO30B_H2F_B,
       I2C1_SCL_MGPIO1A_H2F_A,
       I2C1_SCL_MGPIO1A_H2F_B,
       I2C1_SDA_MGPIO0A_H2F_A,
       I2C1_SDA_MGPIO0A_H2F_B,
       MDCF,
       MDOENF,
       MDOF,
       MMUART0_CTS_MGPIO19B_H2F_A,
       MMUART0_CTS_MGPIO19B_H2F_B,
       MMUART0_DCD_MGPIO22B_H2F_A,
       MMUART0_DCD_MGPIO22B_H2F_B,
       MMUART0_DSR_MGPIO20B_H2F_A,
       MMUART0_DSR_MGPIO20B_H2F_B,
       MMUART0_DTR_MGPIO18B_H2F_A,
       MMUART0_DTR_MGPIO18B_H2F_B,
       MMUART0_RI_MGPIO21B_H2F_A,
       MMUART0_RI_MGPIO21B_H2F_B,
       MMUART0_RTS_MGPIO17B_H2F_A,
       MMUART0_RTS_MGPIO17B_H2F_B,
       MMUART0_RXD_MGPIO28B_H2F_A,
       MMUART0_RXD_MGPIO28B_H2F_B,
       MMUART0_SCK_MGPIO29B_H2F_A,
       MMUART0_SCK_MGPIO29B_H2F_B,
       MMUART0_TXD_MGPIO27B_H2F_A,
       MMUART0_TXD_MGPIO27B_H2F_B,
       MMUART1_DTR_MGPIO12B_H2F_A,
       MMUART1_RTS_MGPIO11B_H2F_A,
       MMUART1_RTS_MGPIO11B_H2F_B,
       MMUART1_RXD_MGPIO26B_H2F_A,
       MMUART1_RXD_MGPIO26B_H2F_B,
       MMUART1_SCK_MGPIO25B_H2F_A,
       MMUART1_SCK_MGPIO25B_H2F_B,
       MMUART1_TXD_MGPIO24B_H2F_A,
       MMUART1_TXD_MGPIO24B_H2F_B,
       MPLL_LOCK,
       PER2_FABRIC_PADDR,
       PER2_FABRIC_PENABLE,
       PER2_FABRIC_PSEL,
       PER2_FABRIC_PWDATA,
       PER2_FABRIC_PWRITE,
       RTC_MATCH,
       SLEEPDEEP,
       SLEEPHOLDACK,
       SLEEPING,
       SMBALERT_NO0,
       SMBALERT_NO1,
       SMBSUS_NO0,
       SMBSUS_NO1,
       SPI0_CLK_OUT,
       SPI0_SDI_MGPIO5A_H2F_A,
       SPI0_SDI_MGPIO5A_H2F_B,
       SPI0_SDO_MGPIO6A_H2F_A,
       SPI0_SDO_MGPIO6A_H2F_B,
       SPI0_SS0_MGPIO7A_H2F_A,
       SPI0_SS0_MGPIO7A_H2F_B,
       SPI0_SS1_MGPIO8A_H2F_A,
       SPI0_SS1_MGPIO8A_H2F_B,
       SPI0_SS2_MGPIO9A_H2F_A,
       SPI0_SS2_MGPIO9A_H2F_B,
       SPI0_SS3_MGPIO10A_H2F_A,
       SPI0_SS3_MGPIO10A_H2F_B,
       SPI0_SS4_MGPIO19A_H2F_A,
       SPI0_SS5_MGPIO20A_H2F_A,
       SPI0_SS6_MGPIO21A_H2F_A,
       SPI0_SS7_MGPIO22A_H2F_A,
       SPI1_CLK_OUT,
       SPI1_SDI_MGPIO11A_H2F_A,
       SPI1_SDI_MGPIO11A_H2F_B,
       SPI1_SDO_MGPIO12A_H2F_A,
       SPI1_SDO_MGPIO12A_H2F_B,
       SPI1_SS0_MGPIO13A_H2F_A,
       SPI1_SS0_MGPIO13A_H2F_B,
       SPI1_SS1_MGPIO14A_H2F_A,
       SPI1_SS1_MGPIO14A_H2F_B,
       SPI1_SS2_MGPIO15A_H2F_A,
       SPI1_SS2_MGPIO15A_H2F_B,
       SPI1_SS3_MGPIO16A_H2F_A,
       SPI1_SS3_MGPIO16A_H2F_B,
       SPI1_SS4_MGPIO17A_H2F_A,
       SPI1_SS5_MGPIO18A_H2F_A,
       SPI1_SS6_MGPIO23A_H2F_A,
       SPI1_SS7_MGPIO24A_H2F_A,
       TCGF,
       TRACECLK,
       TRACEDATA,
       TX_CLK,
       TX_ENF,
       TX_ERRF,
       TXCTL_EN_RIF,
       TXD_RIF,
       TXDF,
       TXEV,
       WDOGTIMEOUT,
       F_ARREADY_HREADYOUT1,
       F_AWREADY_HREADYOUT0,
       F_BID,
       F_BRESP_HRESP0,
       F_BVALID,
       F_RDATA_HRDATA01,
       F_RID,
       F_RLAST,
       F_RRESP_HRESP1,
       F_RVALID,
       F_WREADY,
       MDDR_FABRIC_PRDATA,
       MDDR_FABRIC_PREADY,
       MDDR_FABRIC_PSLVERR,
       CAN_RXBUS_F2H_SCP,
       CAN_TX_EBL_F2H_SCP,
       CAN_TXBUS_F2H_SCP,
       COLF,
       CRSF,
       F2_DMAREADY,
       F2H_INTERRUPT,
       F2HCALIB,
       F_DMAREADY,
       F_FM0_ADDR,
       F_FM0_ENABLE,
       F_FM0_MASTLOCK,
       F_FM0_READY,
       F_FM0_SEL,
       F_FM0_SIZE,
       F_FM0_TRANS1,
       F_FM0_WDATA,
       F_FM0_WRITE,
       F_HM0_RDATA,
       F_HM0_READY,
       F_HM0_RESP,
       FAB_AVALID,
       FAB_HOSTDISCON,
       FAB_IDDIG,
       FAB_LINESTATE,
       FAB_M3_RESET_N,
       FAB_PLL_LOCK,
       FAB_RXACTIVE,
       FAB_RXERROR,
       FAB_RXVALID,
       FAB_RXVALIDH,
       FAB_SESSEND,
       FAB_TXREADY,
       FAB_VBUSVALID,
       FAB_VSTATUS,
       FAB_XDATAIN,
       GTX_CLKPF,
       I2C0_BCLK,
       I2C0_SCL_F2H_SCP,
       I2C0_SDA_F2H_SCP,
       I2C1_BCLK,
       I2C1_SCL_F2H_SCP,
       I2C1_SDA_F2H_SCP,
       MDIF,
       MGPIO0A_F2H_GPIN,
       MGPIO10A_F2H_GPIN,
       MGPIO11A_F2H_GPIN,
       MGPIO11B_F2H_GPIN,
       MGPIO12A_F2H_GPIN,
       MGPIO13A_F2H_GPIN,
       MGPIO14A_F2H_GPIN,
       MGPIO15A_F2H_GPIN,
       MGPIO16A_F2H_GPIN,
       MGPIO17B_F2H_GPIN,
       MGPIO18B_F2H_GPIN,
       MGPIO19B_F2H_GPIN,
       MGPIO1A_F2H_GPIN,
       MGPIO20B_F2H_GPIN,
       MGPIO21B_F2H_GPIN,
       MGPIO22B_F2H_GPIN,
       MGPIO24B_F2H_GPIN,
       MGPIO25B_F2H_GPIN,
       MGPIO26B_F2H_GPIN,
       MGPIO27B_F2H_GPIN,
       MGPIO28B_F2H_GPIN,
       MGPIO29B_F2H_GPIN,
       MGPIO2A_F2H_GPIN,
       MGPIO30B_F2H_GPIN,
       MGPIO31B_F2H_GPIN,
       MGPIO3A_F2H_GPIN,
       MGPIO4A_F2H_GPIN,
       MGPIO5A_F2H_GPIN,
       MGPIO6A_F2H_GPIN,
       MGPIO7A_F2H_GPIN,
       MGPIO8A_F2H_GPIN,
       MGPIO9A_F2H_GPIN,
       MMUART0_CTS_F2H_SCP,
       MMUART0_DCD_F2H_SCP,
       MMUART0_DSR_F2H_SCP,
       MMUART0_DTR_F2H_SCP,
       MMUART0_RI_F2H_SCP,
       MMUART0_RTS_F2H_SCP,
       MMUART0_RXD_F2H_SCP,
       MMUART0_SCK_F2H_SCP,
       MMUART0_TXD_F2H_SCP,
       MMUART1_CTS_F2H_SCP,
       MMUART1_DCD_F2H_SCP,
       MMUART1_DSR_F2H_SCP,
       MMUART1_RI_F2H_SCP,
       MMUART1_RTS_F2H_SCP,
       MMUART1_RXD_F2H_SCP,
       MMUART1_SCK_F2H_SCP,
       MMUART1_TXD_F2H_SCP,
       PER2_FABRIC_PRDATA,
       PER2_FABRIC_PREADY,
       PER2_FABRIC_PSLVERR,
       RCGF,
       RX_CLKPF,
       RX_DVF,
       RX_ERRF,
       RX_EV,
       RXDF,
       SLEEPHOLDREQ,
       SMBALERT_NI0,
       SMBALERT_NI1,
       SMBSUS_NI0,
       SMBSUS_NI1,
       SPI0_CLK_IN,
       SPI0_SDI_F2H_SCP,
       SPI0_SDO_F2H_SCP,
       SPI0_SS0_F2H_SCP,
       SPI0_SS1_F2H_SCP,
       SPI0_SS2_F2H_SCP,
       SPI0_SS3_F2H_SCP,
       SPI1_CLK_IN,
       SPI1_SDI_F2H_SCP,
       SPI1_SDO_F2H_SCP,
       SPI1_SS0_F2H_SCP,
       SPI1_SS1_F2H_SCP,
       SPI1_SS2_F2H_SCP,
       SPI1_SS3_F2H_SCP,
       TX_CLKPF,
       USER_MSS_GPIO_RESET_N,
       USER_MSS_RESET_N,
       XCLK_FAB,
       CLK_BASE,
       CLK_MDDR_APB,
       F_ARADDR_HADDR1,
       F_ARBURST_HTRANS1,
       F_ARID_HSEL1,
       F_ARLEN_HBURST1,
       F_ARLOCK_HMASTLOCK1,
       F_ARSIZE_HSIZE1,
       F_ARVALID_HWRITE1,
       F_AWADDR_HADDR0,
       F_AWBURST_HTRANS0,
       F_AWID_HSEL0,
       F_AWLEN_HBURST0,
       F_AWLOCK_HMASTLOCK0,
       F_AWSIZE_HSIZE0,
       F_AWVALID_HWRITE0,
       F_BREADY,
       F_RMW_AXI,
       F_RREADY,
       F_WDATA_HWDATA01,
       F_WID_HREADY01,
       F_WLAST,
       F_WSTRB,
       F_WVALID,
       FPGA_MDDR_ARESET_N,
       MDDR_FABRIC_PADDR,
       MDDR_FABRIC_PENABLE,
       MDDR_FABRIC_PSEL,
       MDDR_FABRIC_PWDATA,
       MDDR_FABRIC_PWRITE,
       PRESET_N,
       CAN_RXBUS_USBA_DATA1_MGPIO3A_IN,
       CAN_TX_EBL_USBA_DATA2_MGPIO4A_IN,
       CAN_TXBUS_USBA_DATA0_MGPIO2A_IN,
       DM_IN,
       DRAM_DQ_IN,
       DRAM_DQS_IN,
       DRAM_FIFO_WE_IN,
       I2C0_SCL_USBC_DATA1_MGPIO31B_IN,
       I2C0_SDA_USBC_DATA0_MGPIO30B_IN,
       I2C1_SCL_USBA_DATA4_MGPIO1A_IN,
       I2C1_SDA_USBA_DATA3_MGPIO0A_IN,
       MMUART0_CTS_USBC_DATA7_MGPIO19B_IN,
       MMUART0_DCD_MGPIO22B_IN,
       MMUART0_DSR_MGPIO20B_IN,
       MMUART0_DTR_USBC_DATA6_MGPIO18B_IN,
       MMUART0_RI_MGPIO21B_IN,
       MMUART0_RTS_USBC_DATA5_MGPIO17B_IN,
       MMUART0_RXD_USBC_STP_MGPIO28B_IN,
       MMUART0_SCK_USBC_NXT_MGPIO29B_IN,
       MMUART0_TXD_USBC_DIR_MGPIO27B_IN,
       MMUART1_RXD_USBC_DATA3_MGPIO26B_IN,
       MMUART1_SCK_USBC_DATA4_MGPIO25B_IN,
       MMUART1_TXD_USBC_DATA2_MGPIO24B_IN,
       RGMII_GTX_CLK_RMII_CLK_USBB_XCLK_IN,
       RGMII_MDC_RMII_MDC_IN,
       RGMII_MDIO_RMII_MDIO_USBB_DATA7_IN,
       RGMII_RX_CLK_IN,
       RGMII_RX_CTL_RMII_CRS_DV_USBB_DATA2_IN,
       RGMII_RXD0_RMII_RXD0_USBB_DATA0_IN,
       RGMII_RXD1_RMII_RXD1_USBB_DATA1_IN,
       RGMII_RXD2_RMII_RX_ER_USBB_DATA3_IN,
       RGMII_RXD3_USBB_DATA4_IN,
       RGMII_TX_CLK_IN,
       RGMII_TX_CTL_RMII_TX_EN_USBB_NXT_IN,
       RGMII_TXD0_RMII_TXD0_USBB_DIR_IN,
       RGMII_TXD1_RMII_TXD1_USBB_STP_IN,
       RGMII_TXD2_USBB_DATA5_IN,
       RGMII_TXD3_USBB_DATA6_IN,
       SPI0_SCK_USBA_XCLK_IN,
       SPI0_SDI_USBA_DIR_MGPIO5A_IN,
       SPI0_SDO_USBA_STP_MGPIO6A_IN,
       SPI0_SS0_USBA_NXT_MGPIO7A_IN,
       SPI0_SS1_USBA_DATA5_MGPIO8A_IN,
       SPI0_SS2_USBA_DATA6_MGPIO9A_IN,
       SPI0_SS3_USBA_DATA7_MGPIO10A_IN,
       SPI1_SCK_IN,
       SPI1_SDI_MGPIO11A_IN,
       SPI1_SDO_MGPIO12A_IN,
       SPI1_SS0_MGPIO13A_IN,
       SPI1_SS1_MGPIO14A_IN,
       SPI1_SS2_MGPIO15A_IN,
       SPI1_SS3_MGPIO16A_IN,
       SPI1_SS4_MGPIO17A_IN,
       SPI1_SS5_MGPIO18A_IN,
       SPI1_SS6_MGPIO23A_IN,
       SPI1_SS7_MGPIO24A_IN,
       USBC_XCLK_IN,
       CAN_RXBUS_USBA_DATA1_MGPIO3A_OUT,
       CAN_TX_EBL_USBA_DATA2_MGPIO4A_OUT,
       CAN_TXBUS_USBA_DATA0_MGPIO2A_OUT,
       DRAM_ADDR,
       DRAM_BA,
       DRAM_CASN,
       DRAM_CKE,
       DRAM_CLK,
       DRAM_CSN,
       DRAM_DM_RDQS_OUT,
       DRAM_DQ_OUT,
       DRAM_DQS_OUT,
       DRAM_FIFO_WE_OUT,
       DRAM_ODT,
       DRAM_RASN,
       DRAM_RSTN,
       DRAM_WEN,
       I2C0_SCL_USBC_DATA1_MGPIO31B_OUT,
       I2C0_SDA_USBC_DATA0_MGPIO30B_OUT,
       I2C1_SCL_USBA_DATA4_MGPIO1A_OUT,
       I2C1_SDA_USBA_DATA3_MGPIO0A_OUT,
       MMUART0_CTS_USBC_DATA7_MGPIO19B_OUT,
       MMUART0_DCD_MGPIO22B_OUT,
       MMUART0_DSR_MGPIO20B_OUT,
       MMUART0_DTR_USBC_DATA6_MGPIO18B_OUT,
       MMUART0_RI_MGPIO21B_OUT,
       MMUART0_RTS_USBC_DATA5_MGPIO17B_OUT,
       MMUART0_RXD_USBC_STP_MGPIO28B_OUT,
       MMUART0_SCK_USBC_NXT_MGPIO29B_OUT,
       MMUART0_TXD_USBC_DIR_MGPIO27B_OUT,
       MMUART1_RXD_USBC_DATA3_MGPIO26B_OUT,
       MMUART1_SCK_USBC_DATA4_MGPIO25B_OUT,
       MMUART1_TXD_USBC_DATA2_MGPIO24B_OUT,
       RGMII_GTX_CLK_RMII_CLK_USBB_XCLK_OUT,
       RGMII_MDC_RMII_MDC_OUT,
       RGMII_MDIO_RMII_MDIO_USBB_DATA7_OUT,
       RGMII_RX_CLK_OUT,
       RGMII_RX_CTL_RMII_CRS_DV_USBB_DATA2_OUT,
       RGMII_RXD0_RMII_RXD0_USBB_DATA0_OUT,
       RGMII_RXD1_RMII_RXD1_USBB_DATA1_OUT,
       RGMII_RXD2_RMII_RX_ER_USBB_DATA3_OUT,
       RGMII_RXD3_USBB_DATA4_OUT,
       RGMII_TX_CLK_OUT,
       RGMII_TX_CTL_RMII_TX_EN_USBB_NXT_OUT,
       RGMII_TXD0_RMII_TXD0_USBB_DIR_OUT,
       RGMII_TXD1_RMII_TXD1_USBB_STP_OUT,
       RGMII_TXD2_USBB_DATA5_OUT,
       RGMII_TXD3_USBB_DATA6_OUT,
       SPI0_SCK_USBA_XCLK_OUT,
       SPI0_SDI_USBA_DIR_MGPIO5A_OUT,
       SPI0_SDO_USBA_STP_MGPIO6A_OUT,
       SPI0_SS0_USBA_NXT_MGPIO7A_OUT,
       SPI0_SS1_USBA_DATA5_MGPIO8A_OUT,
       SPI0_SS2_USBA_DATA6_MGPIO9A_OUT,
       SPI0_SS3_USBA_DATA7_MGPIO10A_OUT,
       SPI1_SCK_OUT,
       SPI1_SDI_MGPIO11A_OUT,
       SPI1_SDO_MGPIO12A_OUT,
       SPI1_SS0_MGPIO13A_OUT,
       SPI1_SS1_MGPIO14A_OUT,
       SPI1_SS2_MGPIO15A_OUT,
       SPI1_SS3_MGPIO16A_OUT,
       SPI1_SS4_MGPIO17A_OUT,
       SPI1_SS5_MGPIO18A_OUT,
       SPI1_SS6_MGPIO23A_OUT,
       SPI1_SS7_MGPIO24A_OUT,
       USBC_XCLK_OUT,
       CAN_RXBUS_USBA_DATA1_MGPIO3A_OE,
       CAN_TX_EBL_USBA_DATA2_MGPIO4A_OE,
       CAN_TXBUS_USBA_DATA0_MGPIO2A_OE,
       DM_OE,
       DRAM_DQ_OE,
       DRAM_DQS_OE,
       I2C0_SCL_USBC_DATA1_MGPIO31B_OE,
       I2C0_SDA_USBC_DATA0_MGPIO30B_OE,
       I2C1_SCL_USBA_DATA4_MGPIO1A_OE,
       I2C1_SDA_USBA_DATA3_MGPIO0A_OE,
       MMUART0_CTS_USBC_DATA7_MGPIO19B_OE,
       MMUART0_DCD_MGPIO22B_OE,
       MMUART0_DSR_MGPIO20B_OE,
       MMUART0_DTR_USBC_DATA6_MGPIO18B_OE,
       MMUART0_RI_MGPIO21B_OE,
       MMUART0_RTS_USBC_DATA5_MGPIO17B_OE,
       MMUART0_RXD_USBC_STP_MGPIO28B_OE,
       MMUART0_SCK_USBC_NXT_MGPIO29B_OE,
       MMUART0_TXD_USBC_DIR_MGPIO27B_OE,
       MMUART1_RXD_USBC_DATA3_MGPIO26B_OE,
       MMUART1_SCK_USBC_DATA4_MGPIO25B_OE,
       MMUART1_TXD_USBC_DATA2_MGPIO24B_OE,
       RGMII_GTX_CLK_RMII_CLK_USBB_XCLK_OE,
       RGMII_MDC_RMII_MDC_OE,
       RGMII_MDIO_RMII_MDIO_USBB_DATA7_OE,
       RGMII_RX_CLK_OE,
       RGMII_RX_CTL_RMII_CRS_DV_USBB_DATA2_OE,
       RGMII_RXD0_RMII_RXD0_USBB_DATA0_OE,
       RGMII_RXD1_RMII_RXD1_USBB_DATA1_OE,
       RGMII_RXD2_RMII_RX_ER_USBB_DATA3_OE,
       RGMII_RXD3_USBB_DATA4_OE,
       RGMII_TX_CLK_OE,
       RGMII_TX_CTL_RMII_TX_EN_USBB_NXT_OE,
       RGMII_TXD0_RMII_TXD0_USBB_DIR_OE,
       RGMII_TXD1_RMII_TXD1_USBB_STP_OE,
       RGMII_TXD2_USBB_DATA5_OE,
       RGMII_TXD3_USBB_DATA6_OE,
       SPI0_SCK_USBA_XCLK_OE,
       SPI0_SDI_USBA_DIR_MGPIO5A_OE,
       SPI0_SDO_USBA_STP_MGPIO6A_OE,
       SPI0_SS0_USBA_NXT_MGPIO7A_OE,
       SPI0_SS1_USBA_DATA5_MGPIO8A_OE,
       SPI0_SS2_USBA_DATA6_MGPIO9A_OE,
       SPI0_SS3_USBA_DATA7_MGPIO10A_OE,
       SPI1_SCK_OE,
       SPI1_SDI_MGPIO11A_OE,
       SPI1_SDO_MGPIO12A_OE,
       SPI1_SS0_MGPIO13A_OE,
       SPI1_SS1_MGPIO14A_OE,
       SPI1_SS2_MGPIO15A_OE,
       SPI1_SS3_MGPIO16A_OE,
       SPI1_SS4_MGPIO17A_OE,
       SPI1_SS5_MGPIO18A_OE,
       SPI1_SS6_MGPIO23A_OE,
       SPI1_SS7_MGPIO24A_OE,
       USBC_XCLK_OE
    )
/* synthesis black_box

pragma attribute MSS_010 ment_tsu0 CAN_RXBUS_F2H_SCP->CLK_BASE+1.860
pragma attribute MSS_010 ment_tsu1 F2HCALIB->CLK_BASE+1.179
pragma attribute MSS_010 ment_tsu2 F_FM0_ADDR[0]->CLK_BASE+1.727
pragma attribute MSS_010 ment_tsu3 F_FM0_ADDR[10]->CLK_BASE+1.658
pragma attribute MSS_010 ment_tsu4 F_FM0_ADDR[11]->CLK_BASE+1.769
pragma attribute MSS_010 ment_tsu5 F_FM0_ADDR[12]->CLK_BASE+1.779
pragma attribute MSS_010 ment_tsu6 F_FM0_ADDR[13]->CLK_BASE+2.008
pragma attribute MSS_010 ment_tsu7 F_FM0_ADDR[14]->CLK_BASE+1.918
pragma attribute MSS_010 ment_tsu8 F_FM0_ADDR[15]->CLK_BASE+1.906
pragma attribute MSS_010 ment_tsu9 F_FM0_ADDR[16]->CLK_BASE+1.884
pragma attribute MSS_010 ment_tsu10 F_FM0_ADDR[17]->CLK_BASE+1.813
pragma attribute MSS_010 ment_tsu11 F_FM0_ADDR[18]->CLK_BASE+1.881
pragma attribute MSS_010 ment_tsu12 F_FM0_ADDR[19]->CLK_BASE+1.379
pragma attribute MSS_010 ment_tsu13 F_FM0_ADDR[1]->CLK_BASE+1.612
pragma attribute MSS_010 ment_tsu14 F_FM0_ADDR[20]->CLK_BASE+1.690
pragma attribute MSS_010 ment_tsu15 F_FM0_ADDR[21]->CLK_BASE+1.340
pragma attribute MSS_010 ment_tsu16 F_FM0_ADDR[22]->CLK_BASE+1.690
pragma attribute MSS_010 ment_tsu17 F_FM0_ADDR[23]->CLK_BASE+1.383
pragma attribute MSS_010 ment_tsu18 F_FM0_ADDR[24]->CLK_BASE+1.405
pragma attribute MSS_010 ment_tsu19 F_FM0_ADDR[25]->CLK_BASE+1.472
pragma attribute MSS_010 ment_tsu20 F_FM0_ADDR[26]->CLK_BASE+1.522
pragma attribute MSS_010 ment_tsu21 F_FM0_ADDR[27]->CLK_BASE+1.886
pragma attribute MSS_010 ment_tsu22 F_FM0_ADDR[28]->CLK_BASE+1.506
pragma attribute MSS_010 ment_tsu23 F_FM0_ADDR[29]->CLK_BASE+1.205
pragma attribute MSS_010 ment_tsu24 F_FM0_ADDR[2]->CLK_BASE+1.137
pragma attribute MSS_010 ment_tsu25 F_FM0_ADDR[30]->CLK_BASE+1.391
pragma attribute MSS_010 ment_tsu26 F_FM0_ADDR[31]->CLK_BASE+1.286
pragma attribute MSS_010 ment_tsu27 F_FM0_ADDR[3]->CLK_BASE+1.768
pragma attribute MSS_010 ment_tsu28 F_FM0_ADDR[4]->CLK_BASE+2.088
pragma attribute MSS_010 ment_tsu29 F_FM0_ADDR[5]->CLK_BASE+1.656
pragma attribute MSS_010 ment_tsu30 F_FM0_ADDR[6]->CLK_BASE+1.953
pragma attribute MSS_010 ment_tsu31 F_FM0_ADDR[7]->CLK_BASE+1.844
pragma attribute MSS_010 ment_tsu32 F_FM0_ADDR[8]->CLK_BASE+1.831
pragma attribute MSS_010 ment_tsu33 F_FM0_ADDR[9]->CLK_BASE+1.596
pragma attribute MSS_010 ment_tsu34 F_FM0_ENABLE->CLK_BASE+1.977
pragma attribute MSS_010 ment_tsu35 F_FM0_SEL->CLK_BASE+2.225
pragma attribute MSS_010 ment_tsu36 F_FM0_WDATA[0]->CLK_BASE+0.936
pragma attribute MSS_010 ment_tsu37 F_FM0_WDATA[10]->CLK_BASE+0.963
pragma attribute MSS_010 ment_tsu38 F_FM0_WDATA[11]->CLK_BASE+0.890
pragma attribute MSS_010 ment_tsu39 F_FM0_WDATA[12]->CLK_BASE+0.903
pragma attribute MSS_010 ment_tsu40 F_FM0_WDATA[13]->CLK_BASE+0.898
pragma attribute MSS_010 ment_tsu41 F_FM0_WDATA[14]->CLK_BASE+0.945
pragma attribute MSS_010 ment_tsu42 F_FM0_WDATA[15]->CLK_BASE+0.931
pragma attribute MSS_010 ment_tsu43 F_FM0_WDATA[16]->CLK_BASE+0.908
pragma attribute MSS_010 ment_tsu44 F_FM0_WDATA[17]->CLK_BASE+0.912
pragma attribute MSS_010 ment_tsu45 F_FM0_WDATA[18]->CLK_BASE+0.957
pragma attribute MSS_010 ment_tsu46 F_FM0_WDATA[19]->CLK_BASE+0.935
pragma attribute MSS_010 ment_tsu47 F_FM0_WDATA[1]->CLK_BASE+0.892
pragma attribute MSS_010 ment_tsu48 F_FM0_WDATA[20]->CLK_BASE+0.912
pragma attribute MSS_010 ment_tsu49 F_FM0_WDATA[21]->CLK_BASE+0.858
pragma attribute MSS_010 ment_tsu50 F_FM0_WDATA[22]->CLK_BASE+0.856
pragma attribute MSS_010 ment_tsu51 F_FM0_WDATA[23]->CLK_BASE+0.888
pragma attribute MSS_010 ment_tsu52 F_FM0_WDATA[24]->CLK_BASE+0.898
pragma attribute MSS_010 ment_tsu53 F_FM0_WDATA[25]->CLK_BASE+0.981
pragma attribute MSS_010 ment_tsu54 F_FM0_WDATA[26]->CLK_BASE+0.921
pragma attribute MSS_010 ment_tsu55 F_FM0_WDATA[27]->CLK_BASE+0.900
pragma attribute MSS_010 ment_tsu56 F_FM0_WDATA[28]->CLK_BASE+0.961
pragma attribute MSS_010 ment_tsu57 F_FM0_WDATA[29]->CLK_BASE+0.919
pragma attribute MSS_010 ment_tsu58 F_FM0_WDATA[2]->CLK_BASE+0.863
pragma attribute MSS_010 ment_tsu59 F_FM0_WDATA[30]->CLK_BASE+0.885
pragma attribute MSS_010 ment_tsu60 F_FM0_WDATA[31]->CLK_BASE+0.931
pragma attribute MSS_010 ment_tsu61 F_FM0_WDATA[3]->CLK_BASE+0.900
pragma attribute MSS_010 ment_tsu62 F_FM0_WDATA[4]->CLK_BASE+0.859
pragma attribute MSS_010 ment_tsu63 F_FM0_WDATA[5]->CLK_BASE+0.920
pragma attribute MSS_010 ment_tsu64 F_FM0_WDATA[6]->CLK_BASE+0.885
pragma attribute MSS_010 ment_tsu65 F_FM0_WDATA[7]->CLK_BASE+0.930
pragma attribute MSS_010 ment_tsu66 F_FM0_WDATA[8]->CLK_BASE+0.925
pragma attribute MSS_010 ment_tsu67 F_FM0_WDATA[9]->CLK_BASE+0.878
pragma attribute MSS_010 ment_tsu68 F_FM0_WRITE->CLK_BASE+1.779
pragma attribute MSS_010 ment_tsu69 F_HM0_RDATA[0]->CLK_BASE+1.174
pragma attribute MSS_010 ment_tsu70 F_HM0_RDATA[10]->CLK_BASE+1.049
pragma attribute MSS_010 ment_tsu71 F_HM0_RDATA[11]->CLK_BASE+1.135
pragma attribute MSS_010 ment_tsu72 F_HM0_RDATA[12]->CLK_BASE+1.078
pragma attribute MSS_010 ment_tsu73 F_HM0_RDATA[13]->CLK_BASE+1.148
pragma attribute MSS_010 ment_tsu74 F_HM0_RDATA[14]->CLK_BASE+1.091
pragma attribute MSS_010 ment_tsu75 F_HM0_RDATA[15]->CLK_BASE+1.082
pragma attribute MSS_010 ment_tsu76 F_HM0_RDATA[16]->CLK_BASE+1.139
pragma attribute MSS_010 ment_tsu77 F_HM0_RDATA[17]->CLK_BASE+1.106
pragma attribute MSS_010 ment_tsu78 F_HM0_RDATA[18]->CLK_BASE+1.032
pragma attribute MSS_010 ment_tsu79 F_HM0_RDATA[19]->CLK_BASE+1.090
pragma attribute MSS_010 ment_tsu80 F_HM0_RDATA[1]->CLK_BASE+1.095
pragma attribute MSS_010 ment_tsu81 F_HM0_RDATA[20]->CLK_BASE+1.103
pragma attribute MSS_010 ment_tsu82 F_HM0_RDATA[21]->CLK_BASE+1.127
pragma attribute MSS_010 ment_tsu83 F_HM0_RDATA[22]->CLK_BASE+1.133
pragma attribute MSS_010 ment_tsu84 F_HM0_RDATA[23]->CLK_BASE+1.190
pragma attribute MSS_010 ment_tsu85 F_HM0_RDATA[24]->CLK_BASE+1.102
pragma attribute MSS_010 ment_tsu86 F_HM0_RDATA[25]->CLK_BASE+1.070
pragma attribute MSS_010 ment_tsu87 F_HM0_RDATA[26]->CLK_BASE+1.025
pragma attribute MSS_010 ment_tsu88 F_HM0_RDATA[27]->CLK_BASE+1.084
pragma attribute MSS_010 ment_tsu89 F_HM0_RDATA[28]->CLK_BASE+1.091
pragma attribute MSS_010 ment_tsu90 F_HM0_RDATA[29]->CLK_BASE+1.165
pragma attribute MSS_010 ment_tsu91 F_HM0_RDATA[2]->CLK_BASE+1.069
pragma attribute MSS_010 ment_tsu92 F_HM0_RDATA[30]->CLK_BASE+1.160
pragma attribute MSS_010 ment_tsu93 F_HM0_RDATA[31]->CLK_BASE+1.057
pragma attribute MSS_010 ment_tsu94 F_HM0_RDATA[3]->CLK_BASE+1.333
pragma attribute MSS_010 ment_tsu95 F_HM0_RDATA[4]->CLK_BASE+1.267
pragma attribute MSS_010 ment_tsu96 F_HM0_RDATA[5]->CLK_BASE+1.109
pragma attribute MSS_010 ment_tsu97 F_HM0_RDATA[6]->CLK_BASE+1.134
pragma attribute MSS_010 ment_tsu98 F_HM0_RDATA[7]->CLK_BASE+1.001
pragma attribute MSS_010 ment_tsu99 F_HM0_RDATA[8]->CLK_BASE+1.114
pragma attribute MSS_010 ment_tsu100 F_HM0_RDATA[9]->CLK_BASE+1.202
pragma attribute MSS_010 ment_tsu101 F_HM0_READY->CLK_BASE+2.332
pragma attribute MSS_010 ment_tsu102 F_HM0_RESP->CLK_BASE+1.733
pragma attribute MSS_010 ment_tsu103 I2C0_SDA_F2H_SCP->I2C0_SCL_F2H_SCP+0.299
pragma attribute MSS_010 ment_tsu104 I2C1_SDA_F2H_SCP->I2C1_SCL_F2H_SCP+0.313
pragma attribute MSS_010 ment_tsu105 MGPIO0A_F2H_GPIN->CLK_BASE+1.693
pragma attribute MSS_010 ment_tsu106 MGPIO10A_F2H_GPIN->CLK_BASE+1.882
pragma attribute MSS_010 ment_tsu107 MGPIO11A_F2H_GPIN->CLK_BASE+1.131
pragma attribute MSS_010 ment_tsu108 MGPIO11B_F2H_GPIN->CLK_BASE+1.188
pragma attribute MSS_010 ment_tsu109 MGPIO12A_F2H_GPIN->CLK_BASE+1.192
pragma attribute MSS_010 ment_tsu110 MGPIO13A_F2H_GPIN->CLK_BASE+1.271
pragma attribute MSS_010 ment_tsu111 MGPIO14A_F2H_GPIN->CLK_BASE+1.187
pragma attribute MSS_010 ment_tsu112 MGPIO15A_F2H_GPIN->CLK_BASE+1.222
pragma attribute MSS_010 ment_tsu113 MGPIO16A_F2H_GPIN->CLK_BASE+1.166
pragma attribute MSS_010 ment_tsu114 MGPIO17B_F2H_GPIN->CLK_BASE+1.286
pragma attribute MSS_010 ment_tsu115 MGPIO18B_F2H_GPIN->CLK_BASE+1.308
pragma attribute MSS_010 ment_tsu116 MGPIO19B_F2H_GPIN->CLK_BASE+1.330
pragma attribute MSS_010 ment_tsu117 MGPIO1A_F2H_GPIN->CLK_BASE+1.243
pragma attribute MSS_010 ment_tsu118 MGPIO20B_F2H_GPIN->CLK_BASE+1.276
pragma attribute MSS_010 ment_tsu119 MGPIO21B_F2H_GPIN->CLK_BASE+1.316
pragma attribute MSS_010 ment_tsu120 MGPIO22B_F2H_GPIN->CLK_BASE+1.281
pragma attribute MSS_010 ment_tsu121 MGPIO24B_F2H_GPIN->CLK_BASE+1.188
pragma attribute MSS_010 ment_tsu122 MGPIO25B_F2H_GPIN->CLK_BASE+1.597
pragma attribute MSS_010 ment_tsu123 MGPIO26B_F2H_GPIN->CLK_BASE+1.538
pragma attribute MSS_010 ment_tsu124 MGPIO27B_F2H_GPIN->CLK_BASE+1.894
pragma attribute MSS_010 ment_tsu125 MGPIO28B_F2H_GPIN->CLK_BASE+2.127
pragma attribute MSS_010 ment_tsu126 MGPIO29B_F2H_GPIN->CLK_BASE+1.860
pragma attribute MSS_010 ment_tsu127 MGPIO2A_F2H_GPIN->CLK_BASE+1.557
pragma attribute MSS_010 ment_tsu128 MGPIO30B_F2H_GPIN->CLK_BASE+1.699
pragma attribute MSS_010 ment_tsu129 MGPIO31B_F2H_GPIN->CLK_BASE+1.708
pragma attribute MSS_010 ment_tsu130 MGPIO3A_F2H_GPIN->CLK_BASE+1.859
pragma attribute MSS_010 ment_tsu131 MGPIO4A_F2H_GPIN->CLK_BASE+1.698
pragma attribute MSS_010 ment_tsu132 MGPIO5A_F2H_GPIN->CLK_BASE+1.464
pragma attribute MSS_010 ment_tsu133 MGPIO6A_F2H_GPIN->CLK_BASE+1.514
pragma attribute MSS_010 ment_tsu134 MGPIO7A_F2H_GPIN->CLK_BASE+1.569
pragma attribute MSS_010 ment_tsu135 MGPIO8A_F2H_GPIN->CLK_BASE+1.580
pragma attribute MSS_010 ment_tsu136 MGPIO9A_F2H_GPIN->CLK_BASE+1.490
pragma attribute MSS_010 ment_tsu137 MMUART0_CTS_F2H_SCP->CLK_BASE+1.439
pragma attribute MSS_010 ment_tsu138 MMUART0_DCD_F2H_SCP->CLK_BASE+1.464
pragma attribute MSS_010 ment_tsu139 MMUART0_DSR_F2H_SCP->CLK_BASE+1.592
pragma attribute MSS_010 ment_tsu140 MMUART0_RI_F2H_SCP->CLK_BASE+1.294
pragma attribute MSS_010 ment_tsu141 MMUART0_RXD_F2H_SCP->CLK_BASE+1.351
pragma attribute MSS_010 ment_tsu142 MMUART0_SCK_F2H_SCP->CLK_BASE+1.445
pragma attribute MSS_010 ment_tsu143 MMUART0_TXD_F2H_SCP->CLK_BASE+1.401
pragma attribute MSS_010 ment_tsu144 MMUART1_CTS_F2H_SCP->CLK_BASE+1.768
pragma attribute MSS_010 ment_tsu145 MMUART1_DCD_F2H_SCP->CLK_BASE+1.890
pragma attribute MSS_010 ment_tsu146 MMUART1_DSR_F2H_SCP->CLK_BASE+1.827
pragma attribute MSS_010 ment_tsu147 MMUART1_RI_F2H_SCP->CLK_BASE+1.470
pragma attribute MSS_010 ment_tsu148 MMUART1_RXD_F2H_SCP->CLK_BASE+1.454
pragma attribute MSS_010 ment_tsu149 MMUART1_SCK_F2H_SCP->CLK_BASE+1.853
pragma attribute MSS_010 ment_tsu150 MMUART1_TXD_F2H_SCP->CLK_BASE+1.441
pragma attribute MSS_010 ment_tsu151 SMBALERT_NI0->I2C0_SCL_F2H_SCP+-0.228
pragma attribute MSS_010 ment_tsu152 SMBALERT_NI1->I2C1_SCL_F2H_SCP+0.011
pragma attribute MSS_010 ment_tsu153 SMBSUS_NI0->I2C0_SCL_F2H_SCP+-0.211
pragma attribute MSS_010 ment_tsu154 SMBSUS_NI1->I2C1_SCL_F2H_SCP+0.055
pragma attribute MSS_010 ment_tsu155 SPI0_SDI_F2H_SCP->SPI0_CLK_IN+1.444
pragma attribute MSS_010 ment_tsu156 SPI0_SS0_F2H_SCP->SPI0_CLK_IN+0.595
pragma attribute MSS_010 ment_tco0 CLK_BASE->CAN_RXBUS_MGPIO3A_H2F_A+2.590
pragma attribute MSS_010 ment_tco1 CLK_BASE->CAN_RXBUS_MGPIO3A_H2F_B+2.512
pragma attribute MSS_010 ment_tco2 CLK_BASE->CAN_TXBUS_MGPIO2A_H2F_A+3.594
pragma attribute MSS_010 ment_tco3 CLK_BASE->CAN_TXBUS_USBA_DATA0_MGPIO2A_OUT+3.595
pragma attribute MSS_010 ment_tco4 CLK_BASE->CAN_TX_EBL_MGPIO4A_H2F_A+3.437
pragma attribute MSS_010 ment_tco5 CLK_BASE->CAN_TX_EBL_USBA_DATA2_MGPIO4A_OUT+3.252
pragma attribute MSS_010 ment_tco6 CLK_BASE->F_FM0_RDATA[0]+3.180
pragma attribute MSS_010 ment_tco7 CLK_BASE->F_FM0_RDATA[10]+3.209
pragma attribute MSS_010 ment_tco8 CLK_BASE->F_FM0_RDATA[11]+3.325
pragma attribute MSS_010 ment_tco9 CLK_BASE->F_FM0_RDATA[12]+3.240
pragma attribute MSS_010 ment_tco10 CLK_BASE->F_FM0_RDATA[13]+3.202
pragma attribute MSS_010 ment_tco11 CLK_BASE->F_FM0_RDATA[14]+3.200
pragma attribute MSS_010 ment_tco12 CLK_BASE->F_FM0_RDATA[15]+3.196
pragma attribute MSS_010 ment_tco13 CLK_BASE->F_FM0_RDATA[16]+3.229
pragma attribute MSS_010 ment_tco14 CLK_BASE->F_FM0_RDATA[17]+3.428
pragma attribute MSS_010 ment_tco15 CLK_BASE->F_FM0_RDATA[18]+3.192
pragma attribute MSS_010 ment_tco16 CLK_BASE->F_FM0_RDATA[19]+3.346
pragma attribute MSS_010 ment_tco17 CLK_BASE->F_FM0_RDATA[1]+3.263
pragma attribute MSS_010 ment_tco18 CLK_BASE->F_FM0_RDATA[20]+3.095
pragma attribute MSS_010 ment_tco19 CLK_BASE->F_FM0_RDATA[21]+3.051
pragma attribute MSS_010 ment_tco20 CLK_BASE->F_FM0_RDATA[22]+3.081
pragma attribute MSS_010 ment_tco21 CLK_BASE->F_FM0_RDATA[23]+3.083
pragma attribute MSS_010 ment_tco22 CLK_BASE->F_FM0_RDATA[24]+3.103
pragma attribute MSS_010 ment_tco23 CLK_BASE->F_FM0_RDATA[25]+3.100
pragma attribute MSS_010 ment_tco24 CLK_BASE->F_FM0_RDATA[26]+3.207
pragma attribute MSS_010 ment_tco25 CLK_BASE->F_FM0_RDATA[27]+3.099
pragma attribute MSS_010 ment_tco26 CLK_BASE->F_FM0_RDATA[28]+3.088
pragma attribute MSS_010 ment_tco27 CLK_BASE->F_FM0_RDATA[29]+3.082
pragma attribute MSS_010 ment_tco28 CLK_BASE->F_FM0_RDATA[2]+3.228
pragma attribute MSS_010 ment_tco29 CLK_BASE->F_FM0_RDATA[30]+3.362
pragma attribute MSS_010 ment_tco30 CLK_BASE->F_FM0_RDATA[31]+3.224
pragma attribute MSS_010 ment_tco31 CLK_BASE->F_FM0_RDATA[3]+3.272
pragma attribute MSS_010 ment_tco32 CLK_BASE->F_FM0_RDATA[4]+3.277
pragma attribute MSS_010 ment_tco33 CLK_BASE->F_FM0_RDATA[5]+3.401
pragma attribute MSS_010 ment_tco34 CLK_BASE->F_FM0_RDATA[6]+3.262
pragma attribute MSS_010 ment_tco35 CLK_BASE->F_FM0_RDATA[7]+3.245
pragma attribute MSS_010 ment_tco36 CLK_BASE->F_FM0_RDATA[8]+3.228
pragma attribute MSS_010 ment_tco37 CLK_BASE->F_FM0_RDATA[9]+3.254
pragma attribute MSS_010 ment_tco38 CLK_BASE->F_FM0_READYOUT+2.932
pragma attribute MSS_010 ment_tco39 CLK_BASE->F_FM0_RESP+3.070
pragma attribute MSS_010 ment_tco40 CLK_BASE->F_HM0_ADDR[0]+2.980
pragma attribute MSS_010 ment_tco41 CLK_BASE->F_HM0_ADDR[10]+3.270
pragma attribute MSS_010 ment_tco42 CLK_BASE->F_HM0_ADDR[11]+2.915
pragma attribute MSS_010 ment_tco43 CLK_BASE->F_HM0_ADDR[12]+2.997
pragma attribute MSS_010 ment_tco44 CLK_BASE->F_HM0_ADDR[13]+2.967
pragma attribute MSS_010 ment_tco45 CLK_BASE->F_HM0_ADDR[14]+2.972
pragma attribute MSS_010 ment_tco46 CLK_BASE->F_HM0_ADDR[15]+2.899
pragma attribute MSS_010 ment_tco47 CLK_BASE->F_HM0_ADDR[16]+2.929
pragma attribute MSS_010 ment_tco48 CLK_BASE->F_HM0_ADDR[17]+3.017
pragma attribute MSS_010 ment_tco49 CLK_BASE->F_HM0_ADDR[18]+3.014
pragma attribute MSS_010 ment_tco50 CLK_BASE->F_HM0_ADDR[19]+3.215
pragma attribute MSS_010 ment_tco51 CLK_BASE->F_HM0_ADDR[1]+2.916
pragma attribute MSS_010 ment_tco52 CLK_BASE->F_HM0_ADDR[20]+2.949
pragma attribute MSS_010 ment_tco53 CLK_BASE->F_HM0_ADDR[21]+3.069
pragma attribute MSS_010 ment_tco54 CLK_BASE->F_HM0_ADDR[22]+3.006
pragma attribute MSS_010 ment_tco55 CLK_BASE->F_HM0_ADDR[23]+3.029
pragma attribute MSS_010 ment_tco56 CLK_BASE->F_HM0_ADDR[24]+3.051
pragma attribute MSS_010 ment_tco57 CLK_BASE->F_HM0_ADDR[25]+2.874
pragma attribute MSS_010 ment_tco58 CLK_BASE->F_HM0_ADDR[26]+3.174
pragma attribute MSS_010 ment_tco59 CLK_BASE->F_HM0_ADDR[27]+3.051
pragma attribute MSS_010 ment_tco60 CLK_BASE->F_HM0_ADDR[28]+3.420
pragma attribute MSS_010 ment_tco61 CLK_BASE->F_HM0_ADDR[29]+2.848
pragma attribute MSS_010 ment_tco62 CLK_BASE->F_HM0_ADDR[2]+2.895
pragma attribute MSS_010 ment_tco63 CLK_BASE->F_HM0_ADDR[30]+2.836
pragma attribute MSS_010 ment_tco64 CLK_BASE->F_HM0_ADDR[31]+3.212
pragma attribute MSS_010 ment_tco65 CLK_BASE->F_HM0_ADDR[3]+2.895
pragma attribute MSS_010 ment_tco66 CLK_BASE->F_HM0_ADDR[4]+2.879
pragma attribute MSS_010 ment_tco67 CLK_BASE->F_HM0_ADDR[5]+2.975
pragma attribute MSS_010 ment_tco68 CLK_BASE->F_HM0_ADDR[6]+3.067
pragma attribute MSS_010 ment_tco69 CLK_BASE->F_HM0_ADDR[7]+2.913
pragma attribute MSS_010 ment_tco70 CLK_BASE->F_HM0_ADDR[8]+3.268
pragma attribute MSS_010 ment_tco71 CLK_BASE->F_HM0_ADDR[9]+3.056
pragma attribute MSS_010 ment_tco72 CLK_BASE->F_HM0_ENABLE+3.056
pragma attribute MSS_010 ment_tco73 CLK_BASE->F_HM0_SEL+2.804
pragma attribute MSS_010 ment_tco74 CLK_BASE->F_HM0_WDATA[0]+2.966
pragma attribute MSS_010 ment_tco75 CLK_BASE->F_HM0_WDATA[10]+2.820
pragma attribute MSS_010 ment_tco76 CLK_BASE->F_HM0_WDATA[11]+3.044
pragma attribute MSS_010 ment_tco77 CLK_BASE->F_HM0_WDATA[12]+3.044
pragma attribute MSS_010 ment_tco78 CLK_BASE->F_HM0_WDATA[13]+3.102
pragma attribute MSS_010 ment_tco79 CLK_BASE->F_HM0_WDATA[14]+3.041
pragma attribute MSS_010 ment_tco80 CLK_BASE->F_HM0_WDATA[15]+3.106
pragma attribute MSS_010 ment_tco81 CLK_BASE->F_HM0_WDATA[16]+2.942
pragma attribute MSS_010 ment_tco82 CLK_BASE->F_HM0_WDATA[17]+3.198
pragma attribute MSS_010 ment_tco83 CLK_BASE->F_HM0_WDATA[18]+2.971
pragma attribute MSS_010 ment_tco84 CLK_BASE->F_HM0_WDATA[19]+3.131
pragma attribute MSS_010 ment_tco85 CLK_BASE->F_HM0_WDATA[1]+3.062
pragma attribute MSS_010 ment_tco86 CLK_BASE->F_HM0_WDATA[20]+2.991
pragma attribute MSS_010 ment_tco87 CLK_BASE->F_HM0_WDATA[21]+2.943
pragma attribute MSS_010 ment_tco88 CLK_BASE->F_HM0_WDATA[22]+2.957
pragma attribute MSS_010 ment_tco89 CLK_BASE->F_HM0_WDATA[23]+2.947
pragma attribute MSS_010 ment_tco90 CLK_BASE->F_HM0_WDATA[24]+3.103
pragma attribute MSS_010 ment_tco91 CLK_BASE->F_HM0_WDATA[25]+2.675
pragma attribute MSS_010 ment_tco92 CLK_BASE->F_HM0_WDATA[26]+3.018
pragma attribute MSS_010 ment_tco93 CLK_BASE->F_HM0_WDATA[27]+2.697
pragma attribute MSS_010 ment_tco94 CLK_BASE->F_HM0_WDATA[28]+3.072
pragma attribute MSS_010 ment_tco95 CLK_BASE->F_HM0_WDATA[29]+2.738
pragma attribute MSS_010 ment_tco96 CLK_BASE->F_HM0_WDATA[2]+3.088
pragma attribute MSS_010 ment_tco97 CLK_BASE->F_HM0_WDATA[30]+3.056
pragma attribute MSS_010 ment_tco98 CLK_BASE->F_HM0_WDATA[31]+2.679
pragma attribute MSS_010 ment_tco99 CLK_BASE->F_HM0_WDATA[3]+3.158
pragma attribute MSS_010 ment_tco100 CLK_BASE->F_HM0_WDATA[4]+2.885
pragma attribute MSS_010 ment_tco101 CLK_BASE->F_HM0_WDATA[5]+2.976
pragma attribute MSS_010 ment_tco102 CLK_BASE->F_HM0_WDATA[6]+2.986
pragma attribute MSS_010 ment_tco103 CLK_BASE->F_HM0_WDATA[7]+3.203
pragma attribute MSS_010 ment_tco104 CLK_BASE->F_HM0_WDATA[8]+2.929
pragma attribute MSS_010 ment_tco105 CLK_BASE->F_HM0_WDATA[9]+2.968
pragma attribute MSS_010 ment_tco106 CLK_BASE->F_HM0_WRITE+3.001
pragma attribute MSS_010 ment_tco107 CLK_BASE->H2FCALIB+2.957
pragma attribute MSS_010 ment_tco108 CLK_BASE->I2C0_SCL_MGPIO31B_H2F_B+2.498
pragma attribute MSS_010 ment_tco109 CLK_BASE->I2C0_SCL_USBC_DATA1_MGPIO31B_OE+3.305
pragma attribute MSS_010 ment_tco110 CLK_BASE->I2C0_SDA_MGPIO30B_H2F_A+2.494
pragma attribute MSS_010 ment_tco111 CLK_BASE->I2C0_SDA_MGPIO30B_H2F_B+2.502
pragma attribute MSS_010 ment_tco112 CLK_BASE->I2C0_SDA_USBC_DATA0_MGPIO30B_OE+3.089
pragma attribute MSS_010 ment_tco113 CLK_BASE->I2C1_SCL_MGPIO1A_H2F_B+2.658
pragma attribute MSS_010 ment_tco114 CLK_BASE->I2C1_SCL_USBA_DATA4_MGPIO1A_OE+2.793
pragma attribute MSS_010 ment_tco115 CLK_BASE->I2C1_SDA_MGPIO0A_H2F_A+2.598
pragma attribute MSS_010 ment_tco116 CLK_BASE->I2C1_SDA_MGPIO0A_H2F_B+2.748
pragma attribute MSS_010 ment_tco117 CLK_BASE->I2C1_SDA_USBA_DATA3_MGPIO0A_OE+2.523
pragma attribute MSS_010 ment_tco118 CLK_BASE->MMUART0_CTS_MGPIO19B_H2F_A+2.697
pragma attribute MSS_010 ment_tco119 CLK_BASE->MMUART0_CTS_MGPIO19B_H2F_B+2.560
pragma attribute MSS_010 ment_tco120 CLK_BASE->MMUART0_DCD_MGPIO22B_H2F_A+2.568
pragma attribute MSS_010 ment_tco121 CLK_BASE->MMUART0_DCD_MGPIO22B_H2F_B+2.529
pragma attribute MSS_010 ment_tco122 CLK_BASE->MMUART0_DSR_MGPIO20B_H2F_A+2.552
pragma attribute MSS_010 ment_tco123 CLK_BASE->MMUART0_DSR_MGPIO20B_H2F_B+2.581
pragma attribute MSS_010 ment_tco124 CLK_BASE->MMUART0_DTR_MGPIO18B_H2F_A+2.621
pragma attribute MSS_010 ment_tco125 CLK_BASE->MMUART0_DTR_MGPIO18B_H2F_B+2.570
pragma attribute MSS_010 ment_tco126 CLK_BASE->MMUART0_DTR_USBC_DATA6_MGPIO18B_OUT+2.648
pragma attribute MSS_010 ment_tco127 CLK_BASE->MMUART0_RI_MGPIO21B_H2F_A+2.634
pragma attribute MSS_010 ment_tco128 CLK_BASE->MMUART0_RI_MGPIO21B_H2F_B+2.566
pragma attribute MSS_010 ment_tco129 CLK_BASE->MMUART0_RTS_MGPIO17B_H2F_A+2.627
pragma attribute MSS_010 ment_tco130 CLK_BASE->MMUART0_RTS_MGPIO17B_H2F_B+2.641
pragma attribute MSS_010 ment_tco131 CLK_BASE->MMUART0_RTS_USBC_DATA5_MGPIO17B_OUT+2.644
pragma attribute MSS_010 ment_tco132 CLK_BASE->MMUART0_RXD_MGPIO28B_H2F_A+2.567
pragma attribute MSS_010 ment_tco133 CLK_BASE->MMUART0_RXD_MGPIO28B_H2F_B+2.461
pragma attribute MSS_010 ment_tco134 CLK_BASE->MMUART0_SCK_MGPIO29B_H2F_A+2.559
pragma attribute MSS_010 ment_tco135 CLK_BASE->MMUART0_SCK_MGPIO29B_H2F_B+2.524
pragma attribute MSS_010 ment_tco136 CLK_BASE->MMUART0_SCK_USBC_NXT_MGPIO29B_OE+3.106
pragma attribute MSS_010 ment_tco137 CLK_BASE->MMUART0_SCK_USBC_NXT_MGPIO29B_OUT+2.493
pragma attribute MSS_010 ment_tco138 CLK_BASE->MMUART0_TXD_MGPIO27B_H2F_A+3.197
pragma attribute MSS_010 ment_tco139 CLK_BASE->MMUART0_TXD_MGPIO27B_H2F_B+3.246
pragma attribute MSS_010 ment_tco140 CLK_BASE->MMUART0_TXD_USBC_DIR_MGPIO27B_OE+3.130
pragma attribute MSS_010 ment_tco141 CLK_BASE->MMUART0_TXD_USBC_DIR_MGPIO27B_OUT+2.809
pragma attribute MSS_010 ment_tco142 CLK_BASE->MMUART1_DTR_MGPIO12B_H2F_A+2.575
pragma attribute MSS_010 ment_tco143 CLK_BASE->MMUART1_RTS_MGPIO11B_H2F_A+2.565
pragma attribute MSS_010 ment_tco144 CLK_BASE->MMUART1_RTS_MGPIO11B_H2F_B+2.590
pragma attribute MSS_010 ment_tco145 CLK_BASE->MMUART1_RXD_MGPIO26B_H2F_A+2.519
pragma attribute MSS_010 ment_tco146 CLK_BASE->MMUART1_RXD_MGPIO26B_H2F_B+2.422
pragma attribute MSS_010 ment_tco147 CLK_BASE->MMUART1_SCK_MGPIO25B_H2F_A+2.656
pragma attribute MSS_010 ment_tco148 CLK_BASE->MMUART1_SCK_MGPIO25B_H2F_B+2.589
pragma attribute MSS_010 ment_tco149 CLK_BASE->MMUART1_SCK_USBC_DATA4_MGPIO25B_OE+2.858
pragma attribute MSS_010 ment_tco150 CLK_BASE->MMUART1_SCK_USBC_DATA4_MGPIO25B_OUT+2.534
pragma attribute MSS_010 ment_tco151 CLK_BASE->MMUART1_TXD_MGPIO24B_H2F_A+2.519
pragma attribute MSS_010 ment_tco152 CLK_BASE->MMUART1_TXD_MGPIO24B_H2F_B+2.539
pragma attribute MSS_010 ment_tco153 CLK_BASE->MMUART1_TXD_USBC_DATA2_MGPIO24B_OE+3.116
pragma attribute MSS_010 ment_tco154 CLK_BASE->MMUART1_TXD_USBC_DATA2_MGPIO24B_OUT+2.824
pragma attribute MSS_010 ment_tco155 CLK_BASE->RGMII_MDIO_RMII_MDIO_USBB_DATA7_OE+4.369
pragma attribute MSS_010 ment_tco156 CLK_BASE->RGMII_MDIO_RMII_MDIO_USBB_DATA7_OUT+4.123
pragma attribute MSS_010 ment_tco157 CLK_BASE->SPI0_SDI_MGPIO5A_H2F_A+2.494
pragma attribute MSS_010 ment_tco158 CLK_BASE->SPI0_SDI_MGPIO5A_H2F_B+2.561
pragma attribute MSS_010 ment_tco159 CLK_BASE->SPI0_SDO_MGPIO6A_H2F_B+4.118
pragma attribute MSS_010 ment_tco160 CLK_BASE->SPI0_SDO_USBA_STP_MGPIO6A_OE+4.406
pragma attribute MSS_010 ment_tco161 CLK_BASE->SPI0_SDO_USBA_STP_MGPIO6A_OUT+4.973
pragma attribute MSS_010 ment_tco162 CLK_BASE->SPI0_SS0_USBA_NXT_MGPIO7A_OE+2.891
pragma attribute MSS_010 ment_tco163 CLK_BASE->SPI0_SS0_USBA_NXT_MGPIO7A_OUT+3.252
pragma attribute MSS_010 ment_tco164 CLK_BASE->SPI0_SS1_MGPIO8A_H2F_A+2.575
pragma attribute MSS_010 ment_tco165 CLK_BASE->SPI0_SS1_MGPIO8A_H2F_B+2.614
pragma attribute MSS_010 ment_tco166 CLK_BASE->SPI0_SS1_USBA_DATA5_MGPIO8A_OE+2.824
pragma attribute MSS_010 ment_tco167 CLK_BASE->SPI0_SS1_USBA_DATA5_MGPIO8A_OUT+3.328
pragma attribute MSS_010 ment_tco168 CLK_BASE->SPI0_SS2_MGPIO9A_H2F_A+2.551
pragma attribute MSS_010 ment_tco169 CLK_BASE->SPI0_SS2_MGPIO9A_H2F_B+2.578
pragma attribute MSS_010 ment_tco170 CLK_BASE->SPI0_SS2_USBA_DATA6_MGPIO9A_OE+3.038
pragma attribute MSS_010 ment_tco171 CLK_BASE->SPI0_SS2_USBA_DATA6_MGPIO9A_OUT+3.297
pragma attribute MSS_010 ment_tco172 CLK_BASE->SPI0_SS3_MGPIO10A_H2F_A+2.497
pragma attribute MSS_010 ment_tco173 CLK_BASE->SPI0_SS3_MGPIO10A_H2F_B+2.424
pragma attribute MSS_010 ment_tco174 CLK_BASE->SPI0_SS3_USBA_DATA7_MGPIO10A_OE+3.285
pragma attribute MSS_010 ment_tco175 CLK_BASE->SPI0_SS3_USBA_DATA7_MGPIO10A_OUT+3.144
pragma attribute MSS_010 ment_tco176 CLK_BASE->SPI0_SS4_MGPIO19A_H2F_A+2.528
pragma attribute MSS_010 ment_tco177 CLK_BASE->SPI0_SS5_MGPIO20A_H2F_A+2.659
pragma attribute MSS_010 ment_tco178 CLK_BASE->SPI0_SS6_MGPIO21A_H2F_A+2.700
pragma attribute MSS_010 ment_tco179 CLK_BASE->SPI0_SS7_MGPIO22A_H2F_A+2.634
pragma attribute MSS_010 ment_tco180 CLK_BASE->SPI1_SDI_MGPIO11A_H2F_A+2.566
pragma attribute MSS_010 ment_tco181 CLK_BASE->SPI1_SDI_MGPIO11A_H2F_B+2.511
pragma attribute MSS_010 ment_tco182 CLK_BASE->SPI1_SDO_MGPIO12A_H2F_A+2.487
pragma attribute MSS_010 ment_tco183 CLK_BASE->SPI1_SDO_MGPIO12A_H2F_B+2.561
pragma attribute MSS_010 ment_tco184 CLK_BASE->SPI1_SDO_MGPIO12A_OE+4.919
pragma attribute MSS_010 ment_tco185 CLK_BASE->SPI1_SDO_MGPIO12A_OUT+5.574
pragma attribute MSS_010 ment_tco186 CLK_BASE->SPI1_SS0_MGPIO13A_H2F_A+2.517
pragma attribute MSS_010 ment_tco187 CLK_BASE->SPI1_SS0_MGPIO13A_H2F_B+2.493
pragma attribute MSS_010 ment_tco188 CLK_BASE->SPI1_SS0_MGPIO13A_OE+3.387
pragma attribute MSS_010 ment_tco189 CLK_BASE->SPI1_SS0_MGPIO13A_OUT+3.302
pragma attribute MSS_010 ment_tco190 CLK_BASE->SPI1_SS1_MGPIO14A_H2F_A+2.536
pragma attribute MSS_010 ment_tco191 CLK_BASE->SPI1_SS1_MGPIO14A_H2F_B+2.602
pragma attribute MSS_010 ment_tco192 CLK_BASE->SPI1_SS1_MGPIO14A_OE+3.204
pragma attribute MSS_010 ment_tco193 CLK_BASE->SPI1_SS1_MGPIO14A_OUT+3.165
pragma attribute MSS_010 ment_tco194 CLK_BASE->SPI1_SS2_MGPIO15A_H2F_A+2.660
pragma attribute MSS_010 ment_tco195 CLK_BASE->SPI1_SS2_MGPIO15A_H2F_B+2.571
pragma attribute MSS_010 ment_tco196 CLK_BASE->SPI1_SS2_MGPIO15A_OE+3.219
pragma attribute MSS_010 ment_tco197 CLK_BASE->SPI1_SS2_MGPIO15A_OUT+3.182
pragma attribute MSS_010 ment_tco198 CLK_BASE->SPI1_SS3_MGPIO16A_H2F_A+2.583
pragma attribute MSS_010 ment_tco199 CLK_BASE->SPI1_SS3_MGPIO16A_H2F_B+2.690
pragma attribute MSS_010 ment_tco200 CLK_BASE->SPI1_SS3_MGPIO16A_OE+3.137
pragma attribute MSS_010 ment_tco201 CLK_BASE->SPI1_SS3_MGPIO16A_OUT+3.271
pragma attribute MSS_010 ment_tco202 CLK_BASE->SPI1_SS4_MGPIO17A_H2F_A+2.558
pragma attribute MSS_010 ment_tco203 CLK_BASE->SPI1_SS4_MGPIO17A_OE+3.148
pragma attribute MSS_010 ment_tco204 CLK_BASE->SPI1_SS4_MGPIO17A_OUT+3.158
pragma attribute MSS_010 ment_tco205 CLK_BASE->SPI1_SS5_MGPIO18A_H2F_A+2.595
pragma attribute MSS_010 ment_tco206 CLK_BASE->SPI1_SS5_MGPIO18A_OE+3.449
pragma attribute MSS_010 ment_tco207 CLK_BASE->SPI1_SS5_MGPIO18A_OUT+3.139
pragma attribute MSS_010 ment_tco208 CLK_BASE->SPI1_SS6_MGPIO23A_H2F_A+2.566
pragma attribute MSS_010 ment_tco209 CLK_BASE->SPI1_SS6_MGPIO23A_OE+3.352
pragma attribute MSS_010 ment_tco210 CLK_BASE->SPI1_SS6_MGPIO23A_OUT+3.061
pragma attribute MSS_010 ment_tco211 CLK_BASE->SPI1_SS7_MGPIO24A_H2F_A+2.545
pragma attribute MSS_010 ment_tco212 CLK_BASE->SPI1_SS7_MGPIO24A_OE+2.764
pragma attribute MSS_010 ment_tco213 CLK_BASE->SPI1_SS7_MGPIO24A_OUT+3.030
*/
/* synthesis black_box black_box_pad ="" */
 ;
output CAN_RXBUS_MGPIO3A_H2F_A;
output CAN_RXBUS_MGPIO3A_H2F_B;
output CAN_TX_EBL_MGPIO4A_H2F_A;
output CAN_TX_EBL_MGPIO4A_H2F_B;
output CAN_TXBUS_MGPIO2A_H2F_A;
output CAN_TXBUS_MGPIO2A_H2F_B;
output CLK_CONFIG_APB;
output COMMS_INT;
output CONFIG_PRESET_N;
output [7:0] EDAC_ERROR;
output [31:0] F_FM0_RDATA;
output F_FM0_READYOUT;
output F_FM0_RESP;
output [31:0] F_HM0_ADDR;
output F_HM0_ENABLE;
output F_HM0_SEL;
output [1:0] F_HM0_SIZE;
output F_HM0_TRANS1;
output [31:0] F_HM0_WDATA;
output F_HM0_WRITE;
output FAB_CHRGVBUS;
output FAB_DISCHRGVBUS;
output FAB_DMPULLDOWN;
output FAB_DPPULLDOWN;
output FAB_DRVVBUS;
output FAB_IDPULLUP;
output [1:0] FAB_OPMODE;
output FAB_SUSPENDM;
output FAB_TERMSEL;
output FAB_TXVALID;
output [3:0] FAB_VCONTROL;
output FAB_VCONTROLLOADM;
output [1:0] FAB_XCVRSEL;
output [7:0] FAB_XDATAOUT;
output FACC_GLMUX_SEL;
output [1:0] FIC32_0_MASTER;
output [1:0] FIC32_1_MASTER;
output FPGA_RESET_N;
output GTX_CLK;
output [15:0] H2F_INTERRUPT;
output H2F_NMI;
output H2FCALIB;
output I2C0_SCL_MGPIO31B_H2F_A;
output I2C0_SCL_MGPIO31B_H2F_B;
output I2C0_SDA_MGPIO30B_H2F_A;
output I2C0_SDA_MGPIO30B_H2F_B;
output I2C1_SCL_MGPIO1A_H2F_A;
output I2C1_SCL_MGPIO1A_H2F_B;
output I2C1_SDA_MGPIO0A_H2F_A;
output I2C1_SDA_MGPIO0A_H2F_B;
output MDCF;
output MDOENF;
output MDOF;
output MMUART0_CTS_MGPIO19B_H2F_A;
output MMUART0_CTS_MGPIO19B_H2F_B;
output MMUART0_DCD_MGPIO22B_H2F_A;
output MMUART0_DCD_MGPIO22B_H2F_B;
output MMUART0_DSR_MGPIO20B_H2F_A;
output MMUART0_DSR_MGPIO20B_H2F_B;
output MMUART0_DTR_MGPIO18B_H2F_A;
output MMUART0_DTR_MGPIO18B_H2F_B;
output MMUART0_RI_MGPIO21B_H2F_A;
output MMUART0_RI_MGPIO21B_H2F_B;
output MMUART0_RTS_MGPIO17B_H2F_A;
output MMUART0_RTS_MGPIO17B_H2F_B;
output MMUART0_RXD_MGPIO28B_H2F_A;
output MMUART0_RXD_MGPIO28B_H2F_B;
output MMUART0_SCK_MGPIO29B_H2F_A;
output MMUART0_SCK_MGPIO29B_H2F_B;
output MMUART0_TXD_MGPIO27B_H2F_A;
output MMUART0_TXD_MGPIO27B_H2F_B;
output MMUART1_DTR_MGPIO12B_H2F_A;
output MMUART1_RTS_MGPIO11B_H2F_A;
output MMUART1_RTS_MGPIO11B_H2F_B;
output MMUART1_RXD_MGPIO26B_H2F_A;
output MMUART1_RXD_MGPIO26B_H2F_B;
output MMUART1_SCK_MGPIO25B_H2F_A;
output MMUART1_SCK_MGPIO25B_H2F_B;
output MMUART1_TXD_MGPIO24B_H2F_A;
output MMUART1_TXD_MGPIO24B_H2F_B;
output MPLL_LOCK;
output [15:2] PER2_FABRIC_PADDR;
output PER2_FABRIC_PENABLE;
output PER2_FABRIC_PSEL;
output [31:0] PER2_FABRIC_PWDATA;
output PER2_FABRIC_PWRITE;
output RTC_MATCH;
output SLEEPDEEP;
output SLEEPHOLDACK;
output SLEEPING;
output SMBALERT_NO0;
output SMBALERT_NO1;
output SMBSUS_NO0;
output SMBSUS_NO1;
output SPI0_CLK_OUT;
output SPI0_SDI_MGPIO5A_H2F_A;
output SPI0_SDI_MGPIO5A_H2F_B;
output SPI0_SDO_MGPIO6A_H2F_A;
output SPI0_SDO_MGPIO6A_H2F_B;
output SPI0_SS0_MGPIO7A_H2F_A;
output SPI0_SS0_MGPIO7A_H2F_B;
output SPI0_SS1_MGPIO8A_H2F_A;
output SPI0_SS1_MGPIO8A_H2F_B;
output SPI0_SS2_MGPIO9A_H2F_A;
output SPI0_SS2_MGPIO9A_H2F_B;
output SPI0_SS3_MGPIO10A_H2F_A;
output SPI0_SS3_MGPIO10A_H2F_B;
output SPI0_SS4_MGPIO19A_H2F_A;
output SPI0_SS5_MGPIO20A_H2F_A;
output SPI0_SS6_MGPIO21A_H2F_A;
output SPI0_SS7_MGPIO22A_H2F_A;
output SPI1_CLK_OUT;
output SPI1_SDI_MGPIO11A_H2F_A;
output SPI1_SDI_MGPIO11A_H2F_B;
output SPI1_SDO_MGPIO12A_H2F_A;
output SPI1_SDO_MGPIO12A_H2F_B;
output SPI1_SS0_MGPIO13A_H2F_A;
output SPI1_SS0_MGPIO13A_H2F_B;
output SPI1_SS1_MGPIO14A_H2F_A;
output SPI1_SS1_MGPIO14A_H2F_B;
output SPI1_SS2_MGPIO15A_H2F_A;
output SPI1_SS2_MGPIO15A_H2F_B;
output SPI1_SS3_MGPIO16A_H2F_A;
output SPI1_SS3_MGPIO16A_H2F_B;
output SPI1_SS4_MGPIO17A_H2F_A;
output SPI1_SS5_MGPIO18A_H2F_A;
output SPI1_SS6_MGPIO23A_H2F_A;
output SPI1_SS7_MGPIO24A_H2F_A;
output [9:0] TCGF;
output TRACECLK;
output [3:0] TRACEDATA;
output TX_CLK;
output TX_ENF;
output TX_ERRF;
output TXCTL_EN_RIF;
output [3:0] TXD_RIF;
output [7:0] TXDF;
output TXEV;
output WDOGTIMEOUT;
output F_ARREADY_HREADYOUT1;
output F_AWREADY_HREADYOUT0;
output [3:0] F_BID;
output [1:0] F_BRESP_HRESP0;
output F_BVALID;
output [63:0] F_RDATA_HRDATA01;
output [3:0] F_RID;
output F_RLAST;
output [1:0] F_RRESP_HRESP1;
output F_RVALID;
output F_WREADY;
output [15:0] MDDR_FABRIC_PRDATA;
output MDDR_FABRIC_PREADY;
output MDDR_FABRIC_PSLVERR;
input  CAN_RXBUS_F2H_SCP;
input  CAN_TX_EBL_F2H_SCP;
input  CAN_TXBUS_F2H_SCP;
input  COLF;
input  CRSF;
input  [1:0] F2_DMAREADY;
input  [15:0] F2H_INTERRUPT;
input  F2HCALIB;
input  [1:0] F_DMAREADY;
input  [31:0] F_FM0_ADDR;
input  F_FM0_ENABLE;
input  F_FM0_MASTLOCK;
input  F_FM0_READY;
input  F_FM0_SEL;
input  [1:0] F_FM0_SIZE;
input  F_FM0_TRANS1;
input  [31:0] F_FM0_WDATA;
input  F_FM0_WRITE;
input  [31:0] F_HM0_RDATA;
input  F_HM0_READY;
input  F_HM0_RESP;
input  FAB_AVALID;
input  FAB_HOSTDISCON;
input  FAB_IDDIG;
input  [1:0] FAB_LINESTATE;
input  FAB_M3_RESET_N;
input  FAB_PLL_LOCK;
input  FAB_RXACTIVE;
input  FAB_RXERROR;
input  FAB_RXVALID;
input  FAB_RXVALIDH;
input  FAB_SESSEND;
input  FAB_TXREADY;
input  FAB_VBUSVALID;
input  [7:0] FAB_VSTATUS;
input  [7:0] FAB_XDATAIN;
input  GTX_CLKPF;
input  I2C0_BCLK;
input  I2C0_SCL_F2H_SCP;
input  I2C0_SDA_F2H_SCP;
input  I2C1_BCLK;
input  I2C1_SCL_F2H_SCP;
input  I2C1_SDA_F2H_SCP;
input  MDIF;
input  MGPIO0A_F2H_GPIN;
input  MGPIO10A_F2H_GPIN;
input  MGPIO11A_F2H_GPIN;
input  MGPIO11B_F2H_GPIN;
input  MGPIO12A_F2H_GPIN;
input  MGPIO13A_F2H_GPIN;
input  MGPIO14A_F2H_GPIN;
input  MGPIO15A_F2H_GPIN;
input  MGPIO16A_F2H_GPIN;
input  MGPIO17B_F2H_GPIN;
input  MGPIO18B_F2H_GPIN;
input  MGPIO19B_F2H_GPIN;
input  MGPIO1A_F2H_GPIN;
input  MGPIO20B_F2H_GPIN;
input  MGPIO21B_F2H_GPIN;
input  MGPIO22B_F2H_GPIN;
input  MGPIO24B_F2H_GPIN;
input  MGPIO25B_F2H_GPIN;
input  MGPIO26B_F2H_GPIN;
input  MGPIO27B_F2H_GPIN;
input  MGPIO28B_F2H_GPIN;
input  MGPIO29B_F2H_GPIN;
input  MGPIO2A_F2H_GPIN;
input  MGPIO30B_F2H_GPIN;
input  MGPIO31B_F2H_GPIN;
input  MGPIO3A_F2H_GPIN;
input  MGPIO4A_F2H_GPIN;
input  MGPIO5A_F2H_GPIN;
input  MGPIO6A_F2H_GPIN;
input  MGPIO7A_F2H_GPIN;
input  MGPIO8A_F2H_GPIN;
input  MGPIO9A_F2H_GPIN;
input  MMUART0_CTS_F2H_SCP;
input  MMUART0_DCD_F2H_SCP;
input  MMUART0_DSR_F2H_SCP;
input  MMUART0_DTR_F2H_SCP;
input  MMUART0_RI_F2H_SCP;
input  MMUART0_RTS_F2H_SCP;
input  MMUART0_RXD_F2H_SCP;
input  MMUART0_SCK_F2H_SCP;
input  MMUART0_TXD_F2H_SCP;
input  MMUART1_CTS_F2H_SCP;
input  MMUART1_DCD_F2H_SCP;
input  MMUART1_DSR_F2H_SCP;
input  MMUART1_RI_F2H_SCP;
input  MMUART1_RTS_F2H_SCP;
input  MMUART1_RXD_F2H_SCP;
input  MMUART1_SCK_F2H_SCP;
input  MMUART1_TXD_F2H_SCP;
input  [31:0] PER2_FABRIC_PRDATA;
input  PER2_FABRIC_PREADY;
input  PER2_FABRIC_PSLVERR;
input  [9:0] RCGF;
input  RX_CLKPF;
input  RX_DVF;
input  RX_ERRF;
input  RX_EV;
input  [7:0] RXDF;
input  SLEEPHOLDREQ;
input  SMBALERT_NI0;
input  SMBALERT_NI1;
input  SMBSUS_NI0;
input  SMBSUS_NI1;
input  SPI0_CLK_IN;
input  SPI0_SDI_F2H_SCP;
input  SPI0_SDO_F2H_SCP;
input  SPI0_SS0_F2H_SCP;
input  SPI0_SS1_F2H_SCP;
input  SPI0_SS2_F2H_SCP;
input  SPI0_SS3_F2H_SCP;
input  SPI1_CLK_IN;
input  SPI1_SDI_F2H_SCP;
input  SPI1_SDO_F2H_SCP;
input  SPI1_SS0_F2H_SCP;
input  SPI1_SS1_F2H_SCP;
input  SPI1_SS2_F2H_SCP;
input  SPI1_SS3_F2H_SCP;
input  TX_CLKPF;
input  USER_MSS_GPIO_RESET_N;
input  USER_MSS_RESET_N;
input  XCLK_FAB;
input  CLK_BASE;
input  CLK_MDDR_APB;
input  [31:0] F_ARADDR_HADDR1;
input  [1:0] F_ARBURST_HTRANS1;
input  [3:0] F_ARID_HSEL1;
input  [3:0] F_ARLEN_HBURST1;
input  [1:0] F_ARLOCK_HMASTLOCK1;
input  [1:0] F_ARSIZE_HSIZE1;
input  F_ARVALID_HWRITE1;
input  [31:0] F_AWADDR_HADDR0;
input  [1:0] F_AWBURST_HTRANS0;
input  [3:0] F_AWID_HSEL0;
input  [3:0] F_AWLEN_HBURST0;
input  [1:0] F_AWLOCK_HMASTLOCK0;
input  [1:0] F_AWSIZE_HSIZE0;
input  F_AWVALID_HWRITE0;
input  F_BREADY;
input  F_RMW_AXI;
input  F_RREADY;
input  [63:0] F_WDATA_HWDATA01;
input  [3:0] F_WID_HREADY01;
input  F_WLAST;
input  [7:0] F_WSTRB;
input  F_WVALID;
input  FPGA_MDDR_ARESET_N;
input  [10:2] MDDR_FABRIC_PADDR;
input  MDDR_FABRIC_PENABLE;
input  MDDR_FABRIC_PSEL;
input  [15:0] MDDR_FABRIC_PWDATA;
input  MDDR_FABRIC_PWRITE;
input  PRESET_N;
input  CAN_RXBUS_USBA_DATA1_MGPIO3A_IN;
input  CAN_TX_EBL_USBA_DATA2_MGPIO4A_IN;
input  CAN_TXBUS_USBA_DATA0_MGPIO2A_IN;
input  [2:0] DM_IN;
input  [17:0] DRAM_DQ_IN;
input  [2:0] DRAM_DQS_IN;
input  [1:0] DRAM_FIFO_WE_IN;
input  I2C0_SCL_USBC_DATA1_MGPIO31B_IN;
input  I2C0_SDA_USBC_DATA0_MGPIO30B_IN;
input  I2C1_SCL_USBA_DATA4_MGPIO1A_IN;
input  I2C1_SDA_USBA_DATA3_MGPIO0A_IN;
input  MMUART0_CTS_USBC_DATA7_MGPIO19B_IN;
input  MMUART0_DCD_MGPIO22B_IN;
input  MMUART0_DSR_MGPIO20B_IN;
input  MMUART0_DTR_USBC_DATA6_MGPIO18B_IN;
input  MMUART0_RI_MGPIO21B_IN;
input  MMUART0_RTS_USBC_DATA5_MGPIO17B_IN;
input  MMUART0_RXD_USBC_STP_MGPIO28B_IN;
input  MMUART0_SCK_USBC_NXT_MGPIO29B_IN;
input  MMUART0_TXD_USBC_DIR_MGPIO27B_IN;
input  MMUART1_RXD_USBC_DATA3_MGPIO26B_IN;
input  MMUART1_SCK_USBC_DATA4_MGPIO25B_IN;
input  MMUART1_TXD_USBC_DATA2_MGPIO24B_IN;
input  RGMII_GTX_CLK_RMII_CLK_USBB_XCLK_IN;
input  RGMII_MDC_RMII_MDC_IN;
input  RGMII_MDIO_RMII_MDIO_USBB_DATA7_IN;
input  RGMII_RX_CLK_IN;
input  RGMII_RX_CTL_RMII_CRS_DV_USBB_DATA2_IN;
input  RGMII_RXD0_RMII_RXD0_USBB_DATA0_IN;
input  RGMII_RXD1_RMII_RXD1_USBB_DATA1_IN;
input  RGMII_RXD2_RMII_RX_ER_USBB_DATA3_IN;
input  RGMII_RXD3_USBB_DATA4_IN;
input  RGMII_TX_CLK_IN;
input  RGMII_TX_CTL_RMII_TX_EN_USBB_NXT_IN;
input  RGMII_TXD0_RMII_TXD0_USBB_DIR_IN;
input  RGMII_TXD1_RMII_TXD1_USBB_STP_IN;
input  RGMII_TXD2_USBB_DATA5_IN;
input  RGMII_TXD3_USBB_DATA6_IN;
input  SPI0_SCK_USBA_XCLK_IN;
input  SPI0_SDI_USBA_DIR_MGPIO5A_IN;
input  SPI0_SDO_USBA_STP_MGPIO6A_IN;
input  SPI0_SS0_USBA_NXT_MGPIO7A_IN;
input  SPI0_SS1_USBA_DATA5_MGPIO8A_IN;
input  SPI0_SS2_USBA_DATA6_MGPIO9A_IN;
input  SPI0_SS3_USBA_DATA7_MGPIO10A_IN;
input  SPI1_SCK_IN;
input  SPI1_SDI_MGPIO11A_IN;
input  SPI1_SDO_MGPIO12A_IN;
input  SPI1_SS0_MGPIO13A_IN;
input  SPI1_SS1_MGPIO14A_IN;
input  SPI1_SS2_MGPIO15A_IN;
input  SPI1_SS3_MGPIO16A_IN;
input  SPI1_SS4_MGPIO17A_IN;
input  SPI1_SS5_MGPIO18A_IN;
input  SPI1_SS6_MGPIO23A_IN;
input  SPI1_SS7_MGPIO24A_IN;
input  USBC_XCLK_IN;
output CAN_RXBUS_USBA_DATA1_MGPIO3A_OUT;
output CAN_TX_EBL_USBA_DATA2_MGPIO4A_OUT;
output CAN_TXBUS_USBA_DATA0_MGPIO2A_OUT;
output [15:0] DRAM_ADDR;
output [2:0] DRAM_BA;
output DRAM_CASN;
output DRAM_CKE;
output DRAM_CLK;
output DRAM_CSN;
output [2:0] DRAM_DM_RDQS_OUT;
output [17:0] DRAM_DQ_OUT;
output [2:0] DRAM_DQS_OUT;
output [1:0] DRAM_FIFO_WE_OUT;
output DRAM_ODT;
output DRAM_RASN;
output DRAM_RSTN;
output DRAM_WEN;
output I2C0_SCL_USBC_DATA1_MGPIO31B_OUT;
output I2C0_SDA_USBC_DATA0_MGPIO30B_OUT;
output I2C1_SCL_USBA_DATA4_MGPIO1A_OUT;
output I2C1_SDA_USBA_DATA3_MGPIO0A_OUT;
output MMUART0_CTS_USBC_DATA7_MGPIO19B_OUT;
output MMUART0_DCD_MGPIO22B_OUT;
output MMUART0_DSR_MGPIO20B_OUT;
output MMUART0_DTR_USBC_DATA6_MGPIO18B_OUT;
output MMUART0_RI_MGPIO21B_OUT;
output MMUART0_RTS_USBC_DATA5_MGPIO17B_OUT;
output MMUART0_RXD_USBC_STP_MGPIO28B_OUT;
output MMUART0_SCK_USBC_NXT_MGPIO29B_OUT;
output MMUART0_TXD_USBC_DIR_MGPIO27B_OUT;
output MMUART1_RXD_USBC_DATA3_MGPIO26B_OUT;
output MMUART1_SCK_USBC_DATA4_MGPIO25B_OUT;
output MMUART1_TXD_USBC_DATA2_MGPIO24B_OUT;
output RGMII_GTX_CLK_RMII_CLK_USBB_XCLK_OUT;
output RGMII_MDC_RMII_MDC_OUT;
output RGMII_MDIO_RMII_MDIO_USBB_DATA7_OUT;
output RGMII_RX_CLK_OUT;
output RGMII_RX_CTL_RMII_CRS_DV_USBB_DATA2_OUT;
output RGMII_RXD0_RMII_RXD0_USBB_DATA0_OUT;
output RGMII_RXD1_RMII_RXD1_USBB_DATA1_OUT;
output RGMII_RXD2_RMII_RX_ER_USBB_DATA3_OUT;
output RGMII_RXD3_USBB_DATA4_OUT;
output RGMII_TX_CLK_OUT;
output RGMII_TX_CTL_RMII_TX_EN_USBB_NXT_OUT;
output RGMII_TXD0_RMII_TXD0_USBB_DIR_OUT;
output RGMII_TXD1_RMII_TXD1_USBB_STP_OUT;
output RGMII_TXD2_USBB_DATA5_OUT;
output RGMII_TXD3_USBB_DATA6_OUT;
output SPI0_SCK_USBA_XCLK_OUT;
output SPI0_SDI_USBA_DIR_MGPIO5A_OUT;
output SPI0_SDO_USBA_STP_MGPIO6A_OUT;
output SPI0_SS0_USBA_NXT_MGPIO7A_OUT;
output SPI0_SS1_USBA_DATA5_MGPIO8A_OUT;
output SPI0_SS2_USBA_DATA6_MGPIO9A_OUT;
output SPI0_SS3_USBA_DATA7_MGPIO10A_OUT;
output SPI1_SCK_OUT;
output SPI1_SDI_MGPIO11A_OUT;
output SPI1_SDO_MGPIO12A_OUT;
output SPI1_SS0_MGPIO13A_OUT;
output SPI1_SS1_MGPIO14A_OUT;
output SPI1_SS2_MGPIO15A_OUT;
output SPI1_SS3_MGPIO16A_OUT;
output SPI1_SS4_MGPIO17A_OUT;
output SPI1_SS5_MGPIO18A_OUT;
output SPI1_SS6_MGPIO23A_OUT;
output SPI1_SS7_MGPIO24A_OUT;
output USBC_XCLK_OUT;
output CAN_RXBUS_USBA_DATA1_MGPIO3A_OE;
output CAN_TX_EBL_USBA_DATA2_MGPIO4A_OE;
output CAN_TXBUS_USBA_DATA0_MGPIO2A_OE;
output [2:0] DM_OE;
output [17:0] DRAM_DQ_OE;
output [2:0] DRAM_DQS_OE;
output I2C0_SCL_USBC_DATA1_MGPIO31B_OE;
output I2C0_SDA_USBC_DATA0_MGPIO30B_OE;
output I2C1_SCL_USBA_DATA4_MGPIO1A_OE;
output I2C1_SDA_USBA_DATA3_MGPIO0A_OE;
output MMUART0_CTS_USBC_DATA7_MGPIO19B_OE;
output MMUART0_DCD_MGPIO22B_OE;
output MMUART0_DSR_MGPIO20B_OE;
output MMUART0_DTR_USBC_DATA6_MGPIO18B_OE;
output MMUART0_RI_MGPIO21B_OE;
output MMUART0_RTS_USBC_DATA5_MGPIO17B_OE;
output MMUART0_RXD_USBC_STP_MGPIO28B_OE;
output MMUART0_SCK_USBC_NXT_MGPIO29B_OE;
output MMUART0_TXD_USBC_DIR_MGPIO27B_OE;
output MMUART1_RXD_USBC_DATA3_MGPIO26B_OE;
output MMUART1_SCK_USBC_DATA4_MGPIO25B_OE;
output MMUART1_TXD_USBC_DATA2_MGPIO24B_OE;
output RGMII_GTX_CLK_RMII_CLK_USBB_XCLK_OE;
output RGMII_MDC_RMII_MDC_OE;
output RGMII_MDIO_RMII_MDIO_USBB_DATA7_OE;
output RGMII_RX_CLK_OE;
output RGMII_RX_CTL_RMII_CRS_DV_USBB_DATA2_OE;
output RGMII_RXD0_RMII_RXD0_USBB_DATA0_OE;
output RGMII_RXD1_RMII_RXD1_USBB_DATA1_OE;
output RGMII_RXD2_RMII_RX_ER_USBB_DATA3_OE;
output RGMII_RXD3_USBB_DATA4_OE;
output RGMII_TX_CLK_OE;
output RGMII_TX_CTL_RMII_TX_EN_USBB_NXT_OE;
output RGMII_TXD0_RMII_TXD0_USBB_DIR_OE;
output RGMII_TXD1_RMII_TXD1_USBB_STP_OE;
output RGMII_TXD2_USBB_DATA5_OE;
output RGMII_TXD3_USBB_DATA6_OE;
output SPI0_SCK_USBA_XCLK_OE;
output SPI0_SDI_USBA_DIR_MGPIO5A_OE;
output SPI0_SDO_USBA_STP_MGPIO6A_OE;
output SPI0_SS0_USBA_NXT_MGPIO7A_OE;
output SPI0_SS1_USBA_DATA5_MGPIO8A_OE;
output SPI0_SS2_USBA_DATA6_MGPIO9A_OE;
output SPI0_SS3_USBA_DATA7_MGPIO10A_OE;
output SPI1_SCK_OE;
output SPI1_SDI_MGPIO11A_OE;
output SPI1_SDO_MGPIO12A_OE;
output SPI1_SS0_MGPIO13A_OE;
output SPI1_SS1_MGPIO14A_OE;
output SPI1_SS2_MGPIO15A_OE;
output SPI1_SS3_MGPIO16A_OE;
output SPI1_SS4_MGPIO17A_OE;
output SPI1_SS5_MGPIO18A_OE;
output SPI1_SS6_MGPIO23A_OE;
output SPI1_SS7_MGPIO24A_OE;
output USBC_XCLK_OE;
parameter INIT = 'h0;
parameter ACT_UBITS = 'h0;
parameter MEMORYFILE = "";
parameter RTC_MAIN_XTL_FREQ = 0.0;
parameter RTC_MAIN_XTL_MODE = "";
parameter DDR_CLK_FREQ = 0.0;

endmodule
