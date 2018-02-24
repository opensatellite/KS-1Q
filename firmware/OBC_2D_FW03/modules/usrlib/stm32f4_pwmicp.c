#include "stm32f4_pwmicp.h"

void __tim_clk_cmd(TIM_TypeDef *tim,FunctionalState x)
{
    assert_param(IS_TIMER(tim));
    
    if(tim == TIM1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, x);
    
    else if(tim == TIM2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, x);
    else if(tim == TIM3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, x);
    else if(tim == TIM4)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, x);
    else if(tim == TIM5)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, x);
    else if(tim == TIM6)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, x);
    else if(tim == TIM7)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, x);
    
    else if(tim == TIM8)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, x);
    else if(tim == TIM9)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, x);
    else if(tim == TIM10)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, x);
    else if(tim == TIM11)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, x);
    
    else if(tim == TIM12)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, x);
    else if(tim == TIM13)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, x);
    else if(tim == TIM14)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, x);
    
    else;
}

uint8_t __tim_x(TIM_TypeDef* tim)
{
    assert_param(IS_TIMER(tim));
    
    if(tim == TIM1)
        return 1;
    else if(tim == TIM2)
        return 2;
    else if(tim == TIM3)
        return 3;
    else if(tim == TIM4)
        return 4;
    else if(tim == TIM5)
        return 5;
    else if(tim == TIM6)
        return 6;
    else if(tim == TIM7)
        return 7;
    else if(tim == TIM8)
        return 8;
    else if(tim == TIM9)
        return 9;
    else if(tim == TIM10)
        return 10;
    else if(tim == TIM11)
        return 11;
    else if(tim == TIM12)
        return 12;
    else if(tim == TIM13)
        return 13;
    else if(tim == TIM14)
        return 14;
    else return 0;
}

void timer_clock_cntmode(TIM_TypeDef* tim , uint16_t prsc, uint32_t period, uint16_t cntmode)
{
    __tim_clk_cmd(tim,ENABLE);
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler = prsc - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = cntmode;
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(tim,&TIM_TimeBaseStructure);
    
    TIM_ARRPreloadConfig(tim,ENABLE);
    TIM_Cmd(tim, ENABLE);
}

void timer_noclock(TIM_TypeDef* tim)
{
    __tim_clk_cmd(tim,DISABLE);
}

void __timer_preprescaler(TIM_TypeDef* tim, uint16_t TIM_CKD_DIV)
{
    assert_param(IS_TIMER(tim));
    assert_param(IS_TIM_CKD_DIV(TIM_CKD_DIV));
    uint16_t tmpreg = tim->CR1;
    tmpreg &= ~0x0300;
    tmpreg |= TIM_CKD_DIV;
    tim->CR1 = tmpreg;
}

uint32_t __get_APBx_freq(uint8_t x)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    if(x == 1)
    {
        if(clocks.SYSCLK_Frequency == clocks.PCLK1_Frequency)
            return clocks.PCLK1_Frequency;
        else
            return clocks.PCLK1_Frequency*2;
    }
    if(x == 2)
    {
        if(clocks.SYSCLK_Frequency == clocks.PCLK2_Frequency)
            return clocks.PCLK2_Frequency;
        else
            return clocks.PCLK2_Frequency*2;
    }
    return 0;
}

uint32_t __timer_getbasefreq(TIM_TypeDef* tim)
{
    assert_param(IS_TIMER(tim));
    
    uint8_t x = __tim_x(tim);
    
    if((x >= 2 && x <= 7) || (x >= 12 && x <= 14))
    {
        return __get_APBx_freq(1);
    }
    else if(x == 1 || (x >= 8 && x <= 11))
    {
        return __get_APBx_freq(2);
    }
    else return 0;
}

