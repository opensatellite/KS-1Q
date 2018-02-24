///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: TM_CRC32.v
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

module TM_CRC32( ClkI_Dec8,Rst,En_DataI, DataI , En_DataO, DataO);
input ClkI_Dec8,Rst,En_DataI;
input [7:0]DataI ;
output  En_DataO;
output [7:0]DataO;


reg [7:0]Byte_Count;
reg [31:0]CRC32;
reg  En_DataO;
reg [7:0]DataO;

parameter [7:0]temp=0;

always@(posedge ClkI_Dec8 or posedge Rst)
begin 
  if(Rst)begin 
    En_DataO<=0;
    DataO<=0;
    Byte_Count<=8'h00;
    CRC32<=32'hffff_ffff;

    end //end if(!Rst)
  else begin 
    if(En_DataI&&Byte_Count<220)begin
      En_DataO<=1;
      Byte_Count<=Byte_Count+1;
      DataO<=DataI;
    CRC32[0] <= CRC32[24] ^ CRC32[30] ^ temp[0] ^ temp[6];
    CRC32[1] <= CRC32[24] ^ CRC32[25] ^ CRC32[30] ^ CRC32[31] ^ temp[0] ^ temp[1] ^ temp[6] ^ temp[7];
    CRC32[2] <= CRC32[24] ^ CRC32[25] ^ CRC32[26] ^ CRC32[30] ^ CRC32[31] ^ temp[0] ^ temp[1] ^ temp[2] ^ temp[6] ^ temp[7];
    CRC32[3] <= CRC32[25] ^ CRC32[26] ^ CRC32[27] ^ CRC32[31] ^ temp[1] ^ temp[2] ^ temp[3] ^ temp[7];
    CRC32[4] <= CRC32[24] ^ CRC32[26] ^ CRC32[27] ^ CRC32[28] ^ CRC32[30] ^ temp[0] ^ temp[2] ^ temp[3] ^ temp[4] ^ temp[6];
    CRC32[5] <= CRC32[24] ^ CRC32[25] ^ CRC32[27] ^ CRC32[28] ^ CRC32[29] ^ CRC32[30] ^ CRC32[31] ^ temp[0] ^ temp[1] ^ temp[3] ^ temp[4] ^ temp[5] ^ temp[6] ^ temp[7];
    CRC32[6] <= CRC32[25] ^ CRC32[26] ^ CRC32[28] ^ CRC32[29] ^ CRC32[30] ^ CRC32[31] ^ temp[1] ^ temp[2] ^ temp[4] ^ temp[5] ^ temp[6] ^ temp[7];
    CRC32[7] <= CRC32[24] ^ CRC32[26] ^ CRC32[27] ^ CRC32[29] ^ CRC32[31] ^ temp[0] ^ temp[2] ^ temp[3] ^ temp[5] ^ temp[7];
    CRC32[8] <= CRC32[0] ^ CRC32[24] ^ CRC32[25] ^ CRC32[27] ^ CRC32[28] ^ temp[0] ^ temp[1] ^ temp[3] ^ temp[4];
    CRC32[9] <= CRC32[1] ^ CRC32[25] ^ CRC32[26] ^ CRC32[28] ^ CRC32[29] ^ temp[1] ^ temp[2] ^ temp[4] ^ temp[5];
    CRC32[10] <= CRC32[2] ^ CRC32[24] ^ CRC32[26] ^ CRC32[27] ^ CRC32[29] ^ temp[0] ^ temp[2] ^ temp[3] ^ temp[5];
    CRC32[11] <= CRC32[3] ^ CRC32[24] ^ CRC32[25] ^ CRC32[27] ^ CRC32[28] ^ temp[0] ^ temp[1] ^ temp[3] ^ temp[4];
    CRC32[12] <= CRC32[4] ^ CRC32[24] ^ CRC32[25] ^ CRC32[26] ^ CRC32[28] ^ CRC32[29] ^ CRC32[30] ^ temp[0] ^ temp[1] ^ temp[2] ^ temp[4] ^ temp[5] ^ temp[6];
    CRC32[13] <= CRC32[5] ^ CRC32[25] ^ CRC32[26] ^ CRC32[27] ^ CRC32[29] ^ CRC32[30] ^ CRC32[31] ^ temp[1] ^ temp[2] ^ temp[3] ^ temp[5] ^ temp[6] ^ temp[7];
    CRC32[14] <= CRC32[6] ^ CRC32[26] ^ CRC32[27] ^ CRC32[28] ^ CRC32[30] ^ CRC32[31] ^ temp[2] ^ temp[3] ^ temp[4] ^ temp[6] ^ temp[7];
    CRC32[15] <= CRC32[7] ^ CRC32[27] ^ CRC32[28] ^ CRC32[29] ^ CRC32[31] ^ temp[3] ^ temp[4] ^ temp[5] ^ temp[7];
    CRC32[16] <= CRC32[8] ^ CRC32[24] ^ CRC32[28] ^ CRC32[29] ^ temp[0] ^ temp[4] ^ temp[5];
    CRC32[17] <= CRC32[9] ^ CRC32[25] ^ CRC32[29] ^ CRC32[30] ^ temp[1] ^ temp[5] ^ temp[6];
    CRC32[18] <= CRC32[10] ^ CRC32[26] ^ CRC32[30] ^ CRC32[31] ^ temp[2] ^ temp[6] ^ temp[7];
    CRC32[19] <= CRC32[11] ^ CRC32[27] ^ CRC32[31] ^ temp[3] ^ temp[7];
    CRC32[20] <= CRC32[12] ^ CRC32[28] ^ temp[4];
    CRC32[21] <= CRC32[13] ^ CRC32[29] ^ temp[5];
    CRC32[22] <= CRC32[14] ^ CRC32[24] ^ temp[0];
    CRC32[23] <= CRC32[15] ^ CRC32[24] ^ CRC32[25] ^ CRC32[30] ^ temp[0] ^ temp[1] ^ temp[6];
    CRC32[24] <= CRC32[16] ^ CRC32[25] ^ CRC32[26] ^ CRC32[31] ^ temp[1] ^ temp[2] ^ temp[7];
    CRC32[25] <= CRC32[17] ^ CRC32[26] ^ CRC32[27] ^ temp[2] ^ temp[3];
    CRC32[26] <= CRC32[18] ^ CRC32[24] ^ CRC32[27] ^ CRC32[28] ^ CRC32[30] ^ temp[0] ^ temp[3] ^ temp[4] ^ temp[6];
    CRC32[27] <= CRC32[19] ^ CRC32[25] ^ CRC32[28] ^ CRC32[29] ^ CRC32[31] ^ temp[1] ^ temp[4] ^ temp[5] ^ temp[7];
    CRC32[28] <= CRC32[20] ^ CRC32[26] ^ CRC32[29] ^ CRC32[30] ^ temp[2] ^ temp[5] ^ temp[6];
    CRC32[29] <= CRC32[21] ^ CRC32[27] ^ CRC32[30] ^ CRC32[31] ^ temp[3] ^ temp[6] ^ temp[7];
    CRC32[30] <= CRC32[22] ^ CRC32[28] ^ CRC32[31] ^ temp[4] ^ temp[7];
    CRC32[31] <= CRC32[23] ^ CRC32[29] ^ temp[5];
      end //end if(En_DataI)
    else if((219<Byte_Count)&&(Byte_Count<223))begin
      Byte_Count<=Byte_Count+1;
      DataO<=CRC32[31:24];
      CRC32<=CRC32<<8;
      end
    else begin 
      En_DataO<=0;
      DataO<=0;
      Byte_Count<=32'h0000_0000;
     // CRC32<=32'h0000_0000;
      end
    end

end 
//<statements>

endmodule

