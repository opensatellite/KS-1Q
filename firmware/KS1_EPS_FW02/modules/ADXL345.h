#ifndef _adxl345_H__
#define _adxl345_H__

#ifdef __cplusplus
 extern "C" {
#endif
      
#include <stm32f10x.h>
#include <datatypes.h>
     
//#define ADXL345_USE_DRDY  //INT1 or 2 connected, used for data-ready indicator     
     
#define ADXL345_ACC_MILLIG(REGVAL)      (REGVAL * 3.90625f)
     
#ifdef ADXL345_USE_DRDY //forward hook does no effect when there's no data-ready interrupt
bool adxl345_init(uint8_t rate_code,void (*f)(int16_t data[3]));
#else
bool adxl345_init(uint8_t rate_code);
#endif
//code to set data rate (datasheet P.14)
//3200 -> 0x0f, 1600 -> 0x0e, 800 -> 0x0d, 0x0c -> 400sps, 0x0b -> 200sps, 0x0a -> 100sps

void adxl345_deinit(void);

#ifdef ADXL345_USE_DRDY
void __adxl345_isr(void); 
#endif

bool adxl345_dataready(void);//new data ready

uint8_t adxl345_readreg(uint8_t addr);
void adxl345_writereg(uint8_t addr,uint8_t val);
bool adxl345_selftest(void);

#ifdef ADXL345_USE_DRDY
void adxl345_set_forward(void (*f)(int16_t data[3]));
#endif

void adxl345_read(int16_t d[3]);//read immediately
//void adxl345_set_offset(int16 offset[3]);
//void adxl345_set_scale(int16 scale[3]);//out = data*scale/2^15

#ifdef __cplusplus
}
#endif

#endif
