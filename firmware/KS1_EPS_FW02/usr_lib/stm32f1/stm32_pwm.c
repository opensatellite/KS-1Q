//module PWM for STM32

#include "stm32f10x.h"
#include "STM32_PWM.h"

void timer_clock(TIM_TypeDef* tim , uint16_t prsc, uint16_t period)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = prsc - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = period - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(tim,&TIM_TimeBaseStructure);
  
  TIM_ARRPreloadConfig(tim,ENABLE);
  TIM_Cmd(tim, ENABLE);
}

void __timer_preprescaler(TIM_TypeDef* tim, uint16_t TIM_CKD_DIV)
{
  assert_param(IS_TIM_CKD_DIV(TIM_CKD_DIV));
  uint16_t tmpreg = tim->CR1;
  tmpreg &= ~0x0300;
  tmpreg |= TIM_CKD_DIV;
  tim->CR1 = tmpreg;
}

void pwm_set(pwm_channel ch)
{
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = ch.oc > 0 ? TIM_OutputState_Enable : TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OutputNState = ch.oc < 0 ? TIM_OutputNState_Disable : TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  
  switch(ch.oc)
  {
  case 1:
    TIM_OC1PreloadConfig(ch.tim, TIM_OCPreload_Enable);
    TIM_OC1Init(ch.tim,&TIM_OCInitStructure);
    break;
  case 2:
    TIM_OC2PreloadConfig(ch.tim, TIM_OCPreload_Enable);
    TIM_OC2Init(ch.tim,&TIM_OCInitStructure);
    break;
  case 3:
    TIM_OC3PreloadConfig(ch.tim, TIM_OCPreload_Enable);
    TIM_OC3Init(ch.tim,&TIM_OCInitStructure);
    break;
  case 4:
    TIM_OC4PreloadConfig(ch.tim, TIM_OCPreload_Enable);
    TIM_OC4Init(ch.tim,&TIM_OCInitStructure);
    break;
  default:
    return;
  }
  
  if(ch.tim == TIM1 || ch.tim == TIM8)  
    TIM_CtrlPWMOutputs(ch.tim, ENABLE);
}

void pwm_out(pwm_channel ch, uint16_t v)
{
  assert_param((ch.oc >= 1 && ch.oc <= 4)
             ||(ch.oc >= -4 && ch.oc <= -1));
  uint8_t offset = ch.oc << 1;
  __IO uint16_t* oc_reg = &(ch.tim->RCR);
  *(oc_reg + offset) = v;
}

void pwm_on(pwm_channel ch)
{
  const uint16_t table[] = {TIM_Channel_1,TIM_Channel_2,TIM_Channel_3,TIM_Channel_4};
  if(ch.oc > 0) TIM_CCxCmd(ch.tim,  table[ch.oc-1],  TIM_CCx_Enable);
  if(ch.oc < 0) TIM_CCxNCmd(ch.tim, table[-ch.oc-1], TIM_CCx_Enable);
}

void pwm_off(pwm_channel ch)
{
  const uint16_t table[] = {TIM_Channel_1,TIM_Channel_2,TIM_Channel_3,TIM_Channel_4};
  if(ch.oc > 0) TIM_CCxCmd(ch.tim,  table[ch.oc-1],  TIM_CCx_Disable);
  if(ch.oc < 0) TIM_CCxNCmd(ch.tim, table[-ch.oc-1], TIM_CCx_Disable);
}