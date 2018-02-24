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

// Standard behavioral modules
module bhvDelay (nGrst, rst, clk, clkEn,
            inp,
            outp );
  parameter DELAY = 2;
  parameter WIDTH = 2;

  input nGrst, rst, clk, clkEn;
  input [WIDTH-1:0] inp;
  output[WIDTH-1:0] outp;

  integer i;
  // extra register to avoid error/warning if DELAY==0
  reg[WIDTH-1:0] delayLine [0:DELAY];

  generate
    if(DELAY==0)
      assign outp = inp;
    else begin
      assign outp = delayLine[DELAY-1];

      always @(posedge clk or negedge nGrst)
        if(!nGrst)
          for(i=0; i<DELAY; i=i+1)         delayLine[i] <= 'b0;
        else
          if (clkEn)
            if (rst)
              for (i = 0; i<DELAY; i=i+1)  delayLine[i] <= #1 'b0;
            else begin
              for(i=DELAY-1; i>=1; i=i-1)  delayLine[i] <= #1 delayLine[i-1];
              delayLine[0] <= #1 inp;
            end  // clkEn
    end
  endgenerate
endmodule


// generate one clkEn period wide pulse after nGrst, or rst
module bhvOneClk (nGrst, rst, clk, clkEn, inp, outp);
  parameter
    ONNGRST = 0,   // generate on negative nGrst
    ONRST   = 0,   // generate on positive rst
    ONINP   = 1,   // generate on inp
    REDGE   = 1;   // 1 - input rising edge, or 0 - falling edge

  input  nGrst, clk, rst, clkEn;
  input inp;
  output outp;

  reg trig, flop;
  assign #1 outp = trig & ~flop;

  always @(posedge clk or negedge nGrst) begin
    if(!nGrst) begin
      flop <= 1'b0;
      trig <= 1'b0;
    end
    else
      if (rst) begin
        flop <= #1 1'b0;
        trig <= #1 1'b0;
      end
      else
        if(clkEn) begin
          flop <= #1 trig;
          if(ONNGRST) trig <= #1 nGrst;
          if(ONRST)   trig <= #1 1'b1;
          if(ONINP)
            if(REDGE) trig <= #1 inp;
            else      trig <= #1 ~inp;
        end
  end
endmodule


module bhvTest (nGrst, start, clk, clkEn, outp, tstDataOver);
  parameter MM = 5;
  parameter NN = 15;
  parameter KK = 9;   // Decode this state

  input nGrst, start, clk, clkEn;
  output[MM-1:0] outp;
  output tstDataOver;   // decoded KK-1 of the data counter-timer

  wire [MM-1:0] testDat_w, index;
  integer timer;

  assign outp = (timer < KK) ? testDat_w : 0;
  assign tstDataOver = (timer == KK-1);
  assign index = (timer[MM-1:0] < KK) ? timer[MM-1:0] : 'b0;

  bhvEncInpVect #(.TBLSIZE(KK), .WORDSIZE(MM)  )
    bhvEncInpVect_0(.inpIndex(index), .inp_data(testDat_w));


  always @ (posedge clk or negedge nGrst)
    if(!nGrst)  timer <= 0;

    else if(clkEn)    begin
      if(start)   timer <= 0;
      else
        timer <= timer + 1;
    end
endmodule

/*******************************************************************************
                ######   #####     #######
                #     # #     #       #    ######  ####  #####
                #     # #             #    #      #        #
                ######   #####        #    #####   ####    #
                #   #         #       #    #           #   #
                #    #  #     #       #    #      #    #   #
                #     #  #####        #    ######  ####    #
*******************************************************************************/

