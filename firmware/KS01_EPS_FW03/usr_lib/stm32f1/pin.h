/*
  WarMonkey STM32 BSP
*/

#ifndef _PIN_H_
#define _PIN_H_

#include "datatypes.h"
#include "stm32f10x_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GPIO_TypeDef* port;
    uint16_t mask;
}pin;

static inline void pinhigh(pin p) { p.port->BSRR = p.mask; }
static inline void pinlow(pin p)  { p.port->BRR = p.mask; }
static inline bool pinread(pin p) { return ((p.port->IDR & p.mask) != 0) ? true : false; }
//read true input level
static inline void pinflip(pin p) { if(pinread(p)) pinlow(p); else pinhigh(p); }
#define pintoggle    pinflip

#include "pin_define.h"
//#define PA0	((pin){GPIOA,0})
//const pin signal_pin = C_PA0;//use this to assign a pin constant

//Pin modes by novakon(chiataimakro@tom.com)
#define pin_mode_type   GPIOMode_TypeDef		
#define AIN		GPIO_Mode_AIN
#define ANALOG 	        GPIO_Mode_AIN
#define INPUT 		GPIO_Mode_IN_FLOATING
#define FLOATING        GPIO_Mode_IN_FLOATING
#define PULLUP		GPIO_Mode_IPU
#define PULLDOWN	GPIO_Mode_IPD
#define OUTPUT		GPIO_Mode_Out_PP
#define PUSHPULL        GPIO_Mode_Out_PP
#define OPENDRAIN	GPIO_Mode_Out_OD
#define AFINPUT         INPUT   //stm32f1 only
#define AFPULLUP        PULLUP
#define AFPULLDOWN      PULLDOWN
#define AFPUSHPULL	GPIO_Mode_AF_PP
#define AFOPENDRAIN	GPIO_Mode_AF_OD

//Pin Speeds
#define pin_speed_type  GPIOSpeed_TypeDef
#define pin_2MHz	GPIO_Speed_2MHz
#define pin_10MHz	GPIO_Speed_10MHz
#define pin_50MHz	GPIO_Speed_50MHz

//Pin IRQ Trigger Mode
#define RISING              0x01
#define FALLING             0x02
void __pin_confignvic(pin p,uint8_t prepr,uint8_t subpr);
void pin_setirq(pin p,uint8_t triggermode);//set nvic by yourself
void pin_noirq(pin p);

#define pinmode(p,m)	pinmodespd(p,m,pin_50MHz)
extern void pinmodespd(pin p, pin_mode_type mode, pin_speed_type speed);
//set pin mode and speed
//do not forget to turn on GPIO clock before using pinmode

static inline bool __pinreg(pin p) { return ((p.port->ODR & p.mask) != 0) ? true : false; }
//pin output reg read back

extern uint8_t pin_getPortSource(pin p);
extern uint8_t pin_getPinSource(pin p);

#ifdef __cplusplus
}
#endif

#endif
