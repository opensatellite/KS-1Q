//ADC driver for ECG application on STM32 --header
//ECG_ADC.h

#ifndef _STM32_ADC_H__
#define _STM32_ADC_H__

#include "datatypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
//VIN_ADC = supply voltage measure
//IN16 = cpu temperature sensor    
//IN17 = internal reference
    
#define ADC_TEMP_SENSOR    16
#define ADC_INTREF         17
#define ADC_MAX 4095

extern float adc_gain;
#define ADC_V(VAL) (adc_gain*(VAL))

typedef struct {
	uint8_t ch;
	uint8_t sample_time;
}adc_channel_t;

void adc_init(adc_channel_t cfg[], int n, bool continuous);
void adc_deinit();
uint16_t adc_read(int id);
void adc_trig();
void adc_calibration();

#ifdef __cplusplus
}
#endif

#endif
