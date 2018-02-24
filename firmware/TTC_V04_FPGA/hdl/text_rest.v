///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: text_rest.v
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

module text_rest( clk, rst, out );
input clk, rst;
output reg out;

reg [31:0]data;
reg [4:0]bit;
always@(posedge clk or negedge rst)
begin 
    if(!rst)begin 
        data<=32'ha18d9534;
        bit<=0;
        end
    else begin 
        out<=data[bit];
        bit<=bit+1;



        end

end

//<statements>

endmodule

