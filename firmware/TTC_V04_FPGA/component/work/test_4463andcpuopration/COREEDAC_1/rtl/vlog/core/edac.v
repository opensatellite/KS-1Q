// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC TMR
//              TMR and Majority Vote
//
//Revision Information:
//Date         Description
//08Jan2009    Initial Release
//30Jan2013    G4 update
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
//------------------------------------------------------------------------------
 `timescale 1 ns/100 ps

module test_4463andcpuopration_COREEDAC_1_edac (nGrst,
    rst,
    rClk,
    wClk,
    stop_scrub_trp1,
    stop_scrub_trp2,
    stop_scrub_trp3,
    // encoder input/output
    msg_trp1,    // user_wD
    msg_trp2,
    msg_trp3,
    // user RAM signals
    user_wEn_trp1,
    user_wEn_trp2,
    user_wEn_trp3,
    user_wA_trp1,
    user_wA_trp2,
    user_wA_trp3,
    user_rEn_trp1,
    user_rEn_trp2,
    user_rEn_trp3,
    user_rA_trp1,
    user_rA_trp2,
    user_rA_trp3,
    // Decoder inp/outp.  Single input from the RAM as there is only one RAM
    code_from_ram,
    re_code      ,
    // RAM control ports
    ram_wEn,
    ram_wD ,
    ram_wA ,
    ram_rEn,
    ram_rA ,
    //    optional rA delay to compensate for the decoder delay
    ram_rA_lat,
    // optional scrub start in addition to timer-driven one
    start_scrub_trp1,
    start_scrub_trp2,
    start_scrub_trp3,
    now_scrubbing,
    rst_timer_trp1,  // optional timer reset
    rst_timer_trp2,
    rst_timer_trp3,
    inj_err,
    // Flags
    init_done,
    correctable,
    error ,
    slowdown ,
    tmoutflg ,
    scrub_done ,
    scrub_corr  );
  parameter MODE         = 0; // naked ecc or protected RAM
  parameter TMR          = 0; // triple redundancy
  parameter DAT_WIDTH    = 16;
  parameter PAR_WIDTH    = 5;
  parameter ENC_PIPE1    = 0;
  parameter ENC_PIPE2    = 0;
  parameter DEC_PIPE1    = 0;
  parameter DEC_PIPE2    = 0;
  parameter SYND_PIPE    = 0;
  parameter ENC_PIPE     = 0;
  parameter DEC_PIPE     = 0;
  parameter DLY_RD_A_ON  = 0;
  // Scrub
  parameter SCRUB_ON     = 0;
  parameter WRBK_ON      = 0;
  parameter DIV_WDTH     = 20;
  parameter TMOUT_SET    = 10;
  parameter SCRUB_AMIN   = 0;
  parameter SCRUB_AMAX   = 255;
  parameter RAM_LOGDEPTH = 8;
  parameter RAM_PIPE     = 0;
  parameter SINGLECLK    = 1;
  parameter INIT_RAM     = 0;
  parameter TEST         = 0;

  input nGrst, rst, rClk, wClk;
  input stop_scrub_trp1;
  input stop_scrub_trp2;
  input stop_scrub_trp3;

  input [DAT_WIDTH-1:0] msg_trp1; // encoder input/output = user_wD
  input [DAT_WIDTH-1:0] msg_trp2;
  input [DAT_WIDTH-1:0] msg_trp3;
  // user RAM signals
  input user_wEn_trp1;
  input user_wEn_trp2;
  input user_wEn_trp3;
  input [RAM_LOGDEPTH-1:0] user_wA_trp1;
  input [RAM_LOGDEPTH-1:0] user_wA_trp2;
  input [RAM_LOGDEPTH-1:0] user_wA_trp3;
  input user_rEn_trp1;
  input user_rEn_trp2;
  input user_rEn_trp3;
  input [RAM_LOGDEPTH-1:0] user_rA_trp1;
  input [RAM_LOGDEPTH-1:0] user_rA_trp2;
  input [RAM_LOGDEPTH-1:0] user_rA_trp3;
  // Decoder inp/outp. Single input from the RAM as there is only one RAM
  input [PAR_WIDTH+DAT_WIDTH-1:0] code_from_ram;
  output[PAR_WIDTH+DAT_WIDTH-1:0] re_code;
  // RAM control ports
  output ram_wEn;
  output [PAR_WIDTH+DAT_WIDTH-1:0] ram_wD;
  output [RAM_LOGDEPTH-1:0] ram_wA;
  output ram_rEn;
  output [RAM_LOGDEPTH-1:0] ram_rA;
  output [RAM_LOGDEPTH-1:0] ram_rA_lat; // optional rA delay to balance decoder
  input start_scrub_trp1; //optional scrub start in addition to timer-driven one
  input start_scrub_trp2;
  input start_scrub_trp3;
  input rst_timer_trp1;   //optional timer reset
  input rst_timer_trp2;
  input rst_timer_trp3;
  input [PAR_WIDTH+DAT_WIDTH-1:0] inj_err;
  // Flags
  output correctable;
  output now_scrubbing;
  output error;
  output slowdown;
  output tmoutflg;
  output scrub_done;
  output scrub_corr;
  output init_done;

  wire [PAR_WIDTH+DAT_WIDTH-1:0] re_code_w1, re_code_w2, re_code_w3;
  wire ram_wEn_w1, ram_wEn_w2, ram_wEn_w3;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] ram_wD_w1, ram_wD_w2, ram_wD_w3;
  wire [RAM_LOGDEPTH-1:0] ram_wA_w1, ram_wA_w2, ram_wA_w3;
  wire ram_rEn_w1, ram_rEn_w2, ram_rEn_w3;
  wire [RAM_LOGDEPTH-1:0] ram_rA_w1, ram_rA_w2, ram_rA_w3;
  wire [RAM_LOGDEPTH-1:0] ram_rA_lat_w1, ram_rA_lat_w2, ram_rA_lat_w3;
  wire now_scrubbing_trp1, now_scrubbing_trp2, now_scrubbing_trp3;
  wire correctable_w1, correctable_w2, correctable_w3;
  wire error_w1, error_w2, error_w3;
  wire slowdown_w1, slowdown_w2, slowdown_w3 ;
  wire tmoutflg_w1, tmoutflg_w2, tmoutflg_w3 ;
  wire scrub_done_w1, scrub_done_w2, scrub_done_w3 ;
  wire scrub_corr_w1, scrub_corr_w2, scrub_corr_w3;
  wire samerw_w1, samerw_w2, samerw_w3 ;
  wire now_scrubbing_w1, now_scrubbing_w2, now_scrubbing_w3;

  wire samerw_w;

  test_4463andcpuopration_COREEDAC_1_sub_edac # (.MODE(MODE),
              .DAT_WIDTH (DAT_WIDTH),
              .PAR_WIDTH (PAR_WIDTH),
              .ENC_PIPE1 (ENC_PIPE1),
              .ENC_PIPE2 (ENC_PIPE2),
              .DEC_PIPE1 (DEC_PIPE1),
              .DEC_PIPE2 (DEC_PIPE2),
              .SYND_PIPE (SYND_PIPE),
              .ENC_PIPE(ENC_PIPE),
              .DEC_PIPE(DEC_PIPE),
              .DLY_RD_A_ON (DLY_RD_A_ON),
              .SCRUB_ON (SCRUB_ON),
              .WRBK_ON  (WRBK_ON),
              .DIV_WDTH (DIV_WDTH),
              .TMOUT_SET (TMOUT_SET),
              .SCRUB_AMIN (SCRUB_AMIN),
              .SCRUB_AMAX (SCRUB_AMAX),
              .RAM_LOGDEPTH (RAM_LOGDEPTH),
              .RAM_PIPE (RAM_PIPE),
              .INIT_RAM (INIT_RAM),
              .TEST     (TEST),
              .SINGLECLK (SINGLECLK)   )  test_4463andcpuopration_COREEDAC_1_sub_edac_0 (
      .nGrst(nGrst), .rst(rst), .rClk (rClk), .wClk (wClk),
      .stop_scrub(stop_scrub_trp1),
      .msg (msg_trp1),
      .user_wEn (user_wEn_trp1),
      .user_wA (user_wA_trp1),
      .user_rEn (user_rEn_trp1),
      .user_rA (user_rA_trp1),
      .code_from_ram (code_from_ram),
      .re_code (re_code_w1),
      .ram_wEn (ram_wEn_w1),
      .ram_wD (ram_wD_w1),
      .ram_wA (ram_wA_w1),
      .ram_rEn (ram_rEn_w1),
      .ram_rA (ram_rA_w1),
      .ram_rA_lat (ram_rA_lat_w1),
      .start_scrub (start_scrub_trp1),
      .rst_timer (rst_timer_trp1),
      .inj_err (inj_err),
      .init_done(init_done),
      .correctable (correctable_w1),
      .error (error_w1),
      .slowdown (slowdown_w1),
      .tmoutflg (tmoutflg_w1),
      .scrub_done (scrub_done_w1),
      .scrub_corr (scrub_corr_w1),
      .now_scrubbing(now_scrubbing_w1)  );

  //-------------------------  Triple Redundant Modules  -----------------------
  generate
    if (TMR==1) begin
      test_4463andcpuopration_COREEDAC_1_sub_edac # (.MODE(MODE),
              .DAT_WIDTH (DAT_WIDTH),
              .PAR_WIDTH (PAR_WIDTH),
              .ENC_PIPE1 (ENC_PIPE1),
              .ENC_PIPE2 (ENC_PIPE2),
              .DEC_PIPE1 (DEC_PIPE1),
              .DEC_PIPE2 (DEC_PIPE2),
              .SYND_PIPE (SYND_PIPE),
              .ENC_PIPE(ENC_PIPE),
              .DEC_PIPE(DEC_PIPE),
              .DLY_RD_A_ON (DLY_RD_A_ON),
              .SCRUB_ON (SCRUB_ON),
              .WRBK_ON  (WRBK_ON),
              .DIV_WDTH (DIV_WDTH),
              .TMOUT_SET (TMOUT_SET),
              .SCRUB_AMIN (SCRUB_AMIN),
              .SCRUB_AMAX (SCRUB_AMAX),
              .RAM_LOGDEPTH (RAM_LOGDEPTH),
              .RAM_PIPE (RAM_PIPE),
              .INIT_RAM (INIT_RAM),
              .TEST     (TEST),
              .SINGLECLK (SINGLECLK)   )  test_4463andcpuopration_COREEDAC_1_sub_edac_1 (
          .nGrst(nGrst), .rst(rst), .rClk (rClk), .wClk (wClk),
          .stop_scrub (stop_scrub_trp2),
          .msg (msg_trp2),
          .user_wEn (user_wEn_trp2),
          .user_wA (user_wA_trp2),
          .user_rEn (user_rEn_trp2),
          .user_rA (user_rA_trp2),
          .code_from_ram (code_from_ram),
          .re_code (re_code_w2),
          .ram_wEn (ram_wEn_w2),
          .ram_wD (ram_wD_w2),
          .ram_wA (ram_wA_w2),
          .ram_rEn (ram_rEn_w2),
          .ram_rA (ram_rA_w2),
          .ram_rA_lat (ram_rA_lat_w2),
          .start_scrub (start_scrub_trp2),
          .rst_timer (rst_timer_trp2),
          .inj_err (inj_err),
          .init_done(),
          .correctable (correctable_w2),
          .error (error_w2),
          .slowdown (slowdown_w2),
          .tmoutflg (tmoutflg_w2),
          .scrub_done (scrub_done_w2),
          .scrub_corr (scrub_corr_w2),
          .now_scrubbing(now_scrubbing_w2)  );

      test_4463andcpuopration_COREEDAC_1_sub_edac # (.MODE(MODE),
              .DAT_WIDTH (DAT_WIDTH),
              .PAR_WIDTH (PAR_WIDTH),
              .ENC_PIPE1 (ENC_PIPE1),
              .ENC_PIPE2 (ENC_PIPE2),
              .DEC_PIPE1 (DEC_PIPE1),
              .DEC_PIPE2 (DEC_PIPE2),
              .SYND_PIPE (SYND_PIPE),
              .ENC_PIPE(ENC_PIPE),
              .DEC_PIPE(DEC_PIPE),
              .DLY_RD_A_ON (DLY_RD_A_ON),
              .SCRUB_ON (SCRUB_ON),
              .WRBK_ON  (WRBK_ON),
              .DIV_WDTH (DIV_WDTH),
              .TMOUT_SET (TMOUT_SET),
              .SCRUB_AMIN (SCRUB_AMIN),
              .SCRUB_AMAX (SCRUB_AMAX),
              .RAM_LOGDEPTH (RAM_LOGDEPTH),
              .RAM_PIPE (RAM_PIPE),
              .INIT_RAM (INIT_RAM),
              .TEST     (TEST),
              .SINGLECLK (SINGLECLK)  ) test_4463andcpuopration_COREEDAC_1_sub_edac_2 (
          .nGrst(nGrst), .rst(rst), .rClk (rClk), .wClk (wClk),
          .stop_scrub (stop_scrub_trp3),
          .msg (msg_trp3),
          .user_wEn (user_wEn_trp3),
          .user_wA (user_wA_trp3),
          .user_rEn (user_rEn_trp3),
          .user_rA (user_rA_trp3),
          .code_from_ram (code_from_ram),
          .re_code (re_code_w3),
          .ram_wEn (ram_wEn_w3),
          .ram_wD (ram_wD_w3),
          .ram_wA (ram_wA_w3),
          .ram_rEn (ram_rEn_w3),
          .ram_rA (ram_rA_w3),
          .ram_rA_lat (ram_rA_lat_w3),
          .start_scrub (start_scrub_trp3),
          .rst_timer (rst_timer_trp3),
          .inj_err (inj_err),
          .init_done(),
          .correctable (correctable_w3),
          .error (error_w3),
          .slowdown (slowdown_w3),
          .tmoutflg (tmoutflg_w3),
          .scrub_done (scrub_done_w3),
          .scrub_corr (scrub_corr_w3),
          .now_scrubbing(now_scrubbing_w3)  );

      // Majority logic
      edac_kitMajority # (.WIDTH(PAR_WIDTH+DAT_WIDTH)) major_recode  (
          .inp1 (re_code_w1),
          .inp2 (re_code_w2),
          .inp3 (re_code_w3),
          .outp (re_code)     );

      edac_kitMajority # (.WIDTH(1)) major_ram_wEn (
          .inp1 (ram_wEn_w1),
          .inp2 (ram_wEn_w2),
          .inp3 (ram_wEn_w3),
          .outp (ram_wEn)     );

      edac_kitMajority # (.WIDTH(PAR_WIDTH+DAT_WIDTH)) major_ram_wD (
          .inp1 (ram_wD_w1),
          .inp2 (ram_wD_w2),
          .inp3 (ram_wD_w3),
          .outp (ram_wD)     );

      edac_kitMajority # (.WIDTH(RAM_LOGDEPTH)) major_ram_wA (
          .inp1 (ram_wA_w1),
          .inp2 (ram_wA_w2),
          .inp3 (ram_wA_w3),
          .outp (ram_wA)    );

      edac_kitMajority # (.WIDTH(1)) major_ram_rEn (
          .inp1 (ram_rEn_w1),
          .inp2 (ram_rEn_w2),
          .inp3 (ram_rEn_w3),
          .outp (ram_rEn)     );

      edac_kitMajority # (.WIDTH(RAM_LOGDEPTH)) major_ram_rA (
          .inp1 (ram_rA_w1),
          .inp2 (ram_rA_w2),
          .inp3 (ram_rA_w3),
          .outp (ram_rA)     );

      edac_kitMajority # (.WIDTH(RAM_LOGDEPTH)) major_ram_rA_lat (
          .inp1 (ram_rA_lat_w1),
          .inp2 (ram_rA_lat_w2),
          .inp3 (ram_rA_lat_w3),
          .outp (ram_rA_lat)     );

      edac_kitMajority # (.WIDTH(1)) major_correctable (
          .inp1 (correctable_w1),
          .inp2 (correctable_w2),
          .inp3 (correctable_w3),
          .outp (correctable)     );

      edac_kitMajority # (.WIDTH(1)) major_error (
          .inp1 (error_w1),
          .inp2 (error_w2),
          .inp3 (error_w3),
          .outp (error )    );

      edac_kitMajority # (.WIDTH(1)) major_slowdown (
          .inp1 (slowdown_w1),
          .inp2 (slowdown_w2),
          .inp3 (slowdown_w3),
          .outp (slowdown)     );

      edac_kitMajority # (.WIDTH(1)) major_tmoutflg (
          .inp1 (tmoutflg_w1),
          .inp2 (tmoutflg_w2),
          .inp3 (tmoutflg_w3),
          .outp (tmoutflg)     );

      edac_kitMajority # (.WIDTH(1)) major_now_scrub (
          .inp1 (now_scrubbing_w1),
          .inp2 (now_scrubbing_w2),
          .inp3 (now_scrubbing_w3),
          .outp (now_scrubbing)     );

      edac_kitMajority # (.WIDTH(1)) major_scrub_done (
          .inp1 (scrub_done_w1),
          .inp2 (scrub_done_w2),
          .inp3 (scrub_done_w3),
          .outp (scrub_done)     );

      edac_kitMajority # (.WIDTH(1)) major_scrub_corr (
          .inp1 (scrub_corr_w1),
          .inp2 (scrub_corr_w2),
          .inp3 (scrub_corr_w3),
          .outp (scrub_corr)     );

    end
    else begin
      assign re_code     = re_code_w1;
      assign ram_wEn     = ram_wEn_w1;
      assign ram_wD      = ram_wD_w1;
      assign ram_wA      = ram_wA_w1;
      assign ram_rEn     = ram_rEn_w1;
      assign ram_rA      = ram_rA_w1;
      assign ram_rA_lat  = ram_rA_lat_w1;
      assign now_scrubbing  = now_scrubbing_w1;
      assign correctable = correctable_w1;
      assign error       = error_w1;
      assign slowdown    = slowdown_w1;
      assign tmoutflg    = tmoutflg_w1;
      assign scrub_done  = scrub_done_w1;
      assign scrub_corr  = scrub_corr_w1;
    end
  endgenerate

endmodule


