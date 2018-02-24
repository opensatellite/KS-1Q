
module divider
	(
	 clk,
	 init,
     out_16dec,
	 out_2dec
	);

input clk;
input init;

output out_2dec;
output out_16dec;
wire out_2dec;
wire out_16dec;
reg Bit_Count_2dec;
reg [3:0]Bit_Count_16dec;

always @(posedge clk or negedge init)
begin
if(!init) begin
    Bit_Count_2dec<=0;
    Bit_Count_16dec<=4'h0;
    end 

else begin

          Bit_Count_16dec<=Bit_Count_16dec+1;

          Bit_Count_2dec<=Bit_Count_2dec+1;

        end
end

assign out_16dec = Bit_Count_16dec<8 ? 1 : 0;
assign out_2dec = Bit_Count_2dec ? 1 : 0;

endmodule
