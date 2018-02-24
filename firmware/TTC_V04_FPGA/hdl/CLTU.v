///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: CLTU.v
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

module CLTU( IO_Bit0,DataI, ClkI, Rst,DataO,Enable,Block_ErrO, IP_END, Dis_Switch, CCA, TC_Packet_Counter, TC_Error_Counter, Flag_RX_ing);
input IO_Bit0,DataI, ClkI,Rst, CCA;
output DataO,Enable ,Block_ErrO, Dis_Switch, IP_END;
output reg Flag_RX_ing;

parameter start_sequence=16'heb90;
parameter end_sequence=64'hc5c5_c5c5_c5c5_c579;

reg Dis_Switch=0;
reg DataO=0,Enable =0,Block_ErrO=0;
reg IP_END=0;
reg [6:0]Syn_reg=7'h00;
reg [55:0]Shift_reg=56'h0000_0000_0000_00;
reg [6:0]Bit_Counter=7'h00;
reg [6:0]Shift_Bit_Counter=7'h00;
reg [6:0]Parity=7'h00;
reg [2:0]Parity_Counter=3'b000;
reg [4:0]State1=5'h01;
reg [4:0]State2=5'h00;
reg [15:0]Start_Buff=16'h00;

reg [5:0]block_counter;
reg Flag_once=0;//send start sequence 16'heb90
reg [3:0]Start_Sequence_counter=4'h0;

output reg[31:0] TC_Packet_Counter, TC_Error_Counter;

parameter State1_S0=5'b00000;
parameter State1_S1=5'b00001;
parameter State1_S2=5'b00010;
parameter State1_S3=5'b00100;
parameter State1_S4=5'b01000;
parameter State1_S5=5'b10000;



parameter State2_S0=5'b00000;
parameter State2_S1=5'b00001;
parameter State2_S2=5'b00010;
parameter State2_S3=5'b00100;
parameter State2_S4=5'b01000;
parameter State2_S5=5'b10000;





