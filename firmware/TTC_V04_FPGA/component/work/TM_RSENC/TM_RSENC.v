//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Fri Sep 16 12:54:02 2016
// Version: v11.7 11.7.0.119
//////////////////////////////////////////////////////////////////////

`timescale 1ns / 100ps

// TM_RSENC
module TM_RSENC(
    // Inputs
    CLK,
    CLKEN,
    DATAINP,
    NGRST,
    RST,
    START,
    // Outputs
    CODEOUTP,
    RDY,
    RFD,
    RFS
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input        CLK;
input        CLKEN;
input  [7:0] DATAINP;
input        NGRST;
input        RST;
input        START;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output [7:0] CODEOUTP;
output       RDY;
output       RFD;
output       RFS;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire         CLK;
wire         CLKEN;
wire   [7:0] CODEOUTP_0;
wire   [7:0] DATAINP;
wire         NGRST;
wire         RDY_net_0;
wire         RFD_net_0;
wire         RFS_net_0;
wire         RST;
wire         START;
wire         RDY_net_1;
wire         RFD_net_1;
wire         RFS_net_1;
wire   [7:0] CODEOUTP_0_net_0;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign RDY_net_1        = RDY_net_0;
assign RDY              = RDY_net_1;
assign RFD_net_1        = RFD_net_0;
assign RFD              = RFD_net_1;
assign RFS_net_1        = RFS_net_0;
assign RFS              = RFS_net_1;
assign CODEOUTP_0_net_0 = CODEOUTP_0;
assign CODEOUTP[7:0]    = CODEOUTP_0_net_0;
//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------TM_RSENC_TM_RSENC_0_CORERSENC   -   Actel:DirectCore:CORERSENC:3.4.103
TM_RSENC_TM_RSENC_0_CORERSENC #( 
        .CCSDS      ( 1 ),
        .CCSDS_CONV ( 1 ),
        .FAMILY     ( 19 ),
        .MM         ( 8 ),
        .NN         ( 255 ),
        .TT         ( 16 ) )
TM_RSENC_0(
        // Inputs
        .CLK      ( CLK ),
        .CLKEN    ( CLKEN ),
        .NGRST    ( NGRST ),
        .RST      ( RST ),
        .START    ( START ),
        .DATAINP  ( DATAINP ),
        // Outputs
        .RDY      ( RDY_net_0 ),
        .RFD      ( RFD_net_0 ),
        .RFS      ( RFS_net_0 ),
        .CODEOUTP ( CODEOUTP_0 ) 
        );


endmodule
