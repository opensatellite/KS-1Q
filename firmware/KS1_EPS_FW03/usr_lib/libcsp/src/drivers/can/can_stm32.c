//#include <os.h>
#include "bsp.h"
#include "can_stm32.h"
#include "csp/drivers/can.h"

#include <stdio.h>
#include <inttypes.h>

#include <FreeRTOS.h>
#include <task.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_can.h>

#include <stm32f10x_it.h>

#include <cmsis_os.h>

uint32_t can_id;
uint32_t can_mask;
uint32_t bitrate;

static uint32_t bitrate;

static uint32_t can_bitratecofig(uint32_t bitrate)
{
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);

	return (RCC_Clocks.PCLK1_Frequency / bitrate / 8);
}

int can_init(uint32_t id, uint32_t mask, struct  csp_can_config *conf)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	bitrate = conf->bitrate;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = can_bitratecofig(bitrate);
	CAN_Init(CAN1, &CAN_InitStructure);

	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);

	//IDMASK
	//filter owner id and boardcast id
	/*
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = id>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x00f8;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x00f8;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x00f8;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 2;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x00f8;
	CAN_FilterInit(&CAN_FilterInitStructure);
*/

  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	return 0;
};

int can_send(can_id_t id, uint8_t * data, uint8_t dlc)
{
	uint8_t mail_box;
	CanTxMsg TxMessage;

	TxMessage.StdId = 0;
	TxMessage.ExtId = id;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.DLC = dlc;

	for (uint8_t i = 0; i < dlc; i++)
	{
		TxMessage.Data[i] = data[i];
	}

	do
	{
		for(uint8_t i=0xff;i>0;i--);
		mail_box = CAN_Transmit(CAN1, &TxMessage);
	}
	while(mail_box == CAN_TxStatus_NoMailBox);
	//while (CAN_TransmitStatus(CAN1, mail_box) != CANTXOK);
	return 0;
}

void CAN1_RX0_IRQHandler(void)
{
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	
	CanRxMsg RxMessage;
	can_frame_t rx_buf;
	static portBASE_TYPE xTaskWoken = pdFALSE;

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	rx_buf.id = RxMessage.ExtId;
	rx_buf.dlc = RxMessage.DLC;
	for (uint8_t i = 0; i < 8; i++)
	{
		rx_buf.data[i] = RxMessage.Data[i];
	}
	csp_can_rx_frame(&rx_buf, &xTaskWoken);
	
	//if(xTaskWoken)
	//{
		//vTaskYield();
	//}
	
}

