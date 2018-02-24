`timescale 1 ns/100 ps
// Version: v11.7 11.7.0.119


module test_4463andcpuopration_COREEDAC_0_actram(
       DI,
       DO,
       WADDR,
       RADDR,
       WRB,
       RDB,
       WCLOCK,
       RCLOCK
    );
input  [12:0] DI;
output [12:0] DO;
input  [9:0] WADDR;
input  [9:0] RADDR;
input  WRB;
input  RDB;
input  WCLOCK;
input  RCLOCK;

    wire VCC, GND, ADLIB_VCC;
    wire GND_power_net1;
    wire VCC_power_net1;
    assign GND = GND_power_net1;
    assign VCC = VCC_power_net1;
    assign ADLIB_VCC = VCC_power_net1;
    
    RAM1K18 test_4463andcpuopration_COREEDAC_0_actram_R0C0 (.A_DOUT({
        nc0, nc1, nc2, nc3, nc4, DO[12], DO[11], DO[10], DO[9], DO[8], 
        DO[7], DO[6], DO[5], DO[4], DO[3], DO[2], DO[1], DO[0]}), 
        .B_DOUT({nc5, nc6, nc7, nc8, nc9, nc10, nc11, nc12, nc13, nc14, 
        nc15, nc16, nc17, nc18, nc19, nc20, nc21, nc22}), .BUSY(), 
        .A_CLK(RCLOCK), .A_DOUT_CLK(VCC), .A_ARST_N(VCC), .A_DOUT_EN(
        VCC), .A_BLK({RDB, VCC, VCC}), .A_DOUT_ARST_N(VCC), 
        .A_DOUT_SRST_N(VCC), .A_DIN({GND, GND, GND, GND, GND, GND, GND, 
        GND, GND, GND, GND, GND, GND, GND, GND, GND, GND, GND}), 
        .A_ADDR({RADDR[9], RADDR[8], RADDR[7], RADDR[6], RADDR[5], 
        RADDR[4], RADDR[3], RADDR[2], RADDR[1], RADDR[0], GND, GND, 
        GND, GND}), .A_WEN({GND, GND}), .B_CLK(WCLOCK), .B_DOUT_CLK(
        VCC), .B_ARST_N(VCC), .B_DOUT_EN(VCC), .B_BLK({WRB, VCC, VCC}), 
        .B_DOUT_ARST_N(GND), .B_DOUT_SRST_N(VCC), .B_DIN({GND, GND, 
        GND, GND, GND, DI[12], DI[11], DI[10], DI[9], DI[8], DI[7], 
        DI[6], DI[5], DI[4], DI[3], DI[2], DI[1], DI[0]}), .B_ADDR({
        WADDR[9], WADDR[8], WADDR[7], WADDR[6], WADDR[5], WADDR[4], 
        WADDR[3], WADDR[2], WADDR[1], WADDR[0], GND, GND, GND, GND}), 
        .B_WEN({VCC, VCC}), .A_EN(VCC), .A_DOUT_LAT(VCC), .A_WIDTH({
        VCC, GND, GND}), .A_WMODE(GND), .B_EN(VCC), .B_DOUT_LAT(VCC), 
        .B_WIDTH({VCC, GND, GND}), .B_WMODE(GND), .SII_LOCK(GND));
    GND GND_power_inst1 (.Y(GND_power_net1));
    VCC VCC_power_inst1 (.Y(VCC_power_net1));
    
endmodule
