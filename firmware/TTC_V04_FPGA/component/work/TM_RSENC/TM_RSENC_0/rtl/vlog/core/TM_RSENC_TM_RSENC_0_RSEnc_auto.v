// Microsemi Corporation Proprietary and Confidential.
// Copyright 2012 Microsemi Corporation.  All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE MICROSEMI LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
// 
// Description: CoreRSEnc - Reed-Solomon Encoder 
//              Configuration-dependent RTL modules/entities 
// 
// Revision Information:
// Date         Descripion
// ----------------------------------------------------------
// 15Nov10    Production Release Version 3.0
// 05Jan12    CCSDS coverage added      v3.1

// SVN Revision Information:
// SVN $Revision: $
// SVN $Data: $

// Resolved SARs
// SAR     Date    Who         Description
// 
// Notes: 
// 
// This is an automatically generated file

 `timescale 1 ns/100 ps

//======== hdl_gf_multComb_2: combinatorial Mult ========

module TM_RSENC_TM_RSENC_0_gf_mult (a, b, prod);
  parameter MM = 4;

  input [MM-1:0] a, b;
  output[MM-1:0] prod;

// Square Matrix A (elements are groupped by columns)
  wire a00 = a[0];
  wire a10 = a[7];
  wire a20 = a[6]^a[7];
  wire a30 = a[5]^a[6]^a[7];
  wire a40 = a[4]^a[5]^a[6]^a[7];
  wire a50 = a[3]^a[4]^a[5]^a[6]^a[7];
  wire a60 = a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a70 = a[1]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a01 = a[1];
  wire a11 = a[0]^a[7];
  wire a21 = a[7]^a[6]^a[7];
  wire a31 = a[6]^a[7]^a[5]^a[6]^a[7];
  wire a41 = a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7];
  wire a51 = a[4]^a[5]^a[6]^a[7]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a61 = a[3]^a[4]^a[5]^a[6]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a71 = a[2]^a[3]^a[4]^a[5]^a[6]^a[7]^a[1]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a02 = a[2];
  wire a12 = a[1]^a[7];
  wire a22 = a[0]^a[7]^a[6]^a[7];
  wire a32 = a[7]^a[6]^a[7]^a[5]^a[6]^a[7];
  wire a42 = a[6]^a[7]^a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7];
  wire a52 = a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a62 = a[4]^a[5]^a[6]^a[7]^a[3]^a[4]^a[5]^a[6]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a72 = a[3]^a[4]^a[5]^a[6]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7]^a[1]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a03 = a[3];
  wire a13 = a[2];
  wire a23 = a[1]^a[7];
  wire a33 = a[0]^a[7]^a[6]^a[7];
  wire a43 = a[7]^a[6]^a[7]^a[5]^a[6]^a[7];
  wire a53 = a[6]^a[7]^a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7];
  wire a63 = a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a73 = a[4]^a[5]^a[6]^a[7]^a[3]^a[4]^a[5]^a[6]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a04 = a[4];
  wire a14 = a[3];
  wire a24 = a[2];
  wire a34 = a[1]^a[7];
  wire a44 = a[0]^a[7]^a[6]^a[7];
  wire a54 = a[7]^a[6]^a[7]^a[5]^a[6]^a[7];
  wire a64 = a[6]^a[7]^a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7];
  wire a74 = a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a05 = a[5];
  wire a15 = a[4];
  wire a25 = a[3];
  wire a35 = a[2];
  wire a45 = a[1]^a[7];
  wire a55 = a[0]^a[7]^a[6]^a[7];
  wire a65 = a[7]^a[6]^a[7]^a[5]^a[6]^a[7];
  wire a75 = a[6]^a[7]^a[5]^a[6]^a[7]^a[4]^a[5]^a[6]^a[7];
  wire a06 = a[6];
  wire a16 = a[5];
  wire a26 = a[4];
  wire a36 = a[3];
  wire a46 = a[2];
  wire a56 = a[1]^a[7];
  wire a66 = a[0]^a[7]^a[6]^a[7];
  wire a76 = a[7]^a[6]^a[7]^a[5]^a[6]^a[7];
  wire a07 = a[7];
  wire a17 = a[6]^a[7];
  wire a27 = a[5]^a[6]^a[7];
  wire a37 = a[4]^a[5]^a[6]^a[7];
  wire a47 = a[3]^a[4]^a[5]^a[6]^a[7];
  wire a57 = a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a67 = a[1]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];
  wire a77 = a[0]^a[7]^a[6]^a[7]^a[1]^a[7]^a[2]^a[3]^a[4]^a[5]^a[6]^a[7];

  // Row_mat by square_mat product

  wire[MM-1:0] row_prod;

  assign row_prod[0] = b[0]&a00 ^ b[1]&a10 ^ b[2]&a20 ^ b[3]&a30 ^ b[4]&a40 ^ b[5]&a50 ^ b[6]&a60 ^ b[7]&a70 ;
  assign row_prod[1] = b[0]&a01 ^ b[1]&a11 ^ b[2]&a21 ^ b[3]&a31 ^ b[4]&a41 ^ b[5]&a51 ^ b[6]&a61 ^ b[7]&a71 ;
  assign row_prod[2] = b[0]&a02 ^ b[1]&a12 ^ b[2]&a22 ^ b[3]&a32 ^ b[4]&a42 ^ b[5]&a52 ^ b[6]&a62 ^ b[7]&a72 ;
  assign row_prod[3] = b[0]&a03 ^ b[1]&a13 ^ b[2]&a23 ^ b[3]&a33 ^ b[4]&a43 ^ b[5]&a53 ^ b[6]&a63 ^ b[7]&a73 ;
  assign row_prod[4] = b[0]&a04 ^ b[1]&a14 ^ b[2]&a24 ^ b[3]&a34 ^ b[4]&a44 ^ b[5]&a54 ^ b[6]&a64 ^ b[7]&a74 ;
  assign row_prod[5] = b[0]&a05 ^ b[1]&a15 ^ b[2]&a25 ^ b[3]&a35 ^ b[4]&a45 ^ b[5]&a55 ^ b[6]&a65 ^ b[7]&a75 ;
  assign row_prod[6] = b[0]&a06 ^ b[1]&a16 ^ b[2]&a26 ^ b[3]&a36 ^ b[4]&a46 ^ b[5]&a56 ^ b[6]&a66 ^ b[7]&a76 ;
  assign row_prod[7] = b[0]&a07 ^ b[1]&a17 ^ b[2]&a27 ^ b[3]&a37 ^ b[4]&a47 ^ b[5]&a57 ^ b[6]&a67 ^ b[7]&a77 ;

  assign prod = row_prod;
endmodule



//================= CCSDS COMPATIBLE ENCODER =================
//================== hdl_gfIIR ================
module TM_RSENC_TM_RSENC_0_gfIIR (nGrst, rst, clk, clkEn, hold0fb, datNotParity, dInp, codeOut);
  parameter MM = 4;

  input nGrst, rst, clk, clkEn, hold0fb, datNotParity;
  input [MM-1:0] dInp;
  output[MM-1:0] codeOut;

  reg[MM-1:0] codeOut;
  wire[MM-1:0] codeOut_w;
  wire [MM-1:0] sizedZero = 0;
  wire [MM-1:0] pre_fb_w, dInp_reg_w, parity_w;
  wire [MM-1:0] fb_w;
  wire [MM-1:0] shft0_w;
  wire [MM-1:0] shft1_w;
  wire [MM-1:0] shft2_w;
  wire [MM-1:0] shft3_w;
  wire [MM-1:0] shft4_w;
  wire [MM-1:0] shft5_w;
  wire [MM-1:0] shft6_w;
  wire [MM-1:0] shft7_w;
  wire [MM-1:0] shft8_w;
  wire [MM-1:0] shft9_w;
  wire [MM-1:0] shft10_w;
  wire [MM-1:0] shft11_w;
  wire [MM-1:0] shft12_w;
  wire [MM-1:0] shft13_w;
  wire [MM-1:0] shft14_w;
  wire [MM-1:0] shft15_w;
  wire [MM-1:0] shft16_w;
  wire [MM-1:0] shft17_w;
  wire [MM-1:0] shft18_w;
  wire [MM-1:0] shft19_w;
  wire [MM-1:0] shft20_w;
  wire [MM-1:0] shft21_w;
  wire [MM-1:0] shft22_w;
  wire [MM-1:0] shft23_w;
  wire [MM-1:0] shft24_w;
  wire [MM-1:0] shft25_w;
  wire [MM-1:0] shft26_w;
  wire [MM-1:0] shft27_w;
  wire [MM-1:0] shft28_w;
  wire [MM-1:0] shft29_w;
  wire [MM-1:0] shft30_w;

  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(1), .MM(8)) tap_0 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w),  .shftIn(sizedZero), .shftOut(shft0_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(91), .MM(8)) tap_1 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft0_w), .shftOut(shft1_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(127), .MM(8)) tap_2 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft1_w), .shftOut(shft2_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(86), .MM(8)) tap_3 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft2_w), .shftOut(shft3_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(16), .MM(8)) tap_4 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft3_w), .shftOut(shft4_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(30), .MM(8)) tap_5 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft4_w), .shftOut(shft5_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(13), .MM(8)) tap_6 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft5_w), .shftOut(shft6_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(235), .MM(8)) tap_7 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft6_w), .shftOut(shft7_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(97), .MM(8)) tap_8 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft7_w), .shftOut(shft8_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(165), .MM(8)) tap_9 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft8_w), .shftOut(shft9_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(8), .MM(8)) tap_10 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft9_w), .shftOut(shft10_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(42), .MM(8)) tap_11 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft10_w), .shftOut(shft11_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(54), .MM(8)) tap_12 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft11_w), .shftOut(shft12_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(86), .MM(8)) tap_13 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft12_w), .shftOut(shft13_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(171), .MM(8)) tap_14 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft13_w), .shftOut(shft14_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(32), .MM(8)) tap_15 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft14_w), .shftOut(shft15_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(113), .MM(8)) tap_16 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft15_w), .shftOut(shft16_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(32), .MM(8)) tap_17 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft16_w), .shftOut(shft17_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(171), .MM(8)) tap_18 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft17_w), .shftOut(shft18_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(86), .MM(8)) tap_19 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft18_w), .shftOut(shft19_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(54), .MM(8)) tap_20 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft19_w), .shftOut(shft20_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(42), .MM(8)) tap_21 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft20_w), .shftOut(shft21_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(8), .MM(8)) tap_22 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft21_w), .shftOut(shft22_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(165), .MM(8)) tap_23 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft22_w), .shftOut(shft23_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(97), .MM(8)) tap_24 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft23_w), .shftOut(shft24_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(235), .MM(8)) tap_25 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft24_w), .shftOut(shft25_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(13), .MM(8)) tap_26 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft25_w), .shftOut(shft26_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(30), .MM(8)) tap_27 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft26_w), .shftOut(shft27_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(16), .MM(8)) tap_28 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft27_w), .shftOut(shft28_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(86), .MM(8)) tap_29 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft28_w), .shftOut(shft29_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_tap #(.GG(127), .MM(8)) tap_30 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .feedb(fb_w), .shftIn(shft29_w), .shftOut(shft30_w) );
  TM_RSENC_TM_RSENC_0_rsEnc_lastTap #(.GG(91),.MM(8)) tap_31 (.nGrst(nGrst),.rst(rst),.clk(clk),.clkEn(clkEn),
    .feedb(fb_w), .hold0fb(hold0fb), .dInp(dInp),
    .shftIn(shft30_w), .shftOut(parity_w), .pre_fbOut(pre_fb_w),
    .dInp_reg(dInp_reg_w)  );

  assign fb_w = pre_fb_w ^ dInp_reg_w;
  assign codeOut_w = datNotParity ? dInp_reg_w : parity_w;

  // register the code output
  always @ (posedge clk or negedge nGrst)
    if((!nGrst) || rst) codeOut <= #1 0;
    else codeOut <= #1 codeOut_w;
endmodule
