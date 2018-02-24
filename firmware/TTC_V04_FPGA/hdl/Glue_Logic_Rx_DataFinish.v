///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: Glue_Logic.v
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

module Glue_Logic( Clk,Rst,
Flag_A_Tx_Finish,Flag_B_Tx_Finish,Flag_Int,
Flag_A_Tx,Flag_B_Tx,
Flag_A_HDL_Rec_Finish,Flag_B_HDL_Rec_Finish,
Flag_A_Rx,Flag_B_Rx,
SpacecraftID,
Flag_OF,
//
MASK,ADDR,
//
//IO_Status_15,IO_Status_14,IO_Status_13,IO_Status_12,IO_Status_11,IO_Status_10,IO_Status_9,IO_Status_8,
//IO_Status_7,IO_Status_6,IO_Status_5,IO_Status_4,IO_Status_3,IO_Status_2,IO_Status_1,IO_Status_0,
//TM_Length,
//
//TC_Length,Timer,
//
//Timer_Counter,SessionID,
//
//Pack_Send_Count,
//
//Pack_Rec_Count,
//
//Pack_Rec_Err_Count,
RXA, REG0,

PADDR,PSEL,PENABLE,PWRITE,PRDATA,PWDATA,PREADY,

Data_In,

USER_RA_TRP1,
USER_REN_TRP1,
MSG_TRP1,
MSG_WA_TRP1,
USER_WEN_TRP1
 );
input Clk,Rst,Flag_A_Tx_Finish,Flag_B_Tx_Finish,Flag_A_HDL_Rec_Finish,Flag_B_HDL_Rec_Finish,Flag_OF;
input PSEL,PENABLE,PWRITE;
input [31:0]PADDR;
input [31:0]PWDATA;
input [15:0]SpacecraftID;
input [31:0]Data_In;

output reg [31:0]PRDATA;
output reg PREADY;

//output [9:0]USER_RA_TRP1;

output reg Flag_A_Tx,Flag_B_Tx;
output reg Flag_A_Rx,Flag_B_Rx;
output reg [15:0]Flag_Int;
output RXA, REG0;


output reg [9:0]USER_RA_TRP1;
output reg USER_REN_TRP1;
output reg[15:0]MASK,ADDR;
output reg [7:0]MSG_TRP1;
output reg [9:0]MSG_WA_TRP1;
output reg USER_WEN_TRP1;

reg [31:0] APB_ADDRESS;
reg [31:0]ARS;
reg [1:0]Byte_Count;

reg Flag_Enable;


//register address 
parameter [31:0]Flag_Register_Addr_0=32'h30000000,
                Flag_Register_Addr_1=32'h30000001,
                Flag_Register_Addr_2=32'h30000002,
                Flag_Register_Addr_3=32'h30000003,
                Flag_Register_Addr_4=32'h30000004,
                Flag_Register_Addr_5=32'h30000005,
                Flag_Register_Addr_6=32'h30000006,
                Flag_Register_Addr_7=32'h30000007,
                Flag_Register_Addr_8=32'h30000008,
                Flag_Register_Addr_9=32'h30000009,
                Flag_Register_Addr_10=32'h3000000A,
                Flag_Register_Addr_11=32'h3000000B,
                Flag_Register_Addr_12=32'h3000000C,
                Flag_Register_Addr_13=32'h3000000D,
                Flag_Register_Addr_14=32'h3000000E,
                Flag_Register_Addr_15=32'h3000000F;


parameter  [21:0]Tx_Base_Addr_A  =22'b0011_0000_0000_0000_0010_00;//11_0000_0000_0000_0010_0000_0000_0000
parameter [21:0]Tx_Base_Addr_B  =22'b0011_0000_0000_0000_0100_00;
parameter [31:0]Rec_Base_Addr_A =32'b0011_0000_0000_0000_0110_0000_0000_0000;
parameter [31:0]Rec_Base_Addr_B =32'b0011_0000_0000_0000_1000_0000_0000_0000; 

