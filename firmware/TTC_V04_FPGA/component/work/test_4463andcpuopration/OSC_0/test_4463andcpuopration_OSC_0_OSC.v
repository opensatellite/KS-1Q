`timescale 1 ns/100 ps
// Version: v11.7 11.7.0.119


module test_4463andcpuopration_OSC_0_OSC(
       XTL,
       RCOSC_25_50MHZ_CCC,
       RCOSC_25_50MHZ_O2F,
       RCOSC_1MHZ_CCC,
       RCOSC_1MHZ_O2F,
       XTLOSC_CCC,
       XTLOSC_O2F
    );
input  XTL;
output RCOSC_25_50MHZ_CCC;
output RCOSC_25_50MHZ_O2F;
output RCOSC_1MHZ_CCC;
output RCOSC_1MHZ_O2F;
output XTLOSC_CCC;
output XTLOSC_O2F;

    wire N_XTLOSC_CLKOUT, N_XTLOSC_CLKINT;
    
    XTLOSC_FAB I_XTLOSC_FAB (.A(N_XTLOSC_CLKOUT), .CLKOUT(
        N_XTLOSC_CLKINT));
    XTLOSC #( .MODE(2'h3), .FREQUENCY(16.0) )  I_XTLOSC (.XTL(XTL), 
        .CLKOUT(N_XTLOSC_CLKOUT));
    CLKINT I_XTLOSC_FAB_CLKINT (.A(N_XTLOSC_CLKINT), .Y(XTLOSC_O2F));
    
endmodule