void __pwm_set(timer_channel ch,uint8_t mode,bool comp,uint8_t deadcode)
{
    TIM_OCInitTypeDef   TIM_OCInitStructure;
    
    assert_param(IS_PWM_CHANNEL(ch));
    assert_param(IS_PWM_MODE(mode));
    
    TIM_OCInitStructure.TIM_OCMode = mode == 1 ? TIM_OCMode_PWM1 : TIM_OCMode_PWM2;
    if(comp)
    {
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    }
    else
    {
        TIM_OCInitStructure.TIM_OutputState = ch.oc > 0 ? TIM_OutputState_Enable : TIM_OutputState_Disable;
        TIM_OCInitStructure.TIM_OutputNState = ch.oc < 0 ? TIM_OutputNState_Enable : TIM_OutputNState_Disable;
    }
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    
    switch(ch.oc)
    {
    case 1:case -1:
        TIM_OC1PreloadConfig(ch.tim, TIM_OCPreload_Enable);
        TIM_OC1Init(ch.tim,&TIM_OCInitStructure);
        break;
    case 2:case -2:
        TIM_OC2PreloadConfig(ch.tim, TIM_OCPreload_Enable);
        TIM_OC2Init(ch.tim,&TIM_OCInitStructure);
        break;
    case 3:case -3:
        TIM_OC3PreloadConfig(ch.tim, TIM_OCPreload_Enable);
        TIM_OC3Init(ch.tim,&TIM_OCInitStructure);
        break;
    case 4:case -4:
        TIM_OC4PreloadConfig(ch.tim, TIM_OCPreload_Enable);
        TIM_OC4Init(ch.tim,&TIM_OCInitStructure);
        break;
    default:
        return;
    }
    
    if(ch.tim == TIM1 || ch.tim == TIM8)  
        TIM_CtrlPWMOutputs(ch.tim, ENABLE);
}

void pwm_out(timer_channel ch, uint32_t v)
{
    assert_param(IS_TIMER(ch.tim));
    assert_param(IS_PWM_CHANNEL(ch));
    uint8_t offset = ch.oc > 0 ? ch.oc : -ch.oc;
    __IO uint32_t* oc_reg = (uint32_t*)&(ch.tim->RCR);
    *(oc_reg + offset) = v;
}

void pwm_off(timer_channel ch)
{
    assert_param(IS_PWM_CHANNEL(ch));
    const uint16_t table[] = {TIM_Channel_1,TIM_Channel_2,TIM_Channel_3,TIM_Channel_4};
    if(ch.oc > 0) TIM_CCxCmd(ch.tim,  table[ch.oc-1],  TIM_CCx_Disable);
    if(ch.oc < 0) TIM_CCxNCmd(ch.tim, table[-ch.oc-1], TIM_CCxN_Disable);
}

uint16_t __icp_getprescaler(uint16_t prescaler)
{
    assert_param(IS_ICP_PRESCALER(prescaler));
    
    switch(prescaler)
    {
    case 1:
        return TIM_ICPSC_DIV1;
    case 2:
        return TIM_ICPSC_DIV2;
    case 4:
        return TIM_ICPSC_DIV4;
    case 8:
        return TIM_ICPSC_DIV8;
    default:
        return TIM_ICPSC_DIV1;
    }
}

void icp_set(timer_channel ch, uint16_t edge, uint16_t prescaler, uint8_t filter)
{
    assert_param(IS_TIMER(ch.tim));
    assert_param(IS_ICP_CHANNEL(ch));
    assert_param(IS_ICP_PRESCALER(prescaler));
    
    uint16_t icpch[4] = {TIM_Channel_1,TIM_Channel_2,TIM_Channel_3,TIM_Channel_4};
    
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = icpch[ch.oc - 1];
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPolarity = edge;
    TIM_ICInitStructure.TIM_ICPrescaler = __icp_getprescaler(prescaler);
    TIM_ICInitStructure.TIM_ICFilter = filter; 
    TIM_ICInit(ch.tim,&TIM_ICInitStructure);
}

