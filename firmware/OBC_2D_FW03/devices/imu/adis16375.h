#ifndef _ADIS16375_H__
#define _ADIS16375_H__

/*
    Now IMU's SYNC function is enabled, and the program is designed 
    for INTERRUPT READ METHOD

    To change output data rate, change SYNC clock frequency by changing variable
    "outputfreq" in adis16375.c and set "decim" in adis16375_init() to 1
    
    On SYNC clock rising edge, the IMU read 4 samples and average them together
    as output, so SYNC clock freq must equal to DATA RATE YOU WANT

    if you disable SYNC input, output rate will be 9.84ksps / 4 / decim
    but you neet to disable SYNC input in ADIS16375_init()

    search keyword      ***config      for all config points in adis16375.c
*/

#include "os.h"
#include "AHRS/3dmath_helper.h"

#ifdef __cplusplus
 extern "C" {
#endif

//data types
typedef struct
{
    uint8_t tm_sec;       /* Seconds: 0-59 */
    uint8_t tm_min;       /* Minutes: 0-59 */
    uint8_t tm_hour;      /* Hours since midnight: 0-23 */
    uint8_t tm_mday;      /* Day of the month: Day of the month: 1-31 */
    uint8_t tm_mon;       /* Months *since* january: 0-11 */
    uint8_t tm_year;      /* Years since 1900 */
    uint8_t tm_isdst;       /* +1 Daylight Savings Time, 0 No DST*/
}adis16375_time;

//basic functions

#define ADIS16375_OK         0
//init parameters error 
#define ADIS16375_ERR_PARAM  (1<<31)    //init parameters error
#define ADIS16375_ERR_WIRE   (1<<30)    //wire connection error
//self-test errors 
#define ADIS16375_ERR_ACCZ   (1<<21)
#define ADIS16375_ERR_ACCY   (1<<20)
#define ADIS16375_ERR_ACCX   (1<<19)
#define ADIS16375_ERR_GYROZ  (1<<18)
#define ADIS16375_ERR_GYROY  (1<<17)
#define ADIS16375_ERR_GYROX  (1<<16)
//system status
#define ADIS16375_ERR_WDT    (1<<15)    //watchdog
#define ADIS16375_OVERRUN    (1<<7)    //processing overrun
#define ADIS16375_ERR_FLASH  (1<<6)    //flash memory error
#define ADIS16375_ERR_STS    (1<<5)    //self-test error
#define ADIS16375_OVERFLOW   (1<<4)    //sensor overrange
#define ADIS16375_ERR_SPI    (1<<3)    //spi comm error
#define ADIS16375_ALARM      (1)    //alarm occurred

typedef struct {
    spi_handle_t *spi;
    pin sync;
    pin rst;
    pin irq;
    pin cs;
    timer_channel sync_pwmchannel;
    uint16_t decim;
}adis16375_config_t;

uint32_t adis16375_init(adis16375_config_t *cfg);
//(*recv_ok)() will be called after new data arrived
//acc[],gyro[] is a pointer to where the data store
//decim is the data decimation rate, 1 for original, 2 for half speed, to 2048 max
//return 0 for init ok, > 0 for init error 

//set module to sleep mode
void adis16375_deinit(void);

/*
	To R/W register, do not call adis16375_start(), 
	call set_page(), read_reg(), write_reg() JUST AFTER
	adis16375_init() executed
*/
//Set Register Page
void adis16375_set_page(uint8_t x);
//Read Register
uint16_t adis16375_read_reg(uint8_t addr);
//Write Register
void adis16375_write_reg(uint8_t addr,uint16_t data);

//filter selection
#define ADIS16375_FILTER_GYROX  (1<<2)
#define ADIS16375_FILTER_GYROY  (1<<5)
#define ADIS16375_FILTER_GYROZ  (1<<8)
#define ADIS16375_FILTER_ACCX   (1<<11)
#define ADIS16375_FILTER_ACCY   (1<<14)
#define ADIS16375_FILTER_ACCZ   (1<<18) //remap to 0x18
#define ADIS16375_FILTER_ALL    (0x00044924)
//banks = {gyrox,gyroy,gyroz,accx,accy,accz}
//bank A=0,B=1,C=2,D=3
void adis16375_filter(uint32_t options,uint8_t banks[6]);

//start using INTERRUPT READ METHOD
void adis16375_start(void);
//stop using INTERRUPT READ METHOD
void adis16375_stop(void);

#define ADIS16375_GYRO_DPS_I16(INT16) ((float)(INT16)*0.0131072)
#define ADIS16375_GYRO_RAD_I16(INT16) ((float)(INT16)*2.2876379571740053049e-4)
#define ADIS16375_GYRO_DPS_I32(INT32) ((float)(INT32)*2e-7)
#define ADIS16375_GYRO_RAD_I32(INT32) ((float)(INT32)*3.49065850398866e-9)
#define ADIS16375_ACC_G_I16(INT16) ((float)(INT16)*0.008192)
#define ADIS16375_ACC_G_I32(INT32) ((float)(INT32)*1.25e-8)
#define ADIS16375_TEMP(INT16) ((float)(INT16)*0.00565+25.0)
uint16_t adis16375_get_raw16(vec3s *acc,vec3s *gyro,int16_t *temp);//take ~25us
uint16_t adis16375_get_raw32(vec3l *acc,vec3l *gyro,int16_t *temp);//take ~50us
uint16_t adis16375_get_output(vec3f *acc,vec3f *gyro,float *temp);//take ~27us
//WARNING: INTERRUPT READ METHOD only
//acc unit in g, gyro unit in rad/s

//detect refreshing status
bool adis16375_refreshing(void);//return true -> refreshing data

adis16375_time adis16375_get_time(void);//read time
void adis16375_set_time(adis16375_time *t);//set time

//connect these functions to system signels
void adis16375_process(void);//call it every 1ms
void adis16375_DRDY_ISR(void);//data ready pin rising edge interrupt
void adis16375_SPI_ISR(void);//SPI receive interrupt

//advanced functions
//these functions cannot be used while reading data continuously

//calibration section
#define ADIS16375_BIAS_ACCZ     (1<<13)
#define ADIS16375_BIAS_ACCY	(1<<12)
#define ADIS16375_BIAS_ACCX	(1<<11)
#define ADIS16375_BIAS_GYROZ	(1<<10)
#define ADIS16375_BIAS_GYROY	(1<<9)
#define ADIS16375_BIAS_GYROX	(1<<8)
void adis16375_bias_null(uint16_t sensor,uint8_t timebase);
//generate offset correction data 
//average for 64 samples, total time = 64 * 2 ^ timebase / 2460
void adis16375_bias_to_reg(void);
//write offset correction data to register

//filter config
//void adis16375_filter_select(uint16_t ... 

//WARNING! TO PREVENT TOO MUCH WRITE TO INTERNAL FLASH, 
//IF IT IS NOT NECESSERY DO NOT USE:
//1. factory calibration restore
//2. flash memory update
void adis16375_factory(void);//factory calibration restore
void adis16375_write_to_flash(void);//flash memory update

uint32_t adis16375_get_flash_write_cnt(void);//read flash write counter FLSH_CNT_L/H
void adis16375_get_lot_id(uint16_t seq[3]);//read device lot id

#ifdef __cplusplus
}
#endif

#endif
