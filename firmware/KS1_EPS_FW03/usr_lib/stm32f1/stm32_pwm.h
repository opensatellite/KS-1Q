//header PWM for STM32
#ifndef _STM32_PWM_H__
#define _STM32_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  TIM_TypeDef* tim;
  int8_t oc;
}pwm_channel;

#define T1      TIM1
#define T1C1    (pwm_channel){TIM1,1}
#define C_T1C1  {TIM1,1}

void timer_clock(TIM_TypeDef* tim, uint16_t prsc, uint16_t period);
#define timer_preprescaler(TIM,VAL) __timer_preprescaler(TIM,TIM_CKD_DIV##VAL)
void __timer_preprescaler(TIM_TypeDef* tim, uint16_t TIM_CKD_DIV);
void pwm_set(pwm_channel ch);
void pwm_out(pwm_channel ch, uint16_t v);
void pwm_on (pwm_channel ch);
void pwm_off(pwm_channel ch);

#ifdef __cplusplus
}
#endif

#endif

