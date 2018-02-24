// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC
//              Top RTL Module
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
//---------------------------------------------------------  EDAC and RAM  -----
//------------------------------------------------------------------------------

`timescale 1 ns/100 ps

module test_4463andcpuopration_COREEDAC_0_COREEDAC (
    NGRST,
    RST  ,
    RCLK ,
    WCLK ,
    STOP_SCRUB_TRP1,
    STOP_SCRUB_TRP2,
    STOP_SCRUB_TRP3,
    // Decoder output
    DATA_OUT,
    PARITY_OUT,
    // user ports
    USER_WEN_TRP1,
    USER_WEN_TRP2,
    USER_WEN_TRP3,
    MSG_TRP1,
    MSG_TRP2,
    MSG_TRP3,
    USER_WA_TRP1,
    USER_WA_TRP2,
    USER_WA_TRP3,
    USER_REN_TRP1,
    USER_REN_TRP2,
    USER_REN_TRP3,
    USER_RA_TRP1,
    USER_RA_TRP2,
    USER_RA_TRP3,
    // RAM direct output used for test purpose = Decoder input
    CODE_FROM_RAM,
    CODE_FROM_EXT_RAM,
    // test input to inject errors after encoder
    INJ_ERR,
    // optional scrub start in addition to timer-driven one
    START_SCRUB_TRP1,
    START_SCRUB_TRP2,
    START_SCRUB_TRP3,
    RST_TIMER_TRP1,
    RST_TIMER_TRP2,
    RST_TIMER_TRP3,
    //Encoder output to be used in case of external RAM
    CODED,
    RAM_RA_LAT,
    // flags
    INIT_DONE,
    ERROR,
    CORRECTABLE,
    SLOWDOWN,
    TMOUTFLG,
    SCRUB_DONE,
    SCRUB_CORR,
    NOW_SCRUBBING   );
  parameter MODE         = 1;   //Codec only(0), Protect RAM(1)
  parameter TMR          = 0;   // Triple redundancy (1)
  parameter DAT_WIDTH    = 8;
  parameter USER_ENC_PIPE = 0;  //Isn't necessarily the actual number
  parameter USER_DEC_PIPE = 0;  //of pipes. It's user-imposed upper limit
  parameter SCRUB_ON     = 1;
  parameter WRBK_ON      = 1;
  parameter DIV_WDTH     = 10;
  parameter TMOUT_SET    = 1000;
  parameter SCRUB_AMIN   = 0;
  parameter SCRUB_AMAX   = 255;
   // RAM generics
  parameter RAM_DEPTH    = 255;
  parameter RAM_PIPE     = 1;
  parameter URAM        = 0;
  parameter URAM_PORT_B = 0;
  parameter URAM_A_PIPE = 0;
  parameter INIT_RAM    = 0;
   //
  parameter DLY_RD_A_ON  = 0;
  parameter SINGLECLK    = 0;
  parameter TEST         = 0;   // create port to inject errors
  parameter SIM_RAM      = 1  ; // RAM simulation model
  parameter FAMILY       = 15;
//sar68546  parameter DIE_SIZE     = 28;  // Not used  

  localparam RAM_LOGDEPTH = ceil_log2(RAM_DEPTH);
  localparam PAR_WIDTH    = edac_d2p(DAT_WIDTH);
  localparam XORWDTH      = xorWidth(DAT_WIDTH);
  localparam ENC_LAYERS   = ((FAMILY==11) || (FAMILY==12)) ?
                                      ceil_log2(XORWDTH) : ceil_log3(XORWDTH);
  localparam DEC_LAYERS   = ((FAMILY==11) || (FAMILY==12)) ?
                                  ceil_log2(XORWDTH+1) : ceil_log3(XORWDTH+1);
  localparam ENC_PIPE1  = (doPipeEnc(XORWDTH, USER_ENC_PIPE, 1, ENC_LAYERS)-1);
  localparam ENC_PIPE2  = (doPipeEnc(XORWDTH, USER_ENC_PIPE, 2, ENC_LAYERS)-1);

  localparam DEC_PIPE1  = doPipeDec(XORWDTH, USER_DEC_PIPE, 1, DEC_LAYERS)-1;
  localparam DEC_PIPE2  = doPipeDec(XORWDTH, USER_DEC_PIPE, 2, DEC_LAYERS)-1;
  localparam SYND_PIPE  = doPipeDec(XORWDTH, USER_DEC_PIPE, 3, DEC_LAYERS);

  localparam ENC_DLY1 = (ENC_PIPE1>=0 && ENC_PIPE1<5) ? 1 : 0;
  localparam ENC_DLY2 = (ENC_PIPE2>=0 && ENC_PIPE2<5) ? 1 : 0;
  localparam DEC_DLY1 = (DEC_PIPE1>=0 && DEC_PIPE1<6) ? 1 : 0;
  localparam DEC_DLY2 = (DEC_PIPE2>=0 && DEC_PIPE2<6) ? 1 : 0;
  localparam ENC_PIPE = ENC_DLY1 + ENC_DLY2;
  localparam DEC_PIPE = DEC_DLY1 + DEC_DLY2 + SYND_PIPE;

  localparam AX_SILICON_ISSUE = 1;    //sar49523

  input NGRST, RST, RCLK, WCLK;
  input STOP_SCRUB_TRP1;
  input STOP_SCRUB_TRP2;
  input STOP_SCRUB_TRP3;
  // Decoder output
  output[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH] DATA_OUT;
  output[PAR_WIDTH-1:0] PARITY_OUT;
  // user ports
  input USER_WEN_TRP1;
  input USER_WEN_TRP2;
  input USER_WEN_TRP3;
  input [DAT_WIDTH-1:0] MSG_TRP1; // encoder input/output = user_wD
  input [DAT_WIDTH-1:0] MSG_TRP2;
  input [DAT_WIDTH-1:0] MSG_TRP3;
  input [RAM_LOGDEPTH-1:0] USER_WA_TRP1;
  input [RAM_LOGDEPTH-1:0] USER_WA_TRP2;
  input [RAM_LOGDEPTH-1:0] USER_WA_TRP3;
  input USER_REN_TRP1;
  input USER_REN_TRP2;
  input USER_REN_TRP3;
  input [RAM_LOGDEPTH-1:0] USER_RA_TRP1;
  input [RAM_LOGDEPTH-1:0] USER_RA_TRP2;
  input [RAM_LOGDEPTH-1:0] USER_RA_TRP3;

  // RAM direct output used for test purpose = Decoder input
  input  [PAR_WIDTH+DAT_WIDTH-1:0] CODE_FROM_EXT_RAM;
  output [PAR_WIDTH+DAT_WIDTH-1:0] CODE_FROM_RAM;
  // test input to inject errors after encoder
  input [PAR_WIDTH+DAT_WIDTH-1:0] INJ_ERR;
  input START_SCRUB_TRP1; //optional scrub start in addition to timer-driven one
  input START_SCRUB_TRP2;
  input START_SCRUB_TRP3;
  input RST_TIMER_TRP1;   //optional timer reset
  input RST_TIMER_TRP2;
  input RST_TIMER_TRP3;
  output [PAR_WIDTH+DAT_WIDTH-1:0] CODED;
  output [RAM_LOGDEPTH-1:0] RAM_RA_LAT; // optional rA delay to balance decoder
  // Flags
  output CORRECTABLE;
  output ERROR;
  output SLOWDOWN;
  output TMOUTFLG;
  output SCRUB_DONE;
  output NOW_SCRUBBING;
  output INIT_DONE;
  output SCRUB_CORR;

  wire ram_wEn_w;
  wire wClki;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] ram_wD_w;
  wire [RAM_LOGDEPTH-1:0] ram_wA_w;
  wire ram_rEn_w;
  wire [RAM_LOGDEPTH-1:0] ram_ra;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] code_from_ram_int_ext;
  wire [PAR_WIDTH+DAT_WIDTH-1:0] re_code_w;
  wire port_b_en;

  assign DATA_OUT   = re_code_w[PAR_WIDTH+DAT_WIDTH-1:PAR_WIDTH];
  assign PARITY_OUT = re_code_w[PAR_WIDTH-1:0];
  assign CODED      = ram_wD_w;

  // edac decoder input comes either from internal or ext RAM
  assign code_from_ram_int_ext = (MODE==1)? CODE_FROM_RAM : CODE_FROM_EXT_RAM;

  assign wClki = (SINGLECLK) ? RCLK : WCLK;

  test_4463andcpuopration_COREEDAC_0_edac # (.MODE (MODE),
          .TMR  (TMR),
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
          .WRBK_ON (WRBK_ON),
          .DIV_WDTH (DIV_WDTH),
          .TMOUT_SET (TMOUT_SET),
          .SCRUB_AMIN (SCRUB_AMIN),
          .SCRUB_AMAX (SCRUB_AMAX),
          .RAM_LOGDEPTH (RAM_LOGDEPTH),
          .RAM_PIPE (RAM_PIPE),
          .INIT_RAM (INIT_RAM),
          .TEST     (TEST),
          .SINGLECLK (SINGLECLK)   )  test_4463andcpuopration_COREEDAC_0_edac_0 (
      .nGrst(NGRST),
      .rst(RST),
      .rClk (RCLK),
      .wClk (wClki),
      .stop_scrub_trp1 (STOP_SCRUB_TRP1),
      .stop_scrub_trp2 (STOP_SCRUB_TRP2),
      .stop_scrub_trp3 (STOP_SCRUB_TRP3),
      .msg_trp1 (MSG_TRP1), // encoder input
      .msg_trp2 (MSG_TRP2),
      .msg_trp3 (MSG_TRP3),
      .user_wEn_trp1 (USER_WEN_TRP1),
      .user_wEn_trp2 (USER_WEN_TRP2),
      .user_wEn_trp3 (USER_WEN_TRP3),
      .user_wA_trp1 (USER_WA_TRP1),
      .user_wA_trp2 (USER_WA_TRP2),
      .user_wA_trp3 (USER_WA_TRP3),
      .user_rEn_trp1 (USER_REN_TRP1),
      .user_rEn_trp2 (USER_REN_TRP2),
      .user_rEn_trp3 (USER_REN_TRP3),
      .user_rA_trp1 (USER_RA_TRP1),
      .user_rA_trp2 (USER_RA_TRP2),
      .user_rA_trp3 (USER_RA_TRP3),
      .code_from_ram (code_from_ram_int_ext),
      .re_code (re_code_w),
      .ram_wEn (ram_wEn_w),
      .ram_wD (ram_wD_w),
      .ram_wA (ram_wA_w),
      .ram_rEn (ram_rEn_w),
      .ram_rA (ram_ra),
      .ram_rA_lat (RAM_RA_LAT),
      .start_scrub_trp1 (START_SCRUB_TRP1),
      .start_scrub_trp2 (START_SCRUB_TRP2),
      .start_scrub_trp3 (START_SCRUB_TRP3),
      .rst_timer_trp1 (RST_TIMER_TRP1),
      .rst_timer_trp2 (RST_TIMER_TRP2),
      .rst_timer_trp3 (RST_TIMER_TRP3),
      .inj_err(INJ_ERR),
      .init_done(INIT_DONE),
      .error(ERROR),
      .now_scrubbing(NOW_SCRUBBING),
      .correctable (CORRECTABLE),
      .slowdown (SLOWDOWN),
      .tmoutflg (TMOUTFLG),
      .scrub_done (SCRUB_DONE),
      .scrub_corr (SCRUB_CORR)    );


  //----------------------------------------------------------------------------
  // instantiate RAM module if in Protected RAM mode
  generate                                //sar49523
    if( MODE && (SIM_RAM==0) &&
        ( (FAMILY==14) ||     //PA
          (FAMILY==15) ||     //PA3
          (FAMILY==16) ||     //PA3E
          (FAMILY==17) ||     //Fusion
          (FAMILY==18) ||     //SmartFusion
          (FAMILY==20) ||     //IGLOO
          (FAMILY==21) ||     //IGLOOe
          (FAMILY==22) ||     //PA3L
          (FAMILY==23) ))     //IGLOO+
      begin: g3_ram
      test_4463andcpuopration_COREEDAC_0_actram test_4463andcpuopration_COREEDAC_0_actram_0 ( // protected Actel RAM
        .RCLOCK (RCLK),
        .WCLOCK (wClki),
        .WRB    (ram_wEn_w),
        .DI     (ram_wD_w),
        .WADDR  (ram_wA_w),
        .RDB    (ram_rEn_w),
        .RADDR  (ram_ra),
        .DO     (CODE_FROM_RAM)  );
    end
  endgenerate

  generate      // AX, RTAX               //sar49523
    if( MODE && (SIM_RAM==0) && ( (FAMILY==11)||(FAMILY==12) )) begin: ram_ax
      test_4463andcpuopration_COREEDAC_0_ax_ram # (
        .DAT_WIDTH (DAT_WIDTH),
        .RAM_DEPTH (RAM_DEPTH),
        .RAM_PIPE  (RAM_PIPE ),
        .SIM_RAM   (SIM_RAM  ),
        .FAMILY    (FAMILY   ),
        .AX_SILICON_ISSUE(AX_SILICON_ISSUE) ) test_4463andcpuopration_COREEDAC_0_actram_0 (
          .rClock(RCLK),
          .wClock(wClki),
          .wEn   (ram_wEn_w),
          .rEn   (ram_rEn_w),
          .di    (ram_wD_w),
          .wAddr (ram_wA_w),
          .rAddr (ram_ra),
          .do    (CODE_FROM_RAM  )   );
    end
  endgenerate

  generate    //SF2, Igloo2
    if( MODE && (SIM_RAM==0) && (URAM==0) && (            //rtg4
        (FAMILY==19) || (FAMILY==24) || (FAMILY==25) )  )   begin: lsram_g4
      test_4463andcpuopration_COREEDAC_0_actram test_4463andcpuopration_COREEDAC_0_actram_0 ( 
        .RCLOCK (RCLK),
        .WCLOCK (wClki),
        .WRB    (ram_wEn_w),
        .DI     (ram_wD_w),
        .WADDR  (ram_wA_w),
        .RDB    (ram_rEn_w),
        .RADDR  (ram_ra),
        .DO     (CODE_FROM_RAM)  );
    end
  endgenerate

  generate    //SF2, Igloo2
    assign port_b_en = (URAM_PORT_B==1)? 1'b1 : 1'b0;

    if( MODE && (SIM_RAM==0) && (URAM!=0) && (            //rtg4
        (FAMILY==19) || (FAMILY==24) || (FAMILY==25) )  )   begin: uram_g4
      test_4463andcpuopration_COREEDAC_0_uram test_4463andcpuopration_COREEDAC_0_actram_0 ( 
        .DO_A       (CODE_FROM_RAM),
        .DO_B       (),
        .DI     (ram_wD_w),
        .RADDR_A    (ram_ra),
        .RADDR_B    (),
        .WADDR      (ram_wA_w),
        .A_BLK      (1'b1),
        .B_BLK      (port_b_en),
        .C_BLK      (1'b1),
        .A_CLK      (RCLK),
        .B_CLK      (RCLK),
        .WCLOCK     (wClki),
        .WRB        (ram_wEn_w)  );
    end
  endgenerate

  generate    // Simulation model (no hard RAM)
    if (MODE && (SIM_RAM==1) ) begin: simulation_ram
      edac_kitRAM_two_port # (.BITWIDTH (PAR_WIDTH+DAT_WIDTH),
                      .RAM_LOGDEPTH (RAM_LOGDEPTH),
                      .RAM_PIPE (RAM_PIPE ) ) sim_ram_0 (
        .nGrst  (NGRST),
        .RCLOCK (RCLK),
        .WCLOCK (wClki),
        .WRB    (ram_wEn_w),
        .DI     (ram_wD_w),
        .WADDR  (ram_wA_w),
        .RDB    (ram_rEn_w),
        .RADDR  (ram_ra),
        .DO     (CODE_FROM_RAM)  );
    end
  endgenerate

`include "functions.v"

endmodule


module test_4463andcpuopration_COREEDAC_0_ax_ram (                           //sar49523 starts
  rClock,
  wClock,
  wEn,
  rEn,
  di,
  wAddr,
  rAddr,
  do );
  parameter DAT_WIDTH    = 8;
  parameter RAM_DEPTH    = 255;
  parameter RAM_PIPE     = 1;
  parameter SIM_RAM      = 1  ; // RAM simulation model
  parameter FAMILY       = 15;
  parameter AX_SILICON_ISSUE = 1;

  localparam RAM_LOGDEPTH = ceil_log2(RAM_DEPTH);
  localparam PAR_WIDTH    = edac_d2p(DAT_WIDTH);

  input rClock, wClock, wEn, rEn;
  input [PAR_WIDTH+DAT_WIDTH-1:0] di;
  input [RAM_LOGDEPTH-1:0] wAddr, rAddr;
  output [PAR_WIDTH+DAT_WIDTH-1:0] do;

  wire [PAR_WIDTH+DAT_WIDTH-1:0] ram_do;

  generate
    if( (RAM_PIPE==1) && (AX_SILICON_ISSUE==1) ) begin: ax_ram_fabric_pipe
      test_4463andcpuopration_COREEDAC_0_actram test_4463andcpuopration_COREEDAC_0_actram_0 (
        .RCLOCK (rClock),
        .WCLOCK (wClock),
        .WRB    (wEn),
        .DI     (di),
        .WADDR  (wAddr),
        .RDB    (rEn),
        .RADDR  (rAddr),
        .DO     (ram_do)  );

      edac_kitDelay_reg # (.BITWIDTH(PAR_WIDTH+DAT_WIDTH), .DELAY(1) ) pipe_0 (
        .nGrst(1'b1), .rst(1'b0), .clk(rClock), .clkEn(1'b1),
        .inp(ram_do), .outp(do)  );
    end

    else  begin: ax_ram_no_fabric_pipe
      test_4463andcpuopration_COREEDAC_0_actram test_4463andcpuopration_COREEDAC_0_actram_0 (
        .RCLOCK (rClock),
        .WCLOCK (wClock),
        .WRB    (wEn),
        .DI     (di),
        .WADDR  (wAddr),
        .RDB    (rEn),
        .RADDR  (rAddr),
        .DO     (do)  );
    end
  endgenerate

`include "functions.v"

endmodule                             //sar49523 ends




