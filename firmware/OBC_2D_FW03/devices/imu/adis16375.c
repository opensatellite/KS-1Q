//ADIS16375 Driver

#include "adis16375.h"

//////////////////////////// Internal Definations///////////////////////////////
//#define USE_READ_WDT

#define ACC_COMPENSATION_FOR_GYROSCOPE  (1<<7)
#define PERCUSSION_ALIGNMENT		(1<<6)

#define GEN_CONFIG    (ACC_COMPENSATION_FOR_GYROSCOPE)	

static uint16_t reg_min_sec,reg_day_hr,reg_yr_mon;
static bool use_dst;

static spi_handle_t* spi;
static pin cs,rst;
//timer_channel sync_pwmchannel;

static volatile int16_t ms_delay_cnt;
//#define __DELAY(T)      ms_delay_cnt=T;while(ms_delay_cnt > 0);
#define __DELAY(T)		osDelay(T)
//Delay(millsecond)

static void delay_nus(int x) {
	while(x--) {
		for(int i = 0; i < 12; i++) asm("nop");
	}
}

////////////////////////////// Hardware dependent///////////////////////////////

#define SYNC_OUT_ON()  pwm_on(sync_pwmchannel)
#define SYNC_OUT_OFF() pwm_off(sync_pwmchannel)
//1kHz Clock output on SYNC Pin

//check and prevent the device lost response
void adis16375_process(void)
{
    if(ms_delay_cnt > 0)
        ms_delay_cnt--;
}

static void interface_init(adis16375_config_t *cfg)
//Config all IO pins, SPI port and timers to generate SYNC clock
{
    pinmode(cfg->sync,PULLUP);
    //pinmode(cfg->irq,PULLUP);
    pinmode(cfg->rst,OUTPUT);
    pinmode(cfg->cs,OUTPUT);
	
	pinmode(PB5,PULLUP);
    
    spi = cfg->spi;
    cs = cfg->cs;
    rst = cfg->rst;
    
    
#ifdef USE_READ_WDT
    read_timeout = 1;
#endif

}

#define spirw(D)    spi_rw(spi,D)
#define spiwait()   spi_wait(spi)

#define SELECT()    {pinlow(cs);}//CS LOW   
#define DESELECT()  pinhigh(cs)//CS HIGH
//Operation on IMU #CS pin

#define RESET_0()   pinlow(rst)
#define RESET_1()   pinhigh(rst)
//Operation on IMU #RESET pin

#define DRDY_GET()  pinread(irq)
//Read IMU DRDY pin status

//////////////////////////// Hardware independent///////////////////////////////

//set page
static void page(uint8_t id)
{
    SELECT(); 
    spirw(0x8000+id);  
    DESELECT();
    delay_nus(3);
}

//read single register
static uint16_t rreg1(uint8_t addr)
{
    uint16_t res;
    
    SELECT();
    
    spirw(addr<<8);
    delay_nus(2);
    
    res = spirw(0x0000);
    
    DESELECT();
    delay_nus(3);
    
    return res;
}

//write single register
static void wreg1(uint8_t addr,uint16_t data)
{
    SELECT();
    
    spirw(0x8000|(addr<<8)|(data&0x00ff));    //low  8 bytes
    delay_nus(2);
    
    spirw(0x8000|((addr+1)<<8)|((data>>8)&0x00ff));    //high 8 bytes
    
    DESELECT();
    delay_nus(3);
}


//basic functions

//Set Register Page
void adis16375_set_page(uint8_t x)
{
    page(x);
}
//Read Register
uint16_t adis16375_read_reg(uint8_t addr)
{
    return rreg1(addr);
}
//Write Register
void adis16375_write_reg(uint8_t addr,uint16_t data)
{
    wreg1(addr,data);
}

