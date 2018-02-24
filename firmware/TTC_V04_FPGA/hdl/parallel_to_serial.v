///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: parallel_to_serial.v
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

module parallel_to_serial( ClkI, Rst, EnI, DataI,DataO,EnO ,En_Read_Buff);
input ClkI, Rst, EnI;
input [7:0]DataI;
output DataO,EnO,En_Read_Buff;
reg DataO,EnO,En_Read_Buff;
reg [2:0]Bit_Count;
reg [15:0]bit;
//reg null;
wire r;
always@(posedge ClkI or negedge Rst /*or posedge EnI*/)begin 
if(!Rst)begin 
bit<=0;
Bit_Count<=3'h7;
DataO<=0;
EnO<=0;
//null<=0;
En_Read_Buff<=1;
end 
else begin 
  if(EnI)begin
    DataO<=DataI[Bit_Count];
    bit<=bit+1;
    Bit_Count<=Bit_Count-1; 
    EnO<=1;
    if(bit>2022)begin 
      En_Read_Buff<=1;
      end 
    else En_Read_Buff<=0;
    end
  else begin 
    DataO<=DataI[0];
    En_Read_Buff<=1;
    Bit_Count<=7;
    EnO<=0;
    bit<=0;
//    null<=0;
    end
  end
end

assign r=DataI[Bit_Count];
//<statements>

endmodule

