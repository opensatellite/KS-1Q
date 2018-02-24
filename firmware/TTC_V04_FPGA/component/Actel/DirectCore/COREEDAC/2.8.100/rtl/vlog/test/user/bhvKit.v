// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC User Test Bench
//              Behavioral entities
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

 `timescale 1 ns/100 ps

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
              for (i = 0; i<DELAY; i=i+1)  delayLine[i] <= 'b0;
            else begin
              for(i=DELAY-1; i>=1; i=i-1)  delayLine[i] <= delayLine[i-1];
              delayLine[0] <= inp;
            end  // clkEn
    end
  endgenerate
endmodule
/* Use
  bhvDelay # (  .DELAY(1)
                .WIDTH(1) ) pulse_dly_0 (
    .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(1'b1),
    .inp(inp),
    .outp(outp) );
*/


// generate clock and nGrst.  nGrst is longer than a clock period
module bhvClockGen(clk, nGrst);
  parameter
    CLKPERIOD  = 20,
    NGRSTLASTS = 24;
  output reg clk;
  output reg nGrst;

  initial begin
    clk = 0;
    nGrst = 0;
    #(NGRSTLASTS);  nGrst = 1;
  end

  always begin
    #(CLKPERIOD/2) clk = ~clk;
    #(CLKPERIOD - CLKPERIOD/2) clk = ~ clk;
  end
endmodule


module bhvEdge (nGrst, rst, clk, inp, outp);
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
bhvEdge # (.REDGE(1)) edge_detect_0 (
     .nGrst(nGrst), .rst(rst), .clk(clk),
     .inp(in),
     .outp(out) );
*/



// Mark time intervals when user access interrupts scrubbing
// Use_pilot is a timer TC, which is a clock cycle before the interrupt starts
module bhv_uAccess_timer(clk, nGrst, active, u_access, u_pilot);
  parameter SCRUB_AMIN  = 0;
  parameter SCRUB_AMAX  = 100;
  parameter PERIOD      = 10;  // User access happens once per PERIOD clks

  localparam LOG_PERIOD = bhv_ceil_log2(PERIOD);
  input clk, nGrst; 
  input active; 
  output u_access, u_pilot;

  reg[LOG_PERIOD-1:0] rand_range;   // Random number in the range 0 to PERIOD-1
  reg[LOG_PERIOD-1:0] timer;

  always @(posedge clk or negedge nGrst)
    if(!nGrst)  begin
      timer = 0;
      rand_range = PERIOD/2;
    end
    else
      if(active==1'b1)
        if(timer==PERIOD-1) begin
          timer = 0;
          rand_range = $unsigned($random) % PERIOD;
        end
        else
          timer = timer + 1;

  assign u_access = (timer < rand_range) & active;
  assign u_pilot  = (timer == PERIOD-1) & active;

// ---------------------------
  function [31:0] bhv_ceil_log2;
      input integer x;
      integer tmp, res;
    begin
      tmp = 1;
      res = 0;
      while (tmp < x) begin
        tmp = tmp * 2;
        res = res + 1;
      end
      bhv_ceil_log2 = res;
    end
  endfunction
endmodule


module bhv_timeout (nGrst, rstn, clk, 
            start,
            timeout );
  parameter TIME_OUT = 100;
  
  input nGrst, rstn, clk, start;
  output timeout;
  
  integer timer;
  
  always @(posedge clk or negedge nGrst) 
    if ( (nGrst==1'b0) || (rstn==1'b0) ) timer = 0;
    else  
      if(start==1'b1) timer = 0;
      else            timer = timer + 1;

  assign timeout = ((timer==TIME_OUT)||(timer==TIME_OUT+1))? 1'b1 : 1'b0;
endmodule     