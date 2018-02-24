// ********************************************************************/
// Actel Corporation Proprietary and Confidential
// Copyright 2010 Actel Corporation.  All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
//
// Description:	COREAPB3_MUXPTOB3 - Central mux - signals from peripherals to
//				bridge.  Stand-alone module to allow ease of removal if an
//				alternative interconnection scheme is to be used.
//
// Revision Information:
// Date			Description
// ----			-----------------------------------------
// 05Feb10		Production Release Version 3.0
//
// SVN Revision Information:
// SVN $Revision: 16159 $
// SVN $Date: 2012-01-13 20:15:19 +0000 (Fri, 13 Jan 2012) $
//
// Resolved SARs
// SAR      Date     Who   Description
//
// Notes:
// 1. best viewed with tabstops set to "4" (tabs used throughout file)
//
// *********************************************************************/
`timescale 1ns/1ps
module COREAPB3_MUXPTOB3 (
input			[16:0]				PSELS,
input			[31:0]				PRDATAS0,
input			[31:0]				PRDATAS1,
input			[31:0]				PRDATAS2,
input			[31:0]				PRDATAS3,
input			[31:0]				PRDATAS4,
input			[31:0]				PRDATAS5,
input			[31:0]				PRDATAS6,
input			[31:0]				PRDATAS7,
input			[31:0]				PRDATAS8,
input			[31:0]				PRDATAS9,
input			[31:0]				PRDATAS10,
input			[31:0]				PRDATAS11,
input			[31:0]				PRDATAS12,
input			[31:0]				PRDATAS13,
input			[31:0]				PRDATAS14,
input			[31:0]				PRDATAS15,
input			[31:0]				PRDATAS16,
input			[16:0]				PREADYS,
input			[16:0]				PSLVERRS,
output	wire						PREADY,
output	wire						PSLVERR,
output	wire	[31:0]				PRDATA
);
localparam [4:0]PSEL_SL0	= 5'b00000;
localparam [4:0]PSEL_SL1	= 5'b00001;
localparam [4:0]PSEL_SL2	= 5'b00010;
localparam [4:0]PSEL_SL3	= 5'b00011;
localparam [4:0]PSEL_SL4	= 5'b00100;
localparam [4:0]PSEL_SL5	= 5'b00101;
localparam [4:0]PSEL_SL6	= 5'b00110;
localparam [4:0]PSEL_SL7	= 5'b00111;
localparam [4:0]PSEL_SL8	= 5'b01000;
localparam [4:0]PSEL_SL9	= 5'b01001;
localparam [4:0]PSEL_SL10	= 5'b01010;
localparam [4:0]PSEL_SL11	= 5'b01011;
localparam [4:0]PSEL_SL12	= 5'b01100;
localparam [4:0]PSEL_SL13	= 5'b01101;
localparam [4:0]PSEL_SL14	= 5'b01110;
localparam [4:0]PSEL_SL15	= 5'b01111;
localparam [4:0]PSEL_SL16   = 5'b10000;
reg						iPREADY;
reg						iPSLVERR;
reg		[31:0]			iPRDATA;
wire	[4:0]	PSELSBUS;  
wire	[31:0]	lo32;
assign lo32 = 32'b0;
assign PSELSBUS[4] = PSELS[16];
assign PSELSBUS[3] = PSELS[15] | PSELS[14] | PSELS[13] | PSELS[12] |
	PSELS[11] | PSELS[10] | PSELS[9] | PSELS[8];
assign PSELSBUS[2] = PSELS[15] | PSELS[14] | PSELS[13] | PSELS[12] |
	PSELS[7] | PSELS[6] | PSELS[5] | PSELS[4];
assign PSELSBUS[1] = PSELS[15] | PSELS[14] | PSELS[11] | PSELS[10] |
	PSELS[7] | PSELS[6] | PSELS[3] | PSELS[2];
assign PSELSBUS[0] = PSELS[15] | PSELS[13] | PSELS[11] | PSELS[9] |
	PSELS[7] | PSELS[5] | PSELS[3] | PSELS[1];
always @ (*)
begin
	case (PSELSBUS)
		PSEL_SL0 :
			if (PSELS[0])
				iPRDATA[31:0] = PRDATAS0[31:0];
			else
				iPRDATA[31:0] = lo32[31:0];
		PSEL_SL1  : iPRDATA[31:0] = PRDATAS1[31:0];
		PSEL_SL2  : iPRDATA[31:0] = PRDATAS2[31:0];
		PSEL_SL3  : iPRDATA[31:0] = PRDATAS3[31:0];
		PSEL_SL4  : iPRDATA[31:0] = PRDATAS4[31:0];
		PSEL_SL5  : iPRDATA[31:0] = PRDATAS5[31:0];
		PSEL_SL6  : iPRDATA[31:0] = PRDATAS6[31:0];
		PSEL_SL7  : iPRDATA[31:0] = PRDATAS7[31:0];
		PSEL_SL8  : iPRDATA[31:0] = PRDATAS8[31:0];
		PSEL_SL9  : iPRDATA[31:0] = PRDATAS9[31:0];
		PSEL_SL10 : iPRDATA[31:0] = PRDATAS10[31:0];
		PSEL_SL11 : iPRDATA[31:0] = PRDATAS11[31:0];
		PSEL_SL12 : iPRDATA[31:0] = PRDATAS12[31:0];
		PSEL_SL13 : iPRDATA[31:0] = PRDATAS13[31:0];
		PSEL_SL14 : iPRDATA[31:0] = PRDATAS14[31:0];
		PSEL_SL15 : iPRDATA[31:0] = PRDATAS15[31:0];
        PSEL_SL16 : iPRDATA[31:0] = PRDATAS16[31:0];
		default   : iPRDATA[31:0] = lo32[31:0];
	endcase
end
always @ (*)
begin
	case (PSELSBUS)
		PSEL_SL0 :
			if (PSELS[0])
				iPREADY = PREADYS[0];
			else
				iPREADY = 1'b1;
		PSEL_SL1  : iPREADY = PREADYS[1];
		PSEL_SL2  : iPREADY = PREADYS[2];
		PSEL_SL3  : iPREADY = PREADYS[3];
		PSEL_SL4  : iPREADY = PREADYS[4];
		PSEL_SL5  : iPREADY = PREADYS[5];
		PSEL_SL6  : iPREADY = PREADYS[6];
		PSEL_SL7  : iPREADY = PREADYS[7];
		PSEL_SL8  : iPREADY = PREADYS[8];
		PSEL_SL9  : iPREADY = PREADYS[9];
		PSEL_SL10 : iPREADY = PREADYS[10];
		PSEL_SL11 : iPREADY = PREADYS[11];
		PSEL_SL12 : iPREADY = PREADYS[12];
		PSEL_SL13 : iPREADY = PREADYS[13];
		PSEL_SL14 : iPREADY = PREADYS[14];
		PSEL_SL15 : iPREADY = PREADYS[15];
		PSEL_SL16 : iPREADY = PREADYS[16];
		default   : iPREADY = 1'b1;
	endcase
end
always @ (*)
begin
	case (PSELSBUS)
		PSEL_SL0 :
			if (PSELS[0])
				iPSLVERR = PSLVERRS[0];
			else
				iPSLVERR = 1'b0;
		PSEL_SL1  : iPSLVERR = PSLVERRS[1];
		PSEL_SL2  : iPSLVERR = PSLVERRS[2];
		PSEL_SL3  : iPSLVERR = PSLVERRS[3];
		PSEL_SL4  : iPSLVERR = PSLVERRS[4];
		PSEL_SL5  : iPSLVERR = PSLVERRS[5];
		PSEL_SL6  : iPSLVERR = PSLVERRS[6];
		PSEL_SL7  : iPSLVERR = PSLVERRS[7];
		PSEL_SL8  : iPSLVERR = PSLVERRS[8];
		PSEL_SL9  : iPSLVERR = PSLVERRS[9];
		PSEL_SL10 : iPSLVERR = PSLVERRS[10];
		PSEL_SL11 : iPSLVERR = PSLVERRS[11];
		PSEL_SL12 : iPSLVERR = PSLVERRS[12];
		PSEL_SL13 : iPSLVERR = PSLVERRS[13];
		PSEL_SL14 : iPSLVERR = PSLVERRS[14];
		PSEL_SL15 : iPSLVERR = PSLVERRS[15];
		PSEL_SL16 : iPSLVERR = PSLVERRS[16];
		default   : iPSLVERR = 1'b0;
	endcase
end
assign PREADY	= iPREADY;
assign PSLVERR	= iPSLVERR;
assign PRDATA	= iPRDATA[31:0];
endmodule
