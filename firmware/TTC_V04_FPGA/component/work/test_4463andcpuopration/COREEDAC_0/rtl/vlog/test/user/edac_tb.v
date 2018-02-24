// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC User Test Bench
//              Top level entity
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
//  TESTBENCH ------------------------------------------------------------------
//------------------------------------------------------------------------------
`timescale 1 ns/100 ps
`include "test_4463andcpuopration_COREEDAC_0_edac_bhv_auto.v"

//  To count errors separately for user access and for scrubbing based on NOW_SCRUBBING, ERROR and CORRECTABLE signals.  Check if the count matches what was injected  !!!!!!!!

module testbench;

`include "coreparameters.v"

  localparam RAM_LOGDEPTH = ceil_log2(RAM_DEPTH);
  localparam PAR_WIDTH   = edac_d2p(DAT_WIDTH);
  localparam XORWDTH     = xorWidth(DAT_WIDTH);
  localparam ENC_LAYERS  = ((FAMILY==11) || (FAMILY==12)) ?
                                      ceil_log2(XORWDTH) : ceil_log3(XORWDTH);
  localparam DEC_LAYERS  = ((FAMILY==11) || (FAMILY==12)) ?
                                  ceil_log2(XORWDTH+1) : ceil_log3(XORWDTH+1);
  localparam ENC_PIPE1 = (doPipeEnc(XORWDTH, USER_ENC_PIPE, 1, ENC_LAYERS) - 1);
  localparam ENC_PIPE2 = (doPipeEnc(XORWDTH, USER_ENC_PIPE, 2, ENC_LAYERS) - 1);

  localparam DEC_PIPE1 = doPipeDec(XORWDTH, USER_DEC_PIPE, 1, DEC_LAYERS) - 1;
  localparam DEC_PIPE2 = doPipeDec(XORWDTH, USER_DEC_PIPE, 2, DEC_LAYERS) - 1;
  localparam SYND_PIPE = doPipeDec(XORWDTH, USER_DEC_PIPE, 3, DEC_LAYERS);

  localparam ENC_DLY1 = (ENC_PIPE1>=0 && ENC_PIPE1<5) ? 1 : 0;
  localparam ENC_DLY2 = (ENC_PIPE2>=0 && ENC_PIPE2<5) ? 1 : 0;
  localparam DEC_DLY1 = (DEC_PIPE1>=0 && DEC_PIPE1<6) ? 1 : 0;
  localparam DEC_DLY2 = (DEC_PIPE2>=0 && DEC_PIPE2<6) ? 1 : 0;
  localparam ENC_PIPE = ENC_DLY1 + ENC_DLY2;
  localparam DEC_PIPE = DEC_DLY1 + DEC_DLY2 + SYND_PIPE;

  `define CODEC_ONLY  0
  `define FULL_EDAC   1 
  //  -------------------------------------------------------------------------
  //                          TESTBENCH OPTIONS
  //  -------------------------------------------------------------------------
  localparam SCRUB_ERROR = 1;   // 0: No errors during the scrub mode
  localparam SCRUB_DOUBLE_ERROR = 0;
  localparam SCRUB_SINGLE_ERROR_PERIOD = 1; // 1-every word, 2-in a word, etc

  localparam SCRUB_INTERRUPT   = 1;
  localparam SCRUB_IRQ_PERIOD = 21; // period of user access while scrubbing

  localparam CLKPERIOD_RD = 10;
  localparam CLKPERIOD_WR = 11;
  localparam SYNCRST = 0;                                                 //rt
  //  -------------------------------------------------------------------------

  integer ecc_err_flag_count, ecc_corr_flag_count; 
  integer scrub_err_flag_count, scrub_corr_flag_count;
  wire nGrst, wClk, rClk, wr_clk;
  reg[DAT_WIDTH-1:0] tstVect;
  wire [DAT_WIDTH-1:0] gold_usrData, gold_usrData_tickN;
  wire [DAT_WIDTH-1:0] gold_scrubData, gold_scrubData_tickN;
  reg [RAM_LOGDEPTH-1:0] user_wA, check_ecc_rA, check_scrub_rA;
  wire[RAM_LOGDEPTH-1:0] user_rA;
  reg [PAR_WIDTH+DAT_WIDTH-1:0] err_ecc;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] re_code_w;
  reg fail_ecc, fail_scrub;
  integer scrub_init_timer;
  reg fail_standalone_enc, fail_standalone_dec;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] inj_scrub_err, errHit_w;
  wire stop_scrub_rand, stop_scrub, scrub_done;
  reg user_wEn = 1'b0;                             //sar43052
  reg user_rEn = 1'b0;                             //sar43052
  wire now_scrubbing;

  wire valid_re_code_w, valid_rD_w;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] gold_re_code;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] gold_rD;

  wire mode_init_ecc, mode_check_ecc, mode_init_scrub;
  wire mode_run_scrub, mode_check_scrub, mode_report;
  wire activate_interrupt;
  wire read_mode_front;  // front edge of read domain modes
  wire write_mode_front;  // front edge of write domain modes
  wire mode_run_scrub_fe;

  wire [PAR_WIDTH+DAT_WIDTH-1:0] code_from_ram_w;
  wire [DAT_WIDTH-1:0] edac_data_out;
  wire [PAR_WIDTH-1:0] edac_parity_out;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] encoder_outp, from_ext_ram;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] gold_coded, gold_enc_outp, gold_dec_outp;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] errMode0_w, inj_err_w, inj_err;
  reg  [PAR_WIDTH+DAT_WIDTH-1:0] errMode0_read;
  wire error, correctable, slowdown, tmoutflag; 
  wire valid_lone_dec;

  reg [PAR_WIDTH+DAT_WIDTH-1:0] scrub_1or2err;
  reg  [PAR_WIDTH+DAT_WIDTH-1:0] scrub_1or2err_tick, scrub_1or2err_tick2;
  reg ecc_flag_fail;
//  reg rst;        //rt
  wire nGrst2uut, rst2uut;      //rt

  //  -------------------------------------------------------------------------
  assign wClk = (SINGLECLK==1) ? rClk : wr_clk;
  assign re_code_w = {edac_data_out, edac_parity_out};

  //  -------------------------------------------------------------------------
  //  --------------  UUT TEST VECTOR GENERATOR (wClk domain)  ----------------
  //  -------------------------------------------------------------------------
  // Write address in every cell, i.e. 0 to addr 0, 1 to addr 1, etc.
  always @(posedge wClk or negedge nGrst)
    if(nGrst==1'b0)    tstVect <= 'b0;
    else
      if(write_mode_front) tstVect <= 'b0;
      else
        if(tstVect >= RAM_DEPTH-1) tstVect <= 'b0;
        else                       tstVect <= tstVect + 1;
        
  //  -------------------------------------------------------------------------
  //  ---------  STANDALONE ENC & DEC GOLD VECTOR GENERATORS (Mode0) ----------
  //  -------------------------------------------------------------------------
  // Generate golden Encoder output in wClk domain to test standalone Enc
  // when MODE==CODEC_ONLY
  bhv_enc_vlog bhv_enc_0 (.msg(tstVect), .coded(gold_coded));
  bhvDelay #(.DELAY(ENC_PIPE), .WIDTH(PAR_WIDTH+DAT_WIDTH)) encoder_pipe_0
      (.nGrst(nGrst), .rst(1'b0), .clk(wClk), .clkEn(1'b1),
       .inp(gold_coded),
       .outp(gold_enc_outp) );

  // Use delayed copy of the gold_re_code to test a standalone Dec
  bhvDelay #(.DELAY(DEC_PIPE), .WIDTH(PAR_WIDTH+DAT_WIDTH))
    decoder_pipe_0  (.nGrst(nGrst), .rst(1'b0), .clk(rClk),
       .clkEn(1'b1),
       .inp(gold_re_code),
       .outp(gold_dec_outp) );
  // Use delayed copy of the valid outp to test a standalone Dec
  bhvDelay #(.DELAY(DEC_PIPE), .WIDTH(1))                   
    decoder_pipe_1  (.nGrst(nGrst), .rst(1'b0), .clk(rClk), 
       .clkEn(1'b1),                                        
       .inp(valid_re_code_w),                               
       .outp(valid_lone_dec) );                             


  // Inject errMode0_w errors into gold_re_code to use as a test
  // signal for a standalone Dec
  assign from_ext_ram = gold_re_code ^ errMode0_w;
  // Signals to test standalone Dec end
  //  ------------------- End gold vectors ----------------

  // An instance of the Hamming codec that also includes a RAM block: ----------
  //              -----
  // Encoder --> |     |
  //             | XOR | -> RAM -> Decoder.
  // inj_err --> |     |
  //              -----
  //  if(mode_check_ecc)
  //    user_rA = check_ecc_rA;
  //  else
  //    if (mode_check_scrub)
  //      user_rA = check_scrub_rA
  //    else
  //      user_rA = ALL 1's 
  assign user_rA  = mode_check_ecc ? check_ecc_rA : mode_check_scrub ? check_scrub_rA : (1<<(RAM_LOGDEPTH-1) - 1);

  assign nGrst2uut = (SYNCRST==1)? 1'b1 : nGrst;
  assign rst2uut   = (SYNCRST==1)? ~nGrst : 1'b0;
  test_4463andcpuopration_COREEDAC_0_COREEDAC # (
    .MODE (MODE),
    .TMR (TMR),
    .DAT_WIDTH (DAT_WIDTH),
    .USER_ENC_PIPE (USER_ENC_PIPE),
    .USER_DEC_PIPE (USER_DEC_PIPE),
    .SCRUB_ON (SCRUB_ON),
    .WRBK_ON (WRBK_ON),
    .DIV_WDTH (DIV_WDTH),
    .TMOUT_SET (TMOUT_SET),
    .SCRUB_AMIN (SCRUB_AMIN),
    .SCRUB_AMAX (SCRUB_AMAX),
    .RAM_DEPTH (RAM_DEPTH),
    .RAM_PIPE (RAM_PIPE),
    .URAM       (URAM       ),
    .URAM_PORT_B(URAM_PORT_B),
    .URAM_A_PIPE(URAM_A_PIPE),
    .INIT_RAM   (INIT_RAM   ),    
    .DLY_RD_A_ON (DLY_RD_A_ON),
    .SINGLECLK (SINGLECLK),
    .TEST (1),   // creates port to inject errors
    .SIM_RAM (SIM_RAM),
    .FAMILY (FAMILY)   ) uut_0 (
      .NGRST(nGrst2uut),          //rt
      .RST  (rst2uut),            //rt
      .RCLK (rClk), .WCLK (wClk),
      .STOP_SCRUB_TRP1 (stop_scrub),
      .STOP_SCRUB_TRP2 (stop_scrub),
      .STOP_SCRUB_TRP3 (stop_scrub),
      .DATA_OUT        (edac_data_out),
      .PARITY_OUT      (edac_parity_out),
      .USER_WEN_TRP1   (user_wEn),
      .USER_WEN_TRP2   (user_wEn),
      .USER_WEN_TRP3   (user_wEn),
      .MSG_TRP1        (tstVect),
      .MSG_TRP2        (tstVect),
      .MSG_TRP3        (tstVect),
      .USER_WA_TRP1    (user_wA),
      .USER_WA_TRP2    (user_wA),
      .USER_WA_TRP3    (user_wA),
      .USER_REN_TRP1   (user_rEn),
      .USER_REN_TRP2   (user_rEn),
      .USER_REN_TRP3   (user_rEn),
      .USER_RA_TRP1    (user_rA),
      .USER_RA_TRP2    (user_rA),
      .USER_RA_TRP3    (user_rA),
      .CODE_FROM_RAM   (code_from_ram_w),
      .CODE_FROM_EXT_RAM (from_ext_ram),
      .INJ_ERR          (inj_err),
      .START_SCRUB_TRP1 (mode_run_scrub_fe), //start_scrub_w,
      .START_SCRUB_TRP2 (mode_run_scrub_fe),
      .START_SCRUB_TRP3 (mode_run_scrub_fe),
//rt      .RST_TIMER_TRP1   (1'b0),
//rt      .RST_TIMER_TRP2   (1'b0),
//rt      .RST_TIMER_TRP3   (1'b0),
      .RST_TIMER_TRP1   (rst2uut),
      .RST_TIMER_TRP2   (rst2uut),
      .RST_TIMER_TRP3   (rst2uut),
      .CODED(encoder_outp),
      .RAM_RA_LAT (),
      .INIT_DONE  (),    
      .ERROR      (error),
      .CORRECTABLE(correctable),
      .SLOWDOWN   (slowdown),
      .TMOUTFLG   (tmoutflag),
      .SCRUB_DONE (scrub_done),
      .SCRUB_CORR (),
      .NOW_SCRUBBING(now_scrubbing)  );


  //          +-+-+-+-+-+-+-+ +-+-+-+-+-+-+ +-+-+ +-+-+-+-+-+-+
  //          |P|R|E|P|A|R|E| |E|R|R|O|R|S| |T|O| |I|N|J|E|C|T|
  //          +-+-+-+-+-+-+-+ +-+-+-+-+-+-+ +-+-+ +-+-+-+-+-+-+

  //  ---------  ERRORS TO INJECT FOR SCRUB TEST on mode_init_scrub  ----------
  //  -------------------------------------------------------------------------
  // Create one- and two-bit errors only inside the mode_init_scrub
  always @(posedge wClk or negedge nGrst)
    if(nGrst==1'b0) begin
      scrub_init_timer    = 0;
      scrub_1or2err       = 'b0;
      scrub_1or2err_tick  = 'b0;
      scrub_1or2err_tick2 = 'b0;
    end
    else begin
      if( (mode_init_scrub==1'b1) && (SCRUB_ERROR==1) ) begin
        if( (SCRUB_DOUBLE_ERROR==1) && (scrub_init_timer%32==21) )
          scrub_1or2err = 'b11;
        else              //single-bit error
          if(scrub_init_timer % SCRUB_SINGLE_ERROR_PERIOD ==
                            SCRUB_SINGLE_ERROR_PERIOD-1)  scrub_1or2err = 'b10;
          else                                            scrub_1or2err = 'b0;
        scrub_init_timer  = scrub_init_timer + 1;
      end
      else begin
        scrub_init_timer  = 0;
        scrub_1or2err     = 'b0;
      end
      scrub_1or2err_tick2 = scrub_1or2err_tick;
      scrub_1or2err_tick  = scrub_1or2err;
    end

  // As errors are injected after encoder, account for possible encoder delay
  assign inj_scrub_err = (ENC_PIPE==0) ? scrub_1or2err : (ENC_PIPE>1) ?
                                      scrub_1or2err_tick2 : scrub_1or2err_tick;


  //  -----------  ERRORS TO INJECT FOR ECC TEST on mode_init_ecc  ------------
  //  -------------------------------------------------------------------------
  always @(posedge wClk or negedge nGrst)
    if (nGrst==1'b0) err_ecc <= 'b0;
    else begin
      if(mode_init_ecc==1'b1)
        if(err_ecc=='b0) err_ecc <= 'b1;
        else             err_ecc <= err_ecc << 1;
      else               err_ecc <= 'b0;
    end


  //  ----------------  ERRORS TO INJECT FOR CODEC ONLY TEST  -----------------
  //  -------------------------------------------------------------------------

  // Create rClk domain errors to test standalone Decoder when MODE==CODEC_ONLY
  always @(posedge rClk or negedge nGrst)
    if (nGrst==1'b0) errMode0_read <= 'b0;
    else begin
      if(mode_check_ecc==1'b1)
        if(errMode0_read=='b0) errMode0_read <= #1 'b1;
        else                   errMode0_read <= #1 errMode0_read << 1;
      else                     errMode0_read <= #1 'b0;
    end

  // Inject errors when testing Decoder in MODE==CODEC_ONLY
  assign errMode0_w = mode_init_ecc ? 'b0 : errMode0_read;

  //  ------------------  COMBINE ECC AND SCRUB ERRORS  -----------------------  
  // Use INJ_ERR UUT input to purposly corrupt data going from encoder to RAM
  assign errHit_w   = mode_init_scrub ? inj_scrub_err : err_ecc;
  assign inj_err_w  = (MODE) ? errHit_w : errMode0_w;
  
  // Delay the errors to match encoder output
  bhvDelay # (.DELAY(ENC_PIPE),
              .WIDTH(PAR_WIDTH+DAT_WIDTH) ) error_dly_0 (
    .nGrst(nGrst), .rst(1'b0), .clk(wClk), .clkEn(1'b1),
    .inp(inj_err_w),
    .outp(inj_err) );
  
//                          +-+-+-+ +-+-+-+-+-+
//                          |R|U|N| |T|E|S|T|S|
//                          +-+-+-+ +-+-+-+-+-+
  //  -------------------------------------------------------------------------
  //  -----  INITIALIZE RAM via user write on init_ecc & init_scrub modes  ----
  //  -------------------------------------------------------------------------
//  assign init_almost_over = (user_wA==(RAM_DEPTH-1)) ? 1 : 0;
  always @(posedge wClk or negedge nGrst)
    if(nGrst==1'b0) begin
      user_wEn <= 1'b0;
      user_rEn <= 1'b0;
    end
    else
      if(write_mode_front==1'b1)  begin
        user_wEn <= 1'b1;
        user_rEn <= 1'b0;
      end
      else if (user_wA==(RAM_DEPTH-1))  begin 
        user_wEn <= 1'b0;
        user_rEn <= 1'b1;
      end

  // User write addr counter
  always @(posedge wClk or negedge nGrst)
    if(nGrst==1'b0)    user_wA <= 'b0;
    else
      if(write_mode_front) user_wA <= 'b0;
      else
        if(user_wA>= RAM_DEPTH-1) user_wA <= 'b0;
        else                      user_wA <= user_wA + 1;

  //  -------------------------------------------------------------------------
  //  ------------  READ RAM on CHECK ECC via decoder as users do  ------------
  //  -------------------------------------------------------------------------
  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0) check_ecc_rA   = 'b0;
    else
      if(mode_check_ecc==1'b0) check_ecc_rA <= 'b0;
      else
        if(check_ecc_rA>= RAM_DEPTH-1) check_ecc_rA <= 'b0;
        else                           check_ecc_rA <= check_ecc_rA + 1;

  // As every cell contains its address, the correct read data must match
  // the addresses they are read from, that is check_ecc_rA.  But account for the
  // read delay
  assign gold_usrData = check_ecc_rA;
  bhvDelay # (.DELAY(RAM_PIPE+1+DEC_PIPE), .WIDTH(DAT_WIDTH)) gold_usrData_0 (
      .nGrst (nGrst), .rst (1'b0), .clk(rClk), .clkEn(1'b1),
      .inp  (gold_usrData),
      .outp (gold_usrData_tickN)  );

  bhv_enc_vlog bhv_enc_1 (.msg(gold_usrData_tickN), .coded(gold_re_code));

  // Create a signal that marks valid decoder output
  bhvDelay # (.DELAY(RAM_PIPE+1+DEC_PIPE), .WIDTH(1)) dly2_re_code_0 (
      .nGrst (nGrst), .rst (1'b0), .clk(rClk), .clkEn(1'b1),
      .inp  (mode_check_ecc),
      .outp (valid_re_code_w)  );


  //  -------------------------------------------------------------------------
  //  -----------------------  RUN SCRUB on mode_run_scrub  -------------------
  //  -------------------------------------------------------------------------
  // Start scrub
  bhvEdge # (.REDGE(1)) kick_scrub_0 (
    .nGrst(nGrst), .rst(1'b0), .clk(rClk),
    .inp(mode_run_scrub),
    .outp(mode_run_scrub_fe) );

  // Interrupt scrubbing and write user data on and around the address where
  // scrubbing is interrupted
  assign activate_interrupt = SCRUB_INTERRUPT? mode_run_scrub : 1'b0;
  bhv_uAccess_timer # ( .SCRUB_AMIN(SCRUB_AMIN),
                        .SCRUB_AMAX(SCRUB_AMAX),
                        .PERIOD (SCRUB_IRQ_PERIOD)  ) user_interrupt_0 (
    .clk(wClk), .nGrst(nGrst),
    .active(activate_interrupt),
    .u_access(stop_scrub_rand),
    .u_pilot() );


  //  -------------------------------------------------------------------------
  //  -----------  READ RAM on CHECK SCRUB via decoder as users do  -----------
  //  -------------------------------------------------------------------------
  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0) check_scrub_rA   = SCRUB_AMIN;
    else
      if(mode_check_scrub==1'b0) check_scrub_rA <= SCRUB_AMIN;
      else
        if(check_scrub_rA>=SCRUB_AMAX) check_scrub_rA <= #1 'b0;
        else                           check_scrub_rA <= #1 check_scrub_rA + 1;
        
  // To read RAM with check_scrub_rA, enforce usr access during mode_check_scrub
  assign stop_scrub = stop_scrub_rand | ~mode_run_scrub;       

  // As every cell contains its address, the correct read data must match
  // the addresses they are read from, that is check_ecc_rA.  But account for the
  // read delay
  assign gold_scrubData = check_scrub_rA;
  bhvDelay # (.DELAY(RAM_PIPE+1), .WIDTH(DAT_WIDTH)) gold_scrubData_0 (
      .nGrst (nGrst), .rst (1'b0), .clk(rClk), .clkEn(1'b1),
      .inp  (gold_scrubData),
      .outp (gold_scrubData_tickN)  );

  bhv_enc_vlog bhv_enc_2 (.msg(gold_scrubData_tickN), .coded(gold_rD));

  // Create a signal that marks valid decoder output
  bhvDelay # (.DELAY(RAM_PIPE+1), .WIDTH(1)) dly2_rD_0 (
      .nGrst (nGrst), .rst (1'b0), .clk(rClk), .clkEn(1'b1),
      .inp  (mode_check_scrub),
      .outp (valid_rD_w)  );


  //  -------------------------------------------------------------------------
  //  --------------------  CHECK ERROR & CORRECTABLE FLAGS  ------------------
  //  -------------------------------------------------------------------------
  // It is required the flags can be attributed either to user access 
  // (ECC errors) or scrubbing.  The testbench is supposed to detects ECC errors
  // during mode_check_ecc, and scrub errors during mode_run_scrub.  Scrub 
  // errors are those that are marked with active now_scrubbing.   

  // Detect end of the mode_check_ecc
  bhvEdge # (.REDGE(0)) mode_check_ecc_backedge_0 (
    .nGrst(nGrst), .rst(1'b0), .clk(rClk),
    .inp(mode_check_ecc),
    .outp(mode_check_ecc_bk) );

  // The error & correctable flags along with now_scrubbing flag make it 
  // possible to identifay if an error has been detected during regular ECC
  // operations or scrubbing.  But scrubbing does not exclude user interrupts,
  // thus within scrubbing cycle the ECC errors can show up.  User interrupts
  // during scrub also cause multiple readings of the same RAM locations, which
  // can lead to multiple error detections of both types, ECC and scrubbing 
  // that actually refer to the same RAM location.  Therefore during the 
  // scrubbing the flags aren't good for collecting statistics.  
  // A possible solution for separating scrub and ECC errors is as follows:
  // 1. Create external scrub_due_ext signal similar to the scrub_due.  To do
  //    so, use START_SCRUB or TMOUTFLAG to trigger the signal up and SCRUB_DONE
  //    to trigger it down
  // 2. Count the ECC errors only outside the scrub_due_ext signal to get real 
  //    ECC error count
  // 3. Count the scrub errors within the scrub_due_ext signal but only those
  //    that are marked by active now_scrubbing (see below)
  // 4. Register, which address the scrub error belongs to.  Count only the
  //    scrub errors that belong to fresh address and neglect those that belong
  //    to the already scrubbed within the same scrubbing cycle       

  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0) begin
      ecc_err_flag_count    = 0;
      ecc_corr_flag_count   = 0;
      scrub_err_flag_count  = 0;
      scrub_corr_flag_count = 0;
      ecc_flag_fail         = 1'b0;
    end
    else  begin
      if( (mode_init_ecc==1'b1) || (mode_init_scrub==1'b1) )  begin
        ecc_err_flag_count    = 0;
        ecc_corr_flag_count   = 0;
        scrub_err_flag_count  = 0;
        scrub_corr_flag_count = 0;
      end        
      else  begin  
        if(now_scrubbing==1'b1) begin
          if(error==1'b1) 
            scrub_err_flag_count  = scrub_err_flag_count  + 1;
          if(correctable==1'b1) 
            scrub_corr_flag_count = scrub_corr_flag_count + 1;
        end
        else  begin  
          if(error==1'b1) 
            ecc_err_flag_count  = ecc_err_flag_count  + 1;
          if(correctable==1'b1) 
            ecc_corr_flag_count = ecc_corr_flag_count + 1;
        end
      end
      
      // Ecc test injects only ecc errors; no scrub errors must be detected
      if(mode_check_ecc_bk==1'b1) begin 
        if(scrub_err_flag_count > 0)  ecc_flag_fail = 1'b1;
        if(scrub_corr_flag_count > 0) ecc_flag_fail = 1'b1;
      end

    end  
      

  //-------------------  Test sequence generator  -----------------
  mode_seq # (
    .MODE        (MODE),
    .SCRUB_ON    (SCRUB_ON),
    .SINGLECLK   (SINGLECLK),
    .RAM_DEPTH   (RAM_DEPTH),
    .RAM_LOGDEPTH(RAM_LOGDEPTH),
    .ENC_PIPE    (ENC_PIPE),
    .RAM_PIPE    (RAM_PIPE),
    .DEC_PIPE    (DEC_PIPE),
    .SCRUB_AMIN  (SCRUB_AMIN),
    .SCRUB_AMAX  (SCRUB_AMAX),
    .CLKPERIOD_RD(CLKPERIOD_RD),
    .CLKPERIOD_WR(CLKPERIOD_WR),
    .INIT_RAM    (INIT_RAM)    ) test_mode_style_0 (
      .nGrst (nGrst), .rClk  (rClk), .wClk  (wClk),
      .scrub_done         (scrub_done),
      .stop_scrub         (),
      .mode_init_ecc_wr   (mode_init_ecc),
      .mode_check_ecc_rd  (mode_check_ecc),
      .mode_init_scrub_wr (mode_init_scrub),
      .mode_run_scrub_rd  (mode_run_scrub),
      .mode_check_scrub_rd(mode_check_scrub),
      .mode_report_rd     (mode_report),
      .read_mode_front    (read_mode_front),
      .write_mode_front   (write_mode_front)      );

  bhvClockGen # (.CLKPERIOD (CLKPERIOD_RD),     // read clock
                 .NGRSTLASTS (24)  ) clock_1 (
                    .clk(rClk),
                    .nGrst (nGrst) );

  bhvClockGen # (.CLKPERIOD (CLKPERIOD_WR),     // write clock
                 .NGRSTLASTS (24)  ) clock_2 (
                    .clk(wr_clk),
                    .nGrst () );
                    
  //rt
  // Sync reset signal generator 
//  initial begin
//    rst = 1'b0;
//    @ (posedge nGrst);  // wait for nGrst to deassert
//    #3;
//    rst = 1'b1;
//    #BIGPERIOD;
//    rst = 1'b0;
//  end
                      

  //      _   _   _   _   _     _   _   _   _   _   _   _
  //     / \ / \ / \ / \ / \   / \ / \ / \ / \ / \ / \ / \
  //    ( C | H | E | C | K ) ( R | E | S | U | L | T | S )
  //     \_/ \_/ \_/ \_/ \_/   \_/ \_/ \_/ \_/ \_/ \_/ \_/

  initial begin
    $display("");
    $display("----------------------------------------------");
    if(MODE==`FULL_EDAC)
      $display("Testing Protected RAM: EDAC Codec and Scrubber");
    else        $display("Testing EDAC Codec only");
    $display("----------------------------------------------");
    $display("");
  end

  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0) begin
      fail_ecc    = 0;
      fail_scrub  = 0;
      fail_standalone_enc = 0;
      fail_standalone_dec = 0;
    end
    else begin
      if(MODE==`CODEC_ONLY) begin
        if(mode_init_ecc==1'b1) begin         // Check encoder
          if(write_mode_front==1'b1) begin
            $display("Running Encoder test");
            $display("------------------");
          end

          if(encoder_outp!==gold_enc_outp) begin
            fail_standalone_enc = 1;
            $display("Time:%t Encoder ERROR: Expected Word/Parity = %d  %d, Actual = %d / %d",
                  $time,
                  gold_enc_outp[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                  gold_enc_outp[PAR_WIDTH-1:0],
                  encoder_outp[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                  encoder_outp[PAR_WIDTH-1:0]);
          end
          else
            $display("Match: Expected and actual Word/Parity = %d / %d",
                                encoder_outp[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                                encoder_outp[PAR_WIDTH-1:0]);
        end

        if(mode_check_ecc==1'b1) begin   // check decoder
          if(read_mode_front==1'b1) begin
            $display("");
            $display("Running Decoder test");
            $display("------------------");
          end

          if (valid_lone_dec==1'b1) begin
            if(gold_dec_outp!==re_code_w) begin
              fail_standalone_dec = 1;
              $display("Time:%t Codec ERROR: Expected Word/Parity = %d / %d, Actual = %d / %d",
                $time, gold_re_code[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                gold_re_code[PAR_WIDTH-1:0],
                edac_data_out, edac_parity_out);
            end
            else
              $display("Match: Expected and actual Word/Parity = %d / %d",
                                              edac_data_out, edac_parity_out);
          end
        end
      end

      if(MODE==`FULL_EDAC) begin                       // ECC and Scrub
        //  --------------------------  ECC CHECK  ----------------------------
        if(mode_check_ecc==1'b1) begin
          if(read_mode_front==1'b1) begin
            $display("Running Codec test");
            $display("------------------");
          end

          if(valid_re_code_w==1'b1)
            if (gold_re_code!==re_code_w) begin
              fail_ecc <= 1;
              $display("Time:%t Codec ERROR: Expected Word/Parity = %d / %d, Actual = %d / %d",
                  $time, gold_re_code[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                  gold_re_code[PAR_WIDTH-1:0],
                  edac_data_out, edac_parity_out);
            end
            else
              $display("Match: Expected and actual Word/Parity = %d / %d",
              edac_data_out, edac_parity_out);
        end

        //  ------------------------  SCRUB CHECK  ----------------------------
        else if (mode_check_scrub==1'b1)  begin
          if(read_mode_front==1'b1) begin
            $display("");
            $display("Running Scrub Test. Directly reading scrubbed RAM");
            $display("-------------------------------------------------");
          end

          if(valid_rD_w==1'b1)  begin

            // Uncorrectable double error is expected
            if( (SCRUB_DOUBLE_ERROR==1) &&
                  (gold_rD[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH] % 32 == 21) ) begin
              $display("Uncorrectable Double error is expected at time %t.  Checking...", $time);
              if( (error==1'b1) && (correctable==1'b0) ) begin
                $display("  Uncorrectable double error of 'b11 detected.");
                $display("    Uncorrectable Actual RAM Word/Parity = %d / %d",
                  code_from_ram_w[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                  code_from_ram_w[PAR_WIDTH-1:0]);
              end
              else begin
                fail_scrub <= 1;
                $display("Scrubbing Failure: Double error has not been detected !");
              end
            end

            else
              if (gold_rD!==code_from_ram_w) begin
                fail_scrub <= 1;
                $display("Time:%t Scrub ERROR: Expected RAM Word/Parity = %d / %d, Actual = %d / %d",
                  $time, gold_rD[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                  gold_rD[PAR_WIDTH-1:0],
                  code_from_ram_w[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                  code_from_ram_w[PAR_WIDTH-1:0]);
              end
              else
                $display("Match: Expected and Actual RAM Word/Parity = %d / %d",
                code_from_ram_w[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH],
                code_from_ram_w[PAR_WIDTH-1:0]);
          end
        end
      end

      if (mode_report) begin
        $display("");
        if(ecc_flag_fail) $display("ECC flags failed");

        $display("");
        $display ("##############################");
        if (fail_ecc || (fail_scrub && SCRUB_ON && WRBK_ON) ||
                  fail_standalone_dec || fail_standalone_enc  || ecc_flag_fail)
          $display("!!!!! EDAC TEST FAILED !!!!!");
        else
          $display(" EDAC test passed ");
        $display("##############################");
        $stop;
      end
    end

`include "functions.v"
endmodule


//------------------------------------------------------------------------------
//------------------------------ TEST MODE SEQUENCER ---------------------------
//------------------------------------------------------------------------------
// Modes are separated of each other by a gap.  The gap is large enough to
// account for crossing clock domains
 `timescale 1 ns/100 ps

module mode_seq ( nGrst, rClk, wClk,
    scrub_done,
    stop_scrub,
    mode_init_ecc_wr,
    mode_check_ecc_rd,
    mode_init_scrub_wr,
    mode_run_scrub_rd,
    mode_check_scrub_rd,
    mode_report_rd,
    read_mode_front,
    write_mode_front  );
  parameter MODE         = 1;
  parameter SCRUB_ON     = 1;  // different from VHDL
  parameter SINGLECLK    = 1;
  parameter RAM_DEPTH    = 256;
  parameter RAM_LOGDEPTH = 8;
  parameter ENC_PIPE     = 1;
  parameter RAM_PIPE     = 1;
  parameter DEC_PIPE     = 1;
  parameter SCRUB_AMIN   = 0;
  parameter SCRUB_AMAX   = 100;
  parameter CLKPERIOD_RD = 10;
  parameter CLKPERIOD_WR = 11;
  parameter INIT_RAM     = 0;     
  
  localparam NO_MODE = 111;
  
  input nGrst, rClk, wClk, scrub_done;
  output mode_init_ecc_wr, mode_check_ecc_rd, mode_init_scrub_wr; 
  output mode_run_scrub_rd, mode_check_scrub_rd, mode_report_rd;
  output read_mode_front, write_mode_front, stop_scrub;

  reg mode_init_ecc_wr, mode_check_ecc_rd, mode_init_scrub_wr;
  reg mode_run_scrub_rd, mode_check_scrub_rd, mode_report_rd;
  reg start_scrub_skinny, stop_scrubi, scrub_done_rs;
  reg scrub_done_wr, mode_check_ecc_wr, mode_run_scrub_wr, mode_check_scrub_wr; 
  reg mode_report_wr, mode_check_ecc_wr_tick, mode_run_scrub_wr_tick;
  reg mode_check_scrub_wr_tick, mode_report_wr_tick;
  wire scrub_timeout;

  assign stop_scrub       = stop_scrubi;

  assign slow_clk         = (CLKPERIOD_RD > CLKPERIOD_WR) ? rClk : wClk;
  
  // scrub_done to cross clock domains
  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0)             scrub_done_rs <= 1'b0;
    else
      if(scrub_done==1'b1)      scrub_done_rs <= 1'b1;
      else
        if(scrub_done_wr==1'b1) scrub_done_rs <= 1'b0;

  always @(posedge wClk or negedge nGrst)
    if(nGrst==1'b0) scrub_done_wr <= 1'b0;
    else            scrub_done_wr <= scrub_done_rs;
  // scrub_done to cross clock domain ends

  // GENERATE mode SEQUENCE
  initial begin
    stop_scrubi         = 1'b1;
    start_scrub_skinny  = 1'b0;
    mode_init_ecc_wr    = 1'b0;
    mode_check_ecc_wr   = 1'b0;
    mode_init_scrub_wr  = 1'b0;
    mode_run_scrub_wr   = 1'b0;
    mode_check_scrub_wr = 1'b0;
    mode_report_wr      = 1'b0;
    
    @ (posedge nGrst);    // wait for nGrst to deassert
 
    // Keep NO_MODE while initializing RAM 
    if ( (MODE==1)&&(INIT_RAM==1) ) 
      repeat (SCRUB_AMAX-SCRUB_AMIN+3) @ (negedge wClk);
    else
      repeat (2) @ (negedge wClk);  
    repeat (10) @ (negedge slow_clk);    //Gap

    mode_init_ecc_wr = 1'b1;
    repeat (RAM_DEPTH+ENC_PIPE+RAM_PIPE+DEC_PIPE+2) @ (negedge wClk);
    mode_init_ecc_wr = 1'b0;
    repeat (10) @ (negedge slow_clk);    //Gap
      
    mode_check_ecc_wr = 1'b1;
    repeat (RAM_DEPTH+ENC_PIPE+RAM_PIPE+DEC_PIPE+2) @ (negedge wClk);
    mode_check_ecc_wr = 1'b0;
    repeat (10) @ (negedge slow_clk);    //Gap

    // run init scrub, scrub and check scrub a few times 
    stop_scrubi = 1'b0;
    repeat (3) begin
      mode_init_scrub_wr = 1'b1;
      repeat (RAM_DEPTH+ENC_PIPE+RAM_PIPE+DEC_PIPE+2) @ (negedge wClk);
      mode_init_scrub_wr = 1'b0;
      repeat (10) @ (negedge slow_clk);  //Gap
      
      // Kick off a scrubber mode timeout timer
      start_scrub_skinny  = 1'b1;
      repeat (1) @ (negedge slow_clk);
      start_scrub_skinny  = 1'b0;
      
      mode_run_scrub_wr   = 1'b1;
      // wait for scrub_done or timeout
      @ (posedge (scrub_done_wr || scrub_timeout))
      @ (negedge (scrub_done_wr || scrub_timeout))
      mode_run_scrub_wr   = 1'b0;
      repeat (10) @ (negedge slow_clk);  //Gap

      mode_check_scrub_wr = 1'b1;
      repeat (RAM_DEPTH+ENC_PIPE+RAM_PIPE+DEC_PIPE+2) @ (negedge wClk);
      mode_check_scrub_wr = 1'b0;
      repeat (10) @ (negedge slow_clk);  //Gap
    end
    
    mode_report_wr = 1'b1;
    repeat (10) @ (negedge wClk);
    mode_report_wr = 1'b0;
  end
  
  // Scrub mode timeout in case scrub_done does not come
  bhv_timeout # (.TIME_OUT(100*RAM_DEPTH)) scrub_timeout_0 (
    .nGrst(nGrst), 
    .rstn(mode_run_scrub_wr), 
    .clk(wClk), 
    .start(start_scrub_skinny),
    .timeout (scrub_timeout) );

  // Cross from rd to wr domain
  always @(posedge rClk or negedge nGrst)
    if (nGrst==1'b0) begin
      mode_check_ecc_rd   <= 1'b0;
      mode_run_scrub_rd   <= 1'b0;
      mode_check_scrub_rd <= 1'b0;
      mode_report_rd      <= 1'b0;
      mode_check_ecc_wr_tick   <= 1'b0;
      mode_run_scrub_wr_tick   <= 1'b0;
      mode_check_scrub_wr_tick <= 1'b0;
      mode_report_wr_tick      <= 1'b0;
    end
    else begin
      mode_check_ecc_wr_tick   <= #1 mode_check_ecc_wr;
      mode_run_scrub_wr_tick   <= #1 mode_run_scrub_wr;
      mode_check_scrub_wr_tick <= #1 mode_check_scrub_wr;
      mode_report_wr_tick      <= #1 mode_report_wr;
      mode_check_ecc_rd        <= #1 mode_check_ecc_wr_tick;
      mode_run_scrub_rd        <= #1 mode_run_scrub_wr_tick;
      mode_check_scrub_rd      <= #1 mode_check_scrub_wr_tick;
      mode_report_rd           <= #1 mode_report_wr_tick;
    end
  // Cross from rd to wr domain ends

  assign write_modes  = mode_init_ecc_wr  | mode_init_scrub_wr;
  assign read_modes   = mode_check_ecc_rd | mode_run_scrub_rd |
                                          mode_check_scrub_rd | mode_report_rd;

  bhvEdge # (.REDGE(1)) mode_wr_redge_0 (.nGrst(nGrst), .rst(1'b0), .clk(wClk),
    .inp(write_modes),
    .outp(write_mode_front) );

  bhvEdge # (.REDGE(1)) mode_rd_redge_0 (.nGrst(nGrst), .rst(1'b0), .clk(rClk),
    .inp(read_modes),
    .outp(read_mode_front) );

endmodule

