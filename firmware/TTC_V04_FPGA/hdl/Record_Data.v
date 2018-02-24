///////////////////////////////////////////////////////////////////////////////////////////////////
// Company: <Name>
//
// File: Record_Data.v
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

module Record_Data( clk, en, datai,rst );
input clk, en, rst ,datai;

reg [8287:0]data;
reg [10:0]bit_count;
integer write_data,write_data1,write_data2;
reg [15:0]bit;
//initial begin
//
  //
    ////$fdisplay(write_data,"%b,data[0]",datai);
    ////$fclose(write_data);
//end



always@(posedge clk or posedge rst)begin 
if(rst)begin 
bit_count<={11{1'b0}};
data<={8288{1'b0}};
bit<=0;
end 
else begin 
  if(en)begin
    data[0]<=datai;
    data[8287:1]<=data[8286:0];
    bit_count<=1;
    bit<=bit+1;
    end
   //else if(bit_count==1&&!en)begin
    //bit_count<=2;
    //data[0]<=datai;
    //data[4145:1]<=data[4144:0];
    //end
  //else if(bit_count==4) begin 
    //bit_count<=3;
    //end
  //else if(bit_count==2) begin
  else begin
//    bit_count<=4;
    write_data=$fopen("data_save.txt","w");
    write_data1=$fopen("data_save1.bin","w");
    write_data2=$fopen("data_save2.txt","w");
    $fdisplay(write_data2,"%h",data);
    $fdisplay(write_data,"%b",data);
    $fdisplay(write_data1,"%b",data);
    $fclose(write_data);
    $fclose(write_data1);
    $fclose(write_data2);

    end
  end
end

//<statements>

endmodule

