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
Flag_Sending,
//
MASK,ADDR,
Flag_RX_ing,
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
TM_Packet_Counter,
TC_Packet_Counter, TC_Error_Counter,

PADDR,PSEL,PENABLE,PWRITE,PRDATA,PWDATA,PREADY,

Data_In,

USER_RA_TRP1,
USER_REN_TRP1,
MSG_TRP1,
MSG_WA_TRP1,
USER_WEN_TRP1
 );
input Clk,Rst,Flag_A_Tx_Finish,Flag_B_Tx_Finish,Flag_A_HDL_Rec_Finish,Flag_B_HDL_Rec_Finish,Flag_Sending,Flag_RX_ing;
input PSEL,PENABLE,PWRITE;
input [31:0]PADDR;
input [31:0]PWDATA;
input [15:0]SpacecraftID;
input [31:0]Data_In;

input[31:0] TM_Packet_Counter, TC_Packet_Counter, TC_Error_Counter;

output reg [31:0]PRDATA;
output reg PREADY;

//output [9:0]USER_RA_TRP1;

output reg Flag_A_Tx,Flag_B_Tx;
output reg Flag_A_Rx,Flag_B_Rx;
output reg [15:0]Flag_Int;





output reg [3:0]USER_RA_TRP1;  //!![6:0]->[3:0]
output reg USER_REN_TRP1;
output reg[15:0]MASK,ADDR;
output reg [7:0]MSG_TRP1;
output reg [9:0]MSG_WA_TRP1;
output reg USER_WEN_TRP1;

reg [31:0] APB_ADDRESS;
reg [31:0]ARS;
reg [1:0]Byte_Count;

reg Flag_Enable;
reg Flag_Enable1;
reg pos1_Flag_A_Tx_Finish,pos2_Flag_A_Tx_Finish;
reg pos1_Flag_B_Tx_Finish,pos2_Flag_B_Tx_Finish;
reg pos1_Flag_A_HDL_Rec_Finish, pos2_Flag_A_HDL_Rec_Finish;
reg pos1_Flag_B_HDL_Rec_Finish, pos2_Flag_B_HDL_Rec_Finish;


wire [31:0]liyewei;
assign liyewei={Data_In[7:0],Data_In[15:8],Data_In[23:16],Data_In[31:24]} ;

//register address 
parameter Flag_Register_Addr_0=32'h30000000;
parameter                Flag_Register_Addr_1=32'h30000004;
 parameter               Flag_Register_Addr_2=32'h30000008;
 parameter               Flag_Register_Addr_3=32'h3000000C;
 parameter               Flag_Register_Addr_4=32'h30000010;
 parameter               Flag_Register_Addr_5=32'h30000014;
 parameter               Flag_Register_Addr_6=32'h30000018;
 parameter               Flag_Register_Addr_7=32'h3000001C;
 parameter               Flag_Register_Addr_8=32'h30000020;
 parameter               Flag_Register_Addr_9=32'h30000024;
 parameter               Flag_Register_Addr_10=32'h30000028;
 parameter               Flag_Register_Addr_11=32'h3000002C;
 parameter               Flag_Register_Addr_12=32'h30000030;
 parameter               Flag_Register_Addr_13=32'h30000034;
 parameter               Flag_Register_Addr_14=32'h30000038;
 parameter               Flag_Register_Addr_15=32'h3000003C;
