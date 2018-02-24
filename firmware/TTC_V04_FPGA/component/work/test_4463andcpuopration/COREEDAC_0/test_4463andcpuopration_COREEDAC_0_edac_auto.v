// Copyright 2008 Actel Corporation. All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
// 
// Revision: 0.2 April 30, 2008

// This is an automatically generated file

 `timescale 1 ns/100 ps

//Shortened Hamming code (13, 8) encoder
//Number of pipeline stages: 0 
module test_4463andcpuopration_COREEDAC_0_ham_enc_vlog(nGrst, rst, clk, clkEn, msg, coded); 
  parameter DAT_WIDTH = 16;   
  parameter PAR_WIDTH = 5;    
  parameter PIPE1     = 1;    
  parameter PIPE2     =-1;    

  input nGrst, rst, clk, clkEn;            
  input[DAT_WIDTH-1:0] msg;                
  output[PAR_WIDTH+DAT_WIDTH-1:0] coded;   

  wire [4:0] wideXorInp [0:4]; 
  // LIST MSG INPUTS INVOLVED IN CALCULATING PARITY BITS
  // Inputs involved in calculating parity bit 0
  assign wideXorInp[0][ 0] = msg[  6];
  assign wideXorInp[0][ 1] = msg[  5];
  assign wideXorInp[0][ 2] = msg[  3];
  assign wideXorInp[0][ 3] = msg[  1];
  assign wideXorInp[0][ 4] = msg[  0];
  // Inputs involved in calculating parity bit 1
  assign wideXorInp[1][ 0] = msg[  7];
  assign wideXorInp[1][ 1] = msg[  5];
  assign wideXorInp[1][ 2] = msg[  3];
  assign wideXorInp[1][ 3] = msg[  2];
  assign wideXorInp[1][ 4] = msg[  0];
  // Inputs involved in calculating parity bit 2
  assign wideXorInp[2][ 0] = msg[  7];
  assign wideXorInp[2][ 1] = msg[  6];
  assign wideXorInp[2][ 2] = msg[  4];
  assign wideXorInp[2][ 3] = msg[  2];
  assign wideXorInp[2][ 4] = msg[  0];
  // Inputs involved in calculating parity bit 3
  assign wideXorInp[3][ 0] = msg[  5];
  assign wideXorInp[3][ 1] = msg[  4];
  assign wideXorInp[3][ 2] = msg[  2];
  assign wideXorInp[3][ 3] = msg[  1];
  assign wideXorInp[3][ 4] = 1'b0;
  // Inputs involved in calculating parity bit 4
  assign wideXorInp[4][ 0] = msg[  7];
  assign wideXorInp[4][ 1] = msg[  6];
  assign wideXorInp[4][ 2] = msg[  4];
  assign wideXorInp[4][ 3] = msg[  3];
  assign wideXorInp[4][ 4] = msg[  1];

  // Instantiate wide Xor's
  wide_xor # (.WIDTH(5),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_0 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[0]), 
        .outp (coded[0])   ); 

  wide_xor # (.WIDTH(5),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_1 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[1]), 
        .outp (coded[1])   ); 

  wide_xor # (.WIDTH(5),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_2 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[2]), 
        .outp (coded[2])   ); 

  wide_xor # (.WIDTH(5),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_3 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[3]), 
        .outp (coded[3])   ); 

  wide_xor # (.WIDTH(5),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_4 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[4]), 
        .outp (coded[4])   ); 

  assign coded[DAT_WIDTH+PAR_WIDTH-1:PAR_WIDTH] = msg; 
endmodule 



// Shortened Hamming code (13, 8) decoder
// Number of pipeline stages: 0 
module test_4463andcpuopration_COREEDAC_0_ham_dec_vlog (nGrst, rst, clk, clkEn, coded, re_code, errFlag, correctable); 
  parameter DAT_WIDTH = 16; 
  parameter PAR_WIDTH = 5;  
  parameter DEC_DLY   = 0;  
  parameter PIPE1     = 1;  
  parameter PIPE2     =-1;  
  parameter SYND_PIPE = 0;  
  input nGrst, rst, clk, clkEn;             
  input[PAR_WIDTH+DAT_WIDTH-1:0] coded;     
  output[PAR_WIDTH+DAT_WIDTH-1:0] re_code;  
  output errFlag, correctable;              

  wire [5:0] wideXorInp [0:4]; 
  wire [PAR_WIDTH+DAT_WIDTH-1:0] coded_lat; 
  reg  [PAR_WIDTH+DAT_WIDTH-1:0] err_w; 
  wire [PAR_WIDTH-1:0] synd_w, synd; 

  // Correct error
  assign re_code = coded_lat ^ err_w; 
  // Delay decoder input to compensate for decoder pipe delay 
  edac_kitDelay_reg # (.BITWIDTH(DAT_WIDTH+PAR_WIDTH), .DELAY(DEC_DLY) ) 
    dly_coded_0(.nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn), 
                .inp  (coded), 
                .outp (coded_lat)   ); 

  generate  // Inset syndrome pipe if necessary  
    if(SYND_PIPE)
      edac_kitDelay_reg # (.BITWIDTH(PAR_WIDTH), .DELAY(1) ) synd_pipe_1 ( 
        .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn), 
        .inp (synd_w),  
        .outp(synd)  ); 
    else 
      assign synd = synd_w; 
  endgenerate 

  // LIST MSG INPUTS INVOLVED IN CALCULATING PARITY BITS
  // Inputs involved in calculating syndrome 0
  assign wideXorInp[0][ 0] = coded[ 11];
  assign wideXorInp[0][ 1] = coded[ 10];
  assign wideXorInp[0][ 2] = coded[  8];
  assign wideXorInp[0][ 3] = coded[  6];
  assign wideXorInp[0][ 4] = coded[  5];
  assign wideXorInp[0][ 5] = coded[0];
  // Inputs involved in calculating syndrome 1
  assign wideXorInp[1][ 0] = coded[ 12];
  assign wideXorInp[1][ 1] = coded[ 10];
  assign wideXorInp[1][ 2] = coded[  8];
  assign wideXorInp[1][ 3] = coded[  7];
  assign wideXorInp[1][ 4] = coded[  5];
  assign wideXorInp[1][ 5] = coded[1];
  // Inputs involved in calculating syndrome 2
  assign wideXorInp[2][ 0] = coded[ 12];
  assign wideXorInp[2][ 1] = coded[ 11];
  assign wideXorInp[2][ 2] = coded[  9];
  assign wideXorInp[2][ 3] = coded[  7];
  assign wideXorInp[2][ 4] = coded[  5];
  assign wideXorInp[2][ 5] = coded[2];
  // Inputs involved in calculating syndrome 3
  assign wideXorInp[3][ 0] = coded[ 10];
  assign wideXorInp[3][ 1] = coded[  9];
  assign wideXorInp[3][ 2] = coded[  7];
  assign wideXorInp[3][ 3] = coded[  6];
  assign wideXorInp[3][ 4] = 1'b0; 
  assign wideXorInp[3][ 5] = coded[3];
  // Inputs involved in calculating syndrome 4
  assign wideXorInp[4][ 0] = coded[ 12];
  assign wideXorInp[4][ 1] = coded[ 11];
  assign wideXorInp[4][ 2] = coded[  9];
  assign wideXorInp[4][ 3] = coded[  8];
  assign wideXorInp[4][ 4] = coded[  6];
  assign wideXorInp[4][ 5] = coded[4];

  // Instantiate wide Xor's
  wide_xor # (.WIDTH(6),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_0 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[0]), 
        .outp(synd_w[0])   ); 

  wide_xor # (.WIDTH(6),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_1 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[1]), 
        .outp(synd_w[1])   ); 

  wide_xor # (.WIDTH(6),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_2 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[2]), 
        .outp(synd_w[2])   ); 

  wide_xor # (.WIDTH(6),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_3 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[3]), 
        .outp(synd_w[3])   ); 

  wide_xor # (.WIDTH(6),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_4 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[4]), 
        .outp(synd_w[4])   ); 


  // Calculate 'error' and 'correctable' flags 
  assign errFlag = synd[0] | synd[1] | synd[2] | synd[3] | synd[4]; 
  assign correctable = synd[0] ^ synd[1] ^ synd[2] ^ synd[3] ^ synd[4]; 

  // Convert syndrome to error position 
  always @ (synd) begin 
    case (synd) 
      5'd 1 : err_w = 13'b0000000000001;
      5'd 2 : err_w = 13'b0000000000010;
      5'd 4 : err_w = 13'b0000000000100;
      5'd 8 : err_w = 13'b0000000001000;
      5'd16 : err_w = 13'b0000000010000;
      5'd 7 : err_w = 13'b0000000100000;
      5'd25 : err_w = 13'b0000001000000;
      5'd14 : err_w = 13'b0000010000000;
      5'd19 : err_w = 13'b0000100000000;
      5'd28 : err_w = 13'b0001000000000;
      5'd11 : err_w = 13'b0010000000000;
      5'd21 : err_w = 13'b0100000000000;
      5'd22 : err_w = 13'b1000000000000;
      default : err_w = 13'b0; 
    endcase
  end 

endmodule 