void icp_off(timer_channel ch)
{
    assert_param(IS_ICP_CHANNEL(ch));
    uint16_t icpch[4] = {TIM_Channel_1,TIM_Channel_2,TIM_Channel_3,TIM_Channel_4};
    TIM_CCxCmd(ch.tim,icpch[ch.oc-1],TIM_CCx_Disable);
}

uint16_t timer_state(TIM_TypeDef* tim)
{
    assert_param(IS_TIMER(tim));
    return tim->SR;
}

uint16_t icp_check(timer_channel ch)
{
    assert_param(IS_ICP_CHANNEL(ch));
    return (ch.tim->SR >> ch.oc) & 0x0101;
}

void icp_all_clear(TIM_TypeDef* tim)
{
    assert_param(IS_TIMER(tim));
    TIM_ClearFlag(tim,ICP_STATE_MASK);
}

uint32_t icp_get(timer_channel ch)
{
    assert_param(IS_TIMER(ch.tim));
    assert_param(IS_ICP_CHANNEL(ch));
    __IO uint32_t* oc_reg = (uint32_t*)&(ch.tim->RCR);
    return *(oc_reg + ch.oc);
}

#define TIMERS  14
#define CHANNELS_PER_TIMER  4
void (*icp_callbacks[TIMERS*CHANNELS_PER_TIMER])(uint32_t) = {0,};

IRQn_Type __icp_getnvicirq(TIM_TypeDef *tim)
{
    assert_param(IS_TIMER(tim));
    
    if(tim == TIM1)
        return TIM1_CC_IRQn;
    else if(tim == TIM2)
        return TIM2_IRQn;
    else if(tim == TIM3)
        return TIM3_IRQn;
    else if(tim == TIM4)
        return TIM4_IRQn;
    else if(tim == TIM5)
        return TIM5_IRQn;
    else if(tim == TIM6)
        return TIM6_DAC_IRQn;
    else if(tim == TIM7)
        return TIM7_IRQn;
    else if(tim == TIM8)
        return TIM8_CC_IRQn;
    else if(tim == TIM9)
        return TIM1_BRK_TIM9_IRQn;
    else if(tim == TIM10)
        return TIM1_UP_TIM10_IRQn;
    else if(tim == TIM11)
        return TIM1_TRG_COM_TIM11_IRQn;
    else if(tim == TIM12)
        return TIM8_BRK_TIM12_IRQn;
    else if(tim == TIM13)
        return TIM8_UP_TIM13_IRQn;
    else if(tim == TIM14)
        return TIM8_TRG_COM_TIM14_IRQn;
    else
        return (IRQn_Type)0xff;
}

void icp_attach(timer_channel ch, void (*callback)(uint32_t))
{
    assert_param(IS_TIMER(ch.tim));
    assert_param(IS_ICP_CHANNEL(ch));
#ifdef USE_FULL_ASSERT
    //TIM6,7 cannot attach icp callback
    //TIM9,12 only support channel 1 & 2
    //TIM10,11 only support channel 1
    if(ch.tim == TIM6 || ch.tim == TIM7)
        assert_param(0);
    else if(ch.tim == TIM9 || ch.tim == TIM12)
    {
        if(ch.oc != 1 && ch.oc != 2)
            assert_param(0);
        else if(ch.tim == TIM10 || ch.tim == TIM11)
        {
            if(ch.oc != 1) assert_param(0);
        }
        else;
    }
#endif
    icp_callbacks[(__tim_x(ch.tim)-1) * CHANNELS_PER_TIMER + ch.oc - 1] = callback;
    uint16_t tim_it[4] = {TIM_IT_CC1,TIM_IT_CC2,TIM_IT_CC3,TIM_IT_CC4};
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannel = __icp_getnvicirq(ch.tim);
    
    if(callback)
    {
        TIM_ITConfig(ch.tim,tim_it[ch.oc-1],ENABLE);
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    }
    else
    {
        TIM_ITConfig(ch.tim,tim_it[ch.oc-1],DISABLE);
        NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    }
    NVIC_Init(&NVIC_InitStructure);
}