/*reg[7:0] Flag_Register_Addr_0=8'h00,
                Flag_Register_Addr_1=8'h04,
                Flag_Register_Addr_2=8'h08,
                Flag_Register_Addr_3=8'h0C,
                Flag_Register_Addr_4=8'h10,
                Flag_Register_Addr_5=8'h14,
                Flag_Register_Addr_6=8'h18,
                Flag_Register_Addr_7=8'h1C,
                Flag_Register_Addr_8=8'h20,
                Flag_Register_Addr_9=8'h24,
                Flag_Register_Addr_10=8'h28,
                Flag_Register_Addr_11=8'h2C,
                Flag_Register_Addr_12=8'h30,
                Flag_Register_Addr_13=8'h34,
                Flag_Register_Addr_14=8'h38,
                Flag_Register_Addr_15=8'h3C;
*/
parameter  [21:0]Tx_Base_Addr_A  =22'b0011_0000_0000_0000_0010_00;//11_0000_0000_0000_0010_0000_0000_0000
parameter [21:0]Tx_Base_Addr_B  =22'b0011_0000_0000_0000_0100_00;
parameter [31:0]Rec_Base_Addr_A =32'b0011_0000_0000_0000_0110_0000_0000_0000;
parameter [31:0]Rec_Base_Addr_B =32'b0011_0000_0000_0000_1000_0000_0000_0000; 

parameter TM_Length=32'd1784;
parameter TC_Length=32'd1792;

wire  TXA,TXB,RXB;
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