always@(posedge ClkI or negedge Rst)
begin   
	if(!Rst) begin     //reset all
		DataO<=0;
		Enable<=0;
		Block_ErrO<=0;
		Syn_reg<=7'b0000_000;
		Shift_reg<=56'h0000_0000_0000_00;
		Bit_Counter<=7'b0000_000;
		Shift_Bit_Counter<=7'b0000_000;
		Parity<=7'b0000_000;
		Parity_Counter<=3'b000;
		State1<=State1_S1;
		State2<=State2_S0;
		Start_Buff<=16'h00;
		Dis_Switch<=0;
		Flag_once<=0;
		Start_Sequence_counter<=4'h0;
		IP_END<=0;
		block_counter<=6'h00;
		TC_Packet_Counter<=0;
		TC_Error_Counter<=0;
		Flag_RX_ing<=0;
	end
    else if(!CCA&&block_counter)begin
        DataO<=0;
		Enable<=0;
		Syn_reg<=7'b0000_000;
		Shift_reg<=56'h0000_0000_0000_00;
		Bit_Counter<=7'b0000_000;
		Shift_Bit_Counter<=7'b0000_000;
		Parity<=7'b0000_000;
		Parity_Counter<=3'b000;
		State1<=State1_S1;
		State2<=State2_S0;
		Start_Buff<=16'h00;
		Dis_Switch<=0;
		Flag_once<=0;
		Start_Sequence_counter<=4'h0;
		IP_END<=0;
		block_counter<=6'h00;
        Block_ErrO<=1;
		Flag_RX_ing<=0;
    end
   
	else if(CCA) begin
		if(IO_Bit0)begin 
		  Dis_Switch<=1;
		end 

		else  begin 
			case(State1)
				State1_S1:begin 		
					if(start_sequence!=Start_Buff)begin 		//receive and match start_sequence
						Start_Buff[15:1]<=Start_Buff[14:0];
						Start_Buff[0]<=DataI;
						IP_END<=0;
						end 
					else begin
						Flag_RX_ing<=1;
					//	TC_Packet_Counter<=TC_Packet_Counter+1;
						Block_ErrO<=0;
						Shift_reg[55]<=DataI;				//receive the first data , Data[55]
						State1<=State1_S2;
				//        Shift_reg[54-Bit_Counter]<=DataI;
						Syn_reg[0]<=Syn_reg[6]^DataI;		//BCH(63,56) calculate the syndromes
						Syn_reg[1]<=Syn_reg[0];
						Syn_reg[2]<=Syn_reg[1]^Syn_reg[6]^DataI;
						Syn_reg[3]<=Syn_reg[2];
						Syn_reg[4]<=Syn_reg[3];
						Syn_reg[5]<=Syn_reg[4];
						Syn_reg[6]<=Syn_reg[5]^Syn_reg[6]^DataI;
						Bit_Counter<=1;
						end //end if(start_sequence[Bin_Cout]==DataI)
					end  //end State1_S1
				State1_S2:begin
                	if(block_counter>4)begin           //!!!32->4  
						TC_Packet_Counter<=TC_Packet_Counter+1;
						TC_Error_Counter<=TC_Error_Counter+1;
                        Block_ErrO<=1;
						Flag_RX_ing<=0;
                        DataO<=0;
                        Enable<=0;
						Shift_reg<=56'h0000_0000_0000_00;
						State2<=State2_S0;
						State1<=State1_S1;						
                        block_counter<=0;
						Start_Buff<=16'h00;
                        Syn_reg<=7'h00;
                        Parity<=7'h00;
                        Bit_Counter<=0; 
                        Parity_Counter<=0;
                        Flag_once<=0;
                        Start_Sequence_counter<=4'h0;
						end//end default 
					else if(Bit_Counter<63)begin 
						if(Bit_Counter<56)begin 
							Shift_reg[55-Bit_Counter]<=DataI;		//receive Data[54:0](State1_S1->State1_S2)  or  Data[55:0](State1_S2->State1_S2)
							Syn_reg[0]<=Syn_reg[6]^DataI;			//BCH(63,56) calculate the syndromes
							Syn_reg[1]<=Syn_reg[0];
							Syn_reg[2]<=Syn_reg[1]^Syn_reg[6]^DataI;
							Syn_reg[3]<=Syn_reg[2];
							Syn_reg[4]<=Syn_reg[3];
							Syn_reg[5]<=Syn_reg[4];
							Syn_reg[6]<=Syn_reg[5]^Syn_reg[6]^DataI;
							end//if(Bit_Counter<=55)
						else begin
							Parity[6-Parity_Counter]<=DataI;     //receive the calculated syndromes Parity[6:0]
							Parity_Counter<=Parity_Counter+1;
							end//end else if
						if(!Flag_once && Bit_Counter>46)begin
							DataO<=Start_Buff[15-Start_Sequence_counter];
							Start_Sequence_counter<=Start_Sequence_counter+1;
							if(Start_Sequence_counter==15)begin
								Flag_once<=1;
								end
							end
						Bit_Counter<=Bit_Counter+1;				//receive bit counter +1
						end//if(Bit_Counter<=62)
					else if (end_sequence=={Shift_reg,Parity,DataI})begin    //receive and match end_sequence
						TC_Packet_Counter<=TC_Packet_Counter+1;
						State1<=State1_S1;
						Syn_reg<=7'h00;
						Parity<=7'h00;
						Start_Buff<=16'h00;
						Bit_Counter<=0; 
						Parity_Counter<=0;
						Block_ErrO<=0;
						Flag_RX_ing<=0;
						Flag_once<=0;
						Start_Sequence_counter<=4'h0;
						IP_END<=1;
						block_counter<=0;
						end //end else if (end_sequence==Shift_reg)
					else begin 
						block_counter<=block_counter+1;
						if((Syn_reg^Parity^7'b111_1111)!=0) begin  //1 bit error detection and correction
//if( 0) begin
							case(Syn_reg^Parity^7'b111_1111)        //error detection and correction and jump to State2_S1(send data)
								98:  begin Shift_reg[55]<=Shift_reg[55]+1; State2<=State2_S1; end   
								49:  begin Shift_reg[54]<=Shift_reg[54]+1; State2<=State2_S1; end
								122: begin Shift_reg[53]<=Shift_reg[53]+1; State2<=State2_S1; end
								61:  begin Shift_reg[52]<=Shift_reg[52]+1; State2<=State2_S1; end
								124: begin Shift_reg[51]<=Shift_reg[51]+1; State2<=State2_S1; end
								62:  begin Shift_reg[50]<=Shift_reg[50]+1; State2<=State2_S1; end
								31:  begin Shift_reg[49]<=Shift_reg[49]+1; State2<=State2_S1; end
								109: begin Shift_reg[48]<=Shift_reg[48]+1; State2<=State2_S1; end
								84:  begin Shift_reg[47]<=Shift_reg[47]+1; State2<=State2_S1; end
								42:  begin Shift_reg[46]<=Shift_reg[46]+1; State2<=State2_S1; end
								21:  begin Shift_reg[45]<=Shift_reg[45]+1; State2<=State2_S1; end
								104: begin Shift_reg[44]<=Shift_reg[44]+1; State2<=State2_S1; end
								52:  begin Shift_reg[43]<=Shift_reg[43]+1; State2<=State2_S1; end
								26:  begin Shift_reg[42]<=Shift_reg[42]+1; State2<=State2_S1; end
								13:  begin Shift_reg[41]<=Shift_reg[41]+1; State2<=State2_S1; end
								100: begin Shift_reg[40]<=Shift_reg[40]+1; State2<=State2_S1; end
								50:  begin Shift_reg[39]<=Shift_reg[39]+1; State2<=State2_S1; end
								25:  begin Shift_reg[38]<=Shift_reg[38]+1; State2<=State2_S1; end
								110: begin Shift_reg[37]<=Shift_reg[37]+1; State2<=State2_S1; end
								55:  begin Shift_reg[36]<=Shift_reg[36]+1; State2<=State2_S1; end
								121: begin Shift_reg[35]<=Shift_reg[35]+1; State2<=State2_S1; end
								94:  begin Shift_reg[34]<=Shift_reg[34]+1; State2<=State2_S1; end
								47:  begin Shift_reg[33]<=Shift_reg[33]+1; State2<=State2_S1; end
								117: begin Shift_reg[32]<=Shift_reg[32]+1; State2<=State2_S1; end
								88:  begin Shift_reg[31]<=Shift_reg[31]+1; State2<=State2_S1; end
								44:  begin Shift_reg[30]<=Shift_reg[30]+1; State2<=State2_S1; end
								22:  begin Shift_reg[29]<=Shift_reg[29]+1; State2<=State2_S1; end
								11:  begin Shift_reg[28]<=Shift_reg[28]+1; State2<=State2_S1; end
								103: begin Shift_reg[27]<=Shift_reg[27]+1; State2<=State2_S1; end
								81:  begin Shift_reg[26]<=Shift_reg[26]+1; State2<=State2_S1; end
								74:  begin Shift_reg[25]<=Shift_reg[25]+1; State2<=State2_S1; end
								37:  begin Shift_reg[24]<=Shift_reg[24]+1; State2<=State2_S1; end
								112: begin Shift_reg[23]<=Shift_reg[23]+1; State2<=State2_S1; end
								56:  begin Shift_reg[22]<=Shift_reg[22]+1; State2<=State2_S1; end
								28:  begin Shift_reg[21]<=Shift_reg[21]+1; State2<=State2_S1; end
								14:  begin Shift_reg[20]<=Shift_reg[20]+1; State2<=State2_S1; end
								7:   begin Shift_reg[19]<=Shift_reg[19]+1; State2<=State2_S1; end
								97:  begin Shift_reg[18]<=Shift_reg[18]+1; State2<=State2_S1; end
								82:  begin Shift_reg[17]<=Shift_reg[17]+1; State2<=State2_S1; end
								41:  begin Shift_reg[16]<=Shift_reg[16]+1; State2<=State2_S1; end
								118: begin Shift_reg[15]<=Shift_reg[15]+1; State2<=State2_S1; end
								59:  begin Shift_reg[14]<=Shift_reg[14]+1; State2<=State2_S1; end
								127: begin Shift_reg[13]<=Shift_reg[13]+1; State2<=State2_S1; end
								93:  begin Shift_reg[12]<=Shift_reg[12]+1; State2<=State2_S1; end
								76:  begin Shift_reg[11]<=Shift_reg[11]+1; State2<=State2_S1; end
								38:  begin Shift_reg[10]<=Shift_reg[10]+1; State2<=State2_S1; end
								19:  begin Shift_reg[9]<=Shift_reg[9]+1; State2<=State2_S1; end
								107: begin Shift_reg[8]<=Shift_reg[8]+1; State2<=State2_S1; end
								87:  begin Shift_reg[7]<=Shift_reg[7]+1; State2<=State2_S1; end
								73:  begin Shift_reg[6]<=Shift_reg[6]+1; State2<=State2_S1; end
								70:  begin Shift_reg[5]<=Shift_reg[5]+1; State2<=State2_S1; end
								35:  begin Shift_reg[4]<=Shift_reg[4]+1; State2<=State2_S1; end
								115: begin Shift_reg[3]<=Shift_reg[3]+1; State2<=State2_S1; end
								91:  begin Shift_reg[2]<=Shift_reg[2]+1; State2<=State2_S1; end
								79:  begin Shift_reg[1]<=Shift_reg[1]+1; State2<=State2_S1; end
								69:  begin Shift_reg[0]<=Shift_reg[0]+1; State2<=State2_S1; end
								64:  begin Parity[6]<=Parity[6]+1; State2<=State2_S1; end
								32:  begin Parity[5]<=Parity[5]+1; State2<=State2_S1; end
								16:  begin Parity[4]<=Parity[4]+1; State2<=State2_S1; end
								8:   begin Parity[3]<=Parity[3]+1; State2<=State2_S1; end
								4:   begin Parity[2]<=Parity[2]+1; State2<=State2_S1; end
								2:   begin Parity[1]<=Parity[1]+1; State2<=State2_S1; end
								1:   begin Parity[0]<=Parity[0]+1; State2<=State2_S1; end
								default:begin   //cannot find the error bit, discard the block
									TC_Packet_Counter<=TC_Packet_Counter+1;
									TC_Error_Counter<=TC_Error_Counter+1;
									Shift_reg<=56'h0000_0000_0000_00;
									State2<=State2_S0;
									State1<=State1_S1;
									Block_ErrO<=1;
									Flag_RX_ing<=0;
									Start_Buff<=16'h00;
									block_counter<=0;
                                    Syn_reg<=7'h00;
                                    Parity<=7'h00;
                                    Bit_Counter<=0; 
                                    Parity_Counter<=0;
                                    Flag_once<=0;
                                    Start_Sequence_counter<=4'h0;
									end//end default 
								endcase
							end//end if(Syn_reg!=0)
						else begin 						
							State2<=State2_S1;   //Error-free, jump to State2_S1(send data) 
							end //end else if(Syn_reg!=0)
						Bit_Counter<=0; 
						Parity_Counter<=0;
						Syn_reg<=7'h00;
						Parity<=7'h00;
						end//
					end//end State1_S2
				default:begin   //default 
					State1<=State1_S1;
					Shift_reg<=56'h0000_0000_0000_00;
					Bit_Counter<=6'h00;
					Parity<=0;
					end//
			endcase 
			case(State2)
				State2_S0:begin
					Enable<=0;			  
					Shift_Bit_Counter<=0;
					end// end  State_S2
				State2_S1:begin
					if(Shift_Bit_Counter<56)begin
						Enable<=1;
						DataO<=Shift_reg[55-Shift_Bit_Counter];  //send Data[55:0]
						Shift_Bit_Counter<=Shift_Bit_Counter+1;
						end //end State2_S1;
					else begin
						Enable<=0;
						DataO<=0;
						State2<=State2_S2;  //jump to State2_S2(finish)
						Shift_Bit_Counter<=0;
						end //end else if(Shift_Bit_Counter<53)
					end//State2_S1
				State2_S2:begin
					State2<=State2_S0;    //jump to State2_S0(wait to send data)
					DataO<=0;
					end
				default:begin 
					State2<=State2_S0;
					Shift_Bit_Counter<=0;
				  end //end default 
			endcase
		end//else if(!Rst)
	end
end //always 

//<statements>

endmodule

