// ***************************************************************************/
//Microsemi Corporation Proprietary and Confidential
//Copyright 2011 Microsemi Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreRSEncoder
//              Top-level module
//
//Revision Information:
//Date        Description
//16Dec2011   v3.1
//
//SVN Revision Information:
//SVN $Revision: 23073 $
//SVN $Data: $
//
//Resolved SARs
//SAR     Date    Who         Description
//
//Notes: v3.1 implements CCSDS support
//       v3.2 add RTG4 support, adding FAMILY parameter, add reset synchronizer

`timescale 1 ns/100 ps

//================== hdl_rsEnc_top ================
module TM_RSENC_TM_RSENC_0_CORERSENC (NGRST, RST, CLK, CLKEN, START,
                  RDY, RFS, RFD, DATAINP, CODEOUTP);
  parameter TT = 3;
  parameter NN = 15;
  parameter MM = 4;
  parameter CCSDS = 0;
  // If input data are in dual basis, an upfront converter dual to conventional
  // is required: CCSDS_CONV = 1
  parameter CCSDS_CONV = 0;
  
  parameter FAMILY = 19;
  
  localparam KK = NN - 2*TT;

  input NGRST, RST, CLK, CLKEN, START;
  input [MM-1:0] DATAINP;
  output RDY, RFS, RFD;
  output[MM-1:0] CODEOUTP;

  wire[MM-1:0] conv_basis, encoded;
  wire hold0fb_w, datNotParity_w;

  // AS: reset sync
  wire iRST;
  reg r1RST, r2RST, r3RST;
  
  generate
    if (FAMILY == 25) // RTG4
    begin : sync_reset
      // holds synchronous reset high (active) for a couple of clock cycles
      // after async reset (NGRST) is released
      always@(posedge CLK or negedge NGRST)
      begin
        if (!NGRST)
        begin
          r1RST <= 1'b1;
          r2RST <= 1'b1;
          r3RST <= 1'b1;
        end
        else
        begin
          r1RST <= 1'b0;
          r2RST <= r1RST;
          r3RST <= r2RST;
        end
      end
      
      assign iRST = (r3RST | RST);
      
    end
    else begin : nosync_reset
      assign iRST = RST;
    end
  endgenerate
  
  TM_RSENC_TM_RSENC_0_rsEnc_SM #(.KK(KK), .TT(TT), .NN(NN), .MM(MM),
       .CCSDS(CCSDS), .CCSDS_CONV(CCSDS_CONV) ) TM_RSENC_TM_RSENC_0_rsEnc_SM_0 (
    .nGrst(NGRST), .rst(iRST), .clk(CLK), .clkEn(CLKEN),
    .start(START), .hold0fb(hold0fb_w), .datNotParity(datNotParity_w),
    .rfd(RFD), .rfs(RFS), .rdy(RDY)  );

  generate
    if (CCSDS && CCSDS_CONV) begin: convert_inp
      // Convert dual base data in conventional base
      TM_RSENC_TM_RSENC_0_tal1tab tal1tab_0 (
        .nGrst(NGRST), .rst(iRST), .clk(CLK), .clkEn(CLKEN),
        .z(DATAINP[7:0]),
        .u(conv_basis)  );
    end
    else  begin: dont_convert
      assign conv_basis = DATAINP;
    end
  endgenerate

  // Encode conventional data
  TM_RSENC_TM_RSENC_0_gfIIR #(.MM(MM) ) gfIIR_0 (
    .nGrst(NGRST), .rst(iRST), .clk(CLK), .clkEn(CLKEN),
    .hold0fb(hold0fb_w), .datNotParity(datNotParity_w),
    .dInp(conv_basis), .codeOut(encoded));

  generate
    if (CCSDS) begin: ccsds
    // Convert conventional encoded data to dual basis
      TM_RSENC_TM_RSENC_0_taltab taltab_0 (
        .nGrst(NGRST), .rst(iRST), .clk(CLK), .clkEn(CLKEN),
        .u(encoded[7:0]),
        .z(CODEOUTP[7:0])  );
    end
    else  begin: no_ccsds
      assign CODEOUTP = encoded;
    end
  endgenerate

endmodule
