// ***************************************************************************/
//Microsemi Corporation Proprietary and Confidential
//Copyright 2013 Microsemi Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE MICROSEMI LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC Scrubber
//              Single and Independent Clock Scrubber
//
//Revision Information:
//Date         Description
//12Feb2013    Initial Release
//
//SVN Revision Information:
//SVN $Revision: $
//SVN $Data: $
//
//Resolved SARs
//SAR     Date        Who     Description
//74020   12/17/2015  VlaDi   Register divided_clk to get rid of glitches
//
//Notes:
//
//err_irq
//-------------------------------------------------------------  SCRUBBER  -----
//------------------------------------------------------------------------------
`timescale 1 ns/100 ps

//  ------------------  Time Out Divider and Timer  ---------------------------

  // Split timer into two parts: binary divider and a counter with arbitrary
  // terminal count.  Both are up to 32-bit wide
  // DIV_WDTH bits   TIMER_WDTH bits = log2(TMOUT_SET)
  //   ---------     --------------   Once Counter reaches TMOUT_SET
  //  | divider |-->|   timerCnt   |  it raises timeout flag
  //   ---------     --------------

//rt begins
module timeout_timer (nGrst, clk, rst,
  rst_timer, scrub_done, thin_tmout, slowdown, tmoutflg);
  parameter DIV_WDTH  = 10;
  parameter TMOUT_SET = 5;
  parameter SCRUB_AMIN  = 0;
  parameter SCRUB_AMAX  = 255;

  localparam TIMER_WDTH = ceil_log2(TMOUT_SET);
  localparam [DIV_WDTH-1:0] DIVFACTOR  = {DIV_WDTH{1'b1}};

  input nGrst, clk, rst;
  input rst_timer;    // optional timer reset
  input scrub_done;
  output slowdown;
  output tmoutflg;
  output thin_tmout;
  reg slowdown, warn2slow;

  wire[1:0] status;
  wire [DIV_WDTH-1:0] divider;
  wire divided_clk;
  reg [TIMER_WDTH-1:0] timerCnt;
  reg timeOut;
  reg divided_clk_reg;                                          //sar74020
  wire scrub_done_w, grace_end;

  wire rst_binary_div = rst | rst_timer | scrub_done;

  edac_kitCountS # ( .WIDTH(DIV_WDTH), .DCVALUE(DIVFACTOR),   // Binary divider
                .BUILD_DC(1) ) bin_div_0 (
    .nGrst(nGrst), .rst(rst_binary_div), .clk(clk), .clkEn(1'b1),
    .cntEn(1'b1), .Q(divider), .dc(divided_clk) );

  always @(posedge clk or negedge nGrst)                        //sar74020
      if (nGrst==1'b0)
        divided_clk_reg <= 1'b0;
      else
        divided_clk_reg <= divided_clk;                         //sar74020

  edac_kit_pulse_trap trap_scrub_done_0 (
    .nGrst    (nGrst),
    .rst      (rst),
    .fast_clk (clk),
//sar74020    .slow_clk (divided_clk),
    .slow_clk (divided_clk_reg),
    .inp      (scrub_done),
    .outp     (scrub_done_w) );

  assign status = {scrub_done_w, timeOut};
  // Status   Action
  //   00     timerCnt to keep counting; no warn2slow internal flag
  //   01     raise warn2slow internal flag
  //   10, 11 reset timerCnt as a new scrubbing cycle has begun
  always @(posedge clk or negedge nGrst) begin
      if (nGrst==1'b0) begin
        timerCnt   <= 'b0;
        timeOut    <= 1'b0;
        warn2slow  <= 1'b0;
      end
      else
        if(rst_timer == 1'b1) begin
          timerCnt   <= 'b0;
          timeOut    <= 1'b0;
          warn2slow  <= 1'b0;
        end
        else begin
          timeOut <= (timerCnt==TMOUT_SET-1);
//sar74020          if(divided_clk==1'b1)   begin
          if(divided_clk_reg==1'b1)   begin
            case (status)   //status = {scrub_done, timeOut}
              2'b00 : begin       // not scrub_done and not timeOut
                warn2slow <= 1'b0;
                timerCnt  <= timerCnt+1;
              end
              2'b01 : begin       // not scrub_done and  timeOut
                warn2slow <= 1'b1;
              end
              2'b10 :begin        //  scrub_done and not timeOut
                warn2slow <= 1'b0;
                timerCnt  <= 'b0;
              end
              2'b11 :begin        //  scrub_done and timeOut
                warn2slow <= 1'b0;
                timerCnt  <= 'b0;
              end
            endcase
          end
        end
  end

  assign tmoutflg = timeOut;

  // 1-clk wide fr edge of timeOut
  edac_kitEdge # (.REDGE(1)) skinny_timeOut_0 (
    .nGrst(nGrst), .rst(1'b0), .clk(clk),
    .inp(timeOut),
    .outp(thin_tmout)    );
  // ---------------------  End Divider and Timer

  // On warn2slow, start scrubbing if possible.  Wait for a grace period of
  // 2*(SCRUB_AMAX-SCRUB_AMIN), and raise slowdown flag if scrubbing session is
  // not over
  edac_kitCountS # (.WIDTH(ceil_log2(SCRUB_AMAX-SCRUB_AMIN)+1),
                    .DCVALUE(2*(SCRUB_AMAX-SCRUB_AMIN)-1),
                    .BUILD_DC(1) ) grace_period_0 (
    .nGrst(nGrst), .rst(~warn2slow), .clk(clk), .clkEn(1'b1),
    .cntEn(1'b1), .Q(), .dc(grace_end) );

  always @(posedge clk or negedge nGrst)
      if (nGrst==1'b0)
        slowdown   <= 1'b0;
      else
        if(rst_binary_div == 1'b1)
          slowdown   <= 1'b0;
        else
          if(grace_end==1'b1)
            slowdown  <= 1'b1;
//  ----------------------------------------------------------
  function [31:0] ceil_log2;
      input integer x;
      integer tmp, res;
    begin
      tmp = 1;
      res = 0;
      while (tmp < x) begin
        tmp = tmp * 2;
        res = res + 1;
      end
      ceil_log2 = res;
    end
  endfunction
endmodule
//rt ends


//              +-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+ +-+-+-+
//              |S|i|n|g|l|e| |R|e|a|d|-|W|r|i|t|e| |c|l|k|
//              +-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+ +-+-+-+
module scrubi ( nGrst, clk,
                rst,          //rt
                stop_scrub,
                errFlag,
                correctable,
                start_scrub,
                init,
                rst_timer,
                re_code,
                scrub_rA,
                scrub_wA,
                scrub_wD,
                scrub_wEn,
                epsd,
                scrub_due,
                slowdown,
                tmoutflg,
                scrub_done,
                now_scrubbing    );
  parameter DAT_WIDTH = 16;
  parameter PAR_WIDTH = 5;
  parameter WRBK_ON   = 1;    // Optional writeback
  parameter DIV_WDTH  = 20;
  parameter TMOUT_SET = 10;
  parameter SCRUB_AMIN  = 0;
  parameter SCRUB_AMAX  = 255;
  parameter ENC_PIPE  = 0;
  parameter DEC_PIPE  = 0;
  parameter RAM_PIPE  = 0;
  parameter RAM_LOGDEPTH = 8;

  localparam MAX_PIPE = (DEC_PIPE<ENC_PIPE)? ENC_PIPE : DEC_PIPE;
  localparam DECD_DLY = DEC_PIPE + RAM_PIPE;
  localparam WRBK_DLY = DEC_PIPE + RAM_PIPE + 2;

  input nGrst, clk;
  input rst;              //rt
  input stop_scrub;   // makes ports available to user
  input errFlag;
  input correctable;

  input start_scrub;  // optional start in addition to a timer-driven
  input init;         // initialization in progress.  Do not scrub
  input rst_timer;    // optional timer reset

  input [PAR_WIDTH+DAT_WIDTH-1 : 0] re_code ;

  output [RAM_LOGDEPTH-1 : 0] scrub_rA;
  output [RAM_LOGDEPTH-1 : 0] scrub_wA;
  output [PAR_WIDTH+DAT_WIDTH-1 : 0] scrub_wD;
  output scrub_wEn;
  output epsd, scrub_due;
  // optional ports
  output slowdown;
  output tmoutflg;
  output scrub_done, now_scrubbing;

  reg scrub_done;       //rt
  reg scrub_due;
  wire scrub_done_w;    //rt
  // --------------------------------------------------------------------------

  wire wrBk_valid;
  reg fixable_err;
  wire scrub_due_fr;
  reg wrBk_valid_tick;
  wire block_wrBk_tail;
  wire thin_tmout, block_wrBk;
  wire u_access_fr, u_access_bk, block_wrBk_start;
  wire [RAM_LOGDEPTH-1:0] return_scrub_rA;
  wire stop_scrubi, stop_scrub_dly;

// ----------------------------------------------------   BEGIN

  // extend stop_scrub in case ENC_PIPE > 0
  edac_kitDelay_bit_ripple # (.DELAY(ENC_PIPE)) stretch_stop_scrub_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(clk), .clkEn(1'b1),
    .inp(stop_scrub), .outp(),
    .ripple(stop_scrub_dly)   ) ;
  assign stop_scrubi = stop_scrub | stop_scrub_dly;

  // Create now_scrubbing to mark error & correctable flags caused by scrubbing
  edac_kitDelay_bit_reg # (.DELAY(DECD_DLY+1)) now_scrub_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .clk(clk), .clkEn(1'b1), .rst(1'b0),
    .inp(epsd), .outp(now_scrubbing) ) ;


  // ---------------------  Divider and Timer
  timeout_timer # (.DIV_WDTH(DIV_WDTH), .TMOUT_SET(TMOUT_SET),
              .SCRUB_AMAX(SCRUB_AMAX), .SCRUB_AMIN(SCRUB_AMIN) ) tmout_timer_0 (
    .nGrst      (nGrst     ),
    .clk        (clk       ),
    .rst        (rst),                //rt
    .rst_timer  (rst_timer ),
    .scrub_done (scrub_done),
    .thin_tmout (thin_tmout),
    .slowdown   (slowdown  ),
    .tmoutflg  (tmoutflg )  );

  // scrub_due if timeOut or start_scrub. It only goes high after scrub_done;
  // otherwise it just stays high ignoring consecutive timeOut or start_scrub
  always @(posedge clk or negedge nGrst)  begin
    if(nGrst==1'b0) begin
      scrub_due     <= 1'b0;
//rt      wrBk_valid_tick <= 1'b0;
    end
    else                              //rt
      if(rst==1'b1)                   //rt
        scrub_due     <= 1'b0;        //rt
      else  begin
        wrBk_valid_tick <= wrBk_valid;

        if(thin_tmout || start_scrub)
          scrub_due <= 1'b1;
        else
        if  (scrub_done==1'b1)
          scrub_due <= 1'b0;
      end
  end

  // scrub_due front edge. It only can come following scrub_done, as there the
  // scrub_due goes low, which is necessary to form a rising edge later on
  edac_kitEdge # (.REDGE(1)) scrub_due_fr_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(clk),
    .inp(scrub_due),
    .outp(scrub_due_fr)    );


  // Detect front edge of the user access but only the one that follows the
  // wrBk_valid
  assign u_access_fr = wrBk_valid_tick & stop_scrubi;

  // detect rear edge of the user access
  edac_kitEdge # (.REDGE(0)) u_access_bk_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(clk),
    .inp(stop_scrubi),
    .outp(u_access_bk)    );

  assign epsd = scrub_due & ~stop_scrubi & ~init;
  assign wrBk_valid = epsd & ~u_access_bk & ~block_wrBk;

  // Block wrBk after scrub_due starts as well as after u_access ends until the
  // scrubber reads a RAM cell, decoder generates re_code and error/correctable
  assign block_wrBk_start = scrub_due_fr | u_access_bk;

  edac_kitDelay_bit_ripple # (.DELAY(WRBK_DLY)) wrBk_dly_1 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(clk), .clkEn(1'b1),
    .inp(block_wrBk_start), .outp(),
    .ripple(block_wrBk_tail)   ) ;

  assign block_wrBk = block_wrBk_start | block_wrBk_tail;
  // End block_wrBk

  // Scrub rA, wA and The Last Truly Scrubbed Addr reg (return_scrub_rA)
  edac_kitCountC # ( .WIDTH(RAM_LOGDEPTH), .UPDOWN(0), .DCVALUE(1),
                .BUILD_CLKEN(0), .BUILD_RST(1), .BUILD_LD(1),
                .BUILD_DC(0) ) scrub_rA_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(scrub_due_fr),
    .clk(clk), .clkEn(1'b1), .cntEn(epsd),
    .up(1'b1),
    .ld(u_access_bk),
    .din(return_scrub_rA),
    .Q(scrub_rA), .dc() );

  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(WRBK_DLY) ) scrub_wA_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(clk), .clkEn(1'b1),
    .inp(scrub_rA), .outp(scrub_wA)  );

  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(1) ) addr2start_0 (
    .nGrst(nGrst), .clk(clk),
    // load on u_access_fr, reset on scrub_done
//rt    .rst(scrub_done),
//rt    .clkEn(u_access_fr | scrub_done),
    .rst(rst | scrub_done),
    .clkEn(rst | u_access_fr | scrub_done),
    .inp(scrub_wA), .outp(return_scrub_rA)  );

  // Scrub done
  assign scrub_Amax_wr = (scrub_wA == SCRUB_AMAX);
//rt  assign scrub_done    = wrBk_valid & scrub_Amax_wr;
  assign scrub_done_w    = wrBk_valid & scrub_Amax_wr;          //rt

  //rt
  always @(posedge clk)
    if(rst==1'b1)
      scrub_done <= 1'b0;
    else
      scrub_done <= scrub_done_w;
  //rt ends

  // Delay re-code by a tick to match fixable_err flag
  edac_kitDelay_reg # (.BITWIDTH(PAR_WIDTH+DAT_WIDTH), .DELAY(1) ) scrub_wD_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(clk), .clkEn(1'b1),
    .inp(re_code), .outp(scrub_wD)  );

  always @(posedge clk or negedge nGrst)
    if(nGrst==1'b0)
      fixable_err  <= 1'b0;
    else                                    //rt
      if(rst==1'b1)                         //rt
        fixable_err  <= 1'b0;               //rt
      else
        fixable_err <= errFlag & correctable;

  assign scrub_wEn = fixable_err & wrBk_valid;

endmodule



//          +-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+ +-+-+-+
//          |I|n|d|e|p|e|n|d|e|n|t| |R|e|a|d|-|W|r|i|t|e| |c|l|k|
//          +-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+ +-+-+-+
module scrubii ( nGrst, rClk, wClk,
                rst,                                              //rt
                stop_scrub,
                errFlag,
                correctable,
                start_scrub,
                init,
                rst_timer,
                re_code,
                scrub_rA,
                scrub_wA,
                scrub_wD,
                scrub_wEn,
                now_scrubbing,
                r_epsd,
                slowdown,
                tmoutflg,
                scrub_done    );
  parameter DAT_WIDTH = 16;
  parameter PAR_WIDTH = 5;
  parameter WRBK_ON   = 1;    // Optional writeback
  parameter DIV_WDTH     = 20;
  parameter TMOUT_SET    = 10;
  parameter SCRUB_AMIN   = 0;
  parameter SCRUB_AMAX   = 255;
  parameter ENC_PIPE  = 0;
  parameter DEC_PIPE  = 0;
  parameter RAM_PIPE  = 0;
  parameter RAM_LOGDEPTH = 8;

  localparam MAX_PIPE = (DEC_PIPE<ENC_PIPE)? ENC_PIPE : DEC_PIPE;
  localparam DECD_DLY = DEC_PIPE + RAM_PIPE;
  localparam WRBK_DLY = DEC_PIPE + RAM_PIPE + 2;


  input nGrst, rClk, wClk;
  input rst;                                                      //rt
  input stop_scrub;   // makes ports available to user
  input errFlag;
  input correctable;

  input start_scrub;  // optional start in addition to a timer-driven
  input init;         // initialization in progress.  Do not scrub
  input rst_timer;    // optional timer reset

  input [PAR_WIDTH+DAT_WIDTH-1 : 0] re_code ;

  output [RAM_LOGDEPTH-1 : 0] scrub_rA;
  output [RAM_LOGDEPTH-1 : 0] scrub_wA;
  output [PAR_WIDTH+DAT_WIDTH-1 : 0] scrub_wD;
  output scrub_wEn;
  // optional ports
  output now_scrubbing;
  output slowdown;
  output tmoutflg;
  output scrub_done;
  output r_epsd;

  reg scrub_done;
  reg scrub_due, fixable_err, err_irq;
  reg r_user_irq;
  wire thin_tmout;
  wire r_stop_scrub_bk, r_user_irq_bk, err_irq_fr;
  wire update_rtrn_rA;
  wire end_err_irq;
  wire[RAM_LOGDEPTH-1:0] current_scrubA, next_scrubA;
  wire[RAM_LOGDEPTH-1:0] rtrn2same_addr, rtrn2next_addr, rtrn_rA;
  wire r_stop_scrub, w_stop_scrub, r_stop_scrub_fr;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] re_code_tick2;
  wire any_err_irq_fr;
  reg max_passed;
  reg r_init, init_tick;

  //  -------------------------------------------------------------------------
  assign scrub_not_due = ~scrub_due;

  // ---------------------  Divider and Timer
  timeout_timer # (.DIV_WDTH(DIV_WDTH), .TMOUT_SET(TMOUT_SET),
              .SCRUB_AMAX(SCRUB_AMAX), .SCRUB_AMIN(SCRUB_AMIN) ) tmout_timer_0 (
    .nGrst      (nGrst     ),
    .clk        (rClk      ),
    .rst  (rst),                                //rt
    .rst_timer  (rst_timer ),
    .scrub_done (scrub_done),
    .thin_tmout (thin_tmout),
    .slowdown   (slowdown  ),
    .tmoutflg   (tmoutflg )  );

  // scrub_due if timeOut or start_scrub;
  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0)                   scrub_due <= 1'b0;
    else                                                            //rt
      if(rst)                         scrub_due <= 1'b0;            //rt
      else
        if(thin_tmout || start_scrub)   scrub_due <= 1'b1;
        else
          if  (scrub_done==1'b1)        scrub_due <= 1'b0;

  //  ----------------------------  USER INTERRUPT  --------------------------
  //  Synchronize external stop scrub to rClk and wClk
  edac_kit_stop_scrub stop_scrub_gen_0 (
//rt    .nGrst       (nGrst       ),
    .nGrst  (1'b1),
    .rst    (1'b0),                                                 //rt
    .rClk        (rClk        ),
    .wClk        (wClk        ),
    .stop_scrub  (stop_scrub  ),      // in
    .r_stop_scrub(r_stop_scrub),      // out
    .w_stop_scrub(w_stop_scrub) );    // out

  // A SM generates r_user_irq and defines if stop_scrub comes in the midst of
  // err_irq or when scrubber just does regular reading
  always @(posedge rClk or negedge nGrst)
    if(nGrst==1'b0)             r_user_irq <= 1'b0;
    else                                                            //rt
      if(rst==1'b1)             r_user_irq <= 1'b0;                 //rt
      else
        if(r_stop_scrub==1'b1)    r_user_irq <= 1'b1;
        else
          if(r_user_irq_bk==1'b1) r_user_irq <= 1'b0;

  // stop_scrub front edge
  edac_kitEdge # (.REDGE(1)) stop_scrub_fr_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(rClk),
    .inp(r_stop_scrub),
    .outp(r_stop_scrub_fr)    );


  // stop_scrub rear edge
  edac_kitEdge # (.REDGE(0)) stop_scrub_rear_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(rClk),
    .inp(r_stop_scrub),
    .outp(r_stop_scrub_bk)    );

  // Prolong user_irq to cover potential fixable_err's
  edac_kitDelay_bit_reg # (.DELAY(WRBK_DLY)) wrBk_dly_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(rClk), .clkEn(1'b1),
    .inp(r_stop_scrub_bk), .outp(r_user_irq_bk) ) ;

  // To block scrubbing while initializing RAM, take init to rClk domain
//rt  always @(posedge rClk or negedge nGrst) begin
//rt    if(!nGrst) begin
//rt      init_tick <= 1'b1;
//rt      r_init    <= 1'b1;
//rt    end
//rt    else
  always @(posedge rClk) begin
    begin
      init_tick <= init;
      r_init    <= init_tick;
    end
  end

  //  ------------------  SCRUB RA COUNTER AND ITS DELAYED COPIES -------------
  assign r_epsd=scrub_due & ~r_user_irq & ~r_stop_scrub_fr & ~err_irq & ~r_init;

  // Create now_scrubbing to mark error & correctable flags caused by scrubbing
  edac_kitDelay_bit_reg # (.DELAY(DECD_DLY+1)) now_scrub_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .clk(rClk), .clkEn(1'b1), .rst(1'b0),
    .inp(r_epsd), .outp(now_scrubbing) ) ;


  // To capture return addresses, current and next, use back edge of the r_epsd
  edac_kitEdge # (.REDGE(0)) capture_return_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0),
    .clk(rClk),
    .inp(r_epsd),
    .outp(capture_rtrn)    );

  // Store return addresses for both IRQ types
  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(1) )
    rtrn2same_0 (
      .nGrst(nGrst),
      .clk(rClk),
      //14/11/13  .rst(1'b0),
      .rst(scrub_not_due),
      .clkEn(capture_rtrn),
      .inp(current_scrubA), .outp(rtrn2same_addr)  );

  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(1) )
    rtrn2next_0 (
      .nGrst(nGrst),
      .clk(rClk),
      //14/11/13  .rst(1'b0),
      .rst(scrub_not_due),
      .clkEn(capture_rtrn),
      .inp(next_scrubA), .outp(rtrn2next_addr)  );

  // Generate update_rtrn_rA pulse
  edac_kitDelay_bit_reg # (.DELAY(1)) err_irq_fr_tick_0 (
    .nGrst(nGrst),
    .clk(rClk), .clkEn(1'b1),
    .rst(r_stop_scrub),   // Upon user IRQ kill the err_irq ASAP
    .inp(err_irq_fr), .outp(err_irq_fr_tick) ) ;

  edac_kitDelay_bit_reg # (.DELAY(1)) stop_scrub_fr_tick_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(rClk), .clkEn(1'b1),
    .inp(r_stop_scrub_fr), .outp(stop_scrub_fr_tick) ) ;

  assign update_rtrn_rA = err_irq_fr_tick | stop_scrub_fr_tick;
  assign rtrn_rA = (err_irq_fr_tick == 1'b1) ? rtrn2next_addr : rtrn2same_addr;

  edac_kitCountC # ( .WIDTH(RAM_LOGDEPTH), .UPDOWN(0), .DCVALUE(1),
                .BUILD_CLKEN(0), .BUILD_RST(1), .BUILD_LD(1),
                .BUILD_DC(0) ) scrub_rA_0 (
    .nGrst(nGrst), .clk(rClk), .clkEn(1'b1), .up(1'b1),
    .rst(scrub_not_due),
    .cntEn(r_epsd),
    .ld(update_rtrn_rA),
    .din(rtrn_rA),
    .Q(scrub_rA), .dc() );

  // Next after the truly scrubbed addr
  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(WRBK_DLY) )
    scrub_next_a_0 (.nGrst(nGrst), .rst(scrub_not_due), .clk(rClk), .clkEn(1'b1),
                    .inp(scrub_rA), .outp(next_scrubA)  );

  // Truly Scrubbed Address
  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(1) ) scrub_current_a_0 (
    .nGrst(nGrst), .rst(scrub_not_due), .clk(rClk), .clkEn(1'b1),
    .inp(next_scrubA), .outp(current_scrubA)  );

  //  ------------------  ERROR INTERRUPT   ---------------------
//rt  always @(posedge rClk or negedge nGrst)
//rt    if(nGrst==1'b0) begin
//rt      fixable_err <= 1'b0;
//rt      err_irq     <= 1'b0;
//rt    end
//rt    else
  always @(posedge rClk)
    begin
    // Actual scrub starts when r_epsd==1. Thus valid scrub read data appears at
    // Dec output along with error/correctable later: when 'now_scrubbing' is on
      fixable_err <= errFlag & correctable & now_scrubbing;   //err_irq
      if( (end_err_irq==1'b1) || (r_stop_scrub==1'b1) || (scrub_due==1'b0) )
        err_irq     <= 1'b0;
      else
        if (fixable_err==1'b1)  err_irq <= 1'b1;
    end

  // Front edge of the err_irq
  edac_kitEdge # (.REDGE(1)) err_irq_fr_0 (
    .nGrst(nGrst),
    .rst(r_stop_scrub),     // Upon user IRQ kill the err_irq_fr ASAP
    .clk(rClk),
    .inp(err_irq),
    .outp(any_err_irq_fr)    );


  // Not any err_irq is valid but only the first err_irq_fr that follows r_epsd
  assign err_irq_fr = any_err_irq_fr & capture_rtrn;

  // Store address of an error as well as a corrected code for the address
  edac_kitDelay_reg # (.BITWIDTH(RAM_LOGDEPTH), .DELAY(1) ) error_addr_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rst(1'b0), .clk(rClk), .clkEn(err_irq_fr),
    .inp(current_scrubA), .outp(scrub_wA)  );

  // Delay re-code to match fixable_err flag (err_irq_fr)
  edac_kitDelay_reg # (.BITWIDTH(PAR_WIDTH+DAT_WIDTH), .DELAY(2) )
    recode_tick_0(
//rt      .nGrst(nGrst),
      .nGrst(1'b1),
      .rst(1'b0), .clk(rClk), .clkEn(1'b1),
      .inp(re_code), .outp(re_code_tick2)  );

  edac_kitDelay_reg # (.BITWIDTH(DAT_WIDTH+PAR_WIDTH), .DELAY(1) )
    recode_reg_0 (
//rt      .nGrst(nGrst),
      .nGrst(1'b1),
      .rst(1'b0), .clk(rClk), .clkEn(err_irq_fr),
      .inp(re_code_tick2), .outp(scrub_wD)  );

  //  -------------------------  SCRUB DONE LOGIC  ----------------------------
//rt  always @(posedge rClk or negedge nGrst)
  always @(posedge rClk)
//rt    if(nGrst==1'b0) begin
//rt      max_passed  <= 1'b0;
//rt      scrub_done  <= 1'b0;
//rt    end
//rt    else

    begin
      if(scrub_due==1'b0) begin
        max_passed  <= 1'b0;
        scrub_done  <= 1'b0;
      end
      else  begin
        if(scrub_done==1'b1)
          max_passed <= 1'b0;
        else
          if(scrub_rA==SCRUB_AMAX)
            max_passed <= 1'b1;
        if(scrub_rA==SCRUB_AMIN+WRBK_DLY+1)
          scrub_done  <= max_passed;
      end
    end


  //  --------------------  CROSS TO WCLK DOMAIN  -----------------------
  edac_kit_x_domain # (.DELAY(WRBK_DLY-2) ) cross_clk2w_0 (
//rt    .nGrst(nGrst),
    .nGrst(1'b1),
    .rClk(rClk), .wClk(wClk),
    .r_inp(err_irq_fr),
    .w_outp(scrub_wEn),
    .r_ack(end_err_irq),
    .r_stop_scrub(r_stop_scrub),
    .w_stop_scrub(w_stop_scrub) );

endmodule



// -------------------------------- Combined scrubi and scrubii ---------------
module scrub  ( nGrst, rClk, wClk,
                rst,                    //rt
                stop_scrub,
                errFlag,
                correctable,
                start_scrub,
                init,
                rst_timer,
                re_code,
                mux_scrub_rd,
                mux_scrub_wr,
                scrub_due,
                scrub_rA,
                scrub_wA,
                scrub_wD,
                scrub_wEn,
                error_from_scrub,
                correctable_from_scrub,
                slowdown,
                tmoutflg,
                now_scrubbing,
                scrub_done    );

  parameter DAT_WIDTH = 16;
  parameter PAR_WIDTH = 5;
  parameter WRBK_ON   = 1;
  parameter DIV_WDTH     = 20;  // binary divider
  parameter TMOUT_SET    = 10;
  parameter SCRUB_AMIN   = 0;
  parameter SCRUB_AMAX   = 255;
  parameter ENC_PIPE  = 0;
  parameter DEC_PIPE  = 0;
  parameter RAM_PIPE  = 0;
  parameter RAM_LOGDEPTH = 8;  // Binary Log of the RAM depth
  parameter SINGLECLK    = 1;  // single r/w clock (1), or independent (0)

  input nGrst, rClk, wClk ;
  input rst;                        //rt
  input stop_scrub;   // makes ports available to user
  input errFlag;
  input correctable;
  input start_scrub;  // optional start in addition to a timer-driven one
  input init;         // initialization in progress.  Do not scrub
  input rst_timer;    // optional timer reset
  input [PAR_WIDTH+DAT_WIDTH-1 : 0] re_code ;

  output mux_scrub_rd, mux_scrub_wr;
  output [RAM_LOGDEPTH-1 : 0] scrub_rA;
  output [RAM_LOGDEPTH-1 : 0] scrub_wA;
  output [PAR_WIDTH+DAT_WIDTH-1 : 0] scrub_wD;
  output scrub_wEn, scrub_due;
  // optional flags
  output error_from_scrub, correctable_from_scrub;
  output slowdown;
  output tmoutflg;
  output now_scrubbing;
  output scrub_done;

  wire epsd;
  wire now_scrubbingi;

  assign now_scrubbing = now_scrubbingi;
  generate
    if(SINGLECLK) begin: scrubi
      assign mux_scrub_rd = epsd;
      assign mux_scrub_wr = epsd;

      scrubi # (.DAT_WIDTH(DAT_WIDTH),
                .PAR_WIDTH(PAR_WIDTH),
                .WRBK_ON  (WRBK_ON),
                .DIV_WDTH (DIV_WDTH),
                .TMOUT_SET (TMOUT_SET),
                .SCRUB_AMIN(SCRUB_AMIN),
                .SCRUB_AMAX(SCRUB_AMAX),
                .ENC_PIPE(ENC_PIPE),
                .DEC_PIPE(DEC_PIPE),
                .RAM_PIPE(RAM_PIPE),
                .RAM_LOGDEPTH(RAM_LOGDEPTH) ) scrubi_0 (
          .nGrst      (nGrst), .clk (rClk),
          .rst(rst),                          //rt
          .stop_scrub (stop_scrub),
          .errFlag    (errFlag),
          .correctable(correctable),

          .start_scrub(start_scrub),
          .init       (init),
          .rst_timer  (rst_timer),
          .re_code    (re_code),

          .scrub_rA   (scrub_rA),
          .scrub_wA   (scrub_wA),
          .scrub_wD   (scrub_wD),
          .scrub_wEn  (scrub_wEn),
          .now_scrubbing  (now_scrubbingi),
          .scrub_due  (scrub_due),
          .epsd       (epsd),
          .slowdown   (slowdown),
          .tmoutflg   (tmoutflg),
          .scrub_done (scrub_done)    );
    end

    else  begin: scrubii
      assign mux_scrub_rd = epsd;
      assign mux_scrub_wr = scrub_wEn;

      scrubii # ( .DAT_WIDTH(DAT_WIDTH),
                  .PAR_WIDTH(PAR_WIDTH),
                  .WRBK_ON  (WRBK_ON),
                  .DIV_WDTH (DIV_WDTH),
                  .TMOUT_SET (TMOUT_SET),
                  .SCRUB_AMIN(SCRUB_AMIN),
                  .SCRUB_AMAX(SCRUB_AMAX),
                  .ENC_PIPE(ENC_PIPE),
                  .DEC_PIPE(DEC_PIPE),
                  .RAM_PIPE(RAM_PIPE),
                  .RAM_LOGDEPTH(RAM_LOGDEPTH) ) scrubii_0 (
          .nGrst(nGrst), .rClk (rClk), .wClk (wClk),
          .rst (rst),                                                 //rt
          .stop_scrub     (stop_scrub),
          .errFlag        (errFlag),
          .correctable    (correctable),
          .start_scrub    (start_scrub),
          .init           (init),
          .rst_timer      (rst_timer),
          .re_code        (re_code),
          .scrub_rA       (scrub_rA),
          .scrub_wA       (scrub_wA),
          .scrub_wD       (scrub_wD),
          .scrub_wEn      (scrub_wEn),
          .now_scrubbing  (now_scrubbingi),
          .r_epsd         (epsd),
          .slowdown       (slowdown),
          .tmoutflg       (tmoutflg),
          .scrub_done     (scrub_done)    );
    end
  endgenerate

  assign error_from_scrub = errFlag;
  assign correctable_from_scrub = correctable;

endmodule



//rtmodule timeout_timer (nGrst, clk,
//rt  rst,      //rt
//rt  rst_timer, scrub_done, thin_tmout, slowdown, tmoutflg);
//rt  parameter DIV_WDTH  = 10;
//rt  parameter TMOUT_SET = 5;
//rt
//rt  localparam TIMER_WDTH = ceil_log2(TMOUT_SET);
//rt
//rt  input nGrst, clk, rst;
//rt  input rst_timer;    // optional timer reset
//rt  input scrub_done;
//rt  output slowdown;
//rt  output tmoutflg;
//rt  output thin_tmout;
//rt  reg slowdown;
//rt
//rt  wire[1:0] status;
//rt  wire [DIV_WDTH-1:0] divider;
//rt  wire divided_clk;
//rt  reg [TIMER_WDTH-1:0] timerCnt;
//rt  reg timeOut;
//rt  wire rst_timer_w, scrub_done_w;
//rt
//rt  wire rst_binary_div = rst | rst_timer | scrub_done;     //rt
//rt
//rt  edac_kitCountS # ( .WIDTH(DIV_WDTH), .DCVALUE(1),   // Binary divider
//rt                .BUILD_DC(0) ) counter_0 (
//rt    .nGrst(nGrst), .rst(rst_binary_div), .clk(clk), .clkEn(1'b1),
//rt    .cntEn(1'b1), .Q(divider), .dc() );
//rt  assign divided_clk = divider[DIV_WDTH-1];
//rt
//rt  // Catch possibly narrow rst_timer and generate a pulse of divided_clk domain
//rt  // If rst_timer is wide, the output pulse will be wider, too.
//rt  edac_kit_pulse_trap trap_rst_timer_0 (
//rt    .nGrst    (nGrst),
//rt    .rst      (rst),                                      //rt
//rt    .fast_clk (clk),
//rt    .slow_clk (divided_clk),
//rt    .inp      (rst_timer),
//rt    .outp     (rst_timer_w) );
//rt
//rt  edac_kit_pulse_trap trap_scrub_done_0 (
//rt    .nGrst    (nGrst),
//rt    .rst      (rst),                    //rt
//rt    .fast_clk (clk),
//rt    .slow_clk (divided_clk),
//rt    .inp      (scrub_done),
//rt    .outp     (scrub_done_w) );
//rt
//rt  assign status = {scrub_done_w, timeOut};
//rt  // Status   Action
//rt  //   00     timerCnt to keep counting; no slowdown flag
//rt  //   01     raise slowdown flag
//rt  //   10     reset timerCnt as a new scrubbing cycle has begun
//rt  //   11     reset timerCnt as a new scrubbing cycle has begun
//rt  always @(posedge divided_clk or negedge nGrst) begin
//rt      if (nGrst==1'b0) begin
//rt        timerCnt   <= 'b0;
//rt        timeOut    <= 1'b0;
//rt        slowdown   <= 1'b0;
//rt      end
//rt      else
//rt        if(rst_timer_w == 1'b1) begin
//rt          timerCnt   <= 'b0;
//rt          timeOut    <= 1'b0;
//rt          slowdown   <= 1'b0;
//rt        end
//rt        else  begin
//rt          timeOut <= (timerCnt==TMOUT_SET-1);
//rt          case (status)   //status = {scrub_done, timeOut}
//rt            2'b00 : begin       // not scrub_done and not timeOut
//rt              slowdown <= 1'b0;
//rt              timerCnt <= timerCnt+1;
//rt            end
//rt            2'b01 : begin       // not scrub_done and  timeOut
//rt              slowdown <= 1'b1;
//rt            end
//rt            2'b10 :begin        //  scrub_done and not timeOut
//rt              slowdown <= 1'b0;
//rt              timerCnt <= 'b0;
//rt            end
//rt            2'b11 :begin        //  scrub_done and timeOut
//rt              slowdown <= 1'b0;
//rt              timerCnt <= 'b0;
//rt            end
//rt          endcase
//rt        end
//rt  end
//rt
//rt  // Divided_clk-wide timeOut front edge
//rt  edac_kitEdge # (.REDGE(1)) timeOut_flag_0 (
//rt    .nGrst(nGrst), .rst(1'b0), .clk(divided_clk),
//rt    .inp(timeOut),
//rt    .outp(tmoutflg)    );
//rt
//rt  // 1-clk wide fr edge of timeOut
//rt  edac_kitEdge # (.REDGE(1)) skinny_timeOut_0 (
//rt    .nGrst(nGrst), .rst(1'b0), .clk(clk),
//rt    .inp(timeOut),
//rt    .outp(thin_tmout)    );
//rt  // ---------------------  End Divider and Timer
//rt
//rt  function [31:0] ceil_log2;
//rt      input integer x;
//rt      integer tmp, res;
//rt    begin
//rt      tmp = 1;
//rt      res = 0;
//rt      while (tmp < x) begin
//rt        tmp = tmp * 2;
//rt        res = res + 1;
//rt      end
//rt      ceil_log2 = res;
//rt    end
//rt  endfunction
//rtendmodule
//rt

