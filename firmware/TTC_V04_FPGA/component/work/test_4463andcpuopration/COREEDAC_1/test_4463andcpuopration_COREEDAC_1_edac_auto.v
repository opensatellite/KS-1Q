// Copyright 2008 Actel Corporation. All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
// 
// Revision: 0.2 April 30, 2008

// This is an automatically generated file

 `timescale 1 ns/100 ps

//Shortened Hamming code (39, 32) encoder
//Number of pipeline stages: 0 
module test_4463andcpuopration_COREEDAC_1_ham_enc_vlog(nGrst, rst, clk, clkEn, msg, coded); 
  parameter DAT_WIDTH = 16;   
  parameter PAR_WIDTH = 5;    
  parameter PIPE1     = 1;    
  parameter PIPE2     =-1;    

  input nGrst, rst, clk, clkEn;            
  input[DAT_WIDTH-1:0] msg;                
  output[PAR_WIDTH+DAT_WIDTH-1:0] coded;   

  wire [13:0] wideXorInp [0:6]; 
  // LIST MSG INPUTS INVOLVED IN CALCULATING PARITY BITS
  // Inputs involved in calculating parity bit 0
  assign wideXorInp[0][ 0] = msg[ 31];
  assign wideXorInp[0][ 1] = msg[ 27];
  assign wideXorInp[0][ 2] = msg[ 26];
  assign wideXorInp[0][ 3] = msg[ 23];
  assign wideXorInp[0][ 4] = msg[ 21];
  assign wideXorInp[0][ 5] = msg[ 18];
  assign wideXorInp[0][ 6] = msg[ 16];
  assign wideXorInp[0][ 7] = msg[ 13];
  assign wideXorInp[0][ 8] = msg[ 12];
  assign wideXorInp[0][ 9] = msg[  9];
  assign wideXorInp[0][10] = msg[  7];
  assign wideXorInp[0][11] = msg[  4];
  assign wideXorInp[0][12] = msg[  2];
  assign wideXorInp[0][13] = msg[  0];
  // Inputs involved in calculating parity bit 1
  assign wideXorInp[1][ 0] = msg[ 30];
  assign wideXorInp[1][ 1] = msg[ 28];
  assign wideXorInp[1][ 2] = msg[ 25];
  assign wideXorInp[1][ 3] = msg[ 24];
  assign wideXorInp[1][ 4] = msg[ 21];
  assign wideXorInp[1][ 5] = msg[ 19];
  assign wideXorInp[1][ 6] = msg[ 16];
  assign wideXorInp[1][ 7] = msg[ 14];
  assign wideXorInp[1][ 8] = msg[ 11];
  assign wideXorInp[1][ 9] = msg[ 10];
  assign wideXorInp[1][10] = msg[  7];
  assign wideXorInp[1][11] = msg[  5];
  assign wideXorInp[1][12] = msg[  2];
  assign wideXorInp[1][13] = msg[  0];
  // Inputs involved in calculating parity bit 2
  assign wideXorInp[2][ 0] = msg[ 30];
  assign wideXorInp[2][ 1] = msg[ 29];
  assign wideXorInp[2][ 2] = msg[ 26];
  assign wideXorInp[2][ 3] = msg[ 23];
  assign wideXorInp[2][ 4] = msg[ 22];
  assign wideXorInp[2][ 5] = msg[ 19];
  assign wideXorInp[2][ 6] = msg[ 17];
  assign wideXorInp[2][ 7] = msg[ 14];
  assign wideXorInp[2][ 8] = msg[ 12];
  assign wideXorInp[2][ 9] = msg[  9];
  assign wideXorInp[2][10] = msg[  8];
  assign wideXorInp[2][11] = msg[  5];
  assign wideXorInp[2][12] = msg[  3];
  assign wideXorInp[2][13] = msg[  0];
  // Inputs involved in calculating parity bit 3
  assign wideXorInp[3][ 0] = msg[ 31];
  assign wideXorInp[3][ 1] = msg[ 28];
  assign wideXorInp[3][ 2] = msg[ 27];
  assign wideXorInp[3][ 3] = msg[ 24];
  assign wideXorInp[3][ 4] = msg[ 22];
  assign wideXorInp[3][ 5] = msg[ 20];
  assign wideXorInp[3][ 6] = msg[ 17];
  assign wideXorInp[3][ 7] = msg[ 15];
  assign wideXorInp[3][ 8] = msg[ 12];
  assign wideXorInp[3][ 9] = msg[ 10];
  assign wideXorInp[3][10] = msg[  7];
  assign wideXorInp[3][11] = msg[  5];
  assign wideXorInp[3][12] = msg[  3];
  assign wideXorInp[3][13] = msg[  1];
  // Inputs involved in calculating parity bit 4
  assign wideXorInp[4][ 0] = msg[ 31];
  assign wideXorInp[4][ 1] = msg[ 29];
  assign wideXorInp[4][ 2] = msg[ 26];
  assign wideXorInp[4][ 3] = msg[ 25];
  assign wideXorInp[4][ 4] = msg[ 21];
  assign wideXorInp[4][ 5] = msg[ 19];
  assign wideXorInp[4][ 6] = msg[ 18];
  assign wideXorInp[4][ 7] = msg[ 15];
  assign wideXorInp[4][ 8] = msg[ 13];
  assign wideXorInp[4][ 9] = msg[ 10];
  assign wideXorInp[4][10] = msg[  8];
  assign wideXorInp[4][11] = msg[  6];
  assign wideXorInp[4][12] = msg[  3];
  assign wideXorInp[4][13] = msg[  1];
  // Inputs involved in calculating parity bit 5
  assign wideXorInp[5][ 0] = msg[ 30];
  assign wideXorInp[5][ 1] = msg[ 27];
  assign wideXorInp[5][ 2] = msg[ 24];
  assign wideXorInp[5][ 3] = msg[ 23];
  assign wideXorInp[5][ 4] = msg[ 20];
  assign wideXorInp[5][ 5] = msg[ 17];
  assign wideXorInp[5][ 6] = msg[ 16];
  assign wideXorInp[5][ 7] = msg[ 13];
  assign wideXorInp[5][ 8] = msg[ 11];
  assign wideXorInp[5][ 9] = msg[  8];
  assign wideXorInp[5][10] = msg[  6];
  assign wideXorInp[5][11] = msg[  4];
  assign wideXorInp[5][12] = msg[  1];
  assign wideXorInp[5][13] = 1'b0;
  // Inputs involved in calculating parity bit 6
  assign wideXorInp[6][ 0] = msg[ 29];
  assign wideXorInp[6][ 1] = msg[ 28];
  assign wideXorInp[6][ 2] = msg[ 25];
  assign wideXorInp[6][ 3] = msg[ 22];
  assign wideXorInp[6][ 4] = msg[ 20];
  assign wideXorInp[6][ 5] = msg[ 18];
  assign wideXorInp[6][ 6] = msg[ 15];
  assign wideXorInp[6][ 7] = msg[ 14];
  assign wideXorInp[6][ 8] = msg[ 11];
  assign wideXorInp[6][ 9] = msg[  9];
  assign wideXorInp[6][10] = msg[  6];
  assign wideXorInp[6][11] = msg[  4];
  assign wideXorInp[6][12] = msg[  2];
  assign wideXorInp[6][13] = 1'b0;

  // Instantiate wide Xor's
  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_0 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[0]), 
        .outp (coded[0])   ); 

  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_1 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[1]), 
        .outp (coded[1])   ); 

  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_2 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[2]), 
        .outp (coded[2])   ); 

  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_3 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[3]), 
        .outp (coded[3])   ); 

  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_4 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[4]), 
        .outp (coded[4])   ); 

  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_5 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[5]), 
        .outp (coded[5])   ); 

  wide_xor # (.WIDTH(14),     
              .MICRO_W (3),  
              .PIPE1 (PIPE1), 
              .PIPE2 (PIPE2) ) wide_xor_6 (
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst),	
        .inp  (wideXorInp[6]), 
        .outp (coded[6])   ); 

  assign coded[DAT_WIDTH+PAR_WIDTH-1:PAR_WIDTH] = msg; 
endmodule 



// Shortened Hamming code (39, 32) decoder
// Number of pipeline stages: 0 
module test_4463andcpuopration_COREEDAC_1_ham_dec_vlog (nGrst, rst, clk, clkEn, coded, re_code, errFlag, correctable); 
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

  wire [14:0] wideXorInp [0:6]; 
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
  assign wideXorInp[0][ 0] = coded[ 38];
  assign wideXorInp[0][ 1] = coded[ 34];
  assign wideXorInp[0][ 2] = coded[ 33];
  assign wideXorInp[0][ 3] = coded[ 30];
  assign wideXorInp[0][ 4] = coded[ 28];
  assign wideXorInp[0][ 5] = coded[ 25];
  assign wideXorInp[0][ 6] = coded[ 23];
  assign wideXorInp[0][ 7] = coded[ 20];
  assign wideXorInp[0][ 8] = coded[ 19];
  assign wideXorInp[0][ 9] = coded[ 16];
  assign wideXorInp[0][10] = coded[ 14];
  assign wideXorInp[0][11] = coded[ 11];
  assign wideXorInp[0][12] = coded[  9];
  assign wideXorInp[0][13] = coded[  7];
  assign wideXorInp[0][14] = coded[0];
  // Inputs involved in calculating syndrome 1
  assign wideXorInp[1][ 0] = coded[ 37];
  assign wideXorInp[1][ 1] = coded[ 35];
  assign wideXorInp[1][ 2] = coded[ 32];
  assign wideXorInp[1][ 3] = coded[ 31];
  assign wideXorInp[1][ 4] = coded[ 28];
  assign wideXorInp[1][ 5] = coded[ 26];
  assign wideXorInp[1][ 6] = coded[ 23];
  assign wideXorInp[1][ 7] = coded[ 21];
  assign wideXorInp[1][ 8] = coded[ 18];
  assign wideXorInp[1][ 9] = coded[ 17];
  assign wideXorInp[1][10] = coded[ 14];
  assign wideXorInp[1][11] = coded[ 12];
  assign wideXorInp[1][12] = coded[  9];
  assign wideXorInp[1][13] = coded[  7];
  assign wideXorInp[1][14] = coded[1];
  // Inputs involved in calculating syndrome 2
  assign wideXorInp[2][ 0] = coded[ 37];
  assign wideXorInp[2][ 1] = coded[ 36];
  assign wideXorInp[2][ 2] = coded[ 33];
  assign wideXorInp[2][ 3] = coded[ 30];
  assign wideXorInp[2][ 4] = coded[ 29];
  assign wideXorInp[2][ 5] = coded[ 26];
  assign wideXorInp[2][ 6] = coded[ 24];
  assign wideXorInp[2][ 7] = coded[ 21];
  assign wideXorInp[2][ 8] = coded[ 19];
  assign wideXorInp[2][ 9] = coded[ 16];
  assign wideXorInp[2][10] = coded[ 15];
  assign wideXorInp[2][11] = coded[ 12];
  assign wideXorInp[2][12] = coded[ 10];
  assign wideXorInp[2][13] = coded[  7];
  assign wideXorInp[2][14] = coded[2];
  // Inputs involved in calculating syndrome 3
  assign wideXorInp[3][ 0] = coded[ 38];
  assign wideXorInp[3][ 1] = coded[ 35];
  assign wideXorInp[3][ 2] = coded[ 34];
  assign wideXorInp[3][ 3] = coded[ 31];
  assign wideXorInp[3][ 4] = coded[ 29];
  assign wideXorInp[3][ 5] = coded[ 27];
  assign wideXorInp[3][ 6] = coded[ 24];
  assign wideXorInp[3][ 7] = coded[ 22];
  assign wideXorInp[3][ 8] = coded[ 19];
  assign wideXorInp[3][ 9] = coded[ 17];
  assign wideXorInp[3][10] = coded[ 14];
  assign wideXorInp[3][11] = coded[ 12];
  assign wideXorInp[3][12] = coded[ 10];
  assign wideXorInp[3][13] = coded[  8];
  assign wideXorInp[3][14] = coded[3];
  // Inputs involved in calculating syndrome 4
  assign wideXorInp[4][ 0] = coded[ 38];
  assign wideXorInp[4][ 1] = coded[ 36];
  assign wideXorInp[4][ 2] = coded[ 33];
  assign wideXorInp[4][ 3] = coded[ 32];
  assign wideXorInp[4][ 4] = coded[ 28];
  assign wideXorInp[4][ 5] = coded[ 26];
  assign wideXorInp[4][ 6] = coded[ 25];
  assign wideXorInp[4][ 7] = coded[ 22];
  assign wideXorInp[4][ 8] = coded[ 20];
  assign wideXorInp[4][ 9] = coded[ 17];
  assign wideXorInp[4][10] = coded[ 15];
  assign wideXorInp[4][11] = coded[ 13];
  assign wideXorInp[4][12] = coded[ 10];
  assign wideXorInp[4][13] = coded[  8];
  assign wideXorInp[4][14] = coded[4];
  // Inputs involved in calculating syndrome 5
  assign wideXorInp[5][ 0] = coded[ 37];
  assign wideXorInp[5][ 1] = coded[ 34];
  assign wideXorInp[5][ 2] = coded[ 31];
  assign wideXorInp[5][ 3] = coded[ 30];
  assign wideXorInp[5][ 4] = coded[ 27];
  assign wideXorInp[5][ 5] = coded[ 24];
  assign wideXorInp[5][ 6] = coded[ 23];
  assign wideXorInp[5][ 7] = coded[ 20];
  assign wideXorInp[5][ 8] = coded[ 18];
  assign wideXorInp[5][ 9] = coded[ 15];
  assign wideXorInp[5][10] = coded[ 13];
  assign wideXorInp[5][11] = coded[ 11];
  assign wideXorInp[5][12] = coded[  8];
  assign wideXorInp[5][13] = 1'b0; 
  assign wideXorInp[5][14] = coded[5];
  // Inputs involved in calculating syndrome 6
  assign wideXorInp[6][ 0] = coded[ 36];
  assign wideXorInp[6][ 1] = coded[ 35];
  assign wideXorInp[6][ 2] = coded[ 32];
  assign wideXorInp[6][ 3] = coded[ 29];
  assign wideXorInp[6][ 4] = coded[ 27];
  assign wideXorInp[6][ 5] = coded[ 25];
  assign wideXorInp[6][ 6] = coded[ 22];
  assign wideXorInp[6][ 7] = coded[ 21];
  assign wideXorInp[6][ 8] = coded[ 18];
  assign wideXorInp[6][ 9] = coded[ 16];
  assign wideXorInp[6][10] = coded[ 13];
  assign wideXorInp[6][11] = coded[ 11];
  assign wideXorInp[6][12] = coded[  9];
  assign wideXorInp[6][13] = 1'b0; 
  assign wideXorInp[6][14] = coded[6];

  // Instantiate wide Xor's
  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_0 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[0]), 
        .outp(synd_w[0])   ); 

  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_1 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[1]), 
        .outp(synd_w[1])   ); 

  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_2 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[2]), 
        .outp(synd_w[2])   ); 

  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_3 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[3]), 
        .outp(synd_w[3])   ); 

  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_4 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[4]), 
        .outp(synd_w[4])   ); 

  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_5 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[5]), 
        .outp(synd_w[5])   ); 

  wide_xor # (.WIDTH(15),    
              .MICRO_W(3),  
              .PIPE1(PIPE1), 
              .PIPE2(PIPE2) )  wide_xor_6 ( 
        .nGrst(nGrst), .clk(clk), .clkEn(clkEn), 
        .rst(rst), 
        .inp (wideXorInp[6]), 
        .outp(synd_w[6])   ); 


  // Calculate 'error' and 'correctable' flags 
  assign errFlag = synd[0] | synd[1] | synd[2] | synd[3] | synd[4] | synd[5] | synd[6]; 
  assign correctable = synd[0] ^ synd[1] ^ synd[2] ^ synd[3] ^ synd[4] ^ synd[5] ^ synd[6]; 

  // Convert syndrome to error position 
  always @ (synd) begin 
    case (synd) 
      7'd 1 : err_w = 39'b000000000000000000000000000000000000001;
      7'd 2 : err_w = 39'b000000000000000000000000000000000000010;
      7'd 4 : err_w = 39'b000000000000000000000000000000000000100;
      7'd 8 : err_w = 39'b000000000000000000000000000000000001000;
      7'd16 : err_w = 39'b000000000000000000000000000000000010000;
      7'd32 : err_w = 39'b000000000000000000000000000000000100000;
      7'd64 : err_w = 39'b000000000000000000000000000000001000000;
      7'd 7 : err_w = 39'b000000000000000000000000000000010000000;
      7'd56 : err_w = 39'b000000000000000000000000000000100000000;
      7'd67 : err_w = 39'b000000000000000000000000000001000000000;
      7'd28 : err_w = 39'b000000000000000000000000000010000000000;
      7'd97 : err_w = 39'b000000000000000000000000000100000000000;
      7'd14 : err_w = 39'b000000000000000000000000001000000000000;
      7'd112 : err_w = 39'b000000000000000000000000010000000000000;
      7'd11 : err_w = 39'b000000000000000000000000100000000000000;
      7'd52 : err_w = 39'b000000000000000000000001000000000000000;
      7'd69 : err_w = 39'b000000000000000000000010000000000000000;
      7'd26 : err_w = 39'b000000000000000000000100000000000000000;
      7'd98 : err_w = 39'b000000000000000000001000000000000000000;
      7'd13 : err_w = 39'b000000000000000000010000000000000000000;
      7'd49 : err_w = 39'b000000000000000000100000000000000000000;
      7'd70 : err_w = 39'b000000000000000001000000000000000000000;
      7'd88 : err_w = 39'b000000000000000010000000000000000000000;
      7'd35 : err_w = 39'b000000000000000100000000000000000000000;
      7'd44 : err_w = 39'b000000000000001000000000000000000000000;
      7'd81 : err_w = 39'b000000000000010000000000000000000000000;
      7'd22 : err_w = 39'b000000000000100000000000000000000000000;
      7'd104 : err_w = 39'b000000000001000000000000000000000000000;
      7'd19 : err_w = 39'b000000000010000000000000000000000000000;
      7'd76 : err_w = 39'b000000000100000000000000000000000000000;
      7'd37 : err_w = 39'b000000001000000000000000000000000000000;
      7'd42 : err_w = 39'b000000010000000000000000000000000000000;
      7'd82 : err_w = 39'b000000100000000000000000000000000000000;
      7'd21 : err_w = 39'b000001000000000000000000000000000000000;
      7'd41 : err_w = 39'b000010000000000000000000000000000000000;
      7'd74 : err_w = 39'b000100000000000000000000000000000000000;
      7'd84 : err_w = 39'b001000000000000000000000000000000000000;
      7'd38 : err_w = 39'b010000000000000000000000000000000000000;
      7'd25 : err_w = 39'b100000000000000000000000000000000000000;
      default : err_w = 39'b0; 
    endcase
  end 

endmodule 
