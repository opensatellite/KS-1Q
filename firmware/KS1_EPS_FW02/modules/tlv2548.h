#ifndef __TLV2548_H
#define __TLV2548_H

#include "pin.h"
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif
  
    typedef struct {
	spi_handle_t *spi;
	pin cs;			//20pin CS
	pin cstart;		//14pin START
        //pin fs;		    //17pin START
        pin eocint;     //4pin EOC/INT
    }tlv2548_handle_t;
    
    typedef struct {
        bool refs;
        bool refvs;
        bool sclk;
        bool clk1;
        bool clk2;
        bool mode1;
        bool mode2;
        bool swset1;
        bool swset2;
        bool eiset;
        bool fifo1;
        bool fifo2;
	//CFR
    }tlv2548_conversion_list_t;
    
    typedef struct {
	spi_handle_t *spi;
        uint16_t spi_divider;   //spi bus frequency divider
	pin cs;			//20pin CS
        //pin fs;		        //17pin START
	//pin pwdn;		//10pin PWDN
	//bool use_pinrst;
	//pin rst;		//11pin RESET
	pin cstart;		//14pin START
	//pin datardy;	        //25pin DRDY
        pin eocint;             //4pin EOC/INT

	bool clkio;//enable clock i/o on CLKIO pin
	bool clk_input;//clk input (true) or clk output (false)
	bool chopping;
	
	
	uint8_t delay;//delay after mux change, 0-7 -> (0,1,2,4,8,16,32,48)*128periods
	uint8_t data_rate;
        /*
        rate averages autoscan fixed
        3    1        23739    125000
        2    4        15123    31250
        1    16       6168     7813
        0    64       1831     1953
        */
	
	tlv2548_conversion_list_t conversion_list;//for autoscan
	uint8_t in_p,in_n;//for fixed mode
	//void (*adc_hook)(int32_t val);
    }tlv2548_config_t;

    bool tlv2548_init(tlv2548_handle_t *p_handle, tlv2548_config_t *cfg);
    int32_t tlv2548_read(tlv2548_handle_t *p_handle, uint8_t *info);

#ifdef __cplusplus
}
#endif

#endif