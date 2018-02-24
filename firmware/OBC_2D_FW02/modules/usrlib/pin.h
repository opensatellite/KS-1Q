/*
  WarMonkey STM32F4 BSP
*/

#ifndef _PIN_H_
#define _PIN_H_

#include "datatypes.h"
#include "stm32f4xx_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GPIO_TypeDef* port;
    uint16_t mask;
}pin;

static inline void pinhigh(pin p) { p.port->BSRRL = p.mask; }
static inline void pinlow(pin p)  { p.port->BSRRH = p.mask; }
static inline uint8_t pinread(pin p) {return (p.port->IDR & p.mask) ? 1 : 0;}	//read true input level
static inline void pinflip(pin p) { if(pinread(p)) pinlow(p); else pinhigh(p); }
#define pintoggle    pinflip

#include "pin_define.h"
//#define PA0	((pin){GPIOA,0})
//const pin signal_pin = C_PA0;//use this to assign a pin constant


typedef struct {
  GPIOMode_TypeDef m;
  GPIOPuPd_TypeDef i;
  GPIOOType_TypeDef o;
}pin_mode_type;

#define AIN                 (pin_mode_type){M_ANALOG,I_NOPULL,O_PUSHPULL}
#define ANALOG 	            AIN

#define INPUT 		    (pin_mode_type){M_INPUT,I_NOPULL,O_PUSHPULL}
#define FLOATING            INPUT
#define PULLUP		    (pin_mode_type){M_INPUT,I_PULLUP,O_PUSHPULL}
#define PULLDOWN	    (pin_mode_type){M_INPUT,I_PULLDOWN,O_PUSHPULL}

#define OUTPUT              (pin_mode_type){M_OUTPUT,I_NOPULL,O_PUSHPULL}
#define PUSHPULL            OUTPUT
#define OPENDRAIN	    (pin_mode_type){M_OUTPUT,I_NOPULL,O_OPENDRAIN}
#define OPENDRAIN_PULLUP    (pin_mode_type){M_OUTPUT,I_PULLUP,O_OPENDRAIN}
#define OPENDRAIN_PULLDOWN  (pin_mode_type){M_OUTPUT,I_PULLDOWN,O_OPENDRAIN}

#define AFINPUT             (pin_mode_type){M_AF,I_NOPULL,O_PUSHPULL}
#define AFPULLUP            (pin_mode_type){M_AF,I_PULLUP,O_PUSHPULL}
#define AFPULLDOWN          (pin_mode_type){M_AF,I_PULLDOWN,O_PUSHPULL}
#define AFPUSHPULL	    (pin_mode_type){M_AF,I_PULLUP,O_PUSHPULL}
#define AFOPENDRAIN	    (pin_mode_type){M_AF,I_PULLUP,O_OPENDRAIN}

//Pin Mode
#define M_INPUT             GPIO_Mode_IN
#define M_OUTPUT            GPIO_Mode_OUT
#define M_ANALOG            GPIO_Mode_AN
#define M_AF                GPIO_Mode_AF

//Pin Pull U/D
#define I_NOPULL            GPIO_PuPd_NOPULL
#define I_PULLUP            GPIO_PuPd_UP
#define I_PULLDOWN          GPIO_PuPd_DOWN

//Pin Drive PP/OD
#define O_PUSHPULL          GPIO_OType_PP
#define O_OPENDRAIN         GPIO_OType_OD

//Pin Speeds
#define pin_speed_type      GPIOSpeed_TypeDef
#define pin_2MHz	    GPIO_Speed_2MHz
#define pin_25MHz	    GPIO_Speed_25MHz
#define pin_50MHz	    GPIO_Speed_50MHz
#define pin_100MHz	    GPIO_Speed_100MHz

//Pin IRQ Trigger Mode
#define RISING              0x01
#define FALLING             0x02
void pin_setirq(pin p,uint8_t triggermode);
void pin_noirq(pin p);

#define pinmode(p,m)        pinmodespd(p,m,pin_2MHz)
//set pin mode
extern void pin_af_for(pin p,uint8_t GPIO_AF);      //pin AF for which periphial

extern void pinmodespd(pin p, pin_mode_type mode, pin_speed_type speed);
//set pin mode and speed
#define pinmodex(p,m,i,o)       pinmodespdx(p,m,i,o,pin_25MHz)
//customize pin mode
#define pinmodespdx(p,m,i,o,s)  pinmodespd(p,(pin_mode_type){m,i,o},s)
//customize pin mode and speed

#define __pinreg(p)  ((p.port->ODR & p.mask) != 0)	//pin output reg read back
extern uint8_t pin_getPortSource(pin p);
extern uint8_t pin_getPinSource(pin p);

void __pin_periphial_clock(pin p,FunctionalState s);
void __pin_confignvic(pin p,uint8_t prepr,uint8_t subpr);

#ifdef __cplusplus
}
#endif

#endif
