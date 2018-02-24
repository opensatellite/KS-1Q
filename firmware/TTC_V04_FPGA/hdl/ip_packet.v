module ip_packet(Clk, Rst, DataO);
parameter IP_word_size=8;
parameter IP_mem_size=266;
input Clk,Rst;
output DataO;
reg[31:0]bit_count;
reg[8:0]byte_count;
reg[2191:0]Data;
reg[7:0]Data_8bit;
reg DataO;


always@(posedge Rst or posedge Clk)begin
	if(Rst)begin
	//	$readmemh("ip_packet.hex",Data);
		Data_8bit<=8'b0;
		bit_count<=0;
		byte_count<=0;
        Data<=2192'heb90fe39be5b6aea0208f06a8e27a83b55ec04cd5ca7abbf5c30d1d3f870cb20cd02e29d4c20e06c1ccc95f3f129cfff3532776b48973dad8ba27e476dafb6bbefe8fb8f4990cb379c4846c2d93928e1e7305f90f463f6cd8fac336f4c04edbee88069584be0cd27bbcc0ab8421caaa7933492b2252b53ce02ec2e1560e09b2a6fa8fb2aad0304654be01e4096bedebe49b0fd928de665270c1213ebd34e15729a289f6b0088a4042a38757ebfea43fb54bc15d612fe9f427998ac6becbf837a080e37825c8f816ab41ca7fbb8c5786aebe049196d30a226aa0c71bb0270d8565b1ef882799ee48c2084859d49fcd964e1ca3b4262622f8c61dcd9ed26e1abdc7f3ac5c5c5c5c5c5c579;		

    end	
    else begin
        if(bit_count<2192)begin
          DataO<=Data[2191-bit_count];
          bit_count<=bit_count+1;
          end
        else if(bit_count<3192)begin
          DataO<=0;
          bit_count<=bit_count+1;
          end
        else begin 
          bit_count<=0;
          end
	/*	if(byte_count<IP_mem_size-1)begin
			Data_8bit<=Data[byte_count];
			if(bit_count<IP_word_size-1)begin
				DataO<=Data[byte_count][7-bit_count];
				bit_count<=bit_count+1;
				end
			else if(bit_count==IP_word_size-1)begin
				DataO<=Data[byte_count][7-bit_count];
				bit_count<=0;
				byte_count<=byte_count+1;
				end
			end
		else if(byte_count==IP_mem_size-1)begin
			Data_8bit<=Data[byte_count];
			if(bit_count<IP_word_size-1)begin
				DataO<=Data[byte_count][7-bit_count];
				bit_count<=bit_count+1;
				end
			else if(bit_count==IP_word_size-1)begin
				DataO<=Data[byte_count][7-bit_count];
				bit_count<=0;
				byte_count<=0;
				end
			end*/
		end
	end
endmodule
