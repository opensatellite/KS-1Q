///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: Switch.v
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

module Switch( clk ,Rst, ASMIn,En_ASM, EnRIn,RIn, Out,EnOut );
input ASMIn,En_ASM,EnRIn, RIn,clk,Rst;
output Out,EnOut;
reg  Out,EnOut;
//reg [15:0]bit;
always@(posedge clk or negedge Rst)begin 

if(!Rst)begin 
Out<=0;
EnOut<=0;
//bit<=0;
end 
else begin 
  if(!EnRIn&&En_ASM)begin
    Out<=ASMIn;
    EnOut<=1;
//bit<=bit+1;
    end
  else   if(EnRIn)begin
    Out<=RIn;
    EnOut<=1;
//bit<=bit+1;
    end
  else begin 
    Out<=0;
    EnOut<=0;
    end
end

end
endmodule

