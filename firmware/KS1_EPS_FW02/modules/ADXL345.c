#include "delay.h"
#include "pin.h"
#include "ADXL345.h"
//#include "common_spi.h"
#include "spi.h"
#include <stdlib.h>
#include "bsp.h"

#define DEVID 0xe5

//x,y,z
static bool newdata;

void (*forward)(int16_t data[3]);

//private functions

/*connections 
//INT1 -> PB11 EXTI falling edge
//#CS  -> PB12
SCLK -> PA5
MISO -> PA6
MOSI -> PA7
*/


//platform dependent
void __delay(void)
//200ns delay
{
    u8 x = 18;
    while(x--);
}

static void IO_config(void)
{
}

static void SPI_config(void)
{
    
}

#define CS_LOW()  pinlow (AX3S_CS_PIN)   //CS LOW
#define CS_HIGH() pinhigh(AX3S_CS_PIN)   //CS HIGH

#define spicomm(B) spi_rw(SPI2,B)

#ifdef ADXL345_USE_DRDY
static void INT_enable(void)
{
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_s;
    NVIC_s.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_s.NVIC_IRQChannelCmd = ENABLE;
    NVIC_s.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_s.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_s);
}
#endif

void adxl345_deinit(void)
{
    adxl345_writereg(0x2d,0x00);
    
#ifdef ADXL345_USE_DRDY
    //disable interrupt
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
#endif
    
    //disalbe spi ?
    
}

//platform independent
uint8_t adxl345_readreg(uint8_t addr)
{
    uint8_t data;
    
    CS_LOW();
    addr = addr & 0x3f;
    spicomm(addr|0x80);
    data = spicomm(0x00);
    CS_HIGH();
    __delay();
    
    return data;
}

void adxl345_writereg(uint8_t addr,uint8_t val)
{
    CS_LOW();
    spicomm(addr&0x3f);
    spicomm(val);
    CS_HIGH();
    __delay();
}

#ifdef ADXL345_USE_DRDY
bool adxl345_init(uint8_t rate_code,void (*f)(int16_t data[3]))
#else
bool adxl345_init(uint8_t rate_code)
#endif
{
    uint8_t reg;
    
    //data[0] = data[1] = data[2] = 0;
    newdata = false;
#ifdef ADXL345_USE_DRDY
    forward = f;
#else
    forward = NULL;
#endif
        
    IO_config();
    //CS_HIGH();
    
    SPI_config();
    
    delay_nms(2);//delay more than 1.1ms for device stablize
    reg = adxl345_readreg(0x00);
    if(reg != DEVID) return false;
    //wire failure
    
    adxl345_writereg(0x2c,rate_code);
    //set data rate
    adxl345_writereg(0x31,0x2b);//change value in selftest()
    //INT pin set to LOW active, Result right justify, 13bit +-16g mode
    adxl345_writereg(0x2e,0x80);
    adxl345_writereg(0x2f,0x00);
    //#INT1 for DATA_READY
    adxl345_writereg(0x2d,0x08);
    //start measurement
    
#ifdef ADXL345_USE_DRDY
    INT_enable();//enable interrupt
    EXTI_GenerateSWInterrupt(EXTI_Line11);
#endif
    
    //no selftest yet
    return true;
}

#ifdef ADXL345_USE_DRDY
void __adxl345_isr(void)
{
    uint8_t tmp[6];
    uint8_t i;
    int16_t data[3];
    
    CS_LOW();
    spicomm(0xf2);
    for(i=0;i<6;i++)
        tmp[i]=spicomm(0x00);
    CS_HIGH();
    
    data[0] = (tmp[0] | tmp[1]<<8);
    data[1] = (tmp[2] | tmp[3]<<8);
    data[2] = (tmp[4] | tmp[5]<<8);
    
    newdata = true;
    
    if(forward!=0) (*forward)(data);
}
#endif

void adxl345_read(int16_t d[3])
{
    //uint8_t tmp[6];
    uint8_t *tmp = (uint8_t *)d;
    
    CS_LOW();
    spicomm(0xf2);
    for(uint8_t i=0;i<6;i++)
        tmp[i]=spicomm(0x00);
    CS_HIGH();
    /*
    d[0] = (tmp[0] | tmp[1]<<8);
    d[1] = (tmp[2] | tmp[3]<<8);
    d[2] = (tmp[4] | tmp[5]<<8);
    */
    newdata = false;
}

bool adxl345_dataready(void)
{
#ifdef ADXL345_USE_DRDY
    //use data-ready interrupt
    //return newdata;    
#else
    //use query mode
    if(adxl345_readreg(0x30) & 0x80)
        newdata = true;
    else
        newdata = false;
    //detect DATA-READY bit
#endif
    return newdata;
}

#define DX_MAX  (980)
#define DX_MIN  (70)
#define DY_MAX  (-70)
#define DY_MIN  (-980)
#define DZ_MAX  (1300)
#define DZ_MIN  (100)

#define GX_MAX  (80)
#define GX_MIN  (-80)
#define GY_MAX  (80)
#define GY_MIN  (-80)
#define GZ_MAX  (400)
#define GZ_MIN  (120)

#define SELFTEST_SAMPLES  16

bool adxl345_selftest(void)
{
    uint16_t cnt;
    int32_t sum[3];
    int16_t data[3];
    int16_t static_val[3];
    
    //0s
    sum[0] = sum[1] = sum[2] = 0;
    for(cnt=0;cnt<SELFTEST_SAMPLES;cnt++)
    {
        adxl345_read(data);
        //if(data[0] > GX_MAX || data[0] < GX_MIN)    return FALSE;
        //if(data[1] > GY_MAX || data[1] < GY_MIN)    return FALSE;
        //if(data[2] > GZ_MAX || data[2] < GZ_MIN)    return FALSE;
        //static value check, communication check(not used)
        sum[0] += data[0];
        sum[1] += data[1];
        sum[2] += data[2];
        while(!adxl345_dataready());
    }
    static_val[0]= sum[0] / SELFTEST_SAMPLES;
    static_val[1]= sum[1] / SELFTEST_SAMPLES;
    static_val[2]= sum[2] / SELFTEST_SAMPLES;
    //0.5s
    
    adxl345_writereg(0x31,0xab);//enable selftest bit
    
    sum[0] = sum[1] = sum[2] = 0;
    for(cnt=0;cnt<SELFTEST_SAMPLES;cnt++)
    {
        adxl345_read(data);
        sum[0] += data[0];
        sum[1] += data[1];
        sum[2] += data[2];
        while(!adxl345_dataready());
    }
    sum[0] /= SELFTEST_SAMPLES;
    sum[1] /= SELFTEST_SAMPLES;
    sum[2] /= SELFTEST_SAMPLES;
    
    sum[0] -= static_val[0];
    sum[1] -= static_val[1];
    sum[2] -= static_val[2];
    //1s
    
    if(sum[0] > DX_MAX || sum[0] < DX_MIN)    return FALSE;
    if(sum[1] > DY_MAX || sum[1] < DY_MIN)    return FALSE;
    if(sum[2] > DZ_MAX || sum[2] < DZ_MIN)    return FALSE;
    //selftest value check
    
    adxl345_writereg(0x31,0x2b);//disable selftest bit    
    
    return true;
}

#ifdef ADXL345_USE_DRDY
void adxl345_set_forward(void (*f)(int16_t data[3]))
{
    forward = f;
}
#endif
