// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC ECC XOR Gate
//              Layered Wide XOR
//
//Revision Information:
//Date         Description
//08Jan2009    Initial Release
//
//SVN Revision Information:
//SVN $Revision: $
//SVN $Data: $
//
//Resolved SARs
//SAR     Date    Who         Description
//
//Notes:
//

 `timescale 1 ns/100 ps

// One layer of a wide piramidal XOR.
// It has MICRO_W^LOG_WIDTH inputs and MICRO_W^(LOG_WIDTH-1) outputs.
// If the actual number of inputs is different pad it by 0's
// The inputs can be optionally pipelined
//                                                       '0''0'
//      |  |  |         |  |  |         |  |  |        |  |  |
//     -V--V--V-       -V--V--V-       -V--V--V-      -V--V--V-
//     |  XOR  |       |  XOR  |       |  XOR  |      |  XOR  |
//     ----|----       ----|----       ----|----      ----|----
//         V               V               V              V
//     option FF       option FF       option FF      option FF
//     ----|----       ----|----       ----|----      ----|----
//         V               V               V              V

module layer_xor (nGrst, rst, clk, clkEn, inp, outp);
  parameter LOG_WIDTH = 2;
  parameter MICRO_W   = 2;
  parameter PIPE      = 1;
  localparam WIDTHIN  = pow(MICRO_W, LOG_WIDTH);
  localparam WIDTHOUT = pow(MICRO_W, LOG_WIDTH-1);

  input nGrst, rst, clk, clkEn;
  input  [WIDTHIN-1:0] inp;
  output [WIDTHOUT-1:0] outp;

  reg [WIDTHOUT-1:0] layer;
  reg  [WIDTHOUT-1:0] layer_r;
  integer i;

  always @ (inp) begin
    for(i=0; i<WIDTHOUT; i=i+1) begin
      if(MICRO_W==2) layer[i]<= inp[MICRO_W*i] ^ inp[MICRO_W*i+1];
      if(MICRO_W==3) layer[i]<= inp[MICRO_W*i] ^ inp[MICRO_W*i+1] ^
                                                              inp[MICRO_W*i+2];
    end
  end

  generate
    if(PIPE) begin
      always @(posedge clk or negedge nGrst)
        if (nGrst == 1'b0)
          layer_r <= 'b0;
        else
          if (clkEn == 1'b1)
            if(rst) layer_r <= 'b0;
            else    layer_r <= layer;
      assign outp = layer_r;
    end
    else
      assign outp = layer;
  endgenerate

function integer pow;
    input integer base, x;
    integer tmp, res;
  begin
    tmp = 0;
    res = 1;
    while (tmp < x) begin
      tmp = tmp + 1;
      res = res * base;
    end
    pow = res;
  end
endfunction

endmodule


//------------------------------------------------------------------------------
// Combine layers of a wide XOR to form a piramid
module wide_xor (nGrst, clk, rst, clkEn, inp, outp);
  parameter WIDTH   = 2;
  parameter MICRO_W = 2;
  // layer number where a pipe needs to be inserted
  parameter PIPE1 = -1;   // -1 no pipe
  parameter PIPE2 = -1;   // -1 no pipe
  // number of layers
  localparam LAYERS = MICRO_W==2 ? ceil_log2(WIDTH) : ceil_log3(WIDTH);

  input nGrst, rst, clk, clkEn;
  input[WIDTH-1:0] inp;
  output outp;

  wire [pow(MICRO_W, LAYERS)-1:0] layer_w [0:LAYERS];
  wire [pow(MICRO_W, LAYERS)-1:0] temp;

  assign layer_w[0] = inp;
  assign temp = layer_w[LAYERS];
  assign outp = temp[0];

  generate
    genvar i;
    for(i=0; i<LAYERS; i=i+1)
      begin: genXOR
        layer_xor # (.LOG_WIDTH(LAYERS-i),
                     .MICRO_W(MICRO_W),
                     .PIPE((i==PIPE1)|(i==PIPE2)) ) layer_xor_0 (
                 .nGrst(nGrst), .rst(rst), .clk(clk), .clkEn(clkEn),
                 .inp(layer_w[i][pow(MICRO_W, LAYERS-i)-1:0]),
                 .outp(layer_w[i+1][pow(MICRO_W, LAYERS-1-i)-1:0])  );
      end
  endgenerate

function integer ceil_log2;
    input integer x;
    integer tmp, res;
  begin
    tmp = 1;
    res = 0;
    while (tmp < x) begin
      tmp = tmp * 2;
      res = res + 1;
    end
    ceil_log2 = res;
  end
endfunction

function integer ceil_log3;
    input integer x;
    integer tmp, res;
  begin
    tmp = 1;
    res = 0;
    while (tmp < x) begin
      tmp = tmp * 3;
      res = res + 1;
    end
    ceil_log3 = res;
  end
endfunction

// x power of base = base^x
function integer pow;
    input integer base, x;
    integer tmp, res;
  begin
    tmp = 0;
    res = 1;
    while (tmp < x) begin
      tmp = tmp + 1;
      res = res * base;
    end
    pow = res;
  end
endfunction

endmodule
