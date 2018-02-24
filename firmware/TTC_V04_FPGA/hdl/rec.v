///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: rec.v
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

module rec( ClkI, DataI, EnI ,Block_Err);
input ClkI, DataI, EnI,Block_Err;
reg [55:0]Shift_reg=56'h0000_0000_0000_00;
reg [7:0]Bit_Cout=8'h00;



always@(posedge ClkI)
begin
  if(EnI)begin 
    if(Bit_Cout<56)begin
      Shift_reg[55-Bit_Cout]<=DataI;
      Bit_Cout<=Bit_Cout+1;
      end//if(Bit_Cout<56)
    else begin 
      Bit_Cout<=0;
      Shift_reg<=56'h0000_0000_0000_00;
      end//end else if()
    end//end if(EnI) 
else begin 
      Bit_Cout<=0;
      Shift_reg<=56'h0000_0000_0000_00;

end

end


//<statements>

endmodule

