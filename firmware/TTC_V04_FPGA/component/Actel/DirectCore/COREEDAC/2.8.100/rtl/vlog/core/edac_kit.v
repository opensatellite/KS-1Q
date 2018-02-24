// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC RTL Component Library
//              Various Standard RTL Modules
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

`timescale 1 ns/100 ps
`define STANDALONE

// ----------- Simple one-DFF Edge Detector ------------
module edac_kitEdge (nGrst, rst, clk, inp, outp);
  parameter REDGE = 1;   // 1 - rising edge, or 0 - falling edge

  input  nGrst, clk, rst;
  input inp;
  output outp;

  reg inp_tick;
  wire rise_edge, fall_edge;

  assign rise_edge = inp & ~inp_tick;
  assign fall_edge = ~inp & inp_tick;
  assign    outp      = (REDGE) ? rise_edge : fall_edge;

  always @(posedge clk or negedge nGrst) begin
    if(!nGrst)    inp_tick <= 1'b0;
    else
      if (rst)    inp_tick <= 1'b0;
      else        inp_tick <= inp;

  end
endmodule
/*  -- usage
 edac_kitEdge # (.REDGE(1)) edge_detect_0 (
     .nGrst(nGrst), .rst(rst), .clk(clk),
     .inp(in),
     .outp(out) );
*/
// -----------------------------------------------------------------------------