//set data process function and init device
uint32_t adis16375_init(adis16375_config_t *cfg)
{
    uint32_t res;
    //store system status and self-test result
    
    //detect parameters
    if(cfg->decim   == 0    ||
       cfg->decim   >  2048  ) return (1<<15);
    
    //config MCU
    interface_init(cfg);
	
    __DELAY(1);
	//osDelay(500);
    //for wake up from power_down mode
    
	RESET_1();
	__DELAY(10);
	DESELECT();
    RESET_0();
    __DELAY(10);
    RESET_1();
	SELECT();
    
    //wait for start-up 500ms
    
	
	while(pinread(PB5))
	{
		__DELAY(100);
	}
	
	//osDelay(500);
	
    page(0);
    res = rreg1(0x7e);
    if(res != 16375) return ADIS16375_ERR_SPI;//wrong product id, exiting
    
    //test flash memory, exec time 50ms
    page(3);
    wreg1(0x02,(1<<2));//test command
    __DELAY(100);
    page(0);
    res = rreg1(0x08);//get SYS_E_FLAG
    if(res > 1) return (res);//error occurred, not alarm status
    
    //close alarm
    page(3);
    wreg1(0x20,0x0000);//ALM_CONFIG1 = 0;
    wreg1(0x22,0x0000);//ALM_CONFIG2 = 0;
    
    //evaluate self-test, exec time 10ms
    page(3);
    wreg1(0x02,(1<<1));
    __DELAY(20);
    page(0);
    res = rreg1(0x08);//get system status
    if(res & (1<<5))
    {
        res |= rreg1(0x0a) << 16;//get test result
        if(res > 0 ) return (res);//error occurred
    }
    
    //Linear-g compensation for gyroscopes enable
    //RTC daylight savings time disable
    //RTC calendar mode
    page(3);
    wreg1(0x0a,GEN_CONFIG);
    use_dst = false;
    
    //digital signal processing
    //set decimation rate
    //page(3);
    wreg1(0x0c,cfg->decim-1);
    
    //config DRDY and CLKIN ( SYNC ) //***config
    //CLKIN = rising edge, DIO1, enabled
    //DRDY  = positive,    DIO2
    //page(3);
    //wreg1(0x06,0x000d);
    
    //CLKIN = rising edge, DIO1, disable
    //DRDY  = positive,    DIO2
    //page(3);
    wreg1(0x06,0x004d);
    
    page(0);
    res = rreg1(0x08);//get SYS_E_FLAG
    
    return res;
}

//set module to sleep mode
void adis16375_deinit(void)
{
    adis16375_stop();
    //SYNC_OUT_OFF();
    
    //Page 3, Base Address = 0x10, bit 9 
    //power down the device
    page(3);
    wreg1(0x10,(1<<9));
}

void adis16375_filter(uint32_t options,uint8_t banks[6])
{
    uint16_t sel1 = 0, sel2 = 0;
    options = options & ADIS16375_FILTER_ALL;
    sel1 = (uint16_t)options;
    sel2 = options >> 16;
    for(uint8_t i=0;i<6;i++)
        banks[i] &= 0x03;
    sel1 |= (banks[0]|banks[1]<<3|banks[2]<<6|banks[3]<<9|banks[4]<<12);
    sel2 |= banks[5];
    page(3);
    adis16375_write_reg(0x16,sel1);
    adis16375_write_reg(0x18,sel2);
}

//start continuously sampling 
void adis16375_start(void)
{
    page(0);
    
    //DRDY_INT_ON();
}

void adis16375_stop(void)
{
    page(3);
    wreg1(0x10,(1<<9));
}

//take 25us
uint16_t adis16375_get_raw16(vec3s *acc,vec3s *gyro,int16_t *temp)
{
    uint16_t status;
    
    SELECT();
    spirw(0x0800);//request status read
    delay_nus(2);
    status = spirw(0x1200);
    delay_nus(2);
    gyro->x = spirw(0x1600);
    delay_nus(2);
    gyro->y = spirw(0x1A00);
    delay_nus(2);
    gyro->z = spirw(0x1E00);
    delay_nus(2);
    acc->x = spirw(0x2200);
    delay_nus(2);
    acc->y = spirw(0x2600);
    delay_nus(2);
    acc->z = spirw(0x0e00);
    delay_nus(2);
    *temp = spirw(0x0000);
    spiwait();
    DESELECT();
    
    return status;
}

