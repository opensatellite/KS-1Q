/*
    WarMonkey luoshumymail@gmail.com
*/

#include "pin.h"
#include "misc.h"

static void __pin_periphial_clock(pin p,FunctionalState s)
{
    GPIO_TypeDef *GPIOx = p.port;  
    
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
    if (GPIOx == GPIOA)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, s);
    else if (GPIOx == GPIOB)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, s);
    else if (GPIOx == GPIOC)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, s);
    else if (GPIOx == GPIOD)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, s);
    else if (GPIOx == GPIOE)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, s);
    else if (GPIOx == GPIOF)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, s);
    else if (GPIOx == GPIOG)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, s);
    else;
}

uint8_t pin_getPortSource(pin p)
{
    GPIO_TypeDef *GPIOx = p.port;  
    
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
    if (GPIOx == GPIOA)
        return GPIO_PortSourceGPIOA;
    else if (GPIOx == GPIOB)
        return GPIO_PortSourceGPIOB;
    else if (GPIOx == GPIOC)
        return GPIO_PortSourceGPIOC;
    else if (GPIOx == GPIOD)
        return GPIO_PortSourceGPIOD;
    else if (GPIOx == GPIOE)
        return GPIO_PortSourceGPIOE;
    else if (GPIOx == GPIOF)
        return GPIO_PortSourceGPIOF;
    else if (GPIOx == GPIOG)
        return GPIO_PortSourceGPIOG;
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

void pin_setirq(pin p, uint8_t triggermode)
{
    //SYSCFG_EXTILineConfig(pin_getPortSource(p), pin_getPinSource(p));
    GPIO_EXTILineConfig(pin_getPortSource(p), pin_getPinSource(p));
    
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
  //pinlow(p);
  
  GPIO_InitStructure.GPIO_Pin = p.mask;
  GPIO_InitStructure.GPIO_Speed = speed;
  GPIO_InitStructure.GPIO_Mode = mode;
  GPIO_Init(p.port, &GPIO_InitStructure);
}
