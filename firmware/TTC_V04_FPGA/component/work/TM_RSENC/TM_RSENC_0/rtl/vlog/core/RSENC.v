// ***************************************************************************/
//Microsemi Corporation Proprietary and Confidential
//Copyright 2011 Microsemi Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE MICROSEMI LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreRSEncoder
//              Static Verilog modules
//
//Revision Information:
//Date         Description
//16Dec2011    v3.1
//
//SVN Revision Information:
//SVN $Revision: $
//SVN $Data: $
//
//Resolved SARs
//SAR     Date    Who         Description
//
//Notes:
//

`timescale 1 ns/100 ps

module TM_RSENC_TM_RSENC_0_rsEnc_tap (nGrst, rst, clk, clkEn, feedb, shftIn, shftOut);
  parameter GG = 4;   // constant generator polynomial term
  parameter MM = 4;

  input nGrst, rst, clk, clkEn;
  input [MM-1:0] feedb, shftIn;
  output[MM-1:0] shftOut;
  reg   [MM-1:0] shftOut;

  wire [MM-1:0] tapProd_w;
  wire [MM-1:0] GG_w = GG;

  TM_RSENC_TM_RSENC_0_gf_mult # (.MM(MM)) gfComb_0 (.a(GG_w), .b(feedb), .prod(tapProd_w));

  always @ (posedge clk or negedge nGrst)
    if((!nGrst) || rst)
      shftOut <= #1 0;
    else if(clkEn)
      shftOut <= #1 tapProd_w ^ shftIn;
endmodule


module TM_RSENC_TM_RSENC_0_rsEnc_lastTap (nGrst, rst, clk, clkEn, shftIn, dInp, feedb,
                      hold0fb, shftOut, pre_fbOut, dInp_reg);
  parameter GG = 4;   // constant generator polynomial term
  parameter MM = 4;

  input nGrst, rst, clk, clkEn, hold0fb;
  input [MM-1:0] feedb, shftIn, dInp;
  output[MM-1:0] shftOut, pre_fbOut, dInp_reg;
  reg   [MM-1:0] shftOut, pre_fbOut, dInp_reg;

  wire [MM-1:0] tapProd_w;
  wire [MM-1:0] GG_w = GG;

  TM_RSENC_TM_RSENC_0_gf_mult # (.MM(MM)) gfComb_last (.a(GG_w), .b(feedb), .prod(tapProd_w));

  always @ (posedge clk or negedge nGrst)
    if((!nGrst) || rst) begin
      shftOut <= #1 0;
      pre_fbOut <= #1 0;
      dInp_reg  <= #1 0;
    end
    else if(clkEn) begin// AS: SARno12778 fix
      shftOut <= #1 tapProd_w ^ shftIn;
      if(hold0fb)  begin
        pre_fbOut <= #1 0;
        dInp_reg  <= #1 0;
      end
      else begin
        pre_fbOut <= #1 tapProd_w ^ shftIn;
        dInp_reg  <= #1 dInp;
      end
    end  //else if(clkEn)
endmodule



// Encoder latency depends on CCSDS mode.  The CCSDS mode uses optional dual
// to conventional basis converter, then conventional encoder, then conventional
// to dual basis converter:
// Optional dual to conventional => conventional encoder => conventional to dual

// Latency of the conventional encoder = 2.
// Latency of each converter = 1.

module TM_RSENC_TM_RSENC_0_rsEnc_SM(nGrst, rst, clk, clkEn, start,
                          hold0fb,
                          datNotParity,
                          rfd,
                          rfs,
                          rdy);
  parameter KK = 9;
  parameter TT = 3;
  parameter NN = 15;
  parameter MM = 4;
  parameter CCSDS = 0;
  parameter CCSDS_CONV = 0; // Dual to conventional needed

  localparam LATENCY = CCSDS ? CCSDS_CONV + 3 : 2;

  input nGrst, rst, clk, clkEn;
  input start;    // IF: precedes the first valid input data sample
  output rfd;     // IF: ready for data
  output rfs;     // IF: ready for start
  output rdy;     // IF: valid codeword is present at the encoder output
  output datNotParity, hold0fb;

  reg rfd, rfs, datNotParity;
  reg hold0fb_r, hold0fb_r2, pre_rdy, codeOver;
  wire valid_start;
  wire dataOver_w, codeOver_w, rstTimer_w;
  wire[MM-1:0] timer_w;

  assign hold0fb = ((CCSDS!=0)&&CCSDS_CONV) ? hold0fb_r2 : hold0fb_r;
  assign valid_start = start & rfs;
  assign rstTimer_w = rst | start;

  TM_RSENC_TM_RSENC_0_kitCountS # ( .WIDTH(MM), .DCVALUE(NN-2),
                .BUILD_DC(1) ) sm_timer_0 (
    .nGrst(nGrst), .rst(rstTimer_w), .clk(clk), .clkEn(clkEn),
    .cntEn(1'b1), .Q(timer_w), .dc(codeOver_w) );

  assign dataOver_w = (timer_w == KK-1) & rdy;    //wipro 5/21/2012

  TM_RSENC_TM_RSENC_0_kitDelay_bit_reg # (.DELAY(LATENCY)) bit_dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(pre_rdy), .outp(rdy) ) ;

  always @ (posedge clk or negedge nGrst)
    if(nGrst==1'b0) begin
      hold0fb_r     <= 1'b1;
      hold0fb_r2    <= 1'b1;
      datNotParity  <= 1'b0;
      rfs           <= 1'b1;
      rfd           <= 1'b1;
      pre_rdy       <= 1'b0;
      codeOver      <= 1'b0;
    end
    else if(clkEn)
      if(rst) begin
        hold0fb_r     <= 1'b1;
        hold0fb_r2    <= 1'b1;
        datNotParity  <= 1'b0;
        rfs           <= 1'b1;
        rfd           <= 1'b1;
        pre_rdy       <= 1'b0;
        codeOver      <= 1'b0;
      end
      else begin
        hold0fb_r2    <= hold0fb_r;
        datNotParity  <= ~hold0fb;
        codeOver      <= codeOver_w & rdy;

        if(valid_start) begin
          pre_rdy       <= 1'b1;
        end
        // In NOGAPTEST mode codeOver & valid_start get asserted on the same clk
        else
          if(codeOver)  pre_rdy <= 1'b0;

        if(start) begin
          rfs <= 1'b0;
          hold0fb_r     <= 1'b0;
        end else if(codeOver_w == 1'b1) begin
          rfs <= 1'b1;
        end

        if(dataOver_w)  begin
          rfd           <= 1'b0;
          hold0fb_r     <= 1'b1;
        end

        if(rfs)
          rfd           <= 1'b1;
      end
endmodule


//                    #####   #####   #####  ######   #####
//                   #     # #     # #     # #     # #     #
//                   #       #       #       #     # #
//                   #       #        #####  #     #  #####
//                   #       #             # #     #       #
//                   #     # #     # #     # #     # #     #
//                    #####   #####   #####  ######   #####

// Convert conventional symbol representation to dual basis and register
// the output
module TM_RSENC_TM_RSENC_0_taltab (nGrst, rst, clk, clkEn, u, z);
  input nGrst, rst, clk, clkEn;
  input [7:0] u;
  output[7:0] z;
  reg[7:0] z;

  wire[7:0] dual;

  assign dual[7] = u[7]^u[6]^u[5]^u[4]^u[3]^u[2]^u[1];
  assign dual[6] = u[6]^u[5]^u[3]^u[0];
  assign dual[5] = u[6]^u[5]^u[3]^u[1]^u[0];
  assign dual[4] = u[3]^u[2]^u[0];
  assign dual[3] = u[7]^u[6]^u[5]^u[3]^u[2]^u[1]^u[0];
  assign dual[2] = u[7]^u[6]^u[5]^u[4]^u[1];
  assign dual[1] = u[6]^u[4]^u[3]^u[1]^u[0];
  assign dual[0] = u[7]^u[6]^u[2]^u[1]^u[0];

  always @ (posedge clk or negedge nGrst)
    if(nGrst==0)  z <= 8'b0;
    else
      if(clkEn)
        if(rst==1'b1) z <= 8'b0;
        else          z <= dual;
endmodule


// Convert dual basis representation to conventional and register
// the output
module TM_RSENC_TM_RSENC_0_tal1tab (nGrst, rst, clk, clkEn, z, u);
  input nGrst, rst, clk, clkEn;
  input [7:0] z;
  output[7:0] u;
  reg[7:0] u;

  wire[7:0] conv;

  assign conv[7] = z[7]^z[4]^z[3]^z[1]^z[0];
  assign conv[6] = z[7]^z[6]^z[4]^z[3]^z[2]^z[0];
  assign conv[5] = z[5]^z[4]^z[3]^z[2]^z[1];
  assign conv[4] = z[4]^z[3]^z[2];
  assign conv[3] = z[5]^z[4]^z[2]^z[1]^z[0];
  assign conv[2] = z[7]^z[5]^z[4]^z[1]^z[0];
  assign conv[1] = z[6]^z[5];
  assign conv[0] = z[7]^z[4]^z[2];

  always @ (posedge clk or negedge nGrst)
    if(nGrst==0)  u <= 8'b0;
    else
      if(clkEn)
        if(rst==1'b1) u <= 8'b0;
        else          u <= conv;
endmodule

