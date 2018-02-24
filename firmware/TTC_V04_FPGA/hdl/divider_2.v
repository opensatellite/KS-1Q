
module divider_2
	(
	 clk,
	 reset,
	 out
	);

input clk;
input reset;
output out;

parameter TOP = 1;
parameter B = 1;

reg [B:0] cnt;
wire [B:0] cnt_next;

assign cnt_next = (cnt == TOP) ? (0) : (cnt + 1);
assign out = (cnt == TOP) ? (1) : (0);

always @(posedge (clk) or posedge reset)
if(reset) begin
	cnt <= TOP;
end else begin
	cnt <= cnt_next;
end

endmodule
