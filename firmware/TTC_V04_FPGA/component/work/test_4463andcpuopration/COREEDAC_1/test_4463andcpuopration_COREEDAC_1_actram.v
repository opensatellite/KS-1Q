`timescale 1 ns/100 ps
// Version: v11.7 11.7.0.119


module test_4463andcpuopration_COREEDAC_1_actram(
       DI,
       DO,
       WADDR,
       RADDR,
       WRB,
       RDB,
       WCLOCK,
       RCLOCK
    );
input  [38:0] DI;
output [38:0] DO;
input  [3:0] WADDR;
input  [3:0] RADDR;
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
    
    RAM1K18 test_4463andcpuopration_COREEDAC_1_actram_R0C1 (.A_DOUT({
        nc0, nc1, nc2, nc3, nc4, nc5, nc6, nc7, nc8, nc9, nc10, nc11, 
        nc12, nc13, nc14, nc15, nc16, nc17}), .B_DOUT({nc18, nc19, 
        nc20, nc21, nc22, nc23, nc24, nc25, nc26, nc27, nc28, nc29, 
        nc30, nc31, nc32, DO[38], DO[37], DO[36]}), .BUSY(), .A_CLK(
        RCLOCK), .A_DOUT_CLK(VCC), .A_ARST_N(VCC), .A_DOUT_EN(VCC), 
        .A_BLK({RDB, VCC, VCC}), .A_DOUT_ARST_N(VCC), .A_DOUT_SRST_N(
        VCC), .A_DIN({GND, GND, GND, GND, GND, GND, GND, GND, GND, GND, 
        GND, GND, GND, GND, GND, GND, GND, GND}), .A_ADDR({GND, GND, 
        GND, GND, GND, RADDR[3], RADDR[2], RADDR[1], RADDR[0], GND, 
        GND, GND, GND, GND}), .A_WEN({VCC, VCC}), .B_CLK(WCLOCK), 
        .B_DOUT_CLK(VCC), .B_ARST_N(VCC), .B_DOUT_EN(VCC), .B_BLK({WRB, 
        VCC, VCC}), .B_DOUT_ARST_N(VCC), .B_DOUT_SRST_N(VCC), .B_DIN({
        GND, GND, GND, GND, GND, GND, GND, GND, GND, GND, GND, GND, 
        GND, GND, GND, DI[38], DI[37], DI[36]}), .B_ADDR({GND, GND, 
        GND, GND, GND, WADDR[3], WADDR[2], WADDR[1], WADDR[0], GND, 
        GND, GND, GND, GND}), .B_WEN({VCC, VCC}), .A_EN(VCC), 
        .A_DOUT_LAT(VCC), .A_WIDTH({VCC, GND, VCC}), .A_WMODE(GND), 
        .B_EN(VCC), .B_DOUT_LAT(VCC), .B_WIDTH({VCC, GND, VCC}), 
        .B_WMODE(GND), .SII_LOCK(GND));
    RAM1K18 test_4463andcpuopration_COREEDAC_1_actram_R0C0 (.A_DOUT({
        DO[35], DO[34], DO[33], DO[32], DO[31], DO[30], DO[29], DO[28], 
        DO[27], DO[26], DO[25], DO[24], DO[23], DO[22], DO[21], DO[20], 
        DO[19], DO[18]}), .B_DOUT({DO[17], DO[16], DO[15], DO[14], 
        DO[13], DO[12], DO[11], DO[10], DO[9], DO[8], DO[7], DO[6], 
        DO[5], DO[4], DO[3], DO[2], DO[1], DO[0]}), .BUSY(), .A_CLK(
        RCLOCK), .A_DOUT_CLK(VCC), .A_ARST_N(VCC), .A_DOUT_EN(VCC), 
        .A_BLK({RDB, VCC, VCC}), .A_DOUT_ARST_N(VCC), .A_DOUT_SRST_N(
        VCC), .A_DIN({DI[35], DI[34], DI[33], DI[32], DI[31], DI[30], 
        DI[29], DI[28], DI[27], DI[26], DI[25], DI[24], DI[23], DI[22], 
        DI[21], DI[20], DI[19], DI[18]}), .A_ADDR({GND, GND, GND, GND, 
        GND, RADDR[3], RADDR[2], RADDR[1], RADDR[0], GND, GND, GND, 
        GND, GND}), .A_WEN({VCC, VCC}), .B_CLK(WCLOCK), .B_DOUT_CLK(
        VCC), .B_ARST_N(VCC), .B_DOUT_EN(VCC), .B_BLK({WRB, VCC, VCC}), 
        .B_DOUT_ARST_N(VCC), .B_DOUT_SRST_N(VCC), .B_DIN({DI[17], 
        DI[16], DI[15], DI[14], DI[13], DI[12], DI[11], DI[10], DI[9], 
        DI[8], DI[7], DI[6], DI[5], DI[4], DI[3], DI[2], DI[1], DI[0]})
        , .B_ADDR({GND, GND, GND, GND, GND, WADDR[3], WADDR[2], 
        WADDR[1], WADDR[0], GND, GND, GND, GND, GND}), .B_WEN({VCC, 
        VCC}), .A_EN(VCC), .A_DOUT_LAT(VCC), .A_WIDTH({VCC, GND, VCC}), 
        .A_WMODE(GND), .B_EN(VCC), .B_DOUT_LAT(VCC), .B_WIDTH({VCC, 
        GND, VCC}), .B_WMODE(GND), .SII_LOCK(GND));
    GND GND_power_inst1 (.Y(GND_power_net1));
    VCC VCC_power_inst1 (.Y(VCC_power_net1));
    
endmodule
