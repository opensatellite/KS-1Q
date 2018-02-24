//PWM and ICP for STM32F4
#ifndef _STM32F4_PWMICP_H__
#define _STM32F4_PWMICP_H__

#include "datatypes.h"
#include "stm32f4xx_tim.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  TIM_TypeDef* tim;
  int8_t oc;
}timer_channel;

#define T1      TIM1
#define T2      TIM2
#define T3      TIM3
#define T4		TIM4
#define T8      TIM8

#define T1C1    (timer_channel){TIM1,1}
#define T1C2    (timer_channel){TIM1,2}
#define T1C3    (timer_channel){TIM1,3}
#define T1C4    (timer_channel){TIM1,4}
#define T1C1N   (timer_channel){TIM1,-1}
#define T1C2N   (timer_channel){TIM1,-2}
#define T1C3N   (timer_channel){TIM1,-3}
#define T1C4N   (timer_channel){TIM1,-4}

#define T2C1    (timer_channel){TIM2,1}
#define T2C2    (timer_channel){TIM2,2}
#define T2C1N   (timer_channel){TIM2,-1}
#define T2C2N   (timer_channel){TIM2,-2}

#define T3C1    (timer_channel){TIM3,1}
#define T3C2    (timer_channel){TIM3,2}
#define T3C3    (timer_channel){TIM3,3}
#define T3C4    (timer_channel){TIM3,4}
#define T3C1N   (timer_channel){TIM3,-1}
#define T3C2N   (timer_channel){TIM3,-2}
#define T3C3N   (timer_channel){TIM3,-3}
#define T3C4N   (timer_channel){TIM3,-4}

#define T4C1	(timer_channel){TIM4,1}
#define T4C2	(timer_channel){TIM4,2}

#define T8C1    (timer_channel){TIM8,1}
#define T8C2    (timer_channel){TIM8,2}
#define T8C3    (timer_channel){TIM8,3}
#define T8C4    (timer_channel){TIM8,4}
#define T8C1N   (timer_channel){TIM8,-1}
#define T8C2N   (timer_channel){TIM8,-2}
#define T8C3N   (timer_channel){TIM8,-3}
#define T8C4N   (timer_channel){TIM8,-4}

#define T9C1    (timer_channel){TIM9,1}
#define T9C2    (timer_channel){TIM9,2}

#define C_T1C1  {TIM1,1}
#define C_T1C2  {TIM1,2}
#define C_T1C3  {TIM1,3}
#define C_T1C4  {TIM1,4}
#define C_T1C1N {TIM1,-1}
#define C_T1C2N {TIM1,-2}
#define C_T1C3N {TIM1,-3}
#define C_T1C4N {TIM1,-4}

#define C_T2C1  {TIM2,1}
#define C_T2C2  {TIM2,2}
#define C_T2C1N {TIM2,-1}
#define C_T2C2N {TIM2,-2}

#define C_T3C1  {TIM3,1}
#define C_T3C2  {TIM3,2}
#define C_T3C3  {TIM3,3}
#define C_T3C4  {TIM3,4}
#define C_T3C1N {TIM3,-1}
#define C_T3C2N {TIM3,-2}
#define C_T3C3N {TIM3,-3}
#define C_T3C4N {TIM3,-4}

#define C_T4C1	{TIM4,1}
#define C_T4C2	{TIM4,2}

#define C_T8C1  {TIM8,1}
#define C_T8C2  {TIM8,2}
#define C_T8C3  {TIM8,3}
#define C_T8C4  {TIM8,4}
#define C_T8C1N {TIM8,-1}
#define C_T8C2N {TIM8,-2}
#define C_T8C3N {TIM8,-3}
#define C_T8C4N {TIM8,-4}

#define C_T9C1  {TIM9,1}
#define C_T9C2  {TIM9,2}

