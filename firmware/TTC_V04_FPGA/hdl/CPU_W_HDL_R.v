///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: CPU_W_HDL_R.v
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

module CPU_W_HDL_R( ClkI_Dec8,ClkI_Dec2,Rst,
Flag_A_Tx,Flag_B_Tx,DataI,pending,En_Read_Buff,
USER_REN_TRP1,USER_RA_TRP1,Flag_A_Tx_Finish,Flag_B_Tx_Finish,En_CRC32, DataO ,En_ASG, TM_Packet_Counter );
input ClkI_Dec8,ClkI_Dec2, Rst,Flag_A_Tx,Flag_B_Tx,pending,En_Read_Buff;
input [7:0]DataI;
output USER_REN_TRP1,Flag_A_Tx_Finish,Flag_B_Tx_Finish,En_CRC32,En_ASG ;
output [9:0]USER_RA_TRP1;
reg USER_REN_TRP1,Flag_A_Tx_Finish,Flag_B_Tx_Finish,En_CRC32 ,En_ASG ;
wire [9:0]USER_RA_TRP1;
 //[7:0]DataO;
reg [9:0]Addr;
output wire [7:0] DataO ;
output reg[31:0] TM_Packet_Counter;
//reg [3:0]Bit_Count;
reg [1:0]Flag_Type;
reg [4:0]ASM_Count;
reg [4:0]State;
reg [1:0]Bit_Count;
//reg [7:0]temp;

reg [5:0]byte_count;
reg [2:0]clock_dec;

reg pos1_En_ASG;

parameter [15:0]temp_count=16;


parameter State_S0=5'b00000,
          State_S1=5'b00001,
          State_S2=5'b00010,
          State_S3=5'b00100,
          State_S4=5'b01000,
          State_S5=5'b10000,
          State_S6=5'b10001,
          State_S7=5'b10010;


assign USER_RA_TRP1=Addr;
assign DataO=DataI;




always@(posedge ClkI_Dec2 or negedge Rst )
begin 
if(!Rst)begin 
USER_REN_TRP1<=0;
      Flag_B_Tx_Finish<=0;
      Flag_A_Tx_Finish<=0;
Flag_Type<=0;
clock_dec<=0;
En_CRC32<=0;
Bit_Count<=0;
Addr<=10'b0000000000;
State<=State_S1;
En_ASG<=0;
ASM_Count<=0;
TM_Packet_Counter<=0;
pos1_En_ASG<=0;
//temp<=0;
end//end if(!Rst)
else begin 
pos1_En_ASG<=En_ASG;
if(!pos1_En_ASG&&En_ASG)	begin TM_Packet_Counter<=TM_Packet_Counter+1; end
//2dec clock
if(En_CRC32)begin

    if(ASM_Count<14)begin 
        ASM_Count<=ASM_Count+1;
        En_ASG<=0;
        end
    else begin 
        En_ASG<=1;
        end
end
else begin 
        ASM_Count<=0;
        En_ASG<=0;
end

if(En_ASG)begin
  byte_count<=byte_count+1;
  end
else byte_count<=0;
clock_dec<=clock_dec+1;

//8dec clock
if(clock_dec==0)begin

  case(State)
    State_S1:begin
      Flag_B_Tx_Finish<=0;
      Flag_A_Tx_Finish<=0;  
    if(En_Read_Buff&&pending)begin 
      if((Flag_A_Tx&&!Flag_B_Tx)||(Flag_A_Tx&&Flag_B_Tx&&Flag_Type!=1))begin 
	    Flag_Type<=1;
        USER_REN_TRP1<=1;
        Addr[9:0]<=0;
        State<=State_S2;
        end 
	  else if((!Flag_A_Tx&&Flag_B_Tx)||(Flag_A_Tx&&Flag_B_Tx&&Flag_Type!=2))begin 
	    Flag_Type<=2;
        USER_REN_TRP1<=1;
        Addr[9:0]<=10'b10000_00000;
        State<=State_S2;
        end 
    end

    end //end State_S1
    State_S2:begin 
      if(Addr[8:0]<223) begin 
        Addr<=Addr+1;
        end
      if(Addr[8:0]==222) begin 
        State<=State_S3;
        USER_REN_TRP1<=0;
        end //end else if(Addr[8:0]<220)
      En_CRC32<=1;
      end //end State_S2
    State_S3:begin 
      En_CRC32<=0;

      USER_REN_TRP1<=0;
      En_ASG<=0;
      Addr<=0;
    if(Flag_Type==1)begin 
      Flag_A_Tx_Finish<=1;
      State<=State_S1;
      end //end if(Flag_Type)
    else if(Flag_Type==2) begin 
      Flag_B_Tx_Finish<=1;
      State<=State_S1;
      end 
      end //end State_S3
    default:begin 
      State<=State_S1;
      end //end default
    endcase
end//
//end 8dec clock

end //end if else 
end //end always 

//always@(posedge ClkI_Dec2 or posedge Rst)begin
//if(Rst)begin
//byte_count<=0;
//end
//else begin
  //if(En_ASG)begin
    //byte_count<=byte_count+1;
    //end
  //else byte_count<=0;
//end
//end


//always@(posedge clk or posedge Rst)
//begin
//if(Rst)begin
  //Flag_B_Tx_Finish<=0;
  //Flag_A_Tx_Finish<=0;
  //end
//else begin
//
  //if(State==State_S3)begin
    //if(Flag_Type==1&&Flag_A_Tx)begin 
      //Flag_A_Tx_Finish<=1;
      //end //end if(Flag_Type)
    //else if(Flag_Type==2&&Flag_B_Tx) begin 
      //Flag_B_Tx_Finish<=1;
      //end 
      //if(!Flag_A_Tx)Flag_A_Tx_Finish<=0;
	  //if(!Flag_B_Tx)Flag_B_Tx_Finish<=0;
    //end
   //end
//end
//<statements>

endmodule

