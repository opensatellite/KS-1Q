#include <stdint.h>
#include "ntc.h"
#include "ntc_smd.h"
#include "ntc_thru.h"

/*
	VREF <---RS---> ADC <---NTC---> GND
	Get temperature from ADC value
*/

/*
int8_t ntc_cal_upperrs(uint16_t adc, uint16_t rs, uint16_t r25, uint16_t b) {
	float rt = rs / (ADC_FS / (float)adc - 1.0);
	float t = 1.0 / ( ln( rt / r25 ) / b + 1 / 298.15 );
	if(t < -128.0) return -128;
	else if( t > 127.0) return 127;
	return (int8_t)t;
} 
*/

/*
RT = R25 * exp( B(1/T - 1/298.15) );
T = 1 / ( ln(Rt/R25) / B + 1/298.15)

//for upper rs
ADC = RT / (RS + RT) * ADC_FS;
ADC = 1 / (RS/RT + 1) * ADC_FS;
(RS/RT + 1) = ADC_FS / ADC;

Rt = RS / (ADC_FS / ADC - 1);

T = 1 / ( ln(RS / (ADC_FS / ADC - 1) / R25) / B + 1/298.15 )

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

int8_t ntc_cal_ext(uint16_t x) {
	return 0;
}