#define IS_TIMER(TIM)               IS_TIM_ALL_PERIPH(TIM)
#define IS_PWM_MODE(MODE)			((MODE) == 1 || (MODE) == 2)
#define IS_PWM_CHANNEL(TIMER_CH)    (((TIMER_CH).oc >= 1 && (TIMER_CH).oc <= 4) || ((TIMER_CH).oc >= -4 && (TIMER_CH).oc <= -1))
#define IS_ICP_CHANNEL(TIMER_CH)    ((TIMER_CH).oc >= 1 && (TIMER_CH).oc <= 4)
#define IS_ICP_PRESCALER(PRSC)      ((PRSC) == 1 && (PRSC) == 2 && (PRSC) == 4 && (PRSC) == 8)
#define IS_ICP_FILTER(FILTER)       ((FILTER) >= 0 && (FILTER) <= 0xf)

#define TIMER_CNTMODE_UP    TIM_CounterMode_Up
#define TIMER_CNTMODE_DOWN  TIM_CounterMode_Down
#define TIMER_CNTMODE_CTR1  TIM_CounterMode_CenterAligned1
#define TIMER_CNTMODE_CTR2  TIM_CounterMode_CenterAligned2
#define TIMER_CNTMODE_CTR3  TIM_CounterMode_CenterAligned3

#define timer_clock(TIM,PRSC,PERIOD) timer_clock_cntmode(TIM,PRSC,PERIOD,TIMER_CNTMODE_UP)
void timer_clock_cntmode(TIM_TypeDef* tim, uint16_t prsc, uint32_t period, uint16_t cntmode);
void timer_noclock(TIM_TypeDef* tim);
#define timer_preprescaler(TIM,VAL) __timer_preprescaler(TIM,TIM_CKD_DIV##VAL)
void __timer_preprescaler(TIM_TypeDef* tim, uint16_t TIM_CKD_DIV);
uint32_t __timer_getbasefreq(TIM_TypeDef* tim);

#define pwm_set(CH) __pwm_set(CH,1,false,0)
#define pwm_set_complimentary(CH,DEADCODE) __pwm_set(CH,1,true,DEADCODE)
void __pwm_set(timer_channel ch,uint8_t mode,bool comp,uint8_t deadcode);
//deadcode feature not implemented yet
void pwm_out(timer_channel ch, uint32_t v);
void pwm_off(timer_channel ch);

#define ICP_RISING      TIM_ICPolarity_Rising
#define ICP_FALLING     TIM_ICPolarity_Falling
#define ICP_RISEFALL    TIM_ICPolarity_BothEdge
//always use TIM_ICSelection_DirectTI
void icp_set(timer_channel ch, uint16_t edge, uint16_t prescaler, uint8_t filter);
//prsc = 1,2,4,8 , filter = 0~15

#define ICP_STATE_MASK  0x1e1e
#define ICP_CH1_VALID   TIM_FLAG_CC1
#define ICP_CH2_VALID   TIM_FLAG_CC2
#define ICP_CH3_VALID   TIM_FLAG_CC3
#define ICP_CH4_VALID   TIM_FLAG_CC4
#define ICP_CH1_OVF     TIM_FLAG_CC1OF
#define ICP_CH2_OVF     TIM_FLAG_CC2OF
#define ICP_CH3_OVF     TIM_FLAG_CC3OF
#define ICP_CH4_OVF     TIM_FLAG_CC4OF
uint16_t timer_state(TIM_TypeDef*);//check the timer, return TIMx->SR
void icp_all_clear(TIM_TypeDef*);

#define ICP_OVF         0x0100
#define ICP_VALID       0x0001
uint16_t icp_check(timer_channel ch);

uint32_t icp_get(timer_channel ch);
void icp_off(timer_channel ch);
 
//TIM6,7 cannot attach icp callback
//TIM9,12 only support channel 1 & 2
//TIM10,11 only support channel 1
void icp_attach(timer_channel ch, void (*callback)(uint32_t));//to detach, use attach(ch,NULL);

#ifdef __cplusplus
}
#endif

#endif

