#include <os.h>
#include "bsp.h"
#include "can_stm32.h"
#include "csp/drivers/can.h"

#include <stdio.h>
#include <inttypes.h>

#include <FreeRTOS.h>
#include <task.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_can.h>

#include <stm32f4xx_it.h>

uint32_t can_id;
uint32_t can_mask;
uint32_t bitrate;

static uint32_t bitrate;

static uint32_t can_bitratecofig(uint32_t bitrate)
{
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);

	return (RCC_Clocks.PCLK1_Frequency / bitrate / 6);
}

int can_init(uint32_t id, uint32_t mask, struct  csp_can_config *conf)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	bitrate = conf->bitrate;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1|RCC_APB1Periph_CAN2,ENABLE);
	
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = can_bitratecofig(bitrate);
	CAN_Init(CAN2, &CAN_InitStructure);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);

	//IDMASK
	//TNC_ADDR [29:25]  OBC_ADDR[24:20]
	//EPS_ADDR [29:25]  OBC_ADDR[24:20]	
	CAN_FilterInitStructure.CAN_FilterNumber = 14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//CAN_FilterInitStructure.CAN_FilterIdHigh = (EPS_ADDR<<CSP_ID_HOST_SIZE|OBC_ADDR)<<4;
	//CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x3FF0;
	//CAN_FilterInit(&CAN_FilterInitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
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
		mail_box = CAN_Transmit(CAN2, &TxMessage);
		osDelay(10);
	}
	while(mail_box == 4);
	
	//while (CAN_TransmitStatus(CAN1, mail_box) != CANTXOK);
	return 0;
}

void CAN2_RX1_IRQHandler(void)
{
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
	
	CanRxMsg RxMessage;
	can_frame_t rx_buf;
	static portBASE_TYPE xTaskWoken = pdFALSE;

	CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);

	rx_buf.id = RxMessage.ExtId;
	rx_buf.dlc = RxMessage.DLC;
	for (uint8_t i = 0; i < 8; i++)
	{
		rx_buf.data[i] = RxMessage.Data[i];
	}
	csp_can_rx_frame(&rx_buf, &xTaskWoken);
	
}

