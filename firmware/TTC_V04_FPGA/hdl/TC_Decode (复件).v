///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: TC_Decode.v
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

module TC_Decode( Clk, Rst, DataI, En_Data, Block_ErrI, MASK, ADDR, IP_END_I, Block_ErrO, SpacecraftID, DataO, En_DataO,IP_END_O);
input Clk,Rst,En_Data,DataI,IP_END_I;
input[15:0] MASK, ADDR;
output DataO,En_DataO,IP_END_O;
input  Block_ErrI;
output Block_ErrO;
output [15:0]SpacecraftID;

reg DataO,En_DataO,IP_END_O;
reg [7:0]Bit_Count;
reg [5:0]Block_Count;
reg Flag_En;
reg [3:0]Type;
reg Block_ErrO;
wire [15:0]SpacecraftID;
reg [23:0]HEAD_BUFFER;
reg Head_error;
//packet header second data
parameter TypeIP=4'b0010;
parameter Version=4'b0000;

assign SpacecraftID=16'h0100;


always@(posedge Clk or negedge Rst)
begin
	IP_END_O<=IP_END_I;
	if(!Rst)begin
		DataO<=0;
		Block_ErrO<=0;
		Bit_Count<=0;
		Block_Count<=0;
		Flag_En<=0;
		En_DataO<=0;
		Type<=0;
		HEAD_BUFFER<=16'h0000;
		IP_END_O<=0;
		Head_error<=0;
		end
	else if((!Block_ErrI)&&(!Head_error))begin
		Block_ErrO<=Block_ErrI;
		DataO<=DataI;
		En_DataO<=En_Data;
		if(En_Data)begin    
	//calculate block 
			if(!Flag_En)begin
				Flag_En<=1;
				Block_Count<=Block_Count+1;
				end//end if(!Flag_En)
			if(Bit_Count<24)begin
				HEAD_BUFFER[0]<=DataI;
				HEAD_BUFFER[23:1]<=HEAD_BUFFER[22:0];
				if(Bit_Count==23)begin
                    if(({HEAD_BUFFER[22:0],DataI}=={SpacecraftID,TypeIP,Version})||({HEAD_BUFFER[22:0],DataI}=={(MASK&&ADDR),TypeIP,Version}))begin
						Head_error<=0;
						Bit_Count<=24;
						end
					else begin
						Bit_Count<=0;
						Head_error<=1;
						end //end else if(SpacecraftID[15-Bit_Count]==DataI)
					end//end if(Bit_Count==15)begin
				Bit_Count<=Bit_Count+1;
				end//if(Bit_Count<16)
			end//end if(En_Data)
		else begin
			Flag_En<=0;
			if(Block_Count>3)begin  //!!!31->3
				Bit_Count<=0;
				Type<=0;
				HEAD_BUFFER<=24'h0000;
				Block_ErrO<=0;
				Block_Count<=0;
				end//end else if(Type[2]&&(Block_Count>31))
			end//end else if(En_Data)
		end//end if(!Block_ErrI)
	else begin 
		DataO<=0;   //!!!add
		Block_ErrO<=1;
		Block_Count<=0;
		Bit_Count<=0;
		Flag_En<=1'b0;
		En_DataO<=0;
		Type<=0;
		HEAD_BUFFER<=16'h0000;
		Head_error<=0;
		IP_END_O<=0;
		end //end else if(!Block_ErrI)
	end//end always
//<statements>

endmodule

