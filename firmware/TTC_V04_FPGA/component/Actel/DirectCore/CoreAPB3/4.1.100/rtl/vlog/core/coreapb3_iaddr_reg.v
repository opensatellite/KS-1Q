// ********************************************************************/
// Microsemi Corporation Proprietary and Confidential
// Copyright 2011 Microsemi Corporation.  All rights reserved.
//
// ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
// ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
// IN ADVANCE IN WRITING.
//
// Description:	Implements indirect address registers for CoreAPB3
//
//
// SVN Revision Information:
// SVN $Revision: 24054 $
// SVN $Date: 2014-12-08 10:43:40 +0000 (Mon, 08 Dec 2014) $
//
//
// Notes:
// 1. best viewed with tabstops set to "4" (tabs used throughout file)
//
// *********************************************************************/
module coreapb3_iaddr_reg (
    PCLK,
    PRESETN,
    PENABLE,
    PSEL,
    PADDR,
    PWRITE,
    PWDATA,
    PRDATA,
    IADDR_REG
    );
	parameter               SYNC_RESET = 0;
    parameter [5:0]         APB_DWIDTH = 32;    
    parameter [5:0]         MADDR_BITS = 32;
    input                   PCLK;
    input                   PRESETN;
    input                   PENABLE;
    input                   PSEL;
    input   [31:0]          PADDR;
    input                   PWRITE;
    input   [31:0]          PWDATA;
    output  [31:0]          PRDATA;
    output  [31:0]          IADDR_REG;
    reg     [31:0]          PRDATA;
    reg     [31:0]          IADDR_REG;
    wire aresetn;
    wire sresetn; 
    assign aresetn = (SYNC_RESET==1) ? 1'b1 : PRESETN;
    assign sresetn = (SYNC_RESET==1) ? PRESETN : 1'b1;
	
    always @(posedge PCLK or negedge aresetn)
    begin
        if ((!aresetn) || (!sresetn))
        begin
            IADDR_REG <= 32'b0;
        end
        else
        begin
            if (PSEL && PENABLE && PWRITE)
            begin
                if (APB_DWIDTH == 32)
                begin
                    if (PADDR[MADDR_BITS-4-1:0] == {MADDR_BITS-4{1'b0}})
                    begin
                        IADDR_REG <= PWDATA;
                    end
                end
                if (APB_DWIDTH == 16)
                begin
                    if (PADDR[MADDR_BITS-4-1:4] == {MADDR_BITS-4-4{1'b0}})
                    begin
                        case (PADDR[3:0])
                            4'b0000: IADDR_REG[15: 0] <= PWDATA[15:0];
                            4'b0100: IADDR_REG[31:16] <= PWDATA[15:0];
                            4'b1000: IADDR_REG        <= IADDR_REG;
                            4'b1100: IADDR_REG        <= IADDR_REG;
                        endcase
                    end
                end
                if (APB_DWIDTH ==  8)
                begin
                    if (PADDR[MADDR_BITS-4-1:4] == {MADDR_BITS-4-4{1'b0}})
                    begin
                        case (PADDR[3:0])
                            4'b0000: IADDR_REG[ 7: 0] <= PWDATA[7:0];
                            4'b0100: IADDR_REG[15: 8] <= PWDATA[7:0];
                            4'b1000: IADDR_REG[23:16] <= PWDATA[7:0];
                            4'b1100: IADDR_REG[31:24] <= PWDATA[7:0];
                        endcase
                    end
                end
            end
        end
    end
    always @(*)
    begin
        PRDATA = 32'b0;
        if (APB_DWIDTH == 32)
        begin
            if (PADDR[MADDR_BITS-4-1:0] == {MADDR_BITS-4{1'b0}})
            begin
                PRDATA = IADDR_REG;
            end
        end
        if (APB_DWIDTH == 16)
        begin
            if (PADDR[MADDR_BITS-4-1:4] == {MADDR_BITS-4-4{1'b0}})
            begin
                case (PADDR[3:0])
                    4'b0000: PRDATA[15:0] = IADDR_REG[15: 0];
                    4'b0100: PRDATA[15:0] = IADDR_REG[31:16];
                    4'b1000: PRDATA       = 32'b0;
                    4'b1100: PRDATA       = 32'b0;
                endcase
            end
        end
        if (APB_DWIDTH ==  8)
        begin
            if (PADDR[MADDR_BITS-4-1:4] == {MADDR_BITS-4-4{1'b0}})
            begin
                case (PADDR[3:0])
                    4'b0000: PRDATA[7:0] = IADDR_REG[ 7: 0];
                    4'b0100: PRDATA[7:0] = IADDR_REG[15: 8];
                    4'b1000: PRDATA[7:0] = IADDR_REG[23:16];
                    4'b1100: PRDATA[7:0] = IADDR_REG[31:24];
                endcase
            end
        end
    end
endmodule
