///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: APB_v2.v
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

module APB_V3_CPU_R_HDL_W( Clk,Rst,PREADY,PRDATA,INT, PSEL,PENABLE,PWRITE,PADDR,PWDATA );
output  PSEL,PENABLE,PWRITE;
output [31:0]PADDR;
output [15:0]PWDATA;
input Clk,Rst,PREADY,INT;
input [15:0]PRDATA;


reg PSEL,PENABLE,PWRITE;
reg [31:0]PADDR;
reg [15:0]PWDATA;

//fabric to mss data buff 
reg [1751:0]Data_A;
reg [1751:0]Data_B;


reg [1:0]buff_type=2'b00;

reg [4:0]State;
parameter State_S0=5'b00000,
          State_S1=5'b00001,
          State_S2=5'b00010,
          State_S3=5'b00100,
          State_S4=5'b01000,
          State_S5=5'b10000,
          State_S6=5'b10001,
          State_S7=5'b10010;

parameter [31:0]Tx_Base_Addr_A  =32'b0011_0000_0000_0000_0010_0000_0000_0000;
parameter [31:0]Tx_Base_Addr_B  =32'b0011_0000_0000_0000_0100_0000_0000_????;
parameter [31:0]Rec_Base_Addr_A =32'b0011_0000_0000_0000_0110_0000_0000_0000;
parameter [31:0]Rec_Base_Addr_B =32'b0011_0000_0000_0000_1000_0010_0000_0000; 

//register address 
parameter [31:0]Flag_Register_Addr_0=32'h30000000,
                Flag_Register_Addr_1=32'h30000010,
                Flag_Register_Addr_2=32'h30000020,
                Flag_Register_Addr_3=32'h30000030,
                Flag_Register_Addr_4=32'h30000040,
                Flag_Register_Addr_5=32'h30000050,
                Flag_Register_Addr_6=32'h30000060,
                Flag_Register_Addr_7=32'h30000070,
                Flag_Register_Addr_8=32'h30000080,
                Flag_Register_Addr_9=32'h30000090,
                Flag_Register_Addr_10=32'h300000A0,
                Flag_Register_Addr_11=32'h300000B0,
                Flag_Register_Addr_12=32'h300000C0,
                Flag_Register_Addr_13=32'h300000D0,
                Flag_Register_Addr_14=32'h300000E0,
                Flag_Register_Addr_15=32'h300000F0;




parameter A_Buff_Add=1,
          B_Buff_Add=513;

reg [15:0]data_buff;

reg [15:0]Clk_Count;
reg [15:0]Block_Count;
reg [15:0]Block_Num;

reg flag_1=0;


always@(posedge Clk or negedge Rst)
begin 
if(!Rst)begin
  Clk_Count<=16'h0000;
  PSEL<=0;
  flag_1<=0;
  PENABLE<=0;
  PWRITE<=0;
  Block_Count<=16'h0000;
  Block_Num<=16'h0000;
  data_buff<=32'h0000_0000;
  PADDR<=32'h0000_0000;
  PWDATA<=32'h0000_0000;
  State<=5'b00000;
    Data_B<=864'd0000_0000_0000_0000_0000_0000_000;
    Data_A<=896'd0000_0000_0000_0000_0000_0000_0000;
    //Data_B_SP<=160'd0000_0000_0000_0000_0000_0000_000;
    //Data_A_SP<=160'd0000_0000_0000_0000_0000_0000_000;
    //Data_B_IP<=864'd0000_0000_0000_0000_0000_0000_000;
    //Data_A_IP<=896'd0000_0000_0000_0000_0000_0000_0000;
  end//end if(!Rst) 
else begin 
  if(INT&&!flag_1)begin 
    State<=State_S1;
    flag_1<=1;
    end //end if(INT)
  else if(!INT)flag_1<=0;
  case(State)
    State_S1:begin
      if(Clk_Count<2)begin
        PWRITE<=0;
        PSEL<=1;
        PADDR<=Flag_Register_Addr_0;
        Clk_Count<=Clk_Count+1;
        end
      else begin
        if(PRDATA[12])begin
          if(PRDATA[0])begin

            buff_type<=2'b01;
            PWDATA<={PRDATA[15:13],~PRDATA[12],PRDATA[11:3],~PRDATA[2],PRDATA[1:0]};
            end//end if(PRDATA[0])begin 
          else if(PRDATA[1])begin 
            buff_type<=2'b11;
            PWDATA<={PRDATA[15:13],~PRDATA[12],PRDATA[11:4],~PRDATA[3],PRDATA[2:0]};
            end
//clear flag_int
          PWRITE<=1;
          PSEL<=1;
          PENABLE<=1;
          data_buff<=PRDATA;

            State<=State_S2;//read data from buff
            Clk_Count<=0;

          end //end if(PRDATA[13])
        else begin 
          Clk_Count<=0;
          end //end else if(PRDATA[13])
        end//end else if(Clk_Count)
      end //end State_S1
    State_S2:begin 
      if(Clk_Count<2)begin
        PWRITE<=0;
        PSEL<=1;
        if(buff_type==2'b11)begin
          PADDR<=Rec_Base_Addr_B+Block_Count;
          end 
        else if(buff_type==2'b01)begin
          PADDR<=Rec_Base_Addr_A+Block_Count;
          end 
        Clk_Count<=Clk_Count+1;

        end //end if(Clk_Count<1)
      else begin 
        Block_Count<=Block_Count+1;
        Clk_Count<=0;
        if(buff_type==2'b01)begin
          Data_A<=Data_A<<8;
          Data_A[7:0]<=PRDATA;
          end 
        else if(buff_type==2'b11)begin
          Data_B<=Data_B<<8;
          Data_B[7:0]<=PRDATA;
          end 
        if(Block_Count>219)begin 
          State<=State_S3;
          end

        end //end else if(Clk_Count)

      end//end State_S2
    State_S3:begin 
      Block_Count<=0;
      State<=0;
      if(buff_type==2'b01)begin
        PWDATA<={data_buff[15:13],1'b0,data_buff[11:3],1'b1,data_buff[1:0]};
        end 
      else if(buff_type==2'b11)begin 
        PWDATA<={data_buff[15:13],1'b0,data_buff[11:4],1'b1,data_buff[2:0]};
        end 
//clear flag_int
          PADDR<=Flag_Register_Addr_0;
          PWRITE<=1;
          PSEL<=1;
          PENABLE<=1;

      end 
    default:begin end 

    endcase 

end//end else if(!Rst)
end//end always
//<statements>

endmodule

