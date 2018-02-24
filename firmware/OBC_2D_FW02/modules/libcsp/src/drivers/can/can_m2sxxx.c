#include <stdio.h>
#include <inttypes.h>

#include <mss_can/mss_can.h>

#include <FreeRTOS.h>
#include <task.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_can.h>

#include "csp/drivers/can.h"
#include "can_m2sxxx.h"

/* MOB segmentation */
#define CAN_RX_MOBS 16
#define CAN_TX_MOBS 16


/** Identifier and mask */
uint32_t can_id;
uint32_t can_mask;

/* fcpu and bitrate */
//static unsigned long int clock_speed;
static uint32_t bitrate;

/** Mailbox */
typedef enum {
	MBOX_FREE = 0,
	MBOX_USED = 1,
} mbox_t;

int can_init(uint32_t id, uint32_t mask, struct csp_can_config *conf)
{

	//csp_assert((conf && conf->bitrate && conf->clock_speed));

	/* Set id and mask */
	can_mask = mask;

	/* Set callbacks */

	/* Set fcpu and bps */
	bitrate = conf->bitrate;

	MSS_CAN_stop(&g_can0);

	MSS_CAN_init
    (
        &g_can0,
		bitrate | CAN_AUTO_RESTART | CAN_LITTLE_ENDIAN,
        (PCAN_CONFIG_REG)0,
        CAN_RX_MOBS, // Number of Basic CAN rx mailboxes
        CAN_TX_MOBS // Number of Basic CAN tx mailboxes
    );

	MSS_CAN_start(&g_can0);
	
	return 0;
}

int can_send(can_id_t id, uint8_t * data, uint8_t dlc)
{
	CAN_MSGOBJECT pMsg;
	int i;

	pMsg.ID = (uint32_t)id;
	pMsg.IDE = 1;
	pMsg.DLC = dlc;

	for(i=0;i<dlc;i++)
	{
		pMsg.DATA[i] = data[i];
	}

	while(CAN_OK != MSS_CAN_send_message_ready(&g_can0));
	if(MSS_CAN_send_message(&g_can0, &pMsg))
	{
		//Report as send message error for basic CAN
		return 0;
	}
	return -1;
}

void CAN_IRQHandler(void)
{
	uint32_t irq;
	can_frame_t rx_buf;
	CAN_MSGOBJECT pMsg;
	//static portBASE_TYPE xTaskWoken = pdFALSE;

	irq = MSS_CAN_get_int_ebl(&g_can0);

	if(irq == CAN_INT_RX_MSG)
	{
		while(CAN_VALID_MSG != MSS_CAN_get_message_av(&g_can0));
		MSS_CAN_get_message(&g_can0, &pMsg);

		rx_buf.id = pMsg.ID;
		rx_buf.dlc = pMsg.DLC;
		for(uint8_t i = 0;i<8;i++)
		{
			rx_buf.data[i] = pMsg.DATA[i];
		}

		csp_can_rx_frame((can_frame_t *)&rx_buf, NULL);
	}
}