module testbench;
`include "coreparameters.v"

  localparam DBG_RS    = 0;
  localparam NOGAPTEST = 0; // 0 - no gap between codewords at the encoder output
  localparam TSTCYCLES = 3; // number of cycles to test
  localparam KK        = NN - 2*TT;
  localparam CCSDS_DLY = CCSDS ? 1 + CCSDS_CONV : 0;

  wire clk, nGrst;
  wire[MM-1:0] tstVect;
  integer filePtr;
  integer testFail, tstCycleCnt;
  reg rst;
  wire start, rfs_w, rfs_tick3_w, start_tick4_w;
  wire[MM-1:0] rsEncoded_w, goldDat_w; // Actual and golden encoded data
  wire codeRdy_w;
  wire clkEn = 1'b1;
  reg[MM-1:0] goldTimer;
  reg testValid;
  wire start_lat, start_ccsds;

  initial begin
    if(DBG_RS) begin
      filePtr = $fopen("rsResult.log");
      $fdisplay(filePtr, "Testing rsEnc of mm=%1d GF", MM);
    end
    rst <= 1'b0;
    testValid <= 1'b0;
  end

  bhvClockGen clock_1(.clk(clk), .nGrst(nGrst));

  bhvTest #(.MM(MM), .NN(NN), .KK(KK) )
    bhvTest_0  (.nGrst(nGrst), .clk(clk), .clkEn(clkEn),
      .start(rst | start),
      .outp(tstVect),
      .tstDataOver()  );

  // RS encoder under the test
  TM_RSENC_TM_RSENC_0_CORERSENC # (.TT(TT), .NN(NN), .MM(MM),
                          .CCSDS(CCSDS), .CCSDS_CONV(CCSDS_CONV) ) rsEnc_0 (
    .NGRST(nGrst), .RST(rst), .CLK(clk), .CLKEN(clkEn),
    .START(start),
    .RFS(rfs_w), .RFD(), .RDY(codeRdy_w),
    .DATAINP(tstVect), .CODEOUTP(rsEncoded_w) );

  // delay rfs for a few clocks to make a gap between output codewords
  bhvDelay # (.WIDTH(1), .DELAY(3) )
    dlyStart_0 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
                .inp(rfs_w), .outp(rfs_tick3_w) );

  // generate 1-clk pulse on start rising edge
  bhvOneClk #(.ONNGRST(0),   // generate on negative nGrst
    .ONRST(0),   // generate on positive rst
    .ONINP(1),   // generate on inp
    .REDGE(1)  )   // 1 - input rising edge, or 0 - falling edge
    oneClk_0 (.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
              .inp(rfs_tick3_w), .outp(start_tick4_w)  );

  assign start = (NOGAPTEST) ? rfs_w : start_tick4_w;

  //Verify rsEncoder output against gold {data, parity} sequence
  bhvEncOutpVect #(.TBLSIZE(NN), .WORDSIZE(MM)  )
    bhvEncOutpVect_0 (.inpIndex(goldTimer), .outp_data(goldDat_w) );

  bhvDelay # (.DELAY(2), .WIDTH(1) ) basic_enc_latency_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(start),
    .outp (start_lat) );

  bhvDelay # (.DELAY(CCSDS_DLY), .WIDTH(1) ) match_ccsds_dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(start_lat),
    .outp (start_ccsds) );

  always @ (posedge clk or negedge nGrst) begin
    if((!nGrst) || rst) begin
      goldTimer <= #1 0;
      testFail <= 0;
      tstCycleCnt <= 0;
    end
    else if(clkEn) begin
      if(start_ccsds)  begin
        testValid <= #1 1'b1;
        goldTimer  <= #1 0;
      end

      if(start_tick4_w) tstCycleCnt <= tstCycleCnt + 1;
      if(goldTimer > NN-2) goldTimer <= 0;
      else                  goldTimer <= goldTimer + 1;
      if((rsEncoded_w !== goldDat_w) && codeRdy_w && testValid )begin
        testFail <= 1;
        if(DBG_RS) $fdisplay(filePtr,"i=%d Actual codeword=%d goldenCode=%d",
                  goldTimer, rsEncoded_w, goldDat_w);
      end
      if((goldTimer == NN-1) && (tstCycleCnt == TSTCYCLES)) begin
        $fdisplay(filePtr,"");
        $fdisplay(filePtr,"=====================================");
        $display("");
        $display("=====================================");
        if(testFail) begin
          if(DBG_RS) $fdisplay(filePtr,"!!!!!!!! RS Encoder Test Failed !!!!!!!!");
          $display("!!!!!!!! RS Encoder Test Failed !!!!!!!!");
        end
        else         begin
          if(DBG_RS) $fdisplay(filePtr,"------ RS Encoder Test Passed ------");
          $display("------ RS Encoder Test Passed ------");
        end
        $fdisplay(filePtr,"=====================================");
        $fdisplay(filePtr,"");
        $display("=====================================");
        $display("");

        if(DBG_RS) $fclose(filePtr);
        $stop;
      end
    end
  end

endmodule


// generate clock and nGrst.  nGrst is supposed to be longer than a clock period
module bhvClockGen(clk, nGrst);
  parameter
    CLKPERIOD  = 20,
    NGRSTLASTS = 24;
  parameter PAR0 = 0;
  parameter PAR1 = 0;
  output reg clk;
  output reg nGrst;

  initial begin
    clk = 0;
    nGrst = 0;
    #(NGRSTLASTS);  nGrst = 1;
  end

  always
    #(CLKPERIOD/2) clk = ~clk;
endmodule
