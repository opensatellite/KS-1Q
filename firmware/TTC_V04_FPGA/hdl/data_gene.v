///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: data_gene.v
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

module data_gene( clk, rst, data, en );
input clk, rst;
output en;
output [7:0]data;
reg [7:0]byte,data;
reg en;

always@(posedge clk or posedge rst)begin 
if(rst)begin 
en<=0;
data<=0;
byte<=0;
end
else begin 
if(byte<8'hdc)begin
en<=1;
data<=byte;
byte<=byte+1;
end
else en<=0;

end




end
//<statements>

endmodule

