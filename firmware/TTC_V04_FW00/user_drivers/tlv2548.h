#ifndef TLV2548_H
#define TLV2548_H
#include "bsp.h"
#include "si4463.h"

#define Channel(x)	(x<<12)


#define RCFR		0x9000
#define WCFR		0xAc00
#define RFIFO		0xE000
#define SWPD		0x8000


typedef struct
{
	uint8_t	channel;
	uint16_t var;
}adc_result_s;


void tlv2548_init(void);
uint16_t  tlv2548_rchannel(uint16_t channel);
void tlv2548_checkint(void );


#endif
