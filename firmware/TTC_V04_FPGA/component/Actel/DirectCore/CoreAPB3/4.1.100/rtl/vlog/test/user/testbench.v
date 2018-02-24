// ********************************************************************
// Actel Corporation Proprietary and Confidential
//  Copyright 2010 Actel Corporation.  All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
//
// Description:	User Testbench for CoreAPB3
//
// Revision Information:
// Date     Description
// 05Feb10		Production Release Version 3.0
//
// SVN Revision Information:
// SVN $Revision: 23124 $
// SVN $Date: 2014-07-17 15:31:27 +0100 (Thu, 17 Jul 2014) $
//
// Resolved SARs
// SAR      Date     Who   Description
//
// Notes:
// 1. best viewed with tabstops set to "4"
// 2. Most of the behavior is driven from the BFM script for the APB
//    master.  Consult the Actel AMBA BFM documentation for more information.
//
// History:		1/28/10  - TFB created
//
// *********************************************************************

`timescale 1ns/1ps


module testbench ();

// location of this can be overridden at compile time (+incdir switch)
//`include "../../../../coreparameters.v"
`include "coreparameters.v"

//parameter RANGESIZE = 268435456;
//parameter IADDR_ENABLE = 0;


//-----------------------------------------------------------------------------
// top-level parameters
//-----------------------------------------------------------------------------

// vector files for driving the APB master BFM
// NOTE: location of the following files can be overridden at run time
parameter APB_MASTER_VECTFILE	= "coreapb3_usertb_master.vec";

// APB Master System Clock cycle (in ns)
parameter APB_MASTER_CLK_CYCLE	= 30;

// propagation delay in ns
parameter TPD			= 3;


// BFM slave constants
//localparam SLAVE_AWIDTH		= 8;
localparam SLAVE_AWIDTH		= 8;
localparam SLAVE_DEPTH		= (2**SLAVE_AWIDTH) ;


// System signals
reg							SYSRSTN_apb;
reg							SYSCLK_apb;

// APB master bfm signals
wire						PCLK;
wire						PRESETN;
wire	[31:0]				PADDR_apb_bfm_wide;
//wire	[23:0]				PADDR;
wire	[31:0]				PADDR;
wire	[15:0]				PSEL_apb_bfm_wide;
wire						PSEL;
wire						PENABLE;
wire						PWRITE;
wire	[31:0]				PWDATA_apb_bfm_wide;
//wire	[APB_DWIDTH-1:0]	PWDATA;
wire	[31:0]				PWDATA;

// input to bfm
wire	[31:0]				PRDATA_apb_bfm_wide;
//wire	[APB_DWIDTH-1:0]	PRDATA;
wire	[31:0]				PRDATA;
wire						PREADY;
wire						PSLVERR;

// input to APB bfm
//wire	[31:0]				GP_IN_apb_bfm=32'b0;
//wire	[31:0]				GP_OUT_apb_bfm;
wire						FINISHED_apb_bfm;
wire						FAILED_apb_bfm;


// misc. signals
wire	[255:0]				GND256=256'b0;
wire	[31:0]				GND32=32'b0;
wire						GND1=1'b0;
reg							stopsim=0;


//wire	[23:0]				PADDRS;
wire	[31:0]				PADDRS;
//wire	[(((1-IADDR_ENABLE)*24)+((IADDR_ENABLE)*32))-1:0] PADDRS0;
wire						PWRITES;
wire						PENABLES;
//wire	[APB_DWIDTH-1:0]	PWDATAS;
wire	[31:0]				PWDATAS;
wire						PSELS0;
wire						PSELS1;
wire						PSELS2;
wire						PSELS3;
wire						PSELS4;
wire						PSELS5;
wire						PSELS6;
wire						PSELS7;
wire						PSELS8;
wire						PSELS9;
wire						PSELS10;
wire						PSELS11;
wire						PSELS12;
wire						PSELS13;
wire						PSELS14;
wire						PSELS15;
wire						PSELS16;

/*
wire	[APB_DWIDTH-1:0]	PRDATAS0;
wire	[APB_DWIDTH-1:0]	PRDATAS1;
wire	[APB_DWIDTH-1:0]	PRDATAS2;
wire	[APB_DWIDTH-1:0]	PRDATAS3;
wire	[APB_DWIDTH-1:0]	PRDATAS4;
wire	[APB_DWIDTH-1:0]	PRDATAS5;
wire	[APB_DWIDTH-1:0]	PRDATAS6;
wire	[APB_DWIDTH-1:0]	PRDATAS7;
wire	[APB_DWIDTH-1:0]	PRDATAS8;
wire	[APB_DWIDTH-1:0]	PRDATAS9;
wire	[APB_DWIDTH-1:0]	PRDATAS10;
wire	[APB_DWIDTH-1:0]	PRDATAS11;
wire	[APB_DWIDTH-1:0]	PRDATAS12;
wire	[APB_DWIDTH-1:0]	PRDATAS13;
wire	[APB_DWIDTH-1:0]	PRDATAS14;
wire	[APB_DWIDTH-1:0]	PRDATAS15;
*/
wire	[31:0]				PRDATAS0;
wire	[31:0]				PRDATAS1;
wire	[31:0]				PRDATAS2;
wire	[31:0]				PRDATAS3;
wire	[31:0]				PRDATAS4;
wire	[31:0]				PRDATAS5;
wire	[31:0]				PRDATAS6;
wire	[31:0]				PRDATAS7;
wire	[31:0]				PRDATAS8;
wire	[31:0]				PRDATAS9;
wire	[31:0]				PRDATAS10;
wire	[31:0]				PRDATAS11;
wire	[31:0]				PRDATAS12;
wire	[31:0]				PRDATAS13;
wire	[31:0]				PRDATAS14;
wire	[31:0]				PRDATAS15;
wire	[31:0]				PRDATAS16;


wire						PREADYS0;
wire						PREADYS1;
wire						PREADYS2;
wire						PREADYS3;
wire						PREADYS4;
wire						PREADYS5;
wire						PREADYS6;
wire						PREADYS7;
wire						PREADYS8;
wire						PREADYS9;
wire						PREADYS10;
wire						PREADYS11;
wire						PREADYS12;
wire						PREADYS13;
wire						PREADYS14;
wire						PREADYS15;
wire						PREADYS16;

wire						PSLVERRS0;
wire						PSLVERRS1;
wire						PSLVERRS2;
wire						PSLVERRS3;
wire						PSLVERRS4;
wire						PSLVERRS5;
wire						PSLVERRS6;
wire						PSLVERRS7;
wire						PSLVERRS8;
wire						PSLVERRS9;
wire						PSLVERRS10;
wire						PSLVERRS11;
wire						PSLVERRS12;
wire						PSLVERRS13;
wire						PSLVERRS14;
wire						PSLVERRS15;
wire						PSLVERRS16;

/*
wire	[31:0]				PWDATAS_apb_slave;
wire	[31:0]				PRDATAS0_apb_slave;
wire	[31:0]				PRDATAS1_apb_slave;
wire	[31:0]				PRDATAS2_apb_slave;
wire	[31:0]				PRDATAS3_apb_slave;
wire	[31:0]				PRDATAS4_apb_slave;
wire	[31:0]				PRDATAS5_apb_slave;
wire	[31:0]				PRDATAS6_apb_slave;
wire	[31:0]				PRDATAS7_apb_slave;
wire	[31:0]				PRDATAS8_apb_slave;
wire	[31:0]				PRDATAS9_apb_slave;
wire	[31:0]				PRDATAS10_apb_slave;
wire	[31:0]				PRDATAS11_apb_slave;
wire	[31:0]				PRDATAS12_apb_slave;
wire	[31:0]				PRDATAS13_apb_slave;
wire	[31:0]				PRDATAS14_apb_slave;
wire	[31:0]				PRDATAS15_apb_slave;
*/

reg                 s0_write;
reg                 s1_write;
reg                 s2_write;
reg                 s3_write;
reg                 s4_write;
reg                 s5_write;
reg                 s6_write;
reg                 s7_write;
reg                 s8_write;
reg                 s9_write;
reg                 s10_write;
reg                 s11_write;
reg                 s12_write;
reg                 s13_write;
reg                 s14_write;
reg                 s15_write;
reg                 s16_write;

// BFM GPIO
wire	[31:0]		GP_OUT;
wire	[31:0]		GP_IN;


// instantiate DUT(s)
CoreAPB3 #(
	.APB_DWIDTH(APB_DWIDTH),
	.IADDR_OPTION(IADDR_OPTION),
	.APBSLOT0ENABLE(APBSLOT0ENABLE),
	.APBSLOT1ENABLE(APBSLOT1ENABLE),
	.APBSLOT2ENABLE(APBSLOT2ENABLE),
	.APBSLOT3ENABLE(APBSLOT3ENABLE),
	.APBSLOT4ENABLE(APBSLOT4ENABLE),
	.APBSLOT5ENABLE(APBSLOT5ENABLE),
	.APBSLOT6ENABLE(APBSLOT6ENABLE),
	.APBSLOT7ENABLE(APBSLOT7ENABLE),
	.APBSLOT8ENABLE(APBSLOT8ENABLE),
	.APBSLOT9ENABLE(APBSLOT9ENABLE),
	.APBSLOT10ENABLE(APBSLOT10ENABLE),
	.APBSLOT11ENABLE(APBSLOT11ENABLE),
	.APBSLOT12ENABLE(APBSLOT12ENABLE),
	.APBSLOT13ENABLE(APBSLOT13ENABLE),
	.APBSLOT14ENABLE(APBSLOT14ENABLE),
	.APBSLOT15ENABLE(APBSLOT15ENABLE),
	.MADDR_BITS(MADDR_BITS),
	.UPR_NIBBLE_POSN(UPR_NIBBLE_POSN),
	.SC_0  (SC_0 ),
	.SC_1  (SC_1 ),
	.SC_2  (SC_2 ),
	.SC_3  (SC_3 ),
	.SC_4  (SC_4 ),
	.SC_5  (SC_5 ),
	.SC_6  (SC_6 ),
	.SC_7  (SC_7 ),
	.SC_8  (SC_8 ),
	.SC_9  (SC_9 ),
	.SC_10 (SC_10),
	.SC_11 (SC_11),
	.SC_12 (SC_12),
	.SC_13 (SC_13),
	.SC_14 (SC_14),
	.SC_15 (SC_15),
	.FAMILY     (FAMILY)
) u_coreapb3 (
    .IADDR(32'b0),
	.PRESETN(PRESETN),
	.PCLK(PCLK),
	.PADDR(PADDR),
	.PWRITE(PWRITE),
	.PENABLE(PENABLE),
	.PSEL(PSEL),
	.PWDATA(PWDATA),
	.PRDATA(PRDATA),
	.PREADY(PREADY),
	.PSLVERR(PSLVERR),
	.PADDRS(PADDRS),
	.PWRITES(PWRITES),
	.PENABLES(PENABLES),
	.PWDATAS(PWDATAS),
	.PSELS0(PSELS0),
	.PSELS1(PSELS1),
	.PSELS2(PSELS2),
	.PSELS3(PSELS3),
	.PSELS4(PSELS4),
	.PSELS5(PSELS5),
	.PSELS6(PSELS6),
	.PSELS7(PSELS7),
	.PSELS8(PSELS8),
	.PSELS9(PSELS9),
	.PSELS10(PSELS10),
	.PSELS11(PSELS11),
	.PSELS12(PSELS12),
	.PSELS13(PSELS13),
	.PSELS14(PSELS14),
	.PSELS15(PSELS15),
	.PSELS16(PSELS16),
	.PRDATAS0(PRDATAS0),
	.PRDATAS1(PRDATAS1),
	.PRDATAS2(PRDATAS2),
	.PRDATAS3(PRDATAS3),
	.PRDATAS4(PRDATAS4),
	.PRDATAS5(PRDATAS5),
	.PRDATAS6(PRDATAS6),
	.PRDATAS7(PRDATAS7),
	.PRDATAS8(PRDATAS8),
	.PRDATAS9(PRDATAS9),
	.PRDATAS10(PRDATAS10),
	.PRDATAS11(PRDATAS11),
	.PRDATAS12(PRDATAS12),
	.PRDATAS13(PRDATAS13),
	.PRDATAS14(PRDATAS14),
	.PRDATAS15(PRDATAS15),
	.PRDATAS16(PRDATAS16),
	.PREADYS0(PREADYS0),
	.PREADYS1(PREADYS1),
	.PREADYS2(PREADYS2),
	.PREADYS3(PREADYS3),
	.PREADYS4(PREADYS4),
	.PREADYS5(PREADYS5),
	.PREADYS6(PREADYS6),
	.PREADYS7(PREADYS7),
	.PREADYS8(PREADYS8),
	.PREADYS9(PREADYS9),
	.PREADYS10(PREADYS10),
	.PREADYS11(PREADYS11),
	.PREADYS12(PREADYS12),
	.PREADYS13(PREADYS13),
	.PREADYS14(PREADYS14),
	.PREADYS15(PREADYS15),
	.PREADYS16(PREADYS16),
	.PSLVERRS0(PSLVERRS0),
	.PSLVERRS1(PSLVERRS1),
	.PSLVERRS2(PSLVERRS2),
	.PSLVERRS3(PSLVERRS3),
	.PSLVERRS4(PSLVERRS4),
	.PSLVERRS5(PSLVERRS5),
	.PSLVERRS6(PSLVERRS6),
	.PSLVERRS7(PSLVERRS7),
	.PSLVERRS8(PSLVERRS8),
	.PSLVERRS9(PSLVERRS9),
	.PSLVERRS10(PSLVERRS10),
	.PSLVERRS11(PSLVERRS11),
	.PSLVERRS12(PSLVERRS12),
	.PSLVERRS13(PSLVERRS13),
	.PSLVERRS14(PSLVERRS14),
	.PSLVERRS15(PSLVERRS15),
	.PSLVERRS16(PSLVERRS16)
);
//assign PRDATA_apb_bfm_wide		= {{32-APB_DWIDTH{1'b0}},PRDATA};
assign PRDATA_apb_bfm_wide		= PRDATA;

// BFM monitors various signals
assign GP_IN = {
	15'b0,              // 31:17
    s16_write,          // 16
    s15_write,          // 15
    s14_write,          // 14
    s13_write,          // 13
    s12_write,          // 12
    s11_write,          // 11
    s10_write,          // 10
    s9_write,           //  9
    s8_write,           //  8
    s7_write,           //  7
    s6_write,           //  6
    s5_write,           //  5
    s4_write,           //  4
    s3_write,           //  3
    s2_write,           //  2
    s1_write,           //  1
    s0_write            //  0
};


// instantiate APB Master BFM to drive APB mirrored master port
BFM_APB #(
	.VECTFILE(APB_MASTER_VECTFILE),
	.TPD(TPD),
	// passing testbench parameters to BFM ARGVALUE* parameters
	.ARGVALUE0  (APB_DWIDTH),
	.ARGVALUE1  (IADDR_OPTION),
	.ARGVALUE2  (APBSLOT0ENABLE),
	.ARGVALUE3  (APBSLOT1ENABLE),
	.ARGVALUE4  (APBSLOT2ENABLE),
	.ARGVALUE5  (APBSLOT3ENABLE),
	.ARGVALUE6  (APBSLOT4ENABLE),
	.ARGVALUE7  (APBSLOT5ENABLE),
	.ARGVALUE8  (APBSLOT6ENABLE),
	.ARGVALUE9  (APBSLOT7ENABLE),
	.ARGVALUE10 (APBSLOT8ENABLE),
	.ARGVALUE11 (APBSLOT9ENABLE),
	.ARGVALUE12 (APBSLOT10ENABLE),
	.ARGVALUE13 (APBSLOT11ENABLE),
	.ARGVALUE14 (APBSLOT12ENABLE),
	.ARGVALUE15 (APBSLOT13ENABLE),
	.ARGVALUE16 (APBSLOT14ENABLE),
	.ARGVALUE17 (APBSLOT15ENABLE),
	.ARGVALUE18 (MADDR_BITS),
	.ARGVALUE19 (UPR_NIBBLE_POSN),
	.ARGVALUE20 (SC_0),
	.ARGVALUE21 (SC_1),
	.ARGVALUE22 (SC_2),
	.ARGVALUE23 (SC_3),
	.ARGVALUE24 (SC_4),
	.ARGVALUE25 (SC_5),
	.ARGVALUE26 (SC_6),
	.ARGVALUE27 (SC_7),
	.ARGVALUE28 (SC_8),
	.ARGVALUE29 (SC_9),
	.ARGVALUE30 (SC_10),
	.ARGVALUE31 (SC_11),
	.ARGVALUE32 (SC_12),
	.ARGVALUE33 (SC_13),
	.ARGVALUE34 (SC_14),
	.ARGVALUE35 (SC_15)
) u_apb_master (
	.SYSCLK(SYSCLK_apb),
	.SYSRSTN(SYSRSTN_apb),
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PADDR(PADDR_apb_bfm_wide),
	.PSEL(PSEL_apb_bfm_wide),
	.PENABLE(PENABLE),
	.PWRITE(PWRITE),
	.PWDATA(PWDATA_apb_bfm_wide),
	.PRDATA(PRDATA_apb_bfm_wide),
	.PREADY(PREADY),
	.PSLVERR(PSLVERR),
	.INTERRUPT(GND256),
	.GP_OUT(GP_OUT),
	.GP_IN(GP_IN),
	.EXT_WR(),
	.EXT_RD(),
	.EXT_ADDR(),
	.EXT_DATA(),
	.EXT_WAIT(GND1),
	.FINISHED(FINISHED_apb_bfm),
	.FAILED(FAILED_apb_bfm)
);
//assign PADDR				= PADDR_apb_bfm_wide[23:0];
assign PADDR				= PADDR_apb_bfm_wide[31:0];
//assign PSEL					= PSEL_apb_bfm_wide[0];
assign PSEL					= PSEL_apb_bfm_wide[ 0] ||
                              PSEL_apb_bfm_wide[ 1] ||
                              PSEL_apb_bfm_wide[ 2] ||
                              PSEL_apb_bfm_wide[ 3] ||
                              PSEL_apb_bfm_wide[ 4] ||
                              PSEL_apb_bfm_wide[ 5] ||
                              PSEL_apb_bfm_wide[ 6] ||
                              PSEL_apb_bfm_wide[ 7] ||
                              PSEL_apb_bfm_wide[ 8] ||
                              PSEL_apb_bfm_wide[ 9] ||
                              PSEL_apb_bfm_wide[10] ||
                              PSEL_apb_bfm_wide[11] ||
                              PSEL_apb_bfm_wide[12] ||
                              PSEL_apb_bfm_wide[13] ||
                              PSEL_apb_bfm_wide[14] ||
                              PSEL_apb_bfm_wide[15] ;
//assign PWDATA				= PWDATA_apb_bfm_wide[APB_DWIDTH-1:0];
assign PWDATA				= PWDATA_apb_bfm_wide;

// BFM slave 0
BFM_APBSLAVE#(
	//.AWIDTH(((1-IADDR_ENABLE)*SLAVE_AWIDTH)+((IADDR_ENABLE)*28)),
	//.DEPTH(((1-IADDR_ENABLE)*SLAVE_DEPTH)+((IADDR_ENABLE)*(2**28))),
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave0 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS0),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	//.PADDR(PADDRS0[(((1-IADDR_ENABLE)*SLAVE_AWIDTH)+((IADDR_ENABLE)*28)-1):0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS0),
	.PREADY(PREADYS0),
	.PSLVERR(PSLVERRS0)
);

// BFM slave 1
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave1 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS1),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS1),
	.PREADY(PREADYS1),
	.PSLVERR(PSLVERRS1)
);

// BFM slave 2
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave2 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS2),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS2),
	.PREADY(PREADYS2),
	.PSLVERR(PSLVERRS2)
);

// BFM slave 3
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave3 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS3),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS3),
	.PREADY(PREADYS3),
	.PSLVERR(PSLVERRS3)
);

// BFM slave 4
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave4 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS4),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS4),
	.PREADY(PREADYS4),
	.PSLVERR(PSLVERRS4)
);

// BFM slave 5
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave5 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS5),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS5),
	.PREADY(PREADYS5),
	.PSLVERR(PSLVERRS5)
);

// BFM slave 6
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave6 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS6),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS6),
	.PREADY(PREADYS6),
	.PSLVERR(PSLVERRS6)
);

// BFM slave 7
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave7 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS7),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS7),
	.PREADY(PREADYS7),
	.PSLVERR(PSLVERRS7)
);

// BFM slave 8
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave8 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS8),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS8),
	.PREADY(PREADYS8),
	.PSLVERR(PSLVERRS8)
);

// BFM slave 9
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave9 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS9),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS9),
	.PREADY(PREADYS9),
	.PSLVERR(PSLVERRS9)
);

// BFM slave 10
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave10 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS10),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS10),
	.PREADY(PREADYS10),
	.PSLVERR(PSLVERRS10)
);

// BFM slave 11
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave11 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS11),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS11),
	.PREADY(PREADYS11),
	.PSLVERR(PSLVERRS11)
);

// BFM slave 12
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave12 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS12),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS12),
	.PREADY(PREADYS12),
	.PSLVERR(PSLVERRS12)
);

// BFM slave 13
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave13 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS13),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS13),
	.PREADY(PREADYS13),
	.PSLVERR(PSLVERRS13)
);

// BFM slave 14
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave14 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS14),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS14),
	.PREADY(PREADYS14),
	.PSLVERR(PSLVERRS14)
);

// BFM slave 15
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave15 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS15),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS15),
	.PREADY(PREADYS15),
	.PSLVERR(PSLVERRS15)
);

// BFM slave 16 (combined region)
BFM_APBSLAVE #(
	.AWIDTH(SLAVE_AWIDTH),
	.DEPTH(SLAVE_DEPTH),
	.DWIDTH(32),
	.TPD(TPD)
) u_slave16 (
	.PCLK(PCLK),
	.PRESETN(PRESETN),
	.PENABLE(PENABLES),
	.PWRITE(PWRITES),
	.PSEL(PSELS16),
	.PADDR(PADDRS[SLAVE_AWIDTH-1:0]),
	.PWDATA(PWDATAS),
	.PRDATA(PRDATAS16),
	.PREADY(PREADYS16),
	.PSLVERR(PSLVERRS16)
);


/*
// adjust busses to widths
assign PWDATAS_apb_slave = {{32-APB_DWIDTH{1'b0}},PWDATAS[APB_DWIDTH-1:0]};
assign PRDATAS0		=	PRDATAS0_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS1		=	PRDATAS1_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS2		=	PRDATAS2_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS3		=	PRDATAS3_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS4		=	PRDATAS4_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS5		=	PRDATAS5_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS6		=	PRDATAS6_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS7		=	PRDATAS7_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS8		=	PRDATAS8_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS9		=	PRDATAS9_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS10	=	PRDATAS10_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS11	=	PRDATAS11_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS12	=	PRDATAS12_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS13	=	PRDATAS13_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS14	=	PRDATAS14_apb_slave[APB_DWIDTH-1:0];
assign PRDATAS15	=	PRDATAS15_apb_slave[APB_DWIDTH-1:0];
*/

    //---------------------------------------------------------------------
    // Detect writes to individual slots
    //---------------------------------------------------------------------
    always @(posedge PCLK or negedge PRESETN)
    begin
        if (!PRESETN)
        begin
            s0_write  <= 1'b0;
            s1_write  <= 1'b0;
            s2_write  <= 1'b0;
            s3_write  <= 1'b0;
            s4_write  <= 1'b0;
            s5_write  <= 1'b0;
            s6_write  <= 1'b0;
            s7_write  <= 1'b0;
            s8_write  <= 1'b0;
            s9_write  <= 1'b0;
            s10_write <= 1'b0;
            s11_write <= 1'b0;
            s12_write <= 1'b0;
            s13_write <= 1'b0;
            s14_write <= 1'b0;
            s15_write <= 1'b0;
            s16_write <= 1'b0;
        end
        else
        begin
            // Set write indication bits
            if (PSELS0  && PWRITES) s0_write  <= 1'b1;                       // && PENABLES
            if (PSELS1  && PWRITES) s1_write  <= 1'b1;                       // && PENABLES
            if (PSELS2  && PWRITES) s2_write  <= 1'b1;                       // && PENABLES
            if (PSELS3  && PWRITES) s3_write  <= 1'b1;                       // && PENABLES
            if (PSELS4  && PWRITES) s4_write  <= 1'b1;                       // && PENABLES
            if (PSELS5  && PWRITES) s5_write  <= 1'b1;                       // && PENABLES
            if (PSELS6  && PWRITES) s6_write  <= 1'b1;                       // && PENABLES
            if (PSELS7  && PWRITES) s7_write  <= 1'b1;                       // && PENABLES
            if (PSELS8  && PWRITES) s8_write  <= 1'b1;                       // && PENABLES
            if (PSELS9  && PWRITES) s9_write  <= 1'b1;                       // && PENABLES
            if (PSELS10 && PWRITES) s10_write <= 1'b1;                       // && PENABLES
            if (PSELS11 && PWRITES) s11_write <= 1'b1;                       // && PENABLES
            if (PSELS12 && PWRITES) s12_write <= 1'b1;                       // && PENABLES
            if (PSELS13 && PWRITES) s13_write <= 1'b1;                       // && PENABLES
            if (PSELS14 && PWRITES) s14_write <= 1'b1;                       // && PENABLES
            if (PSELS15 && PWRITES) s15_write <= 1'b1;                       // && PENABLES
            if (PSELS16 && PWRITES) s16_write <= 1'b1;                       // && PENABLES
            // Clear write indication bits
            if (GP_OUT[0])  s0_write  <= 1'b0;
            if (GP_OUT[1])  s1_write  <= 1'b0;
            if (GP_OUT[2])  s2_write  <= 1'b0;
            if (GP_OUT[3])  s3_write  <= 1'b0;
            if (GP_OUT[4])  s4_write  <= 1'b0;
            if (GP_OUT[5])  s5_write  <= 1'b0;
            if (GP_OUT[6])  s6_write  <= 1'b0;
            if (GP_OUT[7])  s7_write  <= 1'b0;
            if (GP_OUT[8])  s8_write  <= 1'b0;
            if (GP_OUT[9])  s9_write  <= 1'b0;
            if (GP_OUT[10]) s10_write <= 1'b0;
            if (GP_OUT[11]) s11_write <= 1'b0;
            if (GP_OUT[12]) s12_write <= 1'b0;
            if (GP_OUT[13]) s13_write <= 1'b0;
            if (GP_OUT[14]) s14_write <= 1'b0;
            if (GP_OUT[15]) s15_write <= 1'b0;
            if (GP_OUT[16]) s16_write <= 1'b0;
        end
    end


// System Clocks
initial SYSCLK_apb = 1'b0;
always #(APB_MASTER_CLK_CYCLE/2) SYSCLK_apb = ~SYSCLK_apb;

// Main simulation
initial
begin: main_sim
	SYSRSTN_apb	= 0;
	@ (posedge SYSCLK_apb); #TPD;
	SYSRSTN_apb	= 1;

	// wait until BFM is finished
	while (!(FINISHED_apb_bfm===1'b1))
	begin
		@ (posedge SYSCLK_apb); #TPD;
	end
	stopsim=1;
	#1;
	$stop;
end

endmodule // testbench
