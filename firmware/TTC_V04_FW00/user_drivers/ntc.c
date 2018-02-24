#include <stdint.h>
#include "ntc.h"
#include "ntc_smd.h"
#include "ntc_thru.h"
#include "ntc_ttc.h"

/*
	 VREF <---RS---> ADC <---NTC---> GND
FOR TTC: VREF(3V3) <---NTC---> ADC(2VREF) <---RS---> GND
	 Get temperature from ADC value
*/

int8_t ntc_cal_smd(uint16_t x) {
	int8_t left = lut_smd[x>>5];
	int8_t delta = lut_smd[(x>>5)+1] - left;
	uint8_t rem = ((uint8_t)x) & 0x1f;
	uint8_t dx = (delta * rem) >> 5;
	return left+dx;
} 

int8_t ntc_cal_thru(uint16_t x) {
	int8_t left = lut_thru[x>>5];
	int8_t delta = lut_thru[(x>>5)+1] - left;
	uint8_t rem = ((uint8_t)x) & 0x1f;
	uint8_t dx = (delta * rem) >> 5;
	return left+dx;
}

int8_t ntc_cal_ttc(uint16_t x) {
	int8_t left = lut_ttc[x>>5];
	int8_t delta = lut_ttc[(x>>5)+1] - left;
	uint8_t rem = ((uint8_t)x) & 0x1f;
	uint8_t dx = (delta * rem) >> 5;
	return left+dx;
}

int8_t ntc_cal_ext(uint16_t x) {
	return 0;
}
