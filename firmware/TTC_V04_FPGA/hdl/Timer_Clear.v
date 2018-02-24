///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: Timer_Clear.v
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

module Timer_Clear( clk, rst, flag_a_rx,flag_b_rx, rst_out );
input clk, rst, flag_a_rx,flag_b_rx;
output  reg rst_out;

reg [23:0]count_a;
reg [23:0]count_b;
always@(posedge clk or negedge rst /*or posedge flag_a_rx or posedge flag_b_rx */ )
begin 
if(!rst)begin 
    count_a<=24'h000000;
    count_b<=24'h000000;
    rst_out<=1;
end 
else begin
    if(flag_a_rx)begin 
        count_a<=count_a+1;
        end
    else begin 
        count_a<=24'h000000;
        end
    if(flag_b_rx)begin
        count_b<=count_b+1;
        end
    else begin 
        count_b<=24'h000000;
        end
    if(count_a== 24'h186a00 || count_b==24'h186a00)begin
        rst_out<=0;
        end
end 

end 

//<statements>

endmodule

