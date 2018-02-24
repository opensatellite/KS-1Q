// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC ECC
//              Hamming Encoder and Decoder
//
//Revision Information:
//Date         Description
//08Jan2009    Initial Release
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
//------------------------------ Naked ECC (no RAM) ---------------------------
 `timescale 1 ns/100 ps

module test_4463andcpuopration_COREEDAC_0_ecc (
    nGrst, rst, rClk, wClk,
    // encoder input/output
    msg,
    coded,
    user_wEn,
    user_wA,
    // user wA, wEn delayed to compensate for the encoder latency if any
    user_wEn_2ram,
    user_wA_2ram,
    // decoder
    code_from_ram,
    re_code,
    errFlag, correctable,
    // optional rA delay to compensate for the decoder delay
    ram_rA,
    ram_rA_lat   );
  parameter MODE      = 1;
  parameter DAT_WIDTH = 16;
  parameter PAR_WIDTH = 5;
  parameter ENC_PIPE1 = 0;
  parameter ENC_PIPE2 = 0;
  parameter DEC_PIPE1 = 0;
  parameter DEC_PIPE2 = 0;
  parameter SYND_PIPE = 0;
  parameter ENC_PIPE  = 0;
  parameter DEC_PIPE  = 0;
  parameter SCRUB_ON  = 0;
  parameter RAM_LOGDEPTH = 8;
  parameter RAM_PIPE     = 0;
  parameter DLY_RD_A_ON  = 0;

  input nGrst, rst, rClk, wClk;
  input [DAT_WIDTH-1 : 0] msg;
  output[PAR_WIDTH+DAT_WIDTH-1 : 0] coded;
  input user_wEn;
  input [RAM_LOGDEPTH-1 : 0] user_wA;
  output user_wEn_2ram;
  output[RAM_LOGDEPTH-1 : 0] user_wA_2ram;
  input [PAR_WIDTH+DAT_WIDTH-1 : 0] code_from_ram;
  output[PAR_WIDTH+DAT_WIDTH-1 : 0] re_code;
  output errFlag, correctable;
  input [RAM_LOGDEPTH-1 : 0] ram_rA;
  output[RAM_LOGDEPTH-1 : 0] ram_rA_lat;

  wire [RAM_LOGDEPTH:0] user_wEn_wA, user_wEn_wA_lat; //concat wEn,wA

  //---------------------------------------- Encoder part ---------------------
  test_4463andcpuopration_COREEDAC_0_ham_enc_vlog # ( .DAT_WIDTH (DAT_WIDTH),
              .PAR_WIDTH (PAR_WIDTH),
              .PIPE1     (ENC_PIPE1),
              .PIPE2     (ENC_PIPE2)  ) test_4463andcpuopration_COREEDAC_0_encoder_0 (
        .nGrst (nGrst), .rst (rst), .clk (wClk), .clkEn (1'b1),
        .msg   (msg),
        .coded (coded)    );

  // Delay user_wA and wEn to compensate for the encoder latency if any
  assign user_wEn_wA = {user_wEn, user_wA};

  edac_kitDelay_reg # (.BITWIDTH (RAM_LOGDEPTH+1), .DELAY (ENC_PIPE) )
    dly_user_wA (.nGrst (nGrst), .rst (rst), .clk (wClk), .clkEn (1'b1),
      .inp (user_wEn_wA),
      .outp(user_wEn_wA_lat)   );

  assign user_wEn_2ram = user_wEn_wA_lat[RAM_LOGDEPTH];
  assign user_wA_2ram  = user_wEn_wA_lat[RAM_LOGDEPTH-1:0];
  // -------------------------------------------------------------- end encoder

  // ------------------------------------- Decoder part -----------------------
  test_4463andcpuopration_COREEDAC_0_ham_dec_vlog # (
      .DAT_WIDTH (DAT_WIDTH),
      .PAR_WIDTH (PAR_WIDTH),
      .DEC_DLY(DEC_PIPE), // overall decoder pipeline delay
      .PIPE1     (DEC_PIPE1),
      .PIPE2     (DEC_PIPE2),
      .SYND_PIPE (SYND_PIPE)  ) test_4463andcpuopration_COREEDAC_0_decoder_0 (
        .nGrst (nGrst), .rst(rst), .clk(rClk), .clkEn(1'b1),
        .coded      (code_from_ram),
        .errFlag    (errFlag),
        .correctable(correctable),
        .re_code    (re_code)    );

  // optional Read Addr delay to compensate for the decoder delay
  generate
    if ( MODE && DLY_RD_A_ON )
      edac_kitDelay_reg # (.BITWIDTH (RAM_LOGDEPTH),
                      .DELAY (DEC_PIPE + RAM_PIPE + 1) ) dly_user_rA_0 (  //wi_rAdly
        .nGrst(nGrst), .rst(rst), .clk (rClk), .clkEn(1'b1),
        .inp  (ram_rA),
        .outp (ram_rA_lat)   );
    else
      assign ram_rA_lat = {RAM_LOGDEPTH{1'b0}};
  endgenerate
endmodule
