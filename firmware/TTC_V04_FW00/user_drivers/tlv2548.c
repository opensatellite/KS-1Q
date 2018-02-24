#include "tlv2548.h"


//#define 
void delay(uint32_t c)
{
    while(c--) asm("nop");
}




 void tlv2548_checkint(void)
{
    while(ADC_INT);
}

uint16_t tlv2548_wrcfg(uint16_t data)
{
  
        uint16_t temp;
	
	ADC0_CS_L();

	temp =spi_rw(spi0, data);
	
	ADC0_CS_H();
        
        temp&=0x0fff;

	return temp;
}

uint16_t tlv2548_rchannel(uint16_t channel)
{           
        uint16_t temp=0;
        if(channel<8){
          ADC0_CS_L();
	  temp=spi_rw(spi0, channel<<12);
	  ADC0_CS_H(); 
        }
        return temp;
}


void tlv2548_init()
{

	ADC0_CS_H();
        tlv2548_wrcfg(WCFR);
        delay(100);
        while(tlv2548_wrcfg(RCFR)!=(WCFR&0x0fff)){
                  delay(100);
	tlv2548_wrcfg(WCFR);
                delay(100);
        }
        
}

