// Copyright 2008 Actel Corporation. All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
// 
// Revision: 0.2 April 30, 2008

// This is an automatically generated file

 `timescale 1 ns/100 ps

// Shortened Hamming code (39, 32) table-based encoder
// Use to generate non-synthesizable golden code 

  module bhv_enc_vlog (msg, coded); 
      input [31:0] msg; 
      output reg[38:0] coded; 
    always @ (msg) 
      case (msg) 
             0 : coded = 39'b000000000000000000000000000000000000000;   // 0 
             1 : coded = 39'b000000000000000000000000000000010000111;   // 135 
             2 : coded = 39'b000000000000000000000000000000100111000;   // 312 
             3 : coded = 39'b000000000000000000000000000000110111111;   // 447 
             4 : coded = 39'b000000000000000000000000000001001000011;   // 579 
             5 : coded = 39'b000000000000000000000000000001011000100;   // 708 
             6 : coded = 39'b000000000000000000000000000001101111011;   // 891 
             7 : coded = 39'b000000000000000000000000000001111111100;   // 1020 
             8 : coded = 39'b000000000000000000000000000010000011100;   // 1052 
             9 : coded = 39'b000000000000000000000000000010010011011;   // 1179 
            10 : coded = 39'b000000000000000000000000000010100100100;   // 1316 
            11 : coded = 39'b000000000000000000000000000010110100011;   // 1443 
            12 : coded = 39'b000000000000000000000000000011001011111;   // 1631 
            13 : coded = 39'b000000000000000000000000000011011011000;   // 1752 
            14 : coded = 39'b000000000000000000000000000011101100111;   // 1895 
            15 : coded = 39'b000000000000000000000000000011111100000;   // 2016 
       default : coded = 39'bx; 
    endcase
endmodule
