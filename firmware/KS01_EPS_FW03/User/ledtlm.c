#include "ledtlm.h"
#include "bsp.h"
#include "pin.h"

#include "cmsis_os.h"

#include <csp/csp.h>

osMessageQId  led_packet;

struct ledtlm_s ledtlm_d =
{
	.ptr = 0,
	.num = 0
};

void ledtlm_start() {
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_Period = 6000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
  
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM1->CNT = 0;
	TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
	static uint8_t count = 0;
	static uint8_t temp = 0;
	static int8_t bit_read = -1;
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	
	if(ledtlm_d.num < 0)
	{
		count = 0;
		bit_read = -1;
		temp = 0;
		
		ledtlm_d.ptr = 0;
		
		pinlow(LED_ON_PIN);
		TIM_Cmd(TIM1, DISABLE);
		return;
	}
	
	if(bit_read < 0)
	{
		temp = ledtlm_d.data[ledtlm_d.ptr++];
		ledtlm_d.num --;
		bit_read = 0;
	}
	
	static bool first = TRUE;
	if(first)
	{
		if(temp&0x80)
		{
			/*
			one cycle 3200
			20% cycle = 640
			*/
			pinhigh(LED_ON_PIN);
			TIM1->CNT = 640;
		}
		else
		{
			/*
			one cycle 3200
			5% cycle = 160
			*/
			pinhigh(LED_ON_PIN);
			TIM1->CNT = 160;
		}
		first = FALSE;
	}
	else
	{
		if(temp&0x80)
		{
			/*
			one cycle 3200
			80% cycle = 2560
			*/
			pinlow(LED_ON_PIN);
			TIM1->CNT = 500;
		}
		else
		{
			/*
			one cycle 3200
			95% cycle = 3040
			*/
			pinlow(LED_ON_PIN);
			TIM1->CNT = 3040;
		}
		first = TRUE;
	}
	count ++;
	if(count >= 50) {
		count = 0;
		temp <<= 1;
		bit_read ++;
		if(bit_read >= 8)
		{
			bit_read = -1;	
		}
	}
	
}