//              +-+-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
//              |C|r|o|s|s|i|n|g| |C|l|o|c|k| |D|o|m|a|i|n|s|
//              +-+-+-+-+-+-+-+-+ +-+-+-+-+-+ +-+-+-+-+-+-+-+
// ----------- Two DFF synchronizer ------------
module edac_kitCrossClk (nGrst, rst, destin_clk, inp, outp);
  input  nGrst, rst, destin_clk;
  input inp;
  output outp;

  reg outp, inp_tick;

  always @(posedge destin_clk or negedge nGrst) begin
    if(!nGrst) begin
      inp_tick <= 1'b0;
      outp     <= 1'b0;
    end
    else                                //rt 
      if(rst==1'b1)  begin              //rt
        inp_tick <= 1'b0;               //rt
        outp     <= 1'b0;               //rt
      end                               //rt
      else begin
        inp_tick <= inp;
        outp     <= inp_tick;
      end
  end
endmodule
/*  -- usage
 edac_kitCrossClk  crossClk_domain_0 (
     .nGrst(nGrst), .rst(rst), .destin_clk(clk),
     .inp(in),
     .outp(out) );
*/

//  1-clk pulse Synchronizer from rClk to wClk (cross_clk_domain.vsd)
//    - generate slim 1-wClk output
//    - generate slim 1-rClk acknowledgement in rClk
//    - synchronizer does not see next input pulse until the ack gets generated
//    - optionally stretch the 'no see' interval using delayed ack to reset
//      circuit
//    - use separate sync resets r_stop_scrub and w_stop_scrub
module edac_kit_x_domain (nGrst, rClk, wClk, r_inp, w_outp, r_ack,
                                                  r_stop_scrub, w_stop_scrub);
  parameter DELAY = 1;

  input  nGrst, rClk, wClk, r_inp, r_stop_scrub, w_stop_scrub;
  output w_outp, r_ack;

  reg r_inp_capture, r_d1, r_d2, r_d3, w_d1, w_d2, w_d3;
  wire r_acko;

  always @(posedge rClk or negedge nGrst)
    if(!nGrst) begin
      r_inp_capture <= 1'b0;
      r_d1          <= 1'b0;
      r_d2          <= 1'b0;
      r_d3          <= 1'b0;
    end
    else
      if(r_stop_scrub==1'b1) begin
        r_inp_capture <= 1'b0;
        r_d1          <= 1'b0;
        r_d2          <= 1'b0;
        r_d3          <= 1'b0;
      end
      else begin
        r_d1 <= w_d3;
        r_d2 <= r_d1;
        r_d3 <= r_d2;
        if(r_ack==1'b1) r_inp_capture <= 1'b0;
        else
          if(r_inp==1'b1)   r_inp_capture <= 1'b1;
      end

  always @(posedge wClk or negedge nGrst)
    if(!nGrst) begin
      w_d1          <= 1'b0;
      w_d2          <= 1'b0;
      w_d3          <= 1'b0;
    end
    else
      if(w_stop_scrub==1'b1) begin
        w_d1          <= 1'b0;
        w_d2          <= 1'b0;
        w_d3          <= 1'b0;
      end
      else begin
        w_d1  <= r_inp_capture;
        w_d2  <= w_d1;
        w_d3  <= w_d2;
      end

  assign w_outp = w_d2 & ~w_d3 & ~w_stop_scrub;
  assign r_acko  = r_d2 & ~r_d3 & ~r_stop_scrub;

  edac_kitDelay_bit_reg # (.DELAY(DELAY)) dly_ack_0 (
    .nGrst(nGrst), .rst(r_stop_scrub), .clk(rClk), .clkEn(1'b1),
    .inp(r_acko), .outp(r_ack) ) ;

endmodule

// A trap for a narrow high freq signal generates a pulse of slow freq
module edac_kit_pulse_trap (nGrst, rst, fast_clk, slow_clk, inp, outp);    
  input   nGrst, rst, fast_clk, slow_clk, inp;
  output  outp;
  
  reg fast_trap, slow_trap;

  assign outp = slow_trap;
  
  always @(posedge fast_clk or negedge nGrst) begin                                                            
      if (nGrst==1'b0)
        fast_trap <= 1'b0;
      else
        if(rst==1'b1)
          fast_trap <= 1'b0;
        else  begin
          if(slow_trap==1'b1)
            fast_trap <= 1'b0;
          else 
            if(inp==1'b1)
              fast_trap <= 1'b1;
        end
  end
        
  always @(posedge slow_clk or negedge nGrst) begin                                                            
      if (nGrst==1'b0) 
        slow_trap <= 1'b0;
      else
        if(rst==1'b1)
          slow_trap <= 1'b0;
        else
          slow_trap <= fast_trap;
  end      
endmodule

module edac_kit_stop_scrub (nGrst, 
  rst,                                                  //rt 
  rClk, wClk, stop_scrub,
  r_stop_scrub, w_stop_scrub);
  input  nGrst, rst, rClk, wClk, stop_scrub;
  output r_stop_scrub, w_stop_scrub;

  wire rEn_wr, wEn_rd;

  edac_kitCrossClk  r2w_crossClk_0 (
     .nGrst(nGrst), 
     .rst(rst),                                         //rt
     .destin_clk(wClk),
     .inp(stop_scrub),
     .outp(w_stop_scrub) );

  edac_kitCrossClk  w2r_crossClk_1 (
     .nGrst(nGrst), 
     .rst(rst),                                         //rt
     .destin_clk(rClk),
     .inp(stop_scrub),
     .outp(r_stop_scrub) );

endmodule

// -----------------------------------------------------------------------------

//      ______  _____  _       ___ __   __
//      |  _  \|  ___|| |     / _ \\ \ / /
//      | | | || |__  | |    / /_\ \\ V /
//      | | | ||  __| | |    |  _  | \ /
//      | |/ / | |___ | |____| | | | | |
//      |___/  \____/ \_____/\_| |_/ \_/

//---------------------- SHIFT REGISTER OR 1-BIT DELAY ------------------------
//module edac_kitShiftReg (nGrst, rst, clk, clkEn, di, Q, dn);
//  parameter SRLENGTH = 2;
//
//  input nGrst, rst, clk, clkEn;
//  input di;
//  output[SRLENGTH-1:0] Q;
//  output dn;
//
//  reg[SRLENGTH-1:0] sreg;
//  assign dn = sreg[SRLENGTH-1];
//  assign Q  = sreg;
//
//  always @ (posedge clk or negedge nGrst) begin
//    if((!nGrst) || rst) sreg <= 0;
//    else
//      if(clkEn)
//        begin
//          sreg[SRLENGTH-1:1] <= sreg[SRLENGTH-2:0];
//          sreg[0]            <= di;
//        end
//  end
//endmodule



//----------- Register-based 1-bit Delay has only input and output ---------
module edac_kitDelay_bit_reg(nGrst, rst, clk, clkEn, inp, outp);
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
          if (clkEn)
            if (rst)
              for (i = 0; i<DELAY; i=i+1)  delayLine[i] <= 1'b0;
            else begin
              for(i=DELAY-1; i>=1; i=i-1)  delayLine[i] <= delayLine[i-1];
              delayLine[0] <= inp;
            end  // clkEn
    end
  endgenerate
endmodule
/* Use
  edac_kitDelay_bit_reg # (.DELAY(2)) bit_dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(input), .outp(outp) ) ;
*/


// -----------  Register-based 1-bit delay has 1-bit input & output  -----------
// and additional output 'ripple', which is active while '1' propagates from 
// input to output
module edac_kitDelay_bit_ripple(nGrst, rst, clk, clkEn, inp, outp, ripple);
  parameter
    DELAY = 2;

  input  nGrst, rst, clk, clkEn;
  input  inp;
  output outp, ripple;
  
  // extra flop to avoid error if DELAY==0
  reg [DELAY:0] shift_reg;
  integer i;

  generate
    if(DELAY==0)  begin
      assign outp = inp;
      assign ripple = inp;
    end

    else begin
      assign outp = shift_reg[DELAY-1];
      assign ripple = | shift_reg[DELAY-1:0];

      always @(posedge clk or negedge nGrst)
        if(!nGrst)
          shift_reg <= 'b0;
        else
          if (clkEn)
            if (rst)
              shift_reg <= 'b0;
            else begin
              shift_reg <= shift_reg<<1;
              shift_reg[0] <= inp;
            end  // clkEn
    end
  endgenerate
endmodule
/* Use
  edac_kitDelay_bit_ripple # (.DELAY(2)) bit_dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(input), .outp(outp),
    .ripple(ripple_w)   ) ;
*/




//----------- Register-based Multi-bit Delay has only input and output ---------
module edac_kitDelay_reg(nGrst, rst, clk, clkEn, inp, outp);
  parameter
    BITWIDTH = 16,
    DELAY = 2;

  input  nGrst, rst, clk, clkEn;
  input  [BITWIDTH-1:0] inp;
  output   [BITWIDTH-1:0] outp;
  // extra register to avoid error if DELAY==0
  reg [BITWIDTH-1:0] delayLine [0:DELAY];
  integer i;

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
              for (i = 0; i<DELAY; i=i+1)  delayLine[i] <= 'b0;
            else begin
              for(i=DELAY-1; i>=1; i=i-1)  delayLine[i] <= delayLine[i-1];
              delayLine[0] <= inp;
            end  // clkEn
    end
  endgenerate
endmodule
/* Use
  edac_kitDelay_reg # (.BITWIDTH(WIDTH), .DELAY(1) ) dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .inp(datain), .outp(dataout)  );
*/


//     _____  _____  _   _  _   _  _____  _____ ______
//    /  __ \|  _  || | | || \ | ||_   _||  ___|| ___ \
//    | /  \/| | | || | | ||  \| |  | |  | |__  | |_/ /
//    | |    | | | || | | || . ` |  | |  |  __| |    /
//    | \__/\\ \_/ /| |_| || |\  |  | |  | |___ | |\ \
//     \____/ \___/  \___/ \_| \_/  \_/  \____/ \_| \_|

// simple incremental counter
module edac_kitCountS(nGrst, rst, clk, clkEn, cntEn, Q, dc);
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
  edac_kitCountS # ( .WIDTH(WIDTH), .DCVALUE({WIDTH{1'bx}}),
                .BUILD_DC(0) ) counter_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
    .cntEn(cntEn), .Q(outp), .dc() );
*/

// complex counter
module edac_kitCountC(nGrst, rst, clk, clkEn, cntEn, up, ld, din, Q, dc);
  parameter WIDTH = 16;     // in bits
  parameter UPDOWN = 2;  // 0 - up, 1-down, 2 - up/down
  parameter DCVALUE = (1 << WIDTH) - 1; // state to decode
  parameter BUILD_CLKEN = 1;
  parameter BUILD_RST = 1;
  parameter BUILD_LD = 1;
  parameter BUILD_DC = 1;

  input nGrst, rst, clk, clkEn, cntEn, up, ld;
  input [WIDTH-1:0] din;
  output [WIDTH-1:0] Q;
  output dc;  // decoder output
  reg [WIDTH-1:0] Q;

  assign dc = (BUILD_DC==1)? (Q == DCVALUE) : 1'bx;

  always@(negedge nGrst or posedge clk) begin
    if(!nGrst) Q <= {WIDTH{1'b0}};
    else
      if(clkEn || (BUILD_CLKEN==0) ) begin // if clkEn, or no clkEn input at all
        if(rst && BUILD_RST) Q <= {WIDTH{1'b0}};
        else begin
          if(ld && BUILD_LD)  Q <= din; // sync load
          else
            if(cntEn) begin  // regular count
              if(UPDOWN==0)           Q <= Q + 1'b1;
              if(UPDOWN==1)           Q <= Q - 1'b1;
              if((UPDOWN==2) && up)   Q <= Q + 1'b1;
              if((UPDOWN==2) && !up)  Q <= Q - 1'b1;
            end // count
        end
      end
  end // always
endmodule

/* instance
  edac_kitCountC # ( .WIDTH(WIDTH), .UPDOWN(0), .DCVALUE({WIDTH{1'bx}}),
                .BUILD_CLKEN(1), .BUILD_RST(1), .BUILD_LD(0),
                .BUILD_DC(0) ) counter_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn), .cntEn(cntEn),
    .up(1'bx), .ld(1'bx), .din({WIDTH{1'bx}}), .Q(outp), .dc() );
*/



//                     RRRRRRRR      RRR     RR     RR
//                     RR     RR    RR RR    RRR   RRR
//                     RR     RR   RR   RR   RRRR RRRR
//                     RRRRRRRR   RR     RR  RR RRR RR
//                     RR   RR    RRRRRRRRR  RR     RR
//                     RR    RR   RR     RR  RR     RR
//                     RR     RR  RR     RR  RR     RR
//
// --------- Two-port RAM simulation model ----------
module edac_kitRAM_two_port (nGrst, RCLOCK, WCLOCK,
       WRB, RDB,
       DI,
       RADDR, WADDR,
       DO       );
  parameter
    BITWIDTH     = 16,
    RAM_LOGDEPTH = 2,
    RAM_PIPE     = 2;
  localparam RAM_DEPTH = 1<<RAM_LOGDEPTH;

  input  nGrst, WCLOCK, RCLOCK;
  input  WRB, RDB;
  input  [BITWIDTH-1:0] DI;
  input  [RAM_LOGDEPTH-1:0] RADDR, WADDR;
  output [BITWIDTH-1:0] DO;

  reg [BITWIDTH-1:0] ramArray [0:RAM_DEPTH-1];
  reg [BITWIDTH-1:0] pipe1, pipe2;
  reg [BITWIDTH-1:0] arrOut;
  integer i;

  assign DO = (RAM_PIPE==2) ? pipe2 : (RAM_PIPE==1)? pipe1 : arrOut;

  // write
  always @(posedge WCLOCK or negedge nGrst)
    if(!nGrst)
      for(i=0; i<RAM_DEPTH; i=i+1)
        ramArray[i] <= 'bx;
    else
      if (WRB) ramArray[WADDR] <= DI;

  // read
  always @(posedge RCLOCK or negedge nGrst)
    if(!nGrst) begin
      pipe2   <= 'b0;
      pipe1   <= 'b0;
    end
    else begin
      if (RDB) arrOut <= ramArray[RADDR];
      else     arrOut <= 'bx;
      pipe2 <= pipe1;
      pipe1 <= arrOut;
    end
endmodule


//   ----------------  3-INP MAJORITY FUNCTION  ----------------------
module edac_kitMajority (inp1, inp2, inp3, outp); 
  parameter WIDTH = 16 ;

  input[WIDTH-1:0] inp1, inp2, inp3;
  output [WIDTH-1:0] outp;

  generate
    genvar i;
    for(i=0; i<WIDTH; i=i+1)  begin: majOut
      assign outp[i] =
                (inp1[i] & inp2[i]) | (inp1[i] & inp3[i]) | (inp2[i] & inp3[i]);
    end
  endgenerate
endmodule


// -------------------------------- MUX 2 -> 1 ---------------------------------
module muxVect2to1 (inp1, inp2, sel, outp);
  parameter BITWIDTH = 16;

  input [BITWIDTH-1:0] inp1, inp2;
  input sel;
  output[BITWIDTH-1:0] outp;

  reg[BITWIDTH-1:0] outp;

  always @ (inp1 or inp2 or sel)
    case (sel)
      1'b0 : outp = inp1;
      1'b1 : outp = inp2;
    endcase
endmodule

module muxScal2to1 (inp1, inp2, sel, outp);
  input inp1, inp2;
  input sel;
  output outp;
  reg outp;

  always @ (inp1 or inp2 or sel)
    case (sel)
      1'b0 : outp = inp1;
      1'b1 : outp = inp2;
    endcase
endmodule


// -------------------------------- MUX 3 -> 1 ---------------------------------
module muxVect3to1 (inp1, inp2, inp3, sel, outp);
  parameter BITWIDTH = 16;

  input [BITWIDTH-1:0] inp1, inp2, inp3;
  input [1:0] sel;
  output[BITWIDTH-1:0] outp;

  reg[BITWIDTH-1:0] outp;

  always @ (inp1 or inp2 or inp3 or sel)
    case (sel)
      2'b00 : outp = inp1;
      2'b01 : outp = inp2;
      2'b10 : outp = inp3;
      2'b11 : outp = inp3;      
    endcase
endmodule

module muxScal3to1 (inp1, inp2, inp3, sel, outp);
  input inp1, inp2, inp3;
  input [1:0] sel;
  output outp;
  reg outp;

  always @ (inp1 or inp2 or inp3 or sel)
    case (sel)
      2'b00 : outp = inp1;
      2'b01 : outp = inp2;
      2'b10 : outp = inp3;
      2'b11 : outp = inp3;
    endcase
endmodule

//rt
// Asynchronous global reset synchronizer generates a 1 or 2-clk wide sync'ed 
// pulse on rising edge of the async reset
module edac_kitSync_ngrst (nGrst, clk, pulse);
  parameter PULSE_WIDTH = 1;
  
  input nGrst, clk;
  output pulse;
  
  reg pulse; 
  reg tick1, tick2;
  wire synced_ngrst, pulsei;

  // Synchronize nGrst
  edac_kitDelay_bit_reg # (.DELAY(4)) sync_ngrst_0 (
    .nGrst(nGrst), .rst(1'b0), .clk(clk), .clkEn(1'b1),
    .inp(1'b1), .outp(synced_ngrst) ) ;
  
  always @ (posedge clk)
    tick1 <= synced_ngrst;

  generate if (PULSE_WIDTH==2) 
    begin: two_clk
      always @ (posedge clk)
        tick2 <= tick1;
    
      assign pulsei = synced_ngrst & ~tick2;    
    end
  endgenerate
  
  generate if (PULSE_WIDTH!=2) 
    begin: one_clk
      assign pulsei = synced_ngrst & ~tick1;    
    end
  endgenerate
  
  always @ (posedge clk)
    pulse <= pulsei;

endmodule  
//rt ends

//rt
// Fill a scrubbed part of a RAM from SCRUB_AMIN to SCRUB_AMAX with 0's, which
// are valid Hamming codes.  Initialization starts on sync'ed nGrst signal  
module initializer (wClk, nGrst, 
  init, init_wA, init_done);
  parameter SCRUB_AMIN  = 0; 
  parameter SCRUB_AMAX  = 255; 
  parameter ENC_PIPE    = 0;           // Actual enc delay
  parameter RAM_LOGDEPTH= 8;

  input nGrst, wClk;    //rt rst;
  output init, init_done;
  output [RAM_LOGDEPTH-1:0] init_wA;
  
  reg  init;
  wire init_done_i, pulse_rst;
  
  // nGrst synchronizer
  edac_kitSync_ngrst # (.PULSE_WIDTH(1) ) sync_ngrst_0 (
    .nGrst(nGrst), .clk(wClk), .pulse(pulse_rst));

  // init RS flop
  always @ (negedge nGrst or posedge wClk)
    if(nGrst==1'b0)         init <= 1'b0;
    else
      if(pulse_rst==1'b1)   init <= 1'b1;
      else
        if(init_done==1'b1) init <= 1'b0;
      
  // Initialize init counter with SCRUB_AMIN and run it
  edac_kitCountC # (.WIDTH(RAM_LOGDEPTH), .UPDOWN(0), .DCVALUE(SCRUB_AMAX),
                .BUILD_CLKEN(0), .BUILD_RST(0), .BUILD_LD(1),
                .BUILD_DC(1) ) counter_0 (
    .nGrst(nGrst), .rst(1'b0), .clk(wClk), .clkEn(1'b1), .cntEn(init),
    .up(1'b1), .ld(pulse_rst), .din(SCRUB_AMIN[RAM_LOGDEPTH-1:0]), 
    .Q(init_wA), .dc(init_done_i) );

  // Stretch 'init' output signal for 1 + actual USER_ENC_PIPE wClk cycles.  
  // This helps preventing false ERROR/CORRECTABLE flags, as the 'init' blocks
  // reading from RAM, thus there will be no reading until the last initialized 
  // RAM cell settles.
  // Also delay init_done, which is a notification for a user that RAM is 
  // available for the user access 
  edac_kitDelay_bit_reg #(.DELAY(1+ENC_PIPE) ) init_done_dly_0 (
    .nGrst(nGrst), .clk(wClk), .clkEn(1'b1),  
    .rst(pulse_rst), 
    .inp(init_done_i), 
    .outp(init_done)  );

endmodule
//rt ends

