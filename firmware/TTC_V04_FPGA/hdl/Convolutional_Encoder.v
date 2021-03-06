// -------------------------------------------------------------
// 
// File Name: hdl_prj\hdlsrc\ccsds_encoder\Convolutional_Encoder.v
// Created: 2016-05-20 19:09:34
// 
// Generated by MATLAB 8.3 and HDL Coder 3.4
// 
// -------------------------------------------------------------


// -------------------------------------------------------------
// 
// Module: Convolutional_Encoder
// Source Path: ccsds_encoder/Convolutional Encoder
// Hierarchy Level: 1
// 
// Convolutional Encoder Block
// 
// poly2trellis(7, [171 133])
// 
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module Convolutional_Encoder
          (
           clk,
           reset,
           enb,
           Convolutional_Encoder_in,
           Convolutional_Encoder_out_1,
           Convolutional_Encoder_out_0
          );


  input   clk;
  input   reset;
  input   enb;
  input   Convolutional_Encoder_in;
  output  Convolutional_Encoder_out_1,Convolutional_Encoder_out_0;

  reg  [0:5] shift1_reg;  // ufix1 [6]
  wire [0:5] shift1_reg_next;  // ufix1 [6]
  wire [0:6] sreg1out;  // boolean [7]
  wire sreg1out_0;
  wire sreg1out_1;
  wire sreg1out_2;
  wire sreg1out_3;
  wire sreg1out_6;
  wire encoded_entry1;
  wire sreg1out_5;
  wire encoded_entry2;
  wire Convolutional_Encoder_out_1;
  wire Convolutional_Encoder_out_0;

  // Shift Register for Constraint Length 7
  always @(posedge clk or negedge reset)
    begin : shift1_process
      if (reset == 1'b0  ) begin
        shift1_reg[0] <= 1'b0;
        shift1_reg[1] <= 1'b0;
        shift1_reg[2] <= 1'b0;
        shift1_reg[3] <= 1'b0;
        shift1_reg[4] <= 1'b0;
        shift1_reg[5] <= 1'b0;
      end
      else begin
       // if (enb) begin
          shift1_reg[0] <= shift1_reg_next[0];
          shift1_reg[1] <= shift1_reg_next[1];
          shift1_reg[2] <= shift1_reg_next[2];
          shift1_reg[3] <= shift1_reg_next[3];
          shift1_reg[4] <= shift1_reg_next[4];
          shift1_reg[5] <= shift1_reg_next[5];
      //  end
      end
    end
	
	
	
//always @(posedge clk)
 	//out <= clk ? ~Convolutional_Encoder_out_1 : Convolutional_Encoder_out_0;
	//
//	

  assign sreg1out[0] = Convolutional_Encoder_in;
  assign sreg1out[1] = shift1_reg[0];
  assign sreg1out[2] = shift1_reg[1];
  assign sreg1out[3] = shift1_reg[2];
  assign sreg1out[4] = shift1_reg[3];
  assign sreg1out[5] = shift1_reg[4];
  assign sreg1out[6] = shift1_reg[5];
  assign shift1_reg_next[0] = Convolutional_Encoder_in;
  assign shift1_reg_next[1] = shift1_reg[0];
  assign shift1_reg_next[2] = shift1_reg[1];
  assign shift1_reg_next[3] = shift1_reg[2];
  assign shift1_reg_next[4] = shift1_reg[3];
  assign shift1_reg_next[5] = shift1_reg[4];



  assign sreg1out_0 = sreg1out[0];

  assign sreg1out_1 = sreg1out[1];

  assign sreg1out_2 = sreg1out[2];

  assign sreg1out_3 = sreg1out[3];

  assign sreg1out_6 = sreg1out[6];

  // Output Polynomial: [171]
  assign encoded_entry1 = sreg1out_6 ^ (sreg1out_3 ^ (sreg1out_2 ^ (sreg1out_0 ^ sreg1out_1)));



  assign Convolutional_Encoder_out_0 = encoded_entry1;

  assign sreg1out_5 = sreg1out[5];

  // Output Polynomial: [133]
  assign encoded_entry2 = sreg1out_6 ^ (sreg1out_5 ^ (sreg1out_3 ^ (sreg1out_0 ^ sreg1out_2)));



  assign Convolutional_Encoder_out_1 = encoded_entry2;

endmodule  // Convolutional_Encoder