//take 50us
uint16_t adis16375_get_raw32(vec3l *acc,vec3l *gyro,int16_t *temp)
{
    uint16_t status;
    
    SELECT();
    spirw(0x0800);//request status read
    delay_nus(2);
    status   = spirw(0x1000);
    delay_nus(2);
    
    gyro->x  = spirw(0x1200);
    delay_nus(2);
    gyro->x |= spirw(0x1400) << 16;
    delay_nus(2);
    gyro->y  = spirw(0x1600);
    delay_nus(2);
    gyro->y |= spirw(0x1800) << 16;
    delay_nus(2);
    gyro->z  = spirw(0x1A00);
    delay_nus(2);
    gyro->z |= spirw(0x1C00) << 16;
    delay_nus(2);
    
    acc->x  = spirw(0x1E00);
    delay_nus(2);
    acc->x |= spirw(0x2000) << 16;
    delay_nus(2);
    acc->y  = spirw(0x2200);
    delay_nus(2);
    acc->y |= spirw(0x2400) << 16;
    delay_nus(2);
    acc->z  = spirw(0x2600);
    delay_nus(2);
    acc->z |= spirw(0x0e00) << 16;
    delay_nus(2);
    
    *temp   = spirw(0x0000);
    spiwait();
    DESELECT();
    
    return status;
}

uint16_t adis16375_get_output(vec3f *acc,vec3f *gyro,float *temp)
{
    vec3l acc_i,gyro_i;
    int16_t temp_i;
    uint16_t status;
    status = adis16375_get_raw32(&acc_i,&gyro_i,&temp_i);
    acc->x = ADIS16375_ACC_G_I32(acc_i.x);
    acc->y = ADIS16375_ACC_G_I32(acc_i.y);
    acc->z = ADIS16375_ACC_G_I32(acc_i.z);
    gyro->x = ADIS16375_GYRO_RAD_I32(gyro_i.x);
    gyro->y = ADIS16375_GYRO_RAD_I32(gyro_i.y);
    gyro->z = ADIS16375_GYRO_RAD_I32(gyro_i.z);
    *temp = ADIS16375_TEMP(temp_i);
    return status;
}

adis16375_time adis16375_get_time(void)//read time
{
    adis16375_time p;
    p.tm_isdst = use_dst;
    p.tm_sec = reg_min_sec & 0x003f;
    p.tm_min = (reg_min_sec>>8) & 0x003f;
    p.tm_hour = reg_day_hr & 0x003f;
    p.tm_mday = (reg_day_hr>>8) & 0x001f;
    p.tm_mon = reg_yr_mon & 0x000f;
    p.tm_year = (reg_yr_mon>>8) & 0x007f + 100;
    return p;
}

void adis16375_set_time(adis16375_time *t)//set time
{
    uint16_t time_ms = (t->tm_sec & 0x003f) | ((t->tm_min & 0x003f) << 8);
    uint16_t time_dh = (t->tm_hour & 0x003f) | ((t->tm_mday & 0x001f) << 8);
    uint16_t time_ym = (t->tm_mon & 0x000f) | (((t->tm_year - 100)&0x007f) << 8);
    uint16_t reg_gen = GEN_CONFIG | ((t->tm_isdst) ? 2 : 0);
    use_dst = (bool)(t->tm_isdst);
    
    page(3);
    wreg1(0x0a,reg_gen);
    page(0);
    wreg1(0x70,time_ms);
    wreg1(0x72,time_dh);
    wreg1(0x74,time_ym);
    
}


void adis16375_factory(void)
//factory calibration restore
{
    page(3);
    wreg1(0x02,(1<<6));
}

void adis16375_write_to_flash(void)
//flash memory update
{
    page(3);
    wreg1(0x02,(1<<6));
}

uint32_t adis16375_get_flash_write_cnt(void)
//read flash write counter FLSH_CNT_L/H
{
    uint32_t res;
    
    page(2);
    res = rreg1(0x78);
    res |= rreg1(0x7a)<<16;
    
    return res;
}

void adis16375_get_lot_id(uint16_t seq[3])
{
    page(0);
    seq[0] = rreg1(0x78);
    seq[1] = rreg1(0x7a);
    seq[2] = rreg1(0x7c);
}

void adis16375_bias_null(uint16_t sensor,uint8_t timebase)
{
    page(3);
    wreg1(0x0e,(sensor & 0x3f)|(timebase & 0x0f));
}

void adis16375_bias_to_reg(void)
{
    page(3);
    wreg1(0x02,0x0001);
}
