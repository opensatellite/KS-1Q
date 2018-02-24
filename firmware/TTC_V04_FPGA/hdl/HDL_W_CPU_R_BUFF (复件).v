///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: HDL_W_CPU_R_BUFF.v
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

module HDL_W_CPU_R_BUFF( Clk,Rst,DataI,EnIP,Block_Err,Flag_A_Rx, Flag_B_Rx, IP_END_I, Flag_A_HDL_Rec_Finish,Flag_B_HDL_Rec_Finish,MSG_TRP1,USER_WEN_TRP1,MSG_WA_TRP1,Flag_OF );
input Clk,Rst,DataI,EnIP,Block_Err,Flag_A_Rx,Flag_B_Rx,IP_END_I;
output [31:0]MSG_TRP1;
output USER_WEN_TRP1,Flag_OF,Flag_A_HDL_Rec_Finish,Flag_B_HDL_Rec_Finish;
output [3:0]MSG_WA_TRP1;  //!![6:0]->[3:0]

reg [31:0]MSG_TRP1;
reg [3:0]MSG_WA_TRP1;  //!![6:0]->[3:0]

reg  USER_WEN_TRP1;


reg Flag_Once,Flag_OF,Flag_A_HDL_Rec_Finish,Flag_B_HDL_Rec_Finish;


reg [7:0]Bit_Count;
reg [5:0]Block_Count;
reg [3:0]clk_count;
reg [31:0]Data_A;
reg [31:0]Data_B;



reg [4:0]State,State_Abuff;

parameter State1_S0=5'b00000;
parameter State1_S1=5'b00001;
parameter State1_S2=5'b00010;
parameter State1_S3=5'b00100;
parameter State1_S4=5'b01000;
parameter State1_S5=5'b10000;
parameter State1_S6=5'b10001;

parameter IP_Block=4;  //!!!32->4