/*
parameter [11:0]Flag_Register_Addr_0=12'b0000_1111_1111,
                Tx_Base_Addr_A=12'b0000_????_????,
                Tx_Base_Addr_B=12'b0000_????_????;

*/
wire  TXA,TXB,RXA,RXB,REG0;
assign TXA=({PADDR[31],PADDR[30],PADDR[29],PADDR[28],PADDR[27],PADDR[26],PADDR[25],PADDR[24],PADDR[23],PADDR[22],PADDR[21],PADDR[20],PADDR[19],PADDR[18],PADDR[17],
            PADDR[16],PADDR[15],PADDR[14],PADDR[13],PADDR[12],PADDR[11],PADDR[10]}==Tx_Base_Addr_A)? 1: 0;
assign TXB=({PADDR[31],PADDR[30],PADDR[29],PADDR[28],PADDR[27],PADDR[26],PADDR[25],PADDR[24],PADDR[23],PADDR[22],PADDR[21],PADDR[20],PADDR[19],PADDR[18],PADDR[17],
            PADDR[16],PADDR[15],PADDR[14],PADDR[13],PADDR[12],PADDR[11],PADDR[10]}==Tx_Base_Addr_B)? 1: 0;
/*assign RXA=({PADDR[31],PADDR[30],PADDR[29],PADDR[28],PADDR[27],PADDR[26],PADDR[25],PADDR[24],PADDR[23],PADDR[22],PADDR[21],PADDR[20],PADDR[19],PADDR[18],PADDR[17],
            PADDR[16],PADDR[15],PADDR[14],PADDR[13],PADDR[12],PADDR[11],PADDR[10],10'B0000_0000_00}==Rec_Base_Addr_A)? 1: 0;
assign RXB=({PADDR[31],PADDR[30],PADDR[29],PADDR[28],PADDR[27],PADDR[26],PADDR[25],PADDR[24],PADDR[23],PADDR[22],PADDR[21],PADDR[20],PADDR[19],PADDR[18],PADDR[17],
            PADDR[16],PADDR[15],PADDR[14],PADDR[13],PADDR[12],PADDR[11],PADDR[10],10'B0000_0000_00}==Rec_Base_Addr_B)? 1: 0;
assign REG0=({PADDR[31],PADDR[30],PADDR[29],PADDR[28],PADDR[27],PADDR[26],PADDR[25],PADDR[24],PADDR[23],PADDR[22],PADDR[21],PADDR[20],PADDR[19],PADDR[18],PADDR[17],
            PADDR[16],PADDR[15],PADDR[14],PADDR[13],PADDR[12],PADDR[11],PADDR[10],PADDR[9],PADDR[8],PADDR[7],PADDR[6],PADDR[5],PADDR[4],PADDR[3],PADDR[2],PADDR[1],PADDR[0]}==Flag_Register_Addr_0)? 1: 0;
*/
always@(posedge Clk or posedge Rst)
begin
if(Rst)begin 
 //   Tx_Base_Addr_A <=22'b0011_0000_0000_0000_0010_00;
  Flag_B_Tx<=0;
  Flag_A_Tx<=0;
  Flag_Int<=0;
  ARS<=0;
  Byte_Count<=0;
		Flag_A_Rx<=0;
		Flag_B_Rx<=0;
		MASK<=16'h0000;
		ADDR<=16'h0000;
		Flag_Enable<=0;
		PRDATA<=32'h0000_0000;
		PREADY<=0;
		MSG_TRP1<=0;
		USER_RA_TRP1<=0;
		USER_REN_TRP1<=0;
		MSG_WA_TRP1<=0;
		USER_WEN_TRP1<=0;
    APB_ADDRESS<=0;

end 
else begin 
    APB_ADDRESS<=PADDR;
