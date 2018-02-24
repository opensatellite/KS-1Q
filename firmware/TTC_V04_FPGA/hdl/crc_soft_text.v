///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: crc_soft_text.v
// File history:
//      <Revision number>: <Date>: <Comments>
//      <Revision number>: <Date>: <Comments>
//      <Revision number>: <Date>: <Comments>
//
// Description: 
//
// <Description here>
//
// Targeted device: <Family::SmartFusion2> <Die::M2S010T> <Package::484 FBGA>
// Author: <Name>
//
/////////////////////////////////////////////////////////////////////////////////////////////////// 

//`timescale <time_units> / <precision>

module crc_soft_text( clk, rst, en, data );
input clk, rst;
output en;
output [7:0]data;
reg [7:0]data;
reg en;

always@(posedge clk or posedge rst)begin 
if(rst)begin 
en<=0;
data<=8'h00;
end
else begin 
en<=1;
data<=8'h00;
end
end


//<statements>

endmodule

