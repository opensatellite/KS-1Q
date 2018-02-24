/*
WarMonkey luoshumymail@gmail.com
*/

#include "pin.h"
#include "stm32f4xx_syscfg.h"

void __pin_periphial_clock(pin p,FunctionalState s)
{
    GPIO_TypeDef *GPIOx = p.port;  
    
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
    if (GPIOx == GPIOA)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, s);
    else if (GPIOx == GPIOB)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, s);
    else if (GPIOx == GPIOC)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, s);
    else if (GPIOx == GPIOD)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, s);
    else if (GPIOx == GPIOE)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, s);
    else if (GPIOx == GPIOF)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, s);
    else if (GPIOx == GPIOG)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, s);
    else if (GPIOx == GPIOH)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, s);
    else if (GPIOx == GPIOI)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, s);
    else;
}

void __syscfg_periphial_clock(pin p,FunctionalState s)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, s);
}

uint8_t pin_getPortSource(pin p)
{
    GPIO_TypeDef *GPIOx = p.port;  
    
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
    if (GPIOx == GPIOA)
        return EXTI_PortSourceGPIOA;
    else if (GPIOx == GPIOB)
        return EXTI_PortSourceGPIOB;
    else if (GPIOx == GPIOC)
        return EXTI_PortSourceGPIOC;
    else if (GPIOx == GPIOD)
        return EXTI_PortSourceGPIOD;
    else if (GPIOx == GPIOE)
        return EXTI_PortSourceGPIOE;
    else if (GPIOx == GPIOF)
        return EXTI_PortSourceGPIOF;
    else if (GPIOx == GPIOG)
        return EXTI_PortSourceGPIOG;
    else if (GPIOx == GPIOH)
        return EXTI_PortSourceGPIOH;
    else if (GPIOx == GPIOI)
        return EXTI_PortSourceGPIOI;
    else;
    return 0xff;
}

uint8_t pin_getPinSource(pin p)
{
    uint8_t x = 0;
    if(p.mask & 0xffff0000)
    {
        x += 16;
        p.mask >>= 16;
    }
    if(p.mask & 0x0000ff00)//0b1111_1111_0000_0000
    {
        x += 8;
        p.mask >>= 8;
    }
    if(p.mask & 0x000000f0)//0b1111_0000
    {
        x += 4;
        p.mask >>= 4;
    }
    if(p.mask & 0x0000000c)//0b1100
    {
        x += 2;
        p.mask >>= 2;
    }
    if(p.mask & 0x00000002)//0b10
    {
        x += 1;
    }
    return x;
}

void pin_af_for(pin p,uint8_t GPIO_AF)
{
    GPIO_PinAFConfig(p.port,pin_getPinSource(p),GPIO_AF);
}

void pin_setirq(pin p, uint8_t triggermode)
{
	__syscfg_periphial_clock(p,ENABLE);
    SYSCFG_EXTILineConfig(pin_getPortSource(p), pin_getPinSource(p));
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = (uint32_t)p.mask;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    switch(triggermode)
    {
    case RISING:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        break;
    case FALLING:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        break;
    case RISING|FALLING:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        break;
    default:
        return;
    }
    EXTI_Init(&EXTI_InitStructure);
}

void pin_noirq(pin p)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = (uint32_t)p.mask;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void pinmodespd(pin p, pin_mode_type mode, pin_speed_type speed) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    __pin_periphial_clock(p,ENABLE);
    pinlow(p);
    
    GPIO_InitStructure.GPIO_Pin = p.mask;
    GPIO_InitStructure.GPIO_Speed = speed;
    GPIO_InitStructure.GPIO_Mode = mode.m;
    GPIO_InitStructure.GPIO_PuPd = mode.i;
    GPIO_InitStructure.GPIO_OType = mode.o;
    GPIO_Init(p.port, &GPIO_InitStructure);
}

void __pin_confignvic(pin p,uint8_t prepr,uint8_t subpr)
{
    uint8_t n = pin_getPinSource(p);
    NVIC_InitTypeDef NVIC_InitStructure;
    
    if(n == 0)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    }
    else if(n == 1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    }
    else if(n == 2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    }
    else if(n == 3)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    }
    else if(n == 4)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    }
    else if(n >= 5 && n <= 9)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    }
    else if(n >= 10 && n <= 15)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    }
    else return;
            
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prepr;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpr;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
