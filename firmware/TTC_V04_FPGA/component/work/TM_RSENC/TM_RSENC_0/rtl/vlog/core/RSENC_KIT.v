// ***************************************************************************/
//Microsemi Corporation Proprietary and Confidential
//Copyright 2011 Microsemi Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE MICROSEMI LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreRSEncoder
//              General purpose Verilog modules
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


//    ########    ########   ##           ###     ##    ##
//    ##     ##   ##         ##          ## ##     ##  ##
//    ##     ##   ##         ##         ##   ##     ####
//    ##     ##   ######     ##        ##     ##     ##
//    ##     ##   ##         ##        #########     ##
//    ##     ##   ##         ##        ##     ##     ##
//    ########    ########   ########  ##     ##     ##


//----------- Register-based 1-bit Delay has only input and output ---------
module TM_RSENC_TM_RSENC_0_kitDelay_bit_reg(nGrst, rst, clk, clkEn, inp, outp);
  parameter
    DELAY = 2;

  input  nGrst, rst, clk, clkEn;
  input  inp;
  output outp;
  // extra register to avoid error if DELAY==0
  reg delayLine [0:DELAY];
  integer i;

  generate
    if(DELAY==0)
      assign outp = inp;
    else begin
      assign outp = delayLine[DELAY-1];

      always @(posedge clk or negedge nGrst)
        if(!nGrst)
          for(i=0; i<DELAY; i=i+1)         delayLine[i] <= 1'b0;
        else
          if (rst)
            for (i = 0; i<DELAY; i=i+1)  delayLine[i] <= 1'b0;
          else
            if (clkEn) begin
              for(i=DELAY-1; i>=1; i=i-1)  delayLine[i] <= delayLine[i-1];
              delayLine[0] <= inp;
            end  // clkEn
    end
  endgenerate
endmodule
/* Use
  CoreRSENC_kitDelay_bit_reg # (.DELAY(2)) bit_dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(input), .outp(outp) ) ;
*/
//ccsds ends


//     #####
//    #     #   ####   #    #  #    #  #####  ######  #####
//    #        #    #  #    #  ##   #    #    #       #    #
//    #        #    #  #    #  # #  #    #    #####   #    #
//    #        #    #  #    #  #  # #    #    #       #####
//    #     #  #    #  #    #  #   ##    #    #       #   #
//     #####    ####    ####   #    #    #    ######  #    #

//ccsds starts
// simple incremental counter
module TM_RSENC_TM_RSENC_0_kitCountS(nGrst, rst, clk, clkEn, cntEn, Q, dc);
  parameter WIDTH = 16;
  parameter DCVALUE = (1 << WIDTH) - 1; // state to decode
  parameter BUILD_DC = 1;

  input nGrst, rst, clk, clkEn, cntEn;
  output [WIDTH-1:0] Q;
  output dc;  // decoder output
  reg [WIDTH-1:0] Q;

  assign #1 dc = (BUILD_DC==1)? (Q == DCVALUE) : 1'bx;

  always@(negedge nGrst or posedge clk)
    if(!nGrst) Q <= {WIDTH{1'b0}};
    else
      if(clkEn)
        if(rst)       Q <= {WIDTH{1'b0}};
        else
          if(cntEn)   Q <= Q + 1'b1;
endmodule
/* instance
  TM_RSENC_TM_RSENC_0_CoreRSENC_kitCountS # ( .WIDTH(WIDTH), .DCVALUE({WIDTH{1'bx}}),
                .BUILD_DC(0) ) counter_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .cntEn(cntEn), .Q(outp), .dc() );
*/
//ccsds ends
