#include "tlv2548.h"

// ADC channal select
#define A0        0x0000
#define A1        0x1000
#define A2        0x2000
#define A3        0x3000
#define A4        0x4000
#define A5        0x5000
#define A6        0x6000
#define A7        0x7000

//
#define SPD       0x8000       //SW power down (analog + reference)
#define RCFR      0x9000       //Read CFR register data
#define WCFR      0xA000       //Write CFR register data
#define RFIFO     0xE000       //FIFO read

//
#define CLK_INT   0x0000       //Conversion clock = SCLK
#define SMODE     0x0000       //Single shot mode

#define sel()		{sys_noirq();pinlow(p_handle->cs);}
#define desel()		{pinhigh(p_handle->cs);sys_irq();}
#define spirw(D)	spi_rw(p_handle->spi,D)


bool tlv2548_init(tlv2548_handle_t *p_handle, tlv2548_config_t *cfg)
{
	p_handle->spi = cfg->spi;
	p_handle->cs = cfg->cs;
	//p_handle->fs = cfg->fs;
	p_handle->cstart = cfg->cstart;
	p_handle->eocint = cfg->eocint;

	//CPOL,1Edge
	spi_init(p_handle->spi, 0, 16, cfg->spi_divider);
	pinmode(p_handle->cs, OUTPUT);
	//pinmode(p_handle->fs, OUTPUT);
	pinmode(p_handle->cstart, OUTPUT);
	pinmode(p_handle->eocint, PULLUP);

	pinhigh(p_handle->cs);//static state of cs
	//pinlow(p_handle->fs);
	pinhigh(p_handle->cstart);
	
	sel();
	spirw(WCFR + 0x80);
	desel();

/*
	// refernece select internal
	// internal ref = 4V
	// short sampling 12 SCLKs
	// coversion clock = SCLK
	// single shot mode
	// pin used as interrupt
	uint16_t cfr = 0x0000;
	if (cfg->conversion_list.refs)
		setb(cfr, 11);
	//setb(cfr, 9);
	//cfr |= (CLK_SCLK << 7);

	// TLV2548 Power Down
        

	sel();
	spirw(WCFR + cfr);
        desel();

        sel();
	spirw(SPD);
        desel();

	uint16_t cfr_temp = 0x0000;
        sel();
	cfr_temp = spirw(RCFR);
        desel();
	cfr_temp &= 0x0FFF;

	if (cfr_temp != cfr)
		return false;

	uint32_t data;
	uint8_t *info;

        while(1)
        {
          data = tlv2548_read(p_handle, info);

          printf("[adc]: %ld\r\n",data);
        }

	return true;
	*/

}

int32_t tlv2548_read(tlv2548_handle_t *p_handle, uint8_t *info)
{
	int16_t val;
        int8_t i;
	//uint8_t* p = (uint8_t *)&val;
      
          sel();
          spirw(A1);
          desel();

         while(pinread(p_handle->eocint));
         //while(!(pinread(p_handle->eocint)));
        
          sel();
          val= (spirw(RFIFO)>>4);
          desel();
        
       
                     
	//val = *info;

	return val;
}