//save data 
always@(posedge Clk or negedge Rst)
begin 
	if(IP_END_I)begin
		Bit_Count<=0;
		Block_Count<=0;
		State_Abuff<=State1_S0;
		State<=State1_S0;
        clk_count<=0;
	end
	if(!Rst)begin 
		MSG_WA_TRP1<=10'b00000_00000;
		MSG_TRP1<=32'h0000_0000;
		USER_WEN_TRP1<=0;

		Block_Count<=6'h00;
		Bit_Count<=8'h00;
		Data_A<=32'h0000_0000;
		Data_B<=32'h0000_0000;
		State<=5'b00000;
		Flag_Once<=0;
		State_Abuff<=5'b00000;
		Flag_A_HDL_Rec_Finish<=0;
		Flag_B_HDL_Rec_Finish<=0;
        clk_count<=0;
		end
	else if(Block_Err)begin
		MSG_WA_TRP1<=10'b00000_00000;
		MSG_TRP1<=32'h0000_0000;
		USER_WEN_TRP1<=0;

		Block_Count<=6'h00;
		Bit_Count<=8'h00;
		Data_A<=32'h0000_0000;
		Data_B<=32'h0000_0000;
		State<=5'b00000;
		Flag_Once<=0;
		State_Abuff<=5'b00000;
		Flag_A_HDL_Rec_Finish<=0;
		Flag_B_HDL_Rec_Finish<=0;
        clk_count<=0;
		end
	else begin 

	case(State)
		State1_S0:begin 
			USER_WEN_TRP1<=0;
			if(!Flag_A_Rx)begin
				Flag_A_HDL_Rec_Finish<=0;
				end
			if(!Flag_B_Rx)begin
				Flag_B_HDL_Rec_Finish<=0;
				end
			if((EnIP)&&!Flag_A_Rx&&!Flag_OF)begin 
				State<=State1_S1;
				Data_A[0]<=DataI;
				Bit_Count<=1;
				MSG_WA_TRP1<=0;
				end //end if((EnIP)&&!Flag_A_Rx&&!Flag_OF)
			else if((EnIP)&&!Flag_B_Rx&&!Flag_OF)begin 
				State<=State1_S1;
				Data_A[0]<=DataI;
				Bit_Count<=1;
				MSG_WA_TRP1<=MSG_WA_TRP1+8;//!!!+64->+8   buff a was full,data input save in b buff.offset address 512 word 

				end//end if((EnIP)&&!Flag_B_Rx&&!Flag_OF)begin 
			else if(EnIP)begin 
				Flag_OF<=1;
				end //end else if((EnIP)&&Flag_A_HDL_Rec_Finish&&!Flag_OF)begin 
			else if(!EnIP)begin //the enip is all low, so,reset the flag_of flag .
				Flag_OF<=0;
				end //end 
			end //end State_S0
		State1_S1:begin 
			if(EnIP)begin
				if(!Flag_Once)begin
					Block_Count<=Block_Count+1;
					Flag_Once<=1;
					end 
				if(Block_Count<=IP_Block)begin
					if(USER_WEN_TRP1)begin
						MSG_WA_TRP1<=MSG_WA_TRP1+1;
					end
					case(State_Abuff)
						State1_S0:begin
							if(Bit_Count<32)begin  
								Data_A<=Data_A<<1;
								Data_A[0]<=DataI;
								Bit_Count<=Bit_Count+1;
								USER_WEN_TRP1<=0;
								end //if(Bit_Count<8)
							else begin 
								MSG_TRP1<=Data_A;
								USER_WEN_TRP1<=1;
								State_Abuff<=State1_S1;
								Data_B[0]<=DataI;
								Bit_Count<=1;
								end//end else if(Bit_Count<8)
							end //end State_S0
						State1_S1:begin
							if(Bit_Count<32)begin  
								Data_B<=Data_B<<1;
								Data_B[0]<=DataI;
								Bit_Count<=Bit_Count+1;
								USER_WEN_TRP1<=0;
								end //if(Bit_Count<8)
							else begin 
								MSG_TRP1<=Data_B;
								USER_WEN_TRP1<=1;
								State_Abuff<=State1_S0;
								Data_A[0]<=DataI;
								Bit_Count<=1;
								end//end else if(Bit_Count<8)
							end //end State_S1
						default:begin 
							State<=State1_S3;
							end //end default
						endcase 
					end //if(Block_Count<Block_Num)begin
				end //end if(EnIP)
			else if(Block_Count==IP_Block)begin//data change is finish, reset system
				Bit_Count<=0;
				Block_Count<=0;
				State_Abuff<=State1_S0;
				Flag_Once<=0;
				MSG_TRP1<=Data_A;  //!!!Data_B->Data_A
				USER_WEN_TRP1<=1;
				State<=State1_S2;//
				end //
			else begin
				Flag_Once<=0;
				end //end else if(EnIP) 
			end//end State_S1
		State1_S2:begin
			USER_WEN_TRP1<=0;
            clk_count<=clk_count+1;
            if(clk_count==6)begin
                clk_count<=0;
                if(Block_Err)State<=State1_S3;//block error 
                if(IP_END_I)begin //data crc32 check success
                    if(MSG_WA_TRP1>8)begin  //!!!64->8
                        Flag_B_HDL_Rec_Finish<=1;
                        MSG_WA_TRP1<=0;
                        State<=State1_S0;
                        end 
                    else begin 
                        Flag_A_HDL_Rec_Finish<=1;
                        MSG_WA_TRP1<=0;
                        State<=State1_S0;
                        end 
                    end//end else if(Block_Err)
                else State<=State1_S3;//block error ip_end erro
                end//end  if(clk_count==7)
			end //end State_S2
		State1_S3:begin 
			MSG_WA_TRP1<=10'b00000_00000;
			MSG_TRP1<=8'h00;
			USER_WEN_TRP1<=0;
			Block_Count<=6'h00;

			Bit_Count<=8'h00;
			Data_A<=8'h00;
			Data_B<=8'h00;
			State<=5'b00000;
			Flag_Once<=0;
			State_Abuff<=5'b00000;
			Flag_A_HDL_Rec_Finish<=0;
			Flag_B_HDL_Rec_Finish<=0;
			State<=State1_S0;
			end //end state_s3
		default:
			State<=State1_S0;
		endcase
		end //end else if(!Rst)
	end 
endmodule

