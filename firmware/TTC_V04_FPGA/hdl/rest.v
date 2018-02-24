///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: rest.v
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

module rest(clk , adc, WD_RES,SYSTEM_RST,Flag_OF,time_clear);
input clk,SYSTEM_RST,Flag_OF,WD_RES,time_clear;
output reg adc;

reg Flag_OF_out,WD_RES_out,time_clear_out;
 reg Flag_Once;

always@(posedge clk or negedge SYSTEM_RST )begin 
    if(!SYSTEM_RST )begin
        Flag_OF_out<=0;
        WD_RES_out<=0;
        time_clear_out<=1;
        adc<=1;
        Flag_Once<=0;
        end
    else if(!time_clear||WD_RES||Flag_OF)begin
            adc<=0;
            end
    else if(!Flag_Once)begin 
        adc<=0;
        Flag_Once<=1;
        end
    else begin 

            adc<=1;
          
        end
end

//<statements>

endmodule

