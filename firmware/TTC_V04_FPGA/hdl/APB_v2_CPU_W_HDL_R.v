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

module APB_v2_CPU_W_HDL_R( Clk,Rst,PREADY,PRDATA,INT, PSEL,PENABLE,PWRITE,PADDR,PWDATA );
output  PSEL,PENABLE,PWRITE;
output [31:0]PADDR;
output [15:0]PWDATA;
input Clk,Rst,PREADY,INT;
input [15:0]PRDATA;


reg PSEL,PENABLE,PWRITE;
reg [31:0]PADDR;
reg [15:0]PWDATA;

//fabric to mss data buff 
reg [7:0]Data_A=8'h1a;
reg [7:0]Data_B=8'h1a;


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
parameter [31:0]Tx_Base_Addr_B  =32'b0011_0000_0000_0000_0100_0010_0000_0000;
parameter [31:0]Rec_Base_Addr_A =32'b0011_0000_0000_0000_0110_0000_0000_0000;
parameter [31:0]Rec_Base_Addr_B =32'b0011_0000_0000_0000_1000_0010_0000_0000; 

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

parameter A_Buff_Add=1,
          B_Buff_Add=513;

reg [15:0]data_buff;

reg [15:0]Clk_Count;
reg [15:0]Block_Count;
reg [15:0]Block_Num;
reg [3:0]pack_count;
reg [2:0]int_count;
reg flag_1=0;
reg flag_2=0;
reg flag_3=0;
always@(posedge Clk or posedge Rst)
begin 
if(Rst)begin
  Clk_Count<=16'h0000;
  PSEL<=0;
  flag_1<=0;
  flag_2<=0;
  PENABLE<=0;
int_count<=0;
  PWRITE<=0;
  Block_Count<=16'h0000;
  Block_Num<=16'h0000;
  data_buff<=32'h0000_0000;
  PADDR<=32'h0000_0000;
  PWDATA<=32'h0000_0000;
  State<=5'b00001;
    Data_B<=8'h1a;
    Data_A<=8'hfa;
pack_count<=0;
  end//end if(!Rst) 
else begin 
  if(INT)begin 
    flag_3=1;
    if(Clk_Count<2)begin
      PWRITE<=0;
      PSEL<=1;
      PADDR<=Flag_Register_Addr_0;
      Clk_Count<=Clk_Count+1;
      end
    else begin
    int_count<=int_count+1;
      PWRITE<=1;
      PSEL<=1;
      PENABLE<=1;
      if(PRDATA[5]&&!int_count)begin //A buff empty     Flag_A_Tx_Empty
        PWDATA<={PRDATA[15:13],1'b0,PRDATA[11:8],1'b0,PRDATA[6:0]};
//        State<=State_S1;
        end //end if(PRDATA[13])
      if(PRDATA[6]&&int_count) begin //B buff empty    Flag_B_Tx_Empty
        PWDATA<={PRDATA[15:13],1'b0,PRDATA[11:9],1'b0,PRDATA[7:0]};
//        State<=State_S1;
        end //end else if(PRDATA[13])
      Clk_Count<=0;
      flag_2<=1;

      end
    end//end if(INT)
  //else if(flag_3)begin
    //flag_2<=0;
    //flag_3<=0;
    //end

  else begin 
    case(State)
//    State_S0:begin 

//      end //end State_S1
    State_S1:begin
      if(Clk_Count<2)begin
        PWRITE<=0;
        PSEL<=1;
        PADDR<=Flag_Register_Addr_0;
        Clk_Count<=Clk_Count+1;
        end
      else begin
        if(PRDATA[5])begin
          buff_type<=2'b01;//A buff empty
          State<=State_S2;
          end //end if(PRDATA[13])
        else if(PRDATA[6]) begin 
          buff_type<=2'b11;//B buff empty 
          State<=State_S2;
          end //end else if(PRDATA[13])
          Clk_Count<=0;
        end//end else if(Clk_Count)
      end //end State_S1
    State_S2:begin 
        PWRITE<=1;
        PSEL<=1;
        PENABLE<=1;
        if(buff_type==2'b11)begin
          PADDR<=Tx_Base_Addr_B+Block_Count;
          end 
        else if(buff_type==2'b01)begin
          PADDR<=Tx_Base_Addr_A+Block_Count;
          end 
        Clk_Count<=Clk_Count+1;
        PWDATA<={24'h000000,Clk_Count[7:0]+1};/*Clk_Count[7:0]+1*/
 //       Data_A<=Data_A<<8;
        Block_Count<=Block_Count+1;
        if(Block_Count>222)begin 
          State<=State_S3;
          end
      end//end State_S2
    State_S3:begin 
      flag_2<=0;
      Block_Count<=0;
      Clk_Count<=0;
//      State<=5;
      if(buff_type==2'b01)begin
        PWDATA<={PRDATA[15:8],1'b1,PRDATA[6:0]};
        end 
      else if(buff_type==2'b11)begin 
        PWDATA<={PRDATA[15:9],1'b1,PRDATA[7:0]};
        end 
//clear flag_int
          PADDR<=Flag_Register_Addr_0;
          PWRITE<=1;
          PSEL<=1;
          PENABLE<=1;
        State<=State_S4;
        pack_count<=pack_count+1;
      end 
    State_S4:begin 
      if(pack_count<2)begin
        State<=State_S1;
        end
      end
    default:begin end 

    endcase 

end//end else if(INT)
end//end else if(!Rst)
end//end always
//<statements>

endmodule