#define ITSTATE(TIM)    ((TIM)->SR&(TIM)->DIER)
#define ICP_CALL(X,CH,CCREG)  {if(icp_callbacks[(X-1)*CHANNELS_PER_TIMER+CH-1]) {(*icp_callbacks[(X-1)*CHANNELS_PER_TIMER+CH-1])(CCREG);}}

void TIM1_CC_IRQHandler(void)          //TIM1    
{
    uint16_t status = ITSTATE(TIM1);
    if(status & ICP_CH1_VALID)
        ICP_CALL(1,1,TIM1->CCR1);
    if(status & ICP_CH2_VALID)
        ICP_CALL(1,2,TIM1->CCR2);
    if(status & ICP_CH3_VALID)
        ICP_CALL(1,3,TIM1->CCR3);
    if(status & ICP_CH4_VALID)
        ICP_CALL(1,4,TIM1->CCR4);
    //icp_clear(TIM1);
}

void TIM2_IRQHandler(void)             //TIM2   
{
    uint16_t status = ITSTATE(TIM2);
    if(status & ICP_CH1_VALID)
        ICP_CALL(2,1,TIM2->CCR1);
    if(status & ICP_CH2_VALID)
        ICP_CALL(2,2,TIM2->CCR2);
    if(status & ICP_CH3_VALID)
        ICP_CALL(2,3,TIM2->CCR3);
    if(status & ICP_CH4_VALID)
        ICP_CALL(2,4,TIM2->CCR4);
    //icp_clear(TIM2);
}

void TIM3_IRQHandler(void)             //TIM3         
{
    uint16_t status = ITSTATE(TIM3);
    if(status & ICP_CH1_VALID)
        ICP_CALL(3,1,TIM3->CCR1);
    if(status & ICP_CH2_VALID)
        ICP_CALL(3,2,TIM3->CCR2);
    if(status & ICP_CH3_VALID)
        ICP_CALL(3,3,TIM3->CCR3);
    if(status & ICP_CH4_VALID)
        ICP_CALL(3,4,TIM3->CCR4);
    //icp_clear(TIM3);
}

void TIM4_IRQHandler(void)             //TIM4       
{
    
}

void TIM5_IRQHandler(void)             //TIM5             
{
    
}

void TIM6_DAC_IRQHandler(void)         //TIM6 and DAC1&2 underrun errors     
{
    
}

void TIM7_IRQHandler(void)             //TIM7                   
{
    
}

void TIM8_CC_IRQHandler(void)          //TIM8 Capture Compare               
{
    uint16_t status = ITSTATE(TIM8);
    if(status & ICP_CH1_VALID)
        ICP_CALL(8,1,TIM8->CCR1);
    if(status & ICP_CH2_VALID)
        ICP_CALL(8,2,TIM8->CCR2);
    if(status & ICP_CH3_VALID)
        ICP_CALL(8,3,TIM8->CCR3);
    if(status & ICP_CH4_VALID)
        ICP_CALL(8,4,TIM8->CCR4);
    //icp_clear(TIM8);
}

void TIM1_BRK_TIM9_IRQHandler(void)    //TIM1 Break and TIM9      
{
    uint16_t status = ITSTATE(TIM9);
    if(status & ICP_CH1_VALID)
        ICP_CALL(9,1,TIM9->CCR1);
    if(status & ICP_CH2_VALID)
        ICP_CALL(9,2,TIM9->CCR2);
    //icp_clear(TIM9);
}

void TIM1_UP_TIM10_IRQHandler(void)    //TIM1 Update and TIM10        
{
    
}

void TIM8_BRK_TIM12_IRQHandler(void)   //TIM8 Break and TIM12          
{
    
}

void TIM8_UP_TIM13_IRQHandler(void)    //TIM8 Update and TIM13          
{
    
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)//TIM8 Trigger and Commutation and TIM14
{
    
}
