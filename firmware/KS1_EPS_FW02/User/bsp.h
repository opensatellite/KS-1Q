#ifndef BSP_H
#define BSP_H

#include "stm32f10x.h"
#include "config.h"

#include "dm.h"

//battery
#define VL_DET_PIN		PA0     //ADC12_IN0
#define VH_DET_PIN		PA1     //ADC12_IN1
//radiation
#define RAD_DET_PIN		PA3     //ADC12_IN3
//sun sensor
#define SUN_A_PIN		PA4     //ADC12_IN4
#define SUN_B_PIN		PA5     //ADC12_IN5
#define SUN_C_PIN		PA6     //ADC12_IN6
#define SUN_D_PIN		PA7     //ADC12_IN7
//solar
#define P1_CS_PIN		PC1     //ADC12_IN11
#define P2_CS_PIN		PC0     //ADC12_IN10
//temperature
#define CAM_TMP_PIN		PB0     //ADC12_IN8
#define BAT_TMP_PIN		PC4     //ADC12_IN14
//magnetorquer
#define X_F_PWM_PIN		PB6     //TIM4_CH1
#define X_R_PWM_PIN		PB5     //TIM3_CH2(Remap)
#define Y_F_PWM_PIN		PB4     //TIM3_CH1(Remap)
#define Y_R_PWM_PIN		PB3     //TIM2_CH2(Remap)
#define Z_F_PWM_PIN		PD2     //
#define Z_R_PWM_PIN		PC12    //
//external
#define EX_IO1_PIN		PB10
#define EX_IO2_PIN		PB11
//load current
#define HT_CS_PIN		PA2     //ADC12_IN2
#define BUS_CS_PIN		PB1     //ADC12_IN9
#define LED_CS_PIN		PC2     //ADC12_IN12
#define PSB_CS_PIN		PC3     //ADC12_IN13
#define CAM_CS_PIN		PC5     //ADC12_IN15
//power switch
#define ADC_PWRON_PIN	        PA8
#define CAM_ON_PIN		PA9
#define BUS_ON_PIN		PA10
#define LED_ON_PIN		PA15
#define MTQ_ON_PIN		PB7
#define HT_ON_PIN		PC14
#define DEPLOY_PIN		PB9
//extadc
#define ADC_EOC_PIN		PB12
#define SCLK_PIN		PB13    //SPI2_SCK
#define MISO_PIN		PB14    //SPI2_MISO
#define MOSI_PIN		PB15    //SPI2_MOSI
#define ADC_CS_PIN		PC9
//misc
#define AX3S_CS_PIN		PC8     //accel chip select
#define	RTC_CS_PIN		PB2     //rtc chip select
#define CAM_TX_PIN		PC10    //USART3_TX(remap)
#define CAM_RX_PIN		PC11    //USART3_RX(remap)
#define WTD_IN_PIN		PC15    
#define	CAN_TX_PIN		PA12    //CAN_RX
#define CAN_RX_PIN		PA11    //CAN_TX
#define CAN_LBK_PIN     PC6

//TTC	
#define TTC_ON_PIN		PA10

//internal adc
#define VL_DET          0
#define VH_DET          1
#define HT_CS           2
#define RAD_DET         3
#define SUN_A           4
#define SUN_B           5
#define SUN_C           6
#define SUN_D           7
#define CAM_TMP         8
#define BUS_CS          9
#define P2_CS           10
#define P1_CS           11
#define LED_CS          12
#define PSB_CS          13
#define BAT_TMP         14
#define CAM_CS          15
#define INT_ADC_REF     3260.0f
#define VL_DET_CAL(X)   (X/4096.0f*INT_ADC_REF*2.0f)
#define VH_DET_CAL(X)   (X/4096.0f*INT_ADC_REF*3.0f)
#define P1_CS_CAL(X)    (X/4096.0f*INT_ADC_REF*1.0f)
#define P2_CS_CAL(X)    (X/4096.0f*INT_ADC_REF*1.0f)
#define PSB_CS_CAL(X)   (X/4096.0f*INT_ADC_REF*3.33333f)
#define HT_CS_CAL(X)    (X/4096.0f*INT_ADC_REF*0.25f)
#define LED_CS_CAL(X)   (X/4096.0f*INT_ADC_REF*0.5f)
#define CAM_CS_CAL(X)   (X/4096.0f*INT_ADC_REF*1.0f)
#define BUS_CS_CAL(X)   (X/4096.0f*INT_ADC_REF*0.25f)

#define NTC_THRU_CAL(x)	ntc_cal_thru(x)
#define NTC_SMD_CAL(x)	ntc_cal_smd(x)

//external adc
#define EXT_VL_DET      0
#define MT_X_CS         1
#define MT_Y_CS         2
#define MT_Z_CS         3
#define CASE_TMP1       4
#define CASE_TMP2       5
#define CASE_TMP3       6
#define CASE_TMP4       7

#define CONSOLE         2       //console uart port

#define OBC_ADDR		0x00
#define TTC_ADDR		0x01
#define EPS_ADDR		0x02
#define TNC_ADDR		0x09
#define User_ADDR		0x0A	

#define CAM_POWER_ON()		pinhigh(CAM_ON_PIN)
#define CAM_POWER_OFF()		pinlow(CAM_ON_PIN)

#define TTC_POWER_ON()		pinhigh(TTC_ON_PIN)
#define TTC_POWER_OFF()		pinlow(TTC_ON_PIN)

#define OBC_POWER_ON()		pinhigh(DEPLOY_PIN)
#define OBC_POWER_OFF()		pinlow(DEPLOY_PIN)
#define OBC_RUN			dm_var.deploy

//#define OBC_PRESERVE
//#define HEATER_PROT_INJECTION

#define WTD_IN_H()			{pinmode(HT_ON_PIN,FLOATING);\
								pinmode(WTD_IN_PIN,OUTPUT);\
								pinhigh(WTD_IN_PIN);}

#define WTD_IN_L()			{pinlow(WTD_IN_PIN);\
								pinmode(WTD_IN_PIN,FLOATING);\
								pinmode(HT_ON_PIN,OUTPUT);}

#define HT_POWER_ON()		pinhigh(HT_ON_PIN)
#define HT_POWER_OFF()		pinlow(HT_ON_PIN)
#define HT_ON_STATUS		pinread(HT_ON_PIN)

#define CAM_RUN_FLAG		dm_var.pwrcam

#define user_set_addr	0x0801F800
#define CFG_OFF_EPSRUNWAIT      0
#define CFG_OFF_HEATERVALID     1
#define CFG_OFF_OBCRUNFLAG      2
#define CFG_OFF_MAX             2

void bsp_init();
int chang_cfg(int off,uint16_t val);

#endif