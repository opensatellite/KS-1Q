//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Mon Oct 31 14:44:47 2016
// Version: v11.7 11.7.0.119
//////////////////////////////////////////////////////////////////////

`timescale 1ns / 100ps

// test_4463andcpuopration
module test_4463andcpuopration(
    // Inputs
    ADC_INT,
    BOARD_NO,
    CAN1_TX,
    CCA_NIRQ,
    DEVRST_N,
    L4V_PG,
    RXD4,
    RX_GPIO0,
    RX_GPIO1,
    SPI_MISO,
    TX_GPIO1,
    XTL,
    // Outputs
    \A3.3V_EN ,
    ADC_CS,
    ADC_CSTART,
    ADC_FS,
    ADC_PWDN,
    ANT_SW1,
    ANT_SW2,
    CAN1_RS,
    CAN1_RX,
    CAN2_RS,
    L3V3_EN1,
    L3V3_EN2,
    L4463_RX_CS,
    L4463_TX_CS,
    L4V_EN,
    PA4V_ON,
    RX_SDN,
    SPI_CLK,
    SPI_FRAM_CS,
    SPI_MOSI,
    SW,
    TXD4,
    TX_GPIO0,
    TX_SDN
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input  ADC_INT;
input  BOARD_NO;
input  CAN1_TX;
input  CCA_NIRQ;
input  DEVRST_N;
input  L4V_PG;
input  RXD4;
input  RX_GPIO0;
input  RX_GPIO1;
input  SPI_MISO;
input  TX_GPIO1;
input  XTL;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output \A3.3V_EN ;
output ADC_CS;
output ADC_CSTART;
output ADC_FS;
output ADC_PWDN;
output ANT_SW1;
output ANT_SW2;
output CAN1_RS;
output CAN1_RX;
output CAN2_RS;
output L3V3_EN1;
output L3V3_EN2;
output L4463_RX_CS;
output L4463_TX_CS;
output L4V_EN;
output PA4V_ON;
output RX_SDN;
output SPI_CLK;
output SPI_FRAM_CS;
output SPI_MOSI;
output SW;
output TXD4;
output TX_GPIO0;
output TX_SDN;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire          \A3.3V_EN_net_0 ;
wire          A_1;
wire          ADC_CS_net_0;
wire          ADC_CSTART_net_0;
wire          ADC_FS_net_0;
wire          ADC_INT;
wire          ADC_PWDN_net_0;
wire          ANT_SW1_net_0;
wire          ANT_SW2_net_0;
wire          ASM_OUT;
wire          BOARD_NO;
wire          C_1;
wire          c_in;
wire          c_out0;
wire          c_out0_0;
wire          c_out1;
wire          c_out1_0;
wire          CAN1_RX_net_0;
wire          CAN1_TX;
wire          CCA_NIRQ;
wire          CLTU_0_Block_ErrO;
wire          CLTU_0_Flag_RX_ing;
wire          CLTU_0_IP_END;
wire   [31:0] CLTU_0_TC_Error_Counter;
wire   [31:0] CLTU_0_TC_Packet_Counter;
wire   [31:0] CoreAPB3_0_APBmslave3_PADDR;
wire          CoreAPB3_0_APBmslave3_PENABLE;
wire   [31:0] CoreAPB3_0_APBmslave3_PRDATA;
wire          CoreAPB3_0_APBmslave3_PREADY;
wire          CoreAPB3_0_APBmslave3_PSELx;
wire   [31:0] CoreAPB3_0_APBmslave3_PWDATA;
wire          CoreAPB3_0_APBmslave3_PWRITE;
wire   [7:0]  COREEDAC_0_DATA_OUT;
wire   [31:0] COREEDAC_1_DATA_OUT;
wire   [7:0]  CPU_W_HDL_R_0_DataO;
wire          CPU_W_HDL_R_0_Flag_A_Tx_Finish;
wire          CPU_W_HDL_R_0_Flag_B_Tx_Finish;
wire   [31:0] CPU_W_HDL_R_0_TM_Packet_Counter;
wire   [9:0]  CPU_W_HDL_R_0_USER_RA_TRP1;
wire          CPU_W_HDL_R_0_USER_REN_TRP1;
wire          DEVRST_N;
wire          divider_0_out_16dec;
wire          En_read_buff;
wire          ENASM;
wire          ENASM_0;
wire          ENASM_1;
wire          ENCRC;
wire          ENCRC_0;
wire          EnIP;
wire          FALG_FINISH_A_0;
wire          Flag_RX;
wire   [15:0] Glue_Logic_0_ADDR;
wire          Glue_Logic_0_Flag_B_Rx;
wire          Glue_Logic_0_Flag_B_Tx;
wire   [15:0] Glue_Logic_0_Flag_Int_0;
wire   [15:0] Glue_Logic_0_MASK;
wire   [7:0]  Glue_Logic_0_MSG_TRP1;
wire   [9:0]  Glue_Logic_0_MSG_WA_TRP1;
wire   [3:0]  Glue_Logic_0_USER_RA_TRP1_1;
wire          Glue_Logic_0_USER_REN_TRP1;
wire          Glue_Logic_0_USER_WEN_TRP1;
wire          HDL_W_CPU_R_BUFF_0_Flag_A_HDL_Rec_Finish;
wire          HDL_W_CPU_R_BUFF_0_Flag_B_HDL_Rec_Finish;
wire          HDL_W_CPU_R_BUFF_0_Flag_OF;
wire   [31:0] HDL_W_CPU_R_BUFF_0_MSG_TRP1;
wire   [3:0]  HDL_W_CPU_R_BUFF_0_MSG_WA_TRP1_1;
wire          HDL_W_CPU_R_BUFF_0_USER_WEN_TRP1;
wire          INV_1_Y;
wire          L3V3_EN1_net_0;
wire          L3V3_EN2_net_0;
wire          L4V_EN_net_0;
wire          L4V_PG;
wire          L4463_RX_CS_net_0;
wire          L4463_TX_CS_net_0;
wire          OSC_0_XTLOSC_O2F;
wire          P_CLK;
wire          P_OUT;
wire          PA4V_ON_net_0;
wire          pending;
wire          pending_0;
wire          Randomizer_0_enout;
wire          Randomizer_0_Out1;
wire          RSControl_0_CLKEN;
wire   [7:0]  RSControl_0_DATAINP;
wire          RSControl_0_NGRST;
wire          RSControl_0_R_ST;
wire          RSControl_0_START;
wire          RX_GPIO0;
wire          RX_GPIO1;
wire          RX_SDN_net_0;
wire          RXD4;
wire          RXRandomize_0_Block_ErrO;
wire          RXRandomize_0_IP_END_O;
wire          S_OUT_0;
wire          SPI_CLK_net_0;
wire          SPI_FRAM_CS_net_0;
wire          SPI_MISO;
wire          SPI_MOSI_net_0;
wire          SW_net_0;
wire          Switch_0_EnOut;
wire          SYSRESET_0_POWER_ON_RESET_N;
wire          TC_Decode_0_IP_END_O;
wire   [15:0] TC_Decode_0_SpacecraftID;
wire   [31:0] test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PADDR;
wire          test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PENABLE;
wire   [31:0] test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PRDATA;
wire          test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PREADY;
wire          test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PSELx;
wire          test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PSLVERR;
wire   [31:0] test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PWDATA;
wire          test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PWRITE;
wire          test_4463andcpuopration_MSS_0_GPIO_1_M2F;
wire          test_4463andcpuopration_MSS_0_GPIO_15_M2F;
wire   [7:0]  TM_RSENC_0_CODEOUTP;
wire          TM_RSENC_0_RDY;
wire          TM_RSENC_0_RFD;
wire          TM_RSENC_0_RFS;
wire          TX_GPIO0_net_0;
wire          TX_GPIO1;
wire          TX_SDN_net_0;
wire          TXD4_net_0;
wire          TXD4_0;
wire          XTL;
wire          ADC_CS_net_1;
wire          ADC_FS_net_1;
wire          ADC_PWDN_net_1;
wire          L4V_EN_net_1;
wire          ANT_SW2_net_1;
wire          ANT_SW1_net_1;
wire          PA4V_ON_net_1;
wire          SPI_FRAM_CS_net_1;
wire          L3V3_EN2_net_1;
wire          L3V3_EN1_net_1;
wire          L4463_RX_CS_net_1;
wire          RX_SDN_net_1;
wire          TX_GPIO0_net_1;
wire          L4463_TX_CS_net_1;
wire          TX_SDN_net_1;
wire          \A3.3V_EN_net_1 ;
wire          TXD4_0_net_0;
wire          SPI_MOSI_net_1;
wire          SPI_CLK_net_1;
wire          SW_net_1;
wire          ADC_CSTART_net_1;
wire          CAN1_RX_net_1;
//--------------------------------------------------------------------
// TiedOff Nets
//--------------------------------------------------------------------
wire          GND_net;
wire          VCC_net;
wire   [31:0] IADDR_const_net_0;
wire   [7:0]  MSG_TRP2_const_net_0;
wire   [7:0]  MSG_TRP3_const_net_0;
wire   [9:0]  USER_WA_TRP2_const_net_0;
wire   [9:0]  USER_WA_TRP3_const_net_0;
wire   [9:0]  USER_RA_TRP2_const_net_0;
wire   [9:0]  USER_RA_TRP3_const_net_0;
wire   [12:0] INJ_ERR_const_net_0;
wire   [12:0] CODE_FROM_EXT_RAM_const_net_0;
wire   [31:0] MSG_TRP2_const_net_1;
wire   [31:0] MSG_TRP3_const_net_1;
wire   [3:0]  USER_WA_TRP2_const_net_1;
wire   [3:0]  USER_WA_TRP3_const_net_1;
wire   [3:0]  USER_RA_TRP2_const_net_1;
wire   [3:0]  USER_RA_TRP3_const_net_1;
wire   [38:0] INJ_ERR_const_net_1;
wire   [38:0] CODE_FROM_EXT_RAM_const_net_1;
wire   [31:0] PRDATAS0_const_net_0;
wire   [31:0] PRDATAS1_const_net_0;
wire   [31:0] PRDATAS2_const_net_0;
wire   [31:0] PRDATAS4_const_net_0;
wire   [31:0] PRDATAS5_const_net_0;
wire   [31:0] PRDATAS6_const_net_0;
wire   [31:0] PRDATAS7_const_net_0;
wire   [31:0] PRDATAS8_const_net_0;
wire   [31:0] PRDATAS9_const_net_0;
wire   [31:0] PRDATAS10_const_net_0;
wire   [31:0] PRDATAS11_const_net_0;
wire   [31:0] PRDATAS12_const_net_0;
wire   [31:0] PRDATAS13_const_net_0;
wire   [31:0] PRDATAS14_const_net_0;
wire   [31:0] PRDATAS15_const_net_0;
wire   [31:0] PRDATAS16_const_net_0;
//--------------------------------------------------------------------
// Constant assignments
//--------------------------------------------------------------------
assign GND_net                       = 1'b0;
assign VCC_net                       = 1'b1;
assign IADDR_const_net_0             = 32'h00000000;
assign MSG_TRP2_const_net_0          = 8'h00;
assign MSG_TRP3_const_net_0          = 8'h00;
assign USER_WA_TRP2_const_net_0      = 10'h000;
assign USER_WA_TRP3_const_net_0      = 10'h000;
assign USER_RA_TRP2_const_net_0      = 10'h000;
assign USER_RA_TRP3_const_net_0      = 10'h000;
assign INJ_ERR_const_net_0           = 13'h0000;
assign CODE_FROM_EXT_RAM_const_net_0 = 13'h0000;
assign MSG_TRP2_const_net_1          = 32'h00000000;
assign MSG_TRP3_const_net_1          = 32'h00000000;
assign USER_WA_TRP2_const_net_1      = 4'h0;
assign USER_WA_TRP3_const_net_1      = 4'h0;
assign USER_RA_TRP2_const_net_1      = 4'h0;
assign USER_RA_TRP3_const_net_1      = 4'h0;
assign INJ_ERR_const_net_1           = 39'h0000000000;
assign CODE_FROM_EXT_RAM_const_net_1 = 39'h0000000000;
assign PRDATAS0_const_net_0          = 32'h00000000;
assign PRDATAS1_const_net_0          = 32'h00000000;
assign PRDATAS2_const_net_0          = 32'h00000000;
assign PRDATAS4_const_net_0          = 32'h00000000;
assign PRDATAS5_const_net_0          = 32'h00000000;
assign PRDATAS6_const_net_0          = 32'h00000000;
assign PRDATAS7_const_net_0          = 32'h00000000;
assign PRDATAS8_const_net_0          = 32'h00000000;
assign PRDATAS9_const_net_0          = 32'h00000000;
assign PRDATAS10_const_net_0         = 32'h00000000;
assign PRDATAS11_const_net_0         = 32'h00000000;
assign PRDATAS12_const_net_0         = 32'h00000000;
assign PRDATAS13_const_net_0         = 32'h00000000;
assign PRDATAS14_const_net_0         = 32'h00000000;
assign PRDATAS15_const_net_0         = 32'h00000000;
assign PRDATAS16_const_net_0         = 32'h00000000;
//--------------------------------------------------------------------
// TieOff assignments
//--------------------------------------------------------------------
assign CAN1_RS           = 1'b0;
assign CAN2_RS           = 1'b1;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign ADC_CS_net_1      = ADC_CS_net_0;
assign ADC_CS            = ADC_CS_net_1;
assign ADC_FS_net_1      = ADC_FS_net_0;
assign ADC_FS            = ADC_FS_net_1;
assign ADC_PWDN_net_1    = ADC_PWDN_net_0;
assign ADC_PWDN          = ADC_PWDN_net_1;
assign L4V_EN_net_1      = L4V_EN_net_0;
assign L4V_EN            = L4V_EN_net_1;
assign ANT_SW2_net_1     = ANT_SW2_net_0;
assign ANT_SW2           = ANT_SW2_net_1;
assign ANT_SW1_net_1     = ANT_SW1_net_0;
assign ANT_SW1           = ANT_SW1_net_1;
assign PA4V_ON_net_1     = PA4V_ON_net_0;
assign PA4V_ON           = PA4V_ON_net_1;
assign SPI_FRAM_CS_net_1 = SPI_FRAM_CS_net_0;
assign SPI_FRAM_CS       = SPI_FRAM_CS_net_1;
assign L3V3_EN2_net_1    = L3V3_EN2_net_0;
assign L3V3_EN2          = L3V3_EN2_net_1;
assign L3V3_EN1_net_1    = L3V3_EN1_net_0;
assign L3V3_EN1          = L3V3_EN1_net_1;
assign L4463_RX_CS_net_1 = L4463_RX_CS_net_0;
assign L4463_RX_CS       = L4463_RX_CS_net_1;
assign RX_SDN_net_1      = RX_SDN_net_0;
assign RX_SDN            = RX_SDN_net_1;
assign TX_GPIO0_net_1    = TX_GPIO0_net_0;
assign TX_GPIO0          = TX_GPIO0_net_1;
assign L4463_TX_CS_net_1 = L4463_TX_CS_net_0;
assign L4463_TX_CS       = L4463_TX_CS_net_1;
assign TX_SDN_net_1      = TX_SDN_net_0;
assign TX_SDN            = TX_SDN_net_1;
assign \A3.3V_EN_net_1   = \A3.3V_EN_net_0 ;
assign \A3.3V_EN         = \A3.3V_EN_net_1 ;
assign TXD4_0_net_0      = TXD4_0;
assign TXD4              = TXD4_0_net_0;
assign SPI_MOSI_net_1    = SPI_MOSI_net_0;
assign SPI_MOSI          = SPI_MOSI_net_1;
assign SPI_CLK_net_1     = SPI_CLK_net_0;
assign SPI_CLK           = SPI_CLK_net_1;
assign SW_net_1          = SW_net_0;
assign SW                = SW_net_1;
assign ADC_CSTART_net_1  = ADC_CSTART_net_0;
assign ADC_CSTART        = ADC_CSTART_net_1;
assign CAN1_RX_net_1     = CAN1_RX_net_0;
assign CAN1_RX           = CAN1_RX_net_1;
//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------ASM_Generato
ASM_Generato ASM_Generato_0(
        // Inputs
        .clk        ( P_CLK ),
        .reset      ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .En_ASM     ( ENASM ),
        // Outputs
        .out        ( ASM_OUT ),
        .pending    ( pending ),
        .En_ASM_Out ( ENASM_1 ) 
        );

//--------CLTU
CLTU CLTU_0(
        // Inputs
        .IO_Bit0           ( GND_net ),
        .DataI             ( RX_GPIO0 ),
        .ClkI              ( RX_GPIO1 ),
        .Rst               ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .CCA               ( CCA_NIRQ ),
        // Outputs
        .DataO             ( A_1 ),
        .Enable            ( ENASM_0 ),
        .Block_ErrO        ( CLTU_0_Block_ErrO ),
        .IP_END            ( CLTU_0_IP_END ),
        .Dis_Switch        (  ),
        .Flag_RX_ing       ( CLTU_0_Flag_RX_ing ),
        .TC_Packet_Counter ( CLTU_0_TC_Packet_Counter ),
        .TC_Error_Counter  ( CLTU_0_TC_Error_Counter ) 
        );

//--------Convolutional_Encoder
Convolutional_Encoder Convolutional_Encoder_0(
        // Inputs
        .clk                         ( P_CLK ),
        .reset                       ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .enb                         ( Switch_0_EnOut ),
        .Convolutional_Encoder_in    ( c_in ),
        // Outputs
        .Convolutional_Encoder_out_1 ( c_out1 ),
        .Convolutional_Encoder_out_0 ( c_out0 ) 
        );

//--------CoreAPB3   -   Actel:DirectCore:CoreAPB3:4.1.100
CoreAPB3 #( 
        .APB_DWIDTH      ( 32 ),
        .APBSLOT0ENABLE  ( 0 ),
        .APBSLOT1ENABLE  ( 0 ),
        .APBSLOT2ENABLE  ( 0 ),
        .APBSLOT3ENABLE  ( 1 ),
        .APBSLOT4ENABLE  ( 0 ),
        .APBSLOT5ENABLE  ( 0 ),
        .APBSLOT6ENABLE  ( 0 ),
        .APBSLOT7ENABLE  ( 0 ),
        .APBSLOT8ENABLE  ( 0 ),
        .APBSLOT9ENABLE  ( 0 ),
        .APBSLOT10ENABLE ( 0 ),
        .APBSLOT11ENABLE ( 0 ),
        .APBSLOT12ENABLE ( 0 ),
        .APBSLOT13ENABLE ( 0 ),
        .APBSLOT14ENABLE ( 0 ),
        .APBSLOT15ENABLE ( 0 ),
        .FAMILY          ( 19 ),
        .IADDR_OPTION    ( 0 ),
        .MADDR_BITS      ( 32 ),
        .SC_0            ( 0 ),
        .SC_1            ( 0 ),
        .SC_2            ( 0 ),
        .SC_3            ( 0 ),
        .SC_4            ( 0 ),
        .SC_5            ( 0 ),
        .SC_6            ( 0 ),
        .SC_7            ( 0 ),
        .SC_8            ( 0 ),
        .SC_9            ( 0 ),
        .SC_10           ( 0 ),
        .SC_11           ( 0 ),
        .SC_12           ( 0 ),
        .SC_13           ( 0 ),
        .SC_14           ( 0 ),
        .SC_15           ( 0 ),
        .UPR_NIBBLE_POSN ( 2 ) )
CoreAPB3_0(
        // Inputs
        .PRESETN    ( GND_net ), // tied to 1'b0 from definition
        .PCLK       ( GND_net ), // tied to 1'b0 from definition
        .PWRITE     ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PWRITE ),
        .PENABLE    ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PENABLE ),
        .PSEL       ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PSELx ),
        .PREADYS0   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS0  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS1   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS1  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS2   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS2  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS3   ( CoreAPB3_0_APBmslave3_PREADY ),
        .PSLVERRS3  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS4   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS4  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS5   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS5  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS6   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS6  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS7   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS7  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS8   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS8  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS9   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS9  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS10  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS10 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS11  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS11 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS12  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS12 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS13  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS13 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS14  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS14 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS15  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS15 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS16  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS16 ( GND_net ), // tied to 1'b0 from definition
        .PADDR      ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PADDR ),
        .PWDATA     ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PWDATA ),
        .PRDATAS0   ( PRDATAS0_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS1   ( PRDATAS1_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS2   ( PRDATAS2_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS3   ( CoreAPB3_0_APBmslave3_PRDATA ),
        .PRDATAS4   ( PRDATAS4_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS5   ( PRDATAS5_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS6   ( PRDATAS6_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS7   ( PRDATAS7_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS8   ( PRDATAS8_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS9   ( PRDATAS9_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS10  ( PRDATAS10_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS11  ( PRDATAS11_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS12  ( PRDATAS12_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS13  ( PRDATAS13_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS14  ( PRDATAS14_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS15  ( PRDATAS15_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS16  ( PRDATAS16_const_net_0 ), // tied to 32'h00000000 from definition
        .IADDR      ( IADDR_const_net_0 ), // tied to 32'h00000000 from definition
        // Outputs
        .PREADY     ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PREADY ),
        .PSLVERR    ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PSLVERR ),
        .PWRITES    ( CoreAPB3_0_APBmslave3_PWRITE ),
        .PENABLES   ( CoreAPB3_0_APBmslave3_PENABLE ),
        .PSELS0     (  ),
        .PSELS1     (  ),
        .PSELS2     (  ),
        .PSELS3     ( CoreAPB3_0_APBmslave3_PSELx ),
        .PSELS4     (  ),
        .PSELS5     (  ),
        .PSELS6     (  ),
        .PSELS7     (  ),
        .PSELS8     (  ),
        .PSELS9     (  ),
        .PSELS10    (  ),
        .PSELS11    (  ),
        .PSELS12    (  ),
        .PSELS13    (  ),
        .PSELS14    (  ),
        .PSELS15    (  ),
        .PSELS16    (  ),
        .PRDATA     ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PRDATA ),
        .PADDRS     ( CoreAPB3_0_APBmslave3_PADDR ),
        .PWDATAS    ( CoreAPB3_0_APBmslave3_PWDATA ) 
        );

//--------test_4463andcpuopration_COREEDAC_0_COREEDAC   -   Actel:DirectCore:COREEDAC:2.8.100
test_4463andcpuopration_COREEDAC_0_COREEDAC #( 
        .DAT_WIDTH     ( 8 ),
        .DIV_WDTH      ( 10 ),
        .DLY_RD_A_ON   ( 0 ),
        .FAMILY        ( 19 ),
        .INIT_RAM      ( 0 ),
        .MODE          ( 1 ),
        .RAM_DEPTH     ( 1024 ),
        .RAM_PIPE      ( 0 ),
        .SCRUB_AMAX    ( 255 ),
        .SCRUB_AMIN    ( 0 ),
        .SCRUB_ON      ( 0 ),
        .SIM_RAM       ( 0 ),
        .SINGLECLK     ( 0 ),
        .TEST          ( 0 ),
        .TMOUT_SET     ( 1000 ),
        .TMR           ( 0 ),
        .URAM          ( 0 ),
        .URAM_A_PIPE   ( 1 ),
        .URAM_PORT_B   ( 0 ),
        .USER_DEC_PIPE ( 0 ),
        .USER_ENC_PIPE ( 0 ),
        .WRBK_ON       ( 1 ) )
COREEDAC_0(
        // Inputs
        .NGRST             ( VCC_net ),
        .RST               ( INV_1_Y ),
        .RCLK              ( divider_0_out_16dec ),
        .WCLK              ( OSC_0_XTLOSC_O2F ),
        .STOP_SCRUB_TRP1   ( GND_net ), // tied to 1'b0 from definition
        .STOP_SCRUB_TRP2   ( GND_net ), // tied to 1'b0 from definition
        .STOP_SCRUB_TRP3   ( GND_net ), // tied to 1'b0 from definition
        .USER_WEN_TRP1     ( Glue_Logic_0_USER_WEN_TRP1 ),
        .USER_WEN_TRP2     ( GND_net ), // tied to 1'b0 from definition
        .USER_WEN_TRP3     ( GND_net ), // tied to 1'b0 from definition
        .USER_REN_TRP1     ( CPU_W_HDL_R_0_USER_REN_TRP1 ),
        .USER_REN_TRP2     ( VCC_net ), // tied to 1'b1 from definition
        .USER_REN_TRP3     ( VCC_net ), // tied to 1'b1 from definition
        .START_SCRUB_TRP1  ( GND_net ), // tied to 1'b0 from definition
        .START_SCRUB_TRP2  ( GND_net ), // tied to 1'b0 from definition
        .START_SCRUB_TRP3  ( GND_net ), // tied to 1'b0 from definition
        .RST_TIMER_TRP1    ( GND_net ), // tied to 1'b0 from definition
        .RST_TIMER_TRP2    ( GND_net ), // tied to 1'b0 from definition
        .RST_TIMER_TRP3    ( GND_net ), // tied to 1'b0 from definition
        .MSG_TRP1          ( Glue_Logic_0_MSG_TRP1 ),
        .MSG_TRP2          ( MSG_TRP2_const_net_0 ), // tied to 8'h00 from definition
        .MSG_TRP3          ( MSG_TRP3_const_net_0 ), // tied to 8'h00 from definition
        .USER_WA_TRP1      ( Glue_Logic_0_MSG_WA_TRP1 ),
        .USER_WA_TRP2      ( USER_WA_TRP2_const_net_0 ), // tied to 10'h000 from definition
        .USER_WA_TRP3      ( USER_WA_TRP3_const_net_0 ), // tied to 10'h000 from definition
        .USER_RA_TRP1      ( CPU_W_HDL_R_0_USER_RA_TRP1 ),
        .USER_RA_TRP2      ( USER_RA_TRP2_const_net_0 ), // tied to 10'h000 from definition
        .USER_RA_TRP3      ( USER_RA_TRP3_const_net_0 ), // tied to 10'h000 from definition
        .INJ_ERR           ( INJ_ERR_const_net_0 ), // tied to 13'h0000 from definition
        .CODE_FROM_EXT_RAM ( CODE_FROM_EXT_RAM_const_net_0 ), // tied to 13'h0000 from definition
        // Outputs
        .ERROR             (  ),
        .CORRECTABLE       (  ),
        .SLOWDOWN          (  ),
        .TMOUTFLG          (  ),
        .SCRUB_DONE        (  ),
        .NOW_SCRUBBING     (  ),
        .INIT_DONE         (  ),
        .SCRUB_CORR        (  ),
        .CODE_FROM_RAM     (  ),
        .DATA_OUT          ( COREEDAC_0_DATA_OUT ),
        .PARITY_OUT        (  ),
        .CODED             (  ),
        .RAM_RA_LAT        (  ) 
        );

//--------test_4463andcpuopration_COREEDAC_1_COREEDAC   -   Actel:DirectCore:COREEDAC:2.8.100
test_4463andcpuopration_COREEDAC_1_COREEDAC #( 
        .DAT_WIDTH     ( 32 ),
        .DIV_WDTH      ( 10 ),
        .DLY_RD_A_ON   ( 0 ),
        .FAMILY        ( 19 ),
        .INIT_RAM      ( 0 ),
        .MODE          ( 1 ),
        .RAM_DEPTH     ( 16 ),
        .RAM_PIPE      ( 0 ),
        .SCRUB_AMAX    ( 255 ),
        .SCRUB_AMIN    ( 0 ),
        .SCRUB_ON      ( 0 ),
        .SIM_RAM       ( 0 ),
        .SINGLECLK     ( 0 ),
        .TEST          ( 0 ),
        .TMOUT_SET     ( 1000 ),
        .TMR           ( 0 ),
        .URAM          ( 0 ),
        .URAM_A_PIPE   ( 1 ),
        .URAM_PORT_B   ( 0 ),
        .USER_DEC_PIPE ( 0 ),
        .USER_ENC_PIPE ( 0 ),
        .WRBK_ON       ( 1 ) )
COREEDAC_1(
        // Inputs
        .NGRST             ( VCC_net ),
        .RST               ( INV_1_Y ),
        .RCLK              ( OSC_0_XTLOSC_O2F ),
        .WCLK              ( RX_GPIO1 ),
        .STOP_SCRUB_TRP1   ( GND_net ), // tied to 1'b0 from definition
        .STOP_SCRUB_TRP2   ( GND_net ), // tied to 1'b0 from definition
        .STOP_SCRUB_TRP3   ( GND_net ), // tied to 1'b0 from definition
        .USER_WEN_TRP1     ( HDL_W_CPU_R_BUFF_0_USER_WEN_TRP1 ),
        .USER_WEN_TRP2     ( GND_net ), // tied to 1'b0 from definition
        .USER_WEN_TRP3     ( GND_net ), // tied to 1'b0 from definition
        .USER_REN_TRP1     ( Glue_Logic_0_USER_REN_TRP1 ),
        .USER_REN_TRP2     ( VCC_net ), // tied to 1'b1 from definition
        .USER_REN_TRP3     ( VCC_net ), // tied to 1'b1 from definition
        .START_SCRUB_TRP1  ( GND_net ), // tied to 1'b0 from definition
        .START_SCRUB_TRP2  ( GND_net ), // tied to 1'b0 from definition
        .START_SCRUB_TRP3  ( GND_net ), // tied to 1'b0 from definition
        .RST_TIMER_TRP1    ( GND_net ), // tied to 1'b0 from definition
        .RST_TIMER_TRP2    ( GND_net ), // tied to 1'b0 from definition
        .RST_TIMER_TRP3    ( GND_net ), // tied to 1'b0 from definition
        .MSG_TRP1          ( HDL_W_CPU_R_BUFF_0_MSG_TRP1 ),
        .MSG_TRP2          ( MSG_TRP2_const_net_1 ), // tied to 32'h00000000 from definition
        .MSG_TRP3          ( MSG_TRP3_const_net_1 ), // tied to 32'h00000000 from definition
        .USER_WA_TRP1      ( HDL_W_CPU_R_BUFF_0_MSG_WA_TRP1_1 ),
        .USER_WA_TRP2      ( USER_WA_TRP2_const_net_1 ), // tied to 4'h0 from definition
        .USER_WA_TRP3      ( USER_WA_TRP3_const_net_1 ), // tied to 4'h0 from definition
        .USER_RA_TRP1      ( Glue_Logic_0_USER_RA_TRP1_1 ),
        .USER_RA_TRP2      ( USER_RA_TRP2_const_net_1 ), // tied to 4'h0 from definition
        .USER_RA_TRP3      ( USER_RA_TRP3_const_net_1 ), // tied to 4'h0 from definition
        .INJ_ERR           ( INJ_ERR_const_net_1 ), // tied to 39'h0000000000 from definition
        .CODE_FROM_EXT_RAM ( CODE_FROM_EXT_RAM_const_net_1 ), // tied to 39'h0000000000 from definition
        // Outputs
        .ERROR             (  ),
        .CORRECTABLE       (  ),
        .SLOWDOWN          (  ),
        .TMOUTFLG          (  ),
        .SCRUB_DONE        (  ),
        .NOW_SCRUBBING     (  ),
        .INIT_DONE         (  ),
        .SCRUB_CORR        (  ),
        .CODE_FROM_RAM     (  ),
        .DATA_OUT          ( COREEDAC_1_DATA_OUT ),
        .PARITY_OUT        (  ),
        .CODED             (  ),
        .RAM_RA_LAT        (  ) 
        );

//--------CPU_W_HDL_R
CPU_W_HDL_R CPU_W_HDL_R_0(
        // Inputs
        .ClkI_Dec8         ( divider_0_out_16dec ),
        .ClkI_Dec2         ( P_CLK ),
        .Rst               ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .Flag_A_Tx         ( TXD4_net_0 ),
        .Flag_B_Tx         ( Glue_Logic_0_Flag_B_Tx ),
        .pending           ( pending ),
        .En_Read_Buff      ( En_read_buff ),
        .DataI             ( COREEDAC_0_DATA_OUT ),
        // Outputs
        .USER_REN_TRP1     ( CPU_W_HDL_R_0_USER_REN_TRP1 ),
        .Flag_A_Tx_Finish  ( CPU_W_HDL_R_0_Flag_A_Tx_Finish ),
        .Flag_B_Tx_Finish  ( CPU_W_HDL_R_0_Flag_B_Tx_Finish ),
        .En_CRC32          ( ENCRC ),
        .En_ASG            ( ENASM ),
        .USER_RA_TRP1      ( CPU_W_HDL_R_0_USER_RA_TRP1 ),
        .DataO             ( CPU_W_HDL_R_0_DataO ),
        .TM_Packet_Counter ( CPU_W_HDL_R_0_TM_Packet_Counter ) 
        );

//--------DATA_OUT
DATA_OUT DATA_OUT_0(
        // Inputs
        .Clk                         ( TX_GPIO1 ),
        .Clk_2dec                    ( P_CLK ),
        .Rst                         ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .Convolutional_Encoder_out_1 ( c_out1 ),
        .Convolutional_Encoder_out_0 ( c_out0 ),
        // Outputs
        .DataO                       ( TX_GPIO0_net_0 ),
        .c0                          ( c_out0_0 ),
        .c1                          ( c_out1_0 ) 
        );

//--------divider
divider divider_0(
        // Inputs
        .clk       ( TX_GPIO1 ),
        .init      ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        // Outputs
        .out_16dec ( divider_0_out_16dec ),
        .out_2dec  ( P_CLK ) 
        );

//--------Glue_Logic
Glue_Logic #( 
        .Flag_Register_Addr_0  ( 805306368 ),
        .Flag_Register_Addr_1  ( 805306372 ),
        .Flag_Register_Addr_2  ( 805306376 ),
        .Flag_Register_Addr_3  ( 805306380 ),
        .Flag_Register_Addr_4  ( 805306384 ),
        .Flag_Register_Addr_5  ( 805306388 ),
        .Flag_Register_Addr_6  ( 805306392 ),
        .Flag_Register_Addr_7  ( 805306396 ),
        .Flag_Register_Addr_8  ( 805306400 ),
        .Flag_Register_Addr_9  ( 805306404 ),
        .Flag_Register_Addr_10 ( 805306408 ),
        .Flag_Register_Addr_11 ( 805306412 ),
        .Flag_Register_Addr_12 ( 805306416 ),
        .Flag_Register_Addr_13 ( 805306420 ),
        .Flag_Register_Addr_14 ( 805306424 ),
        .Flag_Register_Addr_15 ( 805306428 ),
        .Rec_Base_Addr_A       ( 805330944 ),
        .Rec_Base_Addr_B       ( 805339136 ),
        .TC_Length             ( 1792 ),
        .TM_Length             ( 1784 ),
        .Tx_Base_Addr_A        ( 786440 ),
        .Tx_Base_Addr_B        ( 786448 ) )
Glue_Logic_0(
        // Inputs
        .Clk                   ( OSC_0_XTLOSC_O2F ),
        .Rst                   ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .Flag_A_Tx_Finish      ( CPU_W_HDL_R_0_Flag_A_Tx_Finish ),
        .Flag_B_Tx_Finish      ( CPU_W_HDL_R_0_Flag_B_Tx_Finish ),
        .Flag_A_HDL_Rec_Finish ( HDL_W_CPU_R_BUFF_0_Flag_A_HDL_Rec_Finish ),
        .Flag_B_HDL_Rec_Finish ( HDL_W_CPU_R_BUFF_0_Flag_B_HDL_Rec_Finish ),
        .Flag_Sending          ( Switch_0_EnOut ),
        .Flag_RX_ing           ( CLTU_0_Flag_RX_ing ),
        .PSEL                  ( CoreAPB3_0_APBmslave3_PSELx ),
        .PENABLE               ( CoreAPB3_0_APBmslave3_PENABLE ),
        .PWRITE                ( CoreAPB3_0_APBmslave3_PWRITE ),
        .SpacecraftID          ( TC_Decode_0_SpacecraftID ),
        .TM_Packet_Counter     ( CPU_W_HDL_R_0_TM_Packet_Counter ),
        .TC_Packet_Counter     ( CLTU_0_TC_Packet_Counter ),
        .TC_Error_Counter      ( CLTU_0_TC_Error_Counter ),
        .PADDR                 ( CoreAPB3_0_APBmslave3_PADDR ),
        .PWDATA                ( CoreAPB3_0_APBmslave3_PWDATA ),
        .Data_In               ( COREEDAC_1_DATA_OUT ),
        // Outputs
        .Flag_A_Tx             ( TXD4_net_0 ),
        .Flag_B_Tx             ( Glue_Logic_0_Flag_B_Tx ),
        .Flag_A_Rx             ( Flag_RX ),
        .Flag_B_Rx             ( Glue_Logic_0_Flag_B_Rx ),
        .PREADY                ( CoreAPB3_0_APBmslave3_PREADY ),
        .USER_REN_TRP1         ( Glue_Logic_0_USER_REN_TRP1 ),
        .USER_WEN_TRP1         ( Glue_Logic_0_USER_WEN_TRP1 ),
        .Flag_Int              ( Glue_Logic_0_Flag_Int_0 ),
        .MASK                  ( Glue_Logic_0_MASK ),
        .ADDR                  ( Glue_Logic_0_ADDR ),
        .PRDATA                ( CoreAPB3_0_APBmslave3_PRDATA ),
        .USER_RA_TRP1          ( Glue_Logic_0_USER_RA_TRP1_1 ),
        .MSG_TRP1              ( Glue_Logic_0_MSG_TRP1 ),
        .MSG_WA_TRP1           ( Glue_Logic_0_MSG_WA_TRP1 ) 
        );

//--------HDL_W_CPU_R_BUFF
HDL_W_CPU_R_BUFF HDL_W_CPU_R_BUFF_0(
        // Inputs
        .Clk                   ( RX_GPIO1 ),
        .Rst                   ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .DataI                 ( FALG_FINISH_A_0 ),
        .EnIP                  ( EnIP ),
        .Block_Err             ( C_1 ),
        .Flag_A_Rx             ( Flag_RX ),
        .Flag_B_Rx             ( Glue_Logic_0_Flag_B_Rx ),
        .IP_END_I              ( TC_Decode_0_IP_END_O ),
        // Outputs
        .Flag_A_HDL_Rec_Finish ( HDL_W_CPU_R_BUFF_0_Flag_A_HDL_Rec_Finish ),
        .Flag_B_HDL_Rec_Finish ( HDL_W_CPU_R_BUFF_0_Flag_B_HDL_Rec_Finish ),
        .USER_WEN_TRP1         ( HDL_W_CPU_R_BUFF_0_USER_WEN_TRP1 ),
        .Flag_OF               ( HDL_W_CPU_R_BUFF_0_Flag_OF ),
        .MSG_TRP1              ( HDL_W_CPU_R_BUFF_0_MSG_TRP1 ),
        .MSG_WA_TRP1           ( HDL_W_CPU_R_BUFF_0_MSG_WA_TRP1_1 ) 
        );

//--------INV
INV INV_0(
        // Inputs
        .A ( ANT_SW1_net_0 ),
        // Outputs
        .Y ( ANT_SW2_net_0 ) 
        );

//--------INV
INV INV_1(
        // Inputs
        .A ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        // Outputs
        .Y ( INV_1_Y ) 
        );

//--------OR2
OR2 OR2_0(
        // Inputs
        .A ( test_4463andcpuopration_MSS_0_GPIO_15_M2F ),
        .B ( GND_net ),
        // Outputs
        .Y ( PA4V_ON_net_0 ) 
        );

//--------test_4463andcpuopration_OSC_0_OSC   -   Actel:SgCore:OSC:2.0.101
test_4463andcpuopration_OSC_0_OSC OSC_0(
        // Inputs
        .XTL                ( XTL ),
        // Outputs
        .RCOSC_25_50MHZ_CCC (  ),
        .RCOSC_25_50MHZ_O2F (  ),
        .RCOSC_1MHZ_CCC     (  ),
        .RCOSC_1MHZ_O2F     (  ),
        .XTLOSC_CCC         (  ),
        .XTLOSC_O2F         ( OSC_0_XTLOSC_O2F ) 
        );

//--------parallel_to_serial
parallel_to_serial parallel_to_serial_0(
        // Inputs
        .ClkI         ( P_CLK ),
        .Rst          ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .EnI          ( TM_RSENC_0_RDY ),
        .DataI        ( TM_RSENC_0_CODEOUTP ),
        // Outputs
        .DataO        ( P_OUT ),
        .EnO          ( S_OUT_0 ),
        .En_Read_Buff ( En_read_buff ) 
        );

//--------Randomizer
Randomizer Randomizer_0(
        // Inputs
        .clk   ( P_CLK ),
        .reset ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .enb   ( S_OUT_0 ),
        .In1   ( P_OUT ),
        // Outputs
        .Out1  ( Randomizer_0_Out1 ),
        .enout ( Randomizer_0_enout ) 
        );

//--------RSControl
RSControl RSControl_0(
        // Inputs
        .ClkI_Dec8 ( divider_0_out_16dec ),
        .Rst       ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .RFD       ( TM_RSENC_0_RFD ),
        .RFS       ( TM_RSENC_0_RFS ),
        .EnI       ( ENCRC ),
        .DataI     ( CPU_W_HDL_R_0_DataO ),
        // Outputs
        .CLKEN     ( RSControl_0_CLKEN ),
        .NGRST     ( RSControl_0_NGRST ),
        .R_ST      ( RSControl_0_R_ST ),
        .START     ( RSControl_0_START ),
        .DATAINP   ( RSControl_0_DATAINP ) 
        );

//--------RXRandomize
RXRandomize RXRandomize_0(
        // Inputs
        .Clk        ( RX_GPIO1 ),
        .Rst        ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .EnI        ( ENASM_0 ),
        .DataI      ( A_1 ),
        .Block_ErrI ( CLTU_0_Block_ErrO ),
        .IP_END     ( CLTU_0_IP_END ),
        // Outputs
        .DataO      ( ENCRC_0 ),
        .EnO        ( pending_0 ),
        .Block_ErrO ( RXRandomize_0_Block_ErrO ),
        .IP_END_O   ( RXRandomize_0_IP_END_O ) 
        );

//--------Switch
Switch Switch_0(
        // Inputs
        .clk    ( P_CLK ),
        .Rst    ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .ASMIn  ( ASM_OUT ),
        .En_ASM ( ENASM_1 ),
        .EnRIn  ( Randomizer_0_enout ),
        .RIn    ( Randomizer_0_Out1 ),
        // Outputs
        .Out    ( c_in ),
        .EnOut  ( Switch_0_EnOut ) 
        );

//--------SYSRESET
SYSRESET SYSRESET_0(
        // Inputs
        .DEVRST_N         ( DEVRST_N ),
        // Outputs
        .POWER_ON_RESET_N ( SYSRESET_0_POWER_ON_RESET_N ) 
        );

//--------TC_Decode
TC_Decode TC_Decode_0(
        // Inputs
        .Clk          ( RX_GPIO1 ),
        .Rst          ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .DataI        ( ENCRC_0 ),
        .En_Data      ( pending_0 ),
        .Block_ErrI   ( RXRandomize_0_Block_ErrO ),
        .IP_END_I     ( RXRandomize_0_IP_END_O ),
        .MASK         ( Glue_Logic_0_MASK ),
        .ADDR         ( Glue_Logic_0_ADDR ),
        // Outputs
        .Block_ErrO   ( C_1 ),
        .DataO        ( FALG_FINISH_A_0 ),
        .En_DataO     ( EnIP ),
        .IP_END_O     ( TC_Decode_0_IP_END_O ),
        .SpacecraftID ( TC_Decode_0_SpacecraftID ) 
        );

//--------test_4463andcpuopration_MSS
test_4463andcpuopration_MSS test_4463andcpuopration_MSS_0(
        // Inputs
        .MCCC_CLK_BASE       ( OSC_0_XTLOSC_O2F ),
        .SPI_0_DI_F2M        ( SPI_MISO ),
        .SPI_0_CLK_F2M       ( SPI_CLK_net_0 ),
        .SPI_0_SS0_F2M       ( VCC_net ),
        .FIC_0_APB_M_PREADY  ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PREADY ),
        .FIC_0_APB_M_PSLVERR ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PSLVERR ),
        .MMUART_0_RXD_F2M    ( RXD4 ),
        .GPIO_26_F2M         ( ADC_INT ),
        .GPIO_27_F2M         ( GND_net ),
        .GPIO_28_F2M         ( GND_net ),
        .GPIO_30_F2M         ( L4V_PG ),
        .GPIO_31_F2M         ( BOARD_NO ),
        .CAN_RX_F2M          ( CAN1_TX ),
        .MSS_RESET_N_F2M     ( SYSRESET_0_POWER_ON_RESET_N ),
        .FIC_0_APB_M_PRDATA  ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PRDATA ),
        .MSS_INT_F2M         ( Glue_Logic_0_Flag_Int_0 ),
        // Outputs
        .SPI_0_DO_M2F        ( SPI_MOSI_net_0 ),
        .SPI_0_CLK_M2F       ( SPI_CLK_net_0 ),
        .SPI_0_SS0_M2F       (  ),
        .SPI_0_SS0_M2F_OE    (  ),
        .FIC_0_APB_M_PSEL    ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PSELx ),
        .FIC_0_APB_M_PWRITE  ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PWRITE ),
        .FIC_0_APB_M_PENABLE ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PENABLE ),
        .GPIO_1_M2F          ( test_4463andcpuopration_MSS_0_GPIO_1_M2F ),
        .GPIO_0_M2F          ( SW_net_0 ),
        .GPIO_5_M2F          (  ),
        .GPIO_8_M2F          ( ADC_CS_net_0 ),
        .GPIO_9_M2F          ( ADC_FS_net_0 ),
        .GPIO_10_M2F         ( ADC_PWDN_net_0 ),
        .GPIO_11_M2F         (  ),
        .GPIO_12_M2F         ( L4V_EN_net_0 ),
        .GPIO_13_M2F         ( ANT_SW1_net_0 ),
        .GPIO_14_M2F         (  ),
        .GPIO_15_M2F         ( test_4463andcpuopration_MSS_0_GPIO_15_M2F ),
        .GPIO_16_M2F         ( L3V3_EN2_net_0 ),
        .GPIO_17_M2F         ( L3V3_EN1_net_0 ),
        .GPIO_18_M2F         ( L4463_RX_CS_net_0 ),
        .GPIO_19_M2F         ( RX_SDN_net_0 ),
        .GPIO_20_M2F         ( L4463_TX_CS_net_0 ),
        .GPIO_21_M2F         ( TX_SDN_net_0 ),
        .GPIO_22_M2F         ( \A3.3V_EN_net_0  ),
        .GPIO_24_M2F         (  ),
        .GPIO_25_M2F         ( SPI_FRAM_CS_net_0 ),
        .MMUART_0_TXD_M2F    ( TXD4_0 ),
        .CAN_TX_M2F          ( CAN1_RX_net_0 ),
        .CAN_TX_EN_N_M2F     (  ),
        .GPIO_29_M2F         ( ADC_CSTART_net_0 ),
        .FIC_0_APB_M_PADDR   ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PADDR ),
        .FIC_0_APB_M_PWDATA  ( test_4463andcpuopration_MSS_0_FIC_0_APB_MASTER_PWDATA ) 
        );

//--------TM_RSENC
TM_RSENC TM_RSENC_0(
        // Inputs
        .CLK      ( divider_0_out_16dec ),
        .CLKEN    ( RSControl_0_CLKEN ),
        .NGRST    ( RSControl_0_NGRST ),
        .RST      ( RSControl_0_R_ST ),
        .START    ( RSControl_0_START ),
        .DATAINP  ( RSControl_0_DATAINP ),
        // Outputs
        .RDY      ( TM_RSENC_0_RDY ),
        .RFD      ( TM_RSENC_0_RFD ),
        .RFS      ( TM_RSENC_0_RFS ),
        .CODEOUTP ( TM_RSENC_0_CODEOUTP ) 
        );


endmodule
