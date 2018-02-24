// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC ECC and Scrubber
//              ECC/Scrubber Aggregate
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

module test_4463andcpuopration_COREEDAC_1_sub_edac (nGrst, rst, rClk, wClk,
    stop_scrub,
    msg,  // encoder input, user_wD
    // user RAM signals
    user_wEn,
    user_wA,
    user_rEn,
    user_rA,
    // decoder inp/outp
    code_from_ram,
    re_code,
    // RAM control ports
    ram_wEn,
    ram_wD,
    ram_wA,
    ram_rEn,
    ram_rA,
    // optional rA delay to compensate for the decoder delay
    ram_rA_lat,
    // optional scrub start in addition to timer-driven one
    start_scrub,
    rst_timer,   // optional timer reset
    inj_err,
    // Flags
    init_done,
    correctable,
    error,
    slowdown,
    tmoutflg,
    scrub_done,
    scrub_corr,
    now_scrubbing );
  parameter MODE          = 0; // naked ecc or protected RAM
  parameter DAT_WIDTH     = 16;
  parameter PAR_WIDTH     = 5;
  parameter ENC_PIPE1     = 0;
  parameter ENC_PIPE2     = 0;
  parameter DEC_PIPE1     = 0;
  parameter DEC_PIPE2     = 0;
  parameter SYND_PIPE     = 0;
  parameter ENC_PIPE      = 0;
  parameter DEC_PIPE      = 0;
  parameter DLY_RD_A_ON   = 0;
  // Scrub
  parameter SCRUB_ON      = 0;
  parameter WRBK_ON       = 0;
  parameter DIV_WDTH      = 20;
  parameter TMOUT_SET     = 10;
  parameter SCRUB_AMIN    = 0;
  parameter SCRUB_AMAX    = 255;
  parameter RAM_LOGDEPTH  = 8;
  parameter RAM_PIPE      = 0;
  parameter INIT_RAM      = 0;
  parameter SINGLECLK     = 1; // single read/write clock(1)
  parameter TEST          = 0;

  input nGrst, rst, rClk, wClk ;
  input stop_scrub;
  input [DAT_WIDTH-1:0] msg;
  // user RAM signals
  input user_wEn;
  input [RAM_LOGDEPTH-1:0] user_wA;
  input user_rEn;
  input [RAM_LOGDEPTH-1:0] user_rA;
  // decoder inp/outp
  input [PAR_WIDTH+DAT_WIDTH-1:0] code_from_ram;
  output [PAR_WIDTH+DAT_WIDTH-1:0] re_code;
  // optional start in addition to timer-driven one
  input start_scrub;
  input rst_timer;   // optional timer reset
  input [PAR_WIDTH+DAT_WIDTH-1:0] inj_err;
  // RAM control ports
  output ram_wEn;
  output [PAR_WIDTH+DAT_WIDTH-1:0] ram_wD;
  output [RAM_LOGDEPTH-1:0] ram_wA;
  output ram_rEn;
  output [RAM_LOGDEPTH-1:0] ram_rA;
  //    optional rA delay to compensate for the decoder delay
  output [RAM_LOGDEPTH-1:0] ram_rA_lat;
  output correctable;
  output error;
  output slowdown;
  output tmoutflg;
  output scrub_done;
  output scrub_corr;
  output now_scrubbing;
  output init_done;

  wire user_wEn_2ram_w, mux_scrub_rd, mux_scrub_wr;
  wire [RAM_LOGDEPTH-1:0] user_wA_2ram_w;
  wire stop_scrub_rd_w;
  wire stop_scrub_wr_w;
  wire errFlag_w      ;
  wire correctable_w  ;

  wire [RAM_LOGDEPTH-1:0] scrub_rA_w;
  wire [RAM_LOGDEPTH-1:0] ram_rA_w;
  wire [RAM_LOGDEPTH-1:0] ram_rA_lat_w;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] coded_w;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] coded_plus_error;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] re_code_w;
  wire scrub_rEn_w;

  wire [RAM_LOGDEPTH-1:0] scrub_wA_w;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] scrub_wD_w;
  wire scrub_wEn_w;
  wire error_from_scrub, correctable_from_scrub;

  wire scrub_due;
  wire [1:0] wr_mux_sel;
  wire init; 
  wire init_rd;   // init on rClk domain
  wire [RAM_LOGDEPTH-1:0] init_wA;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] zero_word = 'b0;
  wire user_read_on, read_ok;
  wire pro_error, pro_correctable;

  //                      +-+-+-+-+ +-+-+-+-+-+
  //                      |F|l|a|g| |v|a|l|i|d|
  //                      +-+-+-+-+ +-+-+-+-+-+
  // Let the flags out only when
  // - a user rEn is on;
  // - a user read takes place rather than scrubbing.  This is achieved by
  //   using error/correctable_from scrub
  // - RAM initialization is over (if INIT mode selected)

  // Make init cross clk domain from wr to rd if necessary
  generate
    if(SINGLECLK==0)  begin:  read_ok_on_r_domain   
      edac_kitCrossClk  cross_read_ok_wr2rd_0 (
        //rt  .nGrst(nGrst), 
        .nGrst(1'b1),
        .rst(1'b0),                                     //rt
        .destin_clk(rClk),
        .inp(init),
        .outp(init_rd) );
      // OR init and init_rd to get the max length  
      assign read_ok = user_rEn & ~(init | init_rd);
    end  
    else              begin:  read_ok_common_domain
      assign read_ok = user_rEn & ~init;
    end
  endgenerate  
  
  edac_kitDelay_bit_reg #(.DELAY(1+DEC_PIPE+RAM_PIPE) ) usr_rd_dly_0 (
    .nGrst(nGrst), 
    .rst(rst), .clk(rClk), .clkEn(1'b1),
    .inp(read_ok),
    .outp(user_read_on)  );

  assign pro_error = ((SCRUB_ON!=0) && (MODE!=0)) ? 
        error_from_scrub & ~correctable_from_scrub : errFlag_w & ~correctable_w;
  assign pro_correctable = ((SCRUB_ON!=0) && (MODE!=0)) ?  
        correctable_from_scrub : correctable_w;
  assign error =        pro_error       & user_read_on;
  assign correctable =  pro_correctable & user_read_on;
  // Flag valid ends

  assign scrub_corr  = scrub_wEn_w;
  assign re_code     = re_code_w;
  assign ram_rA      = ram_rA_w;
  assign ram_rA_lat  = ram_rA_lat_w;

  test_4463andcpuopration_COREEDAC_1_ecc # (
          .MODE     (MODE),
          .DAT_WIDTH(DAT_WIDTH),
          .PAR_WIDTH(PAR_WIDTH),
          .ENC_PIPE1(ENC_PIPE1),
          .ENC_PIPE2(ENC_PIPE2),
          .DEC_PIPE1(DEC_PIPE1),
          .DEC_PIPE2(DEC_PIPE2),
          .SYND_PIPE(SYND_PIPE),
          .ENC_PIPE(ENC_PIPE),
          .DEC_PIPE(DEC_PIPE),
          .SCRUB_ON (SCRUB_ON),
          .RAM_LOGDEPTH(RAM_LOGDEPTH),
          .RAM_PIPE    (RAM_PIPE),
          .DLY_RD_A_ON (DLY_RD_A_ON)   )  test_4463andcpuopration_COREEDAC_1_ecc_0 (
//rt      .nGrst(nGrst), .rst(rst),  
      .nGrst(1'b1), .rst(1'b0), 
      .rClk (rClk), .wClk (wClk),
      .msg (msg),
      .coded (coded_w),
      .user_wEn (user_wEn),
      .user_wA (user_wA),
      //   user wA, wEn delayed to balance the encoder latency if any
      .user_wEn_2ram (user_wEn_2ram_w),
      .user_wA_2ram (user_wA_2ram_w),
      // decoder
      .code_from_ram(code_from_ram),
      .re_code (re_code_w),
      .errFlag (errFlag_w),
      .correctable(correctable_w),
      .ram_rA (ram_rA_w),
      // truly optional rA delay = decoder delay
      .ram_rA_lat(ram_rA_lat_w)   );

  // Create inject error port if in TEST mode
  assign coded_plus_error = TEST ? coded_w ^ inj_err : coded_w;

  // MODE==0 : Codec only
  // MODE   SCRUB_ON   RAM_INIT
  //  0         x         x         // No scrub, no init
  //  1         0         0         //       - " -
  //  1         0         1         // No scrub, only Init
  //  1         1         0         // Scrub, no Init
  //  1         1         1         // Scrub and Init

  //                   +-+-+ +-+-+-+-+-+ +-+-+ +-+-+-+-+
  //                   |N|o| |S|c|u|b|,| |N|o| |I|n|i|t|
  //                   +-+-+ +-+-+-+-+-+ +-+-+ +-+-+-+-+
  generate    // Do not create either scrub or Muxes
    if ( (MODE==0) || (SCRUB_ON==0) ) begin: no_scrub_no_init
      assign ram_wD  = coded_plus_error;
      assign ram_wA  = user_wA_2ram_w;
      assign ram_wEn = user_wEn_2ram_w;
      assign ram_rA_w  = user_rA;
      assign ram_rEn = user_rEn;
      assign init_done = 1'b0;
      assign init = 1'b0;
    end
  endgenerate

  //              +-+-+-+-+-+-+ +-+-+ +-+-+-+-+
  //              |S|c|r|u|b|,| |N|o| |I|n|i|t|
  //              +-+-+-+-+-+-+ +-+-+ +-+-+-+-+
  // Instantiate scrubber and RAM input Muxes to create ports for the scrubber
  generate
    if ( (MODE!=0) && (SCRUB_ON!=0) && (INIT_RAM==0) ) begin: scrub_no_init
      scrub # ( .DAT_WIDTH (DAT_WIDTH),
                .PAR_WIDTH (PAR_WIDTH),
                .WRBK_ON   (WRBK_ON),
                .DIV_WDTH  (DIV_WDTH),
                .TMOUT_SET (TMOUT_SET),
                .SCRUB_AMIN(SCRUB_AMIN),
                .SCRUB_AMAX(SCRUB_AMAX),
                .ENC_PIPE(ENC_PIPE),
                .DEC_PIPE(DEC_PIPE),
                .RAM_PIPE(RAM_PIPE),
                .RAM_LOGDEPTH(RAM_LOGDEPTH),
                .SINGLECLK(SINGLECLK)  )  scrub_0 (
          .nGrst(nGrst), .rClk (rClk), .wClk (wClk),
          .rst(rst),                                  //rt
          .stop_scrub (stop_scrub),
          .errFlag (errFlag_w),
          .correctable (correctable_w),
          .start_scrub (start_scrub),
          .init(1'b0),
          .rst_timer (rst_timer),
          .re_code (re_code_w),
          .mux_scrub_rd(mux_scrub_rd),
          .mux_scrub_wr(mux_scrub_wr),
          .scrub_due(scrub_due),
          .scrub_rA (scrub_rA_w),
          .scrub_wA (scrub_wA_w),
          .scrub_wD (scrub_wD_w),
          .scrub_wEn (scrub_wEn_w),
          .error_from_scrub(error_from_scrub),
          .correctable_from_scrub(correctable_from_scrub),
          .slowdown (slowdown),
          .tmoutflg (tmoutflg),
          .now_scrubbing(now_scrubbing),
          .scrub_done (scrub_done)    );

      // scrub_due & ~stop_scrub = ~scrub_due | stop_scrub

      // Mux RAM inputs to create ports for a scrubber.
      // sel=0 makes the ports available for the scrubber
      muxVect2to1 # (.BITWIDTH (PAR_WIDTH+DAT_WIDTH) ) // write data input
        mux_dIn_0 ( .inp1(scrub_wD_w),
                    .inp2(coded_plus_error),
                    .sel (~mux_scrub_wr),
                    .outp(ram_wD)  );

      muxVect2to1 # (.BITWIDTH (RAM_LOGDEPTH) )       // write addr input
        mux_wA_1  ( .inp1(scrub_wA_w),
                    .inp2(user_wA_2ram_w),
                    .sel (~mux_scrub_wr),
                    .outp(ram_wA)  );

      muxScal2to1  mux_wEn_2 (                        // wEn input
          .inp1(scrub_wEn_w),
          .inp2(user_wEn_2ram_w),
          .sel (~mux_scrub_wr),
          .outp(ram_wEn) );

      muxVect2to1 # (.BITWIDTH(RAM_LOGDEPTH) )        // read addr input
        mux_rA_3  (
          .inp1(scrub_rA_w),
          .inp2(user_rA),
          .sel (~mux_scrub_rd),
          .outp(ram_rA_w)  );

      muxScal2to1  mux_rEn_4 (                        // rEn input
          .inp1(1'b1),
          .inp2(user_rEn),
          .sel (~mux_scrub_rd),
          .outp(ram_rEn)  );

      assign init_done = 1'b0;
      assign init = 1'b0;
    end
  endgenerate

  //                     +-+-+-+-+ +-+-+-+-+ +-+-+-+-+
  //                     |S|c|u|b| |p|l|u|s| |I|n|i|t|
  //                     +-+-+-+-+ +-+-+-+-+ +-+-+-+-+
  // Instantiate scrubber, initializer and RAM input Muxes to create ports for
  // the scrubber and initializer
  generate
    if ( (MODE!=0) && (SCRUB_ON!=0) && (INIT_RAM!=0) ) begin: scrub_init
      scrub # ( .DAT_WIDTH (DAT_WIDTH),
                .PAR_WIDTH (PAR_WIDTH),
                .WRBK_ON   (WRBK_ON),
                .DIV_WDTH  (DIV_WDTH),
                .TMOUT_SET (TMOUT_SET),
                .SCRUB_AMIN(SCRUB_AMIN),
                .SCRUB_AMAX(SCRUB_AMAX),
                .ENC_PIPE(ENC_PIPE),
                .DEC_PIPE(DEC_PIPE),
                .RAM_PIPE(RAM_PIPE),
                .RAM_LOGDEPTH(RAM_LOGDEPTH),
                .SINGLECLK(SINGLECLK)  )  scrub_0 (
          .nGrst(nGrst), .rClk (rClk), .wClk (wClk),
          .rst(rst),                                                      //rt
          .stop_scrub (stop_scrub),
          .errFlag (errFlag_w),
          .correctable (correctable_w),
          .start_scrub (start_scrub),
          .init(init),
          .rst_timer (rst_timer),
          .re_code (re_code_w),
          .mux_scrub_rd(mux_scrub_rd),
          .mux_scrub_wr(mux_scrub_wr),
          .scrub_due(scrub_due),
          .scrub_rA (scrub_rA_w),
          .scrub_wA (scrub_wA_w),
          .scrub_wD (scrub_wD_w),
          .scrub_wEn (scrub_wEn_w),
          .error_from_scrub(error_from_scrub),
          .correctable_from_scrub(correctable_from_scrub),
          .slowdown (slowdown),
          .tmoutflg (tmoutflg),
          .now_scrubbing(now_scrubbing),
          .scrub_done (scrub_done)    );


      initializer # (
        .SCRUB_AMIN  (SCRUB_AMIN  ),
        .SCRUB_AMAX  (SCRUB_AMAX  ),
        .ENC_PIPE(ENC_PIPE),
        .RAM_LOGDEPTH(RAM_LOGDEPTH) ) initializer_0 (
          .wClk(wClk), .nGrst(nGrst), 
//rt          .rst(rst),    
          .init(init),
          .init_wA(init_wA),
          .init_done(init_done) );

      // Multiplex RAM inputs to take signals from one of three circuits
      // sel   RAM ports are available to:
      // 00       Scrubber
      // 01       User
      // 1x       Initializer
      assign wr_mux_sel = {init, ~mux_scrub_wr};

      muxVect3to1 # (.BITWIDTH (PAR_WIDTH+DAT_WIDTH) ) // write data input
        mux_dIn_0 ( .inp1(scrub_wD_w),
                    .inp2(coded_plus_error),
                    .inp3(zero_word),
                    .sel (wr_mux_sel),
                    .outp(ram_wD)  );

      muxVect3to1 # (.BITWIDTH (RAM_LOGDEPTH) )       // write addr input
        mux_wA_1  ( .inp1(scrub_wA_w),
                    .inp2(user_wA_2ram_w),
                    .inp3(init_wA),
                    .sel (wr_mux_sel),
                    .outp(ram_wA)  );

      muxScal3to1  mux_wEn_0 (                        // wEn input
          .inp1(scrub_wEn_w),
          .inp2(user_wEn_2ram_w),
          .inp3(1'b1),
          .sel (wr_mux_sel),
          .outp(ram_wEn) );

      muxVect2to1 # (.BITWIDTH(RAM_LOGDEPTH) )        // read addr input
        mux_rA_3  (
          .inp1(scrub_rA_w),
          .inp2(user_rA),
          .sel (~mux_scrub_rd),
          .outp(ram_rA_w)  );

      muxScal2to1  mux_rEn_4 (                        // rEn input
          .inp1(1'b1),
          .inp2(user_rEn),
          .sel (~mux_scrub_rd),
          .outp(ram_rEn)  );
    end
  endgenerate

endmodule
