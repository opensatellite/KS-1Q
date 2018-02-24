module RXRandomize(Clk, Rst, EnI, DataI, Block_ErrI, IP_END ,DataO, EnO, Block_ErrO, IP_END_O);
input Clk, Rst, EnI, DataI, IP_END, Block_ErrI;
output DataO, EnO, Block_ErrO, IP_END_O;

reg DataO, EnO, Block_ErrO, IP_END_O;
reg[7:0] Ran_SR;
reg[15:0] Start_Buff;

reg Flag_once=0;

parameter start_sequence=16'heb90;

always@(posedge Clk or negedge Rst) begin
	IP_END_O<=IP_END;
    Block_ErrO<=Block_ErrI;
	if(!Rst)begin
		Ran_SR<=8'b1111_1111;
		Start_Buff<=0;
		Flag_once<=0;
		DataO<=0;
		IP_END_O<=0;
		EnO<=0;
		Block_ErrO<=0;
		end
	else if(Block_ErrI)begin
		Ran_SR<=8'b1111_1111;
		Start_Buff<=0;
		Flag_once<=0;
		DataO<=0;
		IP_END_O<=0;
		EnO<=0;
		end
	else if(IP_END)begin
		Flag_once<=0;
		Start_Buff<=0;
		end
	else begin
		
		if(start_sequence!=Start_Buff)begin 		//receive and match start_sequence
        Start_Buff[15:1]<=Start_Buff[14:0];
        Start_Buff[0]<=DataI;
        end 
		else if(!Flag_once) begin
			Ran_SR<=8'b1111_1111;
			Flag_once<=1;
			end
		else if(EnI)begin
			DataO<=DataI ^ Ran_SR[0];

			EnO<=1;
			Ran_SR[0]<=Ran_SR[1];
			Ran_SR[1]<=Ran_SR[2];
			Ran_SR[2]<=Ran_SR[3];
			Ran_SR[3]<=Ran_SR[4];
			Ran_SR[4]<=Ran_SR[5];
			Ran_SR[5]<=Ran_SR[6];
			Ran_SR[6]<=Ran_SR[7];
			Ran_SR[7]<=Ran_SR[0] ^ Ran_SR[1] ^ Ran_SR[2] ^ Ran_SR[3] ^ Ran_SR[4] ^ Ran_SR[6];
			end
		else begin
			DataO<=0;
			EnO<=0;
			end
		end
	end
endmodule