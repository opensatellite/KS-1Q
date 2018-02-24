///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: RSControl.v
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

module RSControl( ClkI_Dec8, Rst, RFD, RFS/*,CODEOUTP*/,DataI,EnI,CLKEN ,NGRST, R_ST ,START,DATAINP/*,DataO,EnO*/ );
input ClkI_Dec8, Rst, RFD, RFS ,EnI;
input [7:0]/*CODEOUTP,*/DataI;

output CLKEN ,NGRST, R_ST ,START/*,EnO*/;
output [7:0]/*DataO,*/DATAINP;

reg [7:0]DATAINP;
reg [7:0]Data_Buff;
reg START,NGRST;
reg [4:0]State;
reg [7:0]Byte_Count;
reg Flag_Once;
parameter State_S0=5'b00000,
          State_S1=5'b00001,
          State_S2=5'b00010,
          State_S3=5'b00100,
          State_S4=5'b01000,
          State_S5=5'b10000,
          State_S6=5'b10001,
          State_S7=5'b10010;


wire R_ST ;
assign R_ST =0;
assign CLKEN=1;
//assign NGRST=1;


always@(posedge ClkI_Dec8 or negedge Rst)
begin 
if(!Rst)begin 
  State<=5'h00;
  NGRST<=0;
  DATAINP<=0;
  Byte_Count<=0;
  START<=0;
  Data_Buff<=8'h00;
  Flag_Once<=0;
end 
else begin 
  case(State)
    State_S0:begin 
      if(!Flag_Once)begin
        NGRST<=0;
        Flag_Once<=1;
        end
      else begin 
        NGRST<=1;
        end
      if(EnI&&RFS)begin 
        START<=1;
        State<=State_S1;
        //Flag_Once<=0;
        Data_Buff[7:0]<=DataI;
//        Byte_Count<=1;
        end//end if(EnI)
      else begin 
        START<=0;
        Byte_Count<=0;
        end
      end
    State_S1:begin 
      if(RFD)begin 
        START<=0;
        DATAINP<=Data_Buff;
        Data_Buff<=DataI;
        end //end iF(RFD)
      else if(Byte_Count!=224)begin 
        State<=State_S3;//data erro
        end 
      else begin 
        State<=State_S2;//data finish
        end
      Byte_Count<=Byte_Count+1;
      end 
    State_S2:begin 
      if(!RFD)begin 
        State<=State_S0;
        Byte_Count<=0;
        end
      else begin 
        if(Byte_Count>255)State<=State_S3;//RFS erro
        end
      Byte_Count<=Byte_Count+1;
      end
    State_S3:begin 
      Byte_Count<=0;
      State<=State_S0;
      NGRST<=0;
      end
    default:begin 
      State<=State_S3;
      end
    endcase 
end //end else begin 
end //end always 
//<statements>

endmodule