wire ADDR1,ADDR2,ADDR3,ADDR4,ADDR5,ADDR6,ADDR7,ADDR8,ADDR9;
assign ADDR1=PADDR=={24'h300000,Flag_Register_Addr_0}?1:0;
assign ADDR2=PADDR=={24'h300000,Flag_Register_Addr_1}?1:0;
assign ADDR3=PADDR=={24'h300000,Flag_Register_Addr_2}?1:0;
assign ADDR4=PADDR=={24'h300000,Flag_Register_Addr_3}?1:0;
assign ADDR5=PADDR=={24'h300000,Flag_Register_Addr_5}?1:0;
assign ADDR6=PADDR=={24'h300000,Flag_Register_Addr_6}?1:0;
assign ADDR7=PADDR=={24'h300000,Flag_Register_Addr_12}?1:0;
assign ADDR8=PADDR=={24'h300000,Flag_Register_Addr_13}?1:0;
assign ADDR9=PADDR=={24'h300000,Flag_Register_Addr_14}?1:0;
*/
always@(posedge Clk or negedge Rst)
begin
if(!Rst)begin 
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
    Flag_Enable1<=0;
    pos1_Flag_A_Tx_Finish<=0;
    pos2_Flag_A_Tx_Finish<=0;
    pos1_Flag_B_Tx_Finish<=0;
    pos2_Flag_B_Tx_Finish<=0;
    pos1_Flag_A_HDL_Rec_Finish<=0;
    pos2_Flag_A_HDL_Rec_Finish<=0;
    pos1_Flag_B_HDL_Rec_Finish<=0;
    pos2_Flag_B_HDL_Rec_Finish<=0;

/*	Flag_Register_Addr_0<=8'h00;
                Flag_Register_Addr_1<=8'h04;
                Flag_Register_Addr_2<=8'h08;
                Flag_Register_Addr_3<=8'h0C;
                Flag_Register_Addr_4<=8'h10;
                Flag_Register_Addr_5<=8'h14;
                Flag_Register_Addr_6<=8'h18;
                Flag_Register_Addr_7<=8'h1C;
                Flag_Register_Addr_8<=8'h20;
                Flag_Register_Addr_9<=8'h24;
                Flag_Register_Addr_10<=8'h28;
                Flag_Register_Addr_11<=8'h2C;
                Flag_Register_Addr_12<=8'h30;
                Flag_Register_Addr_13<=8'h34;
                Flag_Register_Addr_14<=8'h38;
                Flag_Register_Addr_15<=8'h3C; */
end 
else begin 
    APB_ADDRESS<=PADDR;
    //Enable<=PENABLE;
    if(PREADY)begin
		PREADY<=0;
		Flag_Enable1<=0;  //!!!
		Flag_Enable<=0;		//!!!
	end

//cpu read data 
  if(!PWRITE&PSEL)begin
    
    if(PENABLE)begin
  //  if(ADDR1)begin PRDATA<={1'b1,23'h000000,Flag_Int[0],1'b0,Flag_Sending,1'b1,/*Flag_UF,*/Flag_B_Rx,Flag_A_Rx,Flag_B_Tx,Flag_A_Tx};PREADY<=1; end
		
	/*  else if(ADDR2)begin PRDATA<={16'h0000,SpacecraftID}; PREADY<=1; end
	  else if(ADDR3)begin PRDATA<={16'h0000,MASK}; PREADY<=1; end
	  else if(ADDR4)begin PRDATA<={16'h0000,ADDR}; PREADY<=1; end
	  
	  else if(ADDR5)begin PRDATA<={16'h0000,TM_Length}; PREADY<=1; end
	  else if(ADDR6)begin PRDATA<={16'h0000,TC_Length}; PREADY<=1; end
	  
	  else if(ADDR7)begin PRDATA<=TM_Packet_Counter; PREADY<=1; end
	  else if(ADDR8)begin PRDATA<=TC_Packet_Counter; PREADY<=1; end
	  else if(ADDR9)begin PRDATA<=TC_Error_Counter; PREADY<=1; end*/
	  if( PADDR==Flag_Register_Addr_0)begin PRDATA<={1'b1,23'h000000,Flag_Int[0],Flag_RX_ing,Flag_Sending,1'b1,/*Flag_UF,*/Flag_B_Rx,Flag_A_Rx,Flag_B_Tx,Flag_A_Tx};PREADY<=1; end
	  else if(PADDR==Flag_Register_Addr_1)begin PRDATA<={16'h0000,SpacecraftID}; PREADY<=1; end
	  else if(PADDR==Flag_Register_Addr_2)begin PRDATA<={16'h0000,MASK}; PREADY<=1; end
	  else if(PADDR==Flag_Register_Addr_3)begin PRDATA<={16'h0000,ADDR}; PREADY<=1; end
	  
	  else if(PADDR==Flag_Register_Addr_5)begin PRDATA<={16'h0000,TM_Length}; PREADY<=1; end
	  else if(PADDR==Flag_Register_Addr_6)begin PRDATA<={16'h0000,TC_Length}; PREADY<=1; end
	  
	  else if(PADDR==Flag_Register_Addr_12)begin PRDATA<=TM_Packet_Counter; PREADY<=1; end
	  else if(PADDR==Flag_Register_Addr_13)begin PRDATA<=TC_Packet_Counter; PREADY<=1; end
	  else if(PADDR==Flag_Register_Addr_14)begin PRDATA<=TC_Error_Counter; PREADY<=1; end
      else if(APB_ADDRESS[31:10]==Rec_Base_Addr_A[31:10])begin //APB_ADDRESS[31:10]==Rec_Base_Addr_A[31:10]
						
						
						if(PENABLE&&!Flag_Enable)begin
							Flag_Enable<=1;
							USER_RA_TRP1<=APB_ADDRESS[9:0]>>2;
                            USER_REN_TRP1<=1; 
							end
                        else if(PENABLE&&!Flag_Enable1)begin
							Flag_Enable1<=1;
                            USER_REN_TRP1<=0;
							end
						else if(PENABLE)begin       
                            USER_REN_TRP1<=0;
							PRDATA<=liyewei;
							PREADY<=1;
							end
						/*else begin
                            USER_REN_TRP1<=0;
							PREADY<=0;
						end*/

        end
      else if(APB_ADDRESS[31:10]==Rec_Base_Addr_B[31:10])begin 
						
						if(PENABLE&&!Flag_Enable)begin
							Flag_Enable<=1;
							USER_RA_TRP1<=((APB_ADDRESS[9:0])>>2)+8;   //!! +64 -> +8
                            USER_REN_TRP1<=1; 
							end
                        else if(PENABLE&&!Flag_Enable1)begin
							Flag_Enable1<=1;
                            USER_REN_TRP1<=0;
							end
						else if(PENABLE)begin
                            USER_REN_TRP1<=0;
							PRDATA<=liyewei;
							PREADY<=1;
							end
					/*	else begin
							PREADY<=0;
						end*/
        end
       else begin ARS<=0;PREADY<=1; PRDATA<=32'hFFFF_FFFF;end
      end
    end //if(!PWRITE&PSEL)begin 
  else if(PWRITE&PSEL)begin
	Flag_Enable1<=0;  //!!!
	Flag_Enable<=0;		//!!! 
    if(PENABLE)begin
      if( PADDR==Flag_Register_Addr_0)begin
        PREADY<=1;
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
              PREADY<=1;
              end
            3:begin 
              PREADY<=0;
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
      else if(TXB)begin 
          case(Byte_Count)
            0:begin
              MSG_TRP1<=PWDATA[7:0];
              ARS[7:0]<=PWDATA[7:0];
              MSG_WA_TRP1<=PADDR[9:0]+512; 
              Byte_Count<=Byte_Count+1;
              end
            1:begin 
              MSG_TRP1<=PWDATA[15:8];
              ARS[15:8]<=PWDATA[15:8];
              MSG_WA_TRP1<=PADDR[9:0]+513;
              Byte_Count<=Byte_Count+1;
              end
            2:begin 
              MSG_TRP1<=PWDATA[23:16];
              ARS[23:16]<=PWDATA[23:16];
              MSG_WA_TRP1<=PADDR[9:0]+514;
              Byte_Count<=Byte_Count+1;
              PREADY<=1;
              end
            3:begin 
              PREADY<=0;
              ARS[31:24]<=PWDATA[31:24];
              MSG_TRP1<=PWDATA[31:24];
              MSG_WA_TRP1<=PADDR[9:0]+515;
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
  //if(Flag_A_Tx_Finish&&Flag_A_Tx)Flag_A_Tx<=0;
  //if(Flag_B_Tx_Finish&&Flag_B_Tx)Flag_B_Tx<=0;
    pos1_Flag_A_Tx_Finish<=Flag_A_Tx_Finish;
    pos2_Flag_A_Tx_Finish<=pos1_Flag_A_Tx_Finish;

    pos1_Flag_B_Tx_Finish<=Flag_B_Tx_Finish;
    pos2_Flag_B_Tx_Finish<=pos1_Flag_B_Tx_Finish;
    if(pos1_Flag_A_Tx_Finish&&!pos2_Flag_A_Tx_Finish) Flag_A_Tx<=0;
    if(pos1_Flag_B_Tx_Finish&&!pos2_Flag_B_Tx_Finish) Flag_B_Tx<=0;

  if((Flag_A_Tx&&Flag_A_Tx_Finish)||(Flag_B_Tx&&Flag_B_Tx_Finish)/*||Flag_B_RX||Flag_A_RX*/)Flag_Int[0]<=1;

    pos1_Flag_A_HDL_Rec_Finish<=Flag_A_HDL_Rec_Finish;
    pos2_Flag_A_HDL_Rec_Finish<=pos1_Flag_A_HDL_Rec_Finish;
    if(!pos2_Flag_A_HDL_Rec_Finish&&pos1_Flag_A_HDL_Rec_Finish)begin
        Flag_A_Rx<=1; //HDL set 1  
		Flag_Int<=1;
        end
    pos1_Flag_B_HDL_Rec_Finish<=Flag_B_HDL_Rec_Finish;
    pos2_Flag_B_HDL_Rec_Finish<=pos1_Flag_B_HDL_Rec_Finish;
    if(!pos2_Flag_B_HDL_Rec_Finish&&pos1_Flag_B_HDL_Rec_Finish)begin
        Flag_B_Rx<=1; //HDL set 1  
		Flag_Int<=1;
        end


end 
end//
assign input_data=MSG_TRP1[0];
endmodule

