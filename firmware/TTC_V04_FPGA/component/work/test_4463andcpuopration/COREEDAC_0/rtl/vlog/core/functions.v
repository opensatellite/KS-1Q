// ***************************************************************************/
//Actel Corporation Proprietary and Confidential
//Copyright 2008 Actel Corporation. All rights reserved.
//
//ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
//ACCORDANCE WITH THE ACTEL LICENSE AGREEMENT AND MUST BE APPROVED
//IN ADVANCE IN WRITING.
//
//Description:  CoreEDAC Verilog Functions
//              Various RTL Functions
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
//            ______                    _    _
//           |  ____|                  | |  (_)
//           | |__  _   _  _ __    ___ | |_  _   ___   _ __   ___
//           |  __|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|
//           | |   | |_| || | | || (__ | |_ | || (_) || | | |\__ \
//           |_|    \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/

  function [31:0] ceil_log2;
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

  function [31:0] ceil_log3;
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

  // calculate Parity bitwidth based on DAT_WIDTH
  function [31:0] edac_d2p;
    input integer datwidth;
    begin
      if      (datwidth < 5)  edac_d2p = 4;
      else if (datwidth < 12) edac_d2p = 5;
      else if (datwidth < 27) edac_d2p = 6;
      else if (datwidth < 58) edac_d2p = 7;
      else                    edac_d2p = 8;
    end
  endfunction

  // calculate number of 1's in the Hsiao code, i.e. wide XOR width
  function [6:0] xorWidth;
    input integer kk;
    begin
      case (kk)
         4 : xorWidth =  3;
         5 : xorWidth =  3;
         6 : xorWidth =  4;
         7 : xorWidth =  5;
         8 : xorWidth =  5;
         9 : xorWidth =  6;
        10 : xorWidth =  6;
        11 : xorWidth =  7;
        12 : xorWidth =  6;
        13 : xorWidth =  7;
        14 : xorWidth =  7;
        15 : xorWidth =  8;
        16 : xorWidth =  8;
        17 : xorWidth =  9;
        18 : xorWidth =  9;
        19 : xorWidth = 10;
        20 : xorWidth = 10;
        21 : xorWidth = 11;
        22 : xorWidth = 12;
        23 : xorWidth = 13;
        24 : xorWidth = 14;
        25 : xorWidth = 15;
        26 : xorWidth = 15;
        27 : xorWidth = 12;
        28 : xorWidth = 13;
        29 : xorWidth = 13;
        30 : xorWidth = 13;
        31 : xorWidth = 14;
        32 : xorWidth = 14;
        33 : xorWidth = 15;
        34 : xorWidth = 15;
        35 : xorWidth = 15;
        36 : xorWidth = 16;
        37 : xorWidth = 17;
        38 : xorWidth = 18;
        39 : xorWidth = 18;
        40 : xorWidth = 19;
        41 : xorWidth = 20;
        42 : xorWidth = 20;
        43 : xorWidth = 21;
        44 : xorWidth = 22;
        45 : xorWidth = 23;
        46 : xorWidth = 23;
        47 : xorWidth = 24;
        48 : xorWidth = 25;
        49 : xorWidth = 25;
        50 : xorWidth = 26;
        51 : xorWidth = 27;
        52 : xorWidth = 28;
        53 : xorWidth = 28;
        54 : xorWidth = 29;
        55 : xorWidth = 30;
        56 : xorWidth = 30;
        57 : xorWidth = 31;
        58 : xorWidth = 23;
        59 : xorWidth = 23;
        60 : xorWidth = 24;
        61 : xorWidth = 25;
        62 : xorWidth = 25;
        63 : xorWidth = 26;
        64 : xorWidth = 26;
   default : xorWidth =  3;
      endcase
    end
  endfunction

  // At which layer pipe needs to be inserted
  // Due to ModelSim crash make the function return sized number
  function [4:0] doPipeEnc;
    input integer widthXor;
    input integer paramEncPipe;
    input integer whichPipe;
    input integer layers;

    reg[4:0] pipe1, pipe2;
  begin
    pipe1 = 0;
    pipe2 = 0;
    case (layers)
         2  : if (paramEncPipe > 0) pipe1 = 1;
      3, 4  : if (paramEncPipe > 0) pipe1 = 2;
         5  : if (paramEncPipe==1)  pipe1 = 3;
              else
                if (paramEncPipe > 1) begin
                  pipe1 = 2;
                  pipe2 = 4;
                end
    default : begin
                pipe1 = 0;
                pipe2 = 0;
              end
    endcase

    case (whichPipe)
      1 : doPipeEnc = pipe1;
      2 : doPipeEnc = pipe2;
      default: doPipeEnc = 0;
    endcase
  end
  endfunction


  function [4:0] doPipeDec;
    input integer widthXor;
    input integer paramDecPipe;
    input integer whichPipe;
    input integer layers;

    reg[4:0] pipe1, pipe2, syndPipe;
  begin
    pipe1 = 0;
    pipe2 = 0;
    syndPipe = 0;
    case (layers)
      2     : if (paramDecPipe==1) begin
                pipe1 = 2;
                pipe2 = 0;
                syndPipe = 0;
              end
              else
                if (paramDecPipe > 1) begin
                  pipe1 = 1;
                  pipe2 = 0;
                  syndPipe = 1;
                end
      3, 4  : if (paramDecPipe==1) begin
                pipe1 = 3;
                pipe2 = 0;
                syndPipe = 0;
              end
              else
                if (paramDecPipe > 1) begin
                  pipe1 = 2;
                  pipe2 = 0;
                  syndPipe = 1;
                end
      5     : if (paramDecPipe==1) begin
                pipe1 = 3;
                pipe2 = 0;
                syndPipe = 0;
              end
              else
                if (paramDecPipe==2) begin
                  pipe1 = 2;
                  pipe2 = 4;
                  syndPipe = 0;
                end
                else
                  if (paramDecPipe > 2) begin
                    pipe1 = 3;
                    pipe2 = 5;
                    syndPipe = 1;
                  end
    default : begin
                pipe1 = 0;
                pipe2 = 0;
                syndPipe = 0;
              end
    endcase

    case (whichPipe)
      1 : doPipeDec = pipe1;
      2 : doPipeDec = pipe2;
      3 : doPipeDec = syndPipe;
      default: doPipeDec = 0;
    endcase
  end
  endfunction
