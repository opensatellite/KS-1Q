///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: DATA_OUT.v
// File history:
//      <Revision number>: <Date>: <Comments>
//      <Revision number>: <Date>: <Comments>
//      <Revision number>: <Date>: <Comments>
//
// Description: 
//
// <Description here>
//
// Targeted device: <Family::SmartFusion2> <Die::M2S010> <Package::144 TQ>
// Author: <Name>
//
/////////////////////////////////////////////////////////////////////////////////////////////////// 

//`timescale <time_units> / <precision>

module DATA_OUT(Clk, Clk_2dec,Rst,Convolutional_Encoder_out_1,Convolutional_Encoder_out_0,  DataO ,c0,c1);
input Clk,Clk_2dec,Rst,Convolutional_Encoder_out_1,Convolutional_Encoder_out_0;
output reg DataO;
output reg c1,c0;


always@(posedge Clk or negedge Rst)
begin 
  if(!Rst)begin
    DataO<=0;
    end
  else begin 

    DataO <= Clk_2dec ? Convolutional_Encoder_out_0 : ~Convolutional_Encoder_out_1;
    c1<=~Convolutional_Encoder_out_1;
    c0<=Convolutional_Encoder_out_0;
    end






end 
//<statements>

endmodule