//cpu read data 
  if(!PWRITE&PSEL)begin
    if(PREADY)PREADY<=0;
    if(PENABLE)begin
      if( APB_ADDRESS==Flag_Register_Addr_0)begin PRDATA<={1'b1,23'h000000,Flag_Int[0],3'b011,/*Flag_OF,Flag_UF,*/Flag_B_Rx,Flag_A_Rx,Flag_B_Tx,Flag_A_Tx};PREADY<=1; end
      else if(APB_ADDRESS[31:10]==Rec_Base_Addr_A[31:10])begin //APB_ADDRESS[31:10]==Rec_Base_Addr_A[31:10]
						USER_RA_TRP1<=APB_ADDRESS[9:0]>>2;
						USER_REN_TRP1<=1; 
						if(!PENABLE) begin
							Flag_Enable<=0;
							PREADY<=0;
							end
						if(PENABLE&&!Flag_Enable)begin
							Flag_Enable<=1;
							PREADY<=0;
							end
						else if(PENABLE)begin
							PRDATA<=Data_In;
							PREADY<=1;
							end
						else begin
							PREADY<=0;
						end

        end
      else if(APB_ADDRESS[31:10]==Rec_Base_Addr_B[31:10])begin 
						USER_RA_TRP1<=((APB_ADDRESS[9:0]-512)>>2)+512;
						USER_REN_TRP1<=1; 
						if(!PENABLE) begin
							Flag_Enable<=0;
							PREADY<=0;
							end
						if(PENABLE&&!Flag_Enable)begin
							Flag_Enable<=1;
							PREADY<=0;
							end
						else if(PENABLE)begin
							PRDATA<=Data_In;
							PREADY<=1;
							end
						else begin
							PREADY<=0;
						end
        end
      else if(TXA)begin 
        PRDATA<=ARS+2; 
        PREADY<=1;
        end
      else if(RXB)begin 
        PRDATA<=ARS+1; 
        PREADY<=1;
        end

       else begin ARS<=0;PREADY<=1; PRDATA<=32'hFFFF_FFFF;end
      end
    end //if(!PWRITE&PSEL)begin 
  else if(PWRITE&PSEL)begin 
    if(PREADY)PREADY<=0;
    if(PENABLE)begin
      if( PADDR==Flag_Register_Addr_0)begin
        ARS<=PWDATA;PREADY<=1;
        if(PWDATA[0])begin Flag_A_Tx<=1;end
        if(PWDATA[1])begin Flag_B_Tx<=1;end
					if(!PWDATA[2])Flag_A_Rx<=0;  // CPU set 0
					if(!PWDATA[3])Flag_B_Rx<=0;
        if(!PWDATA[7])begin Flag_Int[0]<=0;end
        end
      else if(TXA)begin 
          case(Byte_Count)
            0:begin
              MSG_TRP1<=PWDATA[7:0];
              ARS[7:0]<=PWDATA[7:0];
              MSG_WA_TRP1<=PADDR[9:0]; 
              Byte_Count<=Byte_Count+1;
              end
            1:begin 
              MSG_TRP1<=PWDATA[15:8];
              ARS[15:8]<=PWDATA[15:8];
              MSG_WA_TRP1<=PADDR[9:0]+1;
              Byte_Count<=Byte_Count+1;
              end
            2:begin 
              MSG_TRP1<=PWDATA[23:16];
              ARS[23:16]<=PWDATA[23:16];
              MSG_WA_TRP1<=PADDR[9:0]+2;
              Byte_Count<=Byte_Count+1;

              end
            3:begin 
              PREADY<=1;
              ARS[31:24]<=PWDATA[31:24];
              MSG_TRP1<=PWDATA[31:24];
              MSG_WA_TRP1<=PADDR[9:0]+3;
              Byte_Count<=0;
              end
            default:begin 
              Byte_Count<=0;
              end
            endcase
          
          USER_WEN_TRP1<=1; 
          //ARS<=PWDATA;
          //PREADY<=1;
          end
      else begin ARS<=32'hffff_0000; PREADY<=1;end
    end
    end
  if(Flag_A_Tx_Finish)Flag_A_Tx<=0;
  if(Flag_B_Tx_Finish)Flag_B_Tx<=0;
  if((Flag_A_Tx&&Flag_A_Tx_Finish)||(Flag_B_Tx&&Flag_B_Tx_Finish)/*||Flag_B_RX||Flag_A_RX*/)Flag_Int[0]<=1;
        if(Flag_A_HDL_Rec_Finish)begin
			Flag_A_Rx<=1; //HDL set 1  
			Flag_Int<=1;
			end
        if(Flag_B_HDL_Rec_Finish)begin
			Flag_B_Rx<=1;
			Flag_Int<=1;
			end


end 
end//


endmodule

