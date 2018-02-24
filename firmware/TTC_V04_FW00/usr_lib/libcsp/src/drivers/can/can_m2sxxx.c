#include <stdio.h>
#include <inttypes.h>

#include <mss_can/mss_can.h>

#include <FreeRTOS.h>
#include <task.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_can.h>

#include "csp/drivers/can.h"
#include "can_m2sxxx.h"

/** Identifier and mask */
uint32_t can_id;
uint32_t can_mask;

/** Mailbox */
typedef enum
{
	MBOX_FREE = 0, MBOX_USED = 1,
} mbox_t;

int can_init(uint32_t id, uint32_t mask, struct csp_can_config *conf)
{

	//csp_assert((conf && conf->bitrate && conf->clock_speed));

	/* Set id and mask */
	can_mask = mask;

	/* Set callbacks */

	INTERRUPT_CTRL_BITBAND->CAN_INTR_ENBL = 1;

        uint8_t ret = MSS_CAN_init(&g_can0,
                     conf->bitrate | CAN_AUTO_RESTART | CAN_LITTLE_ENDIAN,
                     (PCAN_CONFIG_REG)0,
                     32, // Number of Basic CAN rx mailboxes
                     32  // Number of Basid CAN tx mailboxes
        );
        if(ret) return ret;
        
	MSS_CAN_set_int_ebl(&g_can0, CAN_INT_RX_MSG_LOST);
	//MSS_CAN_set_int_ebl(&g_can0, CAN_INT_STUCK_AT_0);
        MSS_CAN_set_mode(&g_can0, CANOP_MODE_NORMAL);
        
        CAN_FILTEROBJECT pFilter;
        /* Configure for receive */
        pFilter.ACR.L=0x00000000 ;
        pFilter.AMR.L= 0xFFFFFFFF;
        pFilter.AMCR_D.MASK= 0xFFFF;
        pFilter.AMCR_D.CODE= 0x00;
        ret = MSS_CAN_config_buffer(&g_can0, &pFilter);
        if(ret) return ret;
        
	MSS_CAN_start(&g_can0);

	return 0;
}

int can_send(can_id_t id, uint8_t * data, uint8_t dlc)
{
	CAN_MSGOBJECT pMsg;
	int i;

	pMsg.ID = (uint32_t) id;
	pMsg.IDE = 1;
	pMsg.DLC = dlc;

	for (i = 0; i < dlc; i++)
	{
		pMsg.DATA[i] = data[i];
	}

	//while (CAN_OK != MSS_CAN_send_message_ready(&g_can0))
        while(MSS_CAN_get_tx_buffer_status(&g_can0) == 0xffffffff)
		vTaskDelay(1);
        
        //printf("T:%08x\n", pMsg.ID);
	int ret = MSS_CAN_send_message(&g_can0, &pMsg);
        if(ret == CAN_VALID_MSG) return 0;
        else return 1;
}

void CAN_IRQHandler()
{
	uint32_t irq;
	can_frame_t rx_buf;
	CAN_MSGOBJECT pMsg;
	portBASE_TYPE xTaskWoken = pdFALSE;

	irq = MSS_CAN_get_int_ebl(&g_can0);
        /*
        if (irq & CAN_INT_GLOBAL) printf("can interrupt flag:CAN_INT_GLOBAL\r\n\n");
        if (irq & CAN_INT_ARB_LOSS) printf("can interrupt flag:CAN_INT_ARB_LOSS\r\n\n");    
        if (irq & CAN_INT_OVR_LOAD) printf("can interrupt flag:CAN_INT_OVR_LOAD\r\n\n");    
        if (irq & CAN_INT_BIT_ERR) printf("can interrupt flag:CAN_INT_BIT_ERR\r\n\n");       
        if (irq & CAN_INT_STUFF_ERR) printf("can interrupt flag:CAN_INT_STUFF_ERR\r\n\n");  
        if (irq & CAN_INT_ACK_ERR) printf("can interrupt flag:CAN_INT_ACK_ERR\r\n\n");   
        if (irq & CAN_INT_FORM_ERR) printf("can interrupt flag:CAN_INT_FORM_ERR\r\n\n"); 
        if (irq & CAN_INT_CRC_ERR)  printf("can interrupt flag:CAN_INT_CRC_ERR\r\n\n");
        if (irq & CAN_INT_BUS_OFF)  printf("can interrupt flag:CAN_INT_BUS_OFF\r\n\n");    
        if (irq & CAN_INT_RX_MSG_LOST)  printf("can interrupt flag:CAN_INT_RX_MSG_LOST\r\n\n");    
        if (irq & CAN_INT_TX_MSG)  printf("can interrupt flag:CAN_INT_TX_MSG\r\n\n");       
        if (irq & CAN_INT_RX_MSG)  printf("can interrupt flag:CAN_INT_RX_MSG\r\n\n");  
        if (irq & CAN_INT_RTR_MSG)  printf("can interrupt flag:CAN_INT_RTR_MSG\r\n\n");
        if (irq & CAN_INT_STUCK_AT_0)  printf("can interrupt flag:CAN_INT_STUCK_AT_0\r\n\n");   
        if (irq & CAN_INT_SST_FAILURE)  printf("can interrupt flag:CAN_INT_SST_FAILURE\r\n\n");
        */
        
        
        if (irq & CAN_INT_RX_MSG_LOST)
          MSS_CAN_clear_int_status(&g_can0, CAN_INT_RX_MSG_LOST);
        
        //if (irq & CAN_INT_STUCK_AT_0)
        //  MSS_CAN_clear_int_status(&g_can0, CAN_INT_STUCK_AT_0);
        
	if (irq & CAN_INT_RX_MSG)
	{
          int k = 0;
          while (MSS_CAN_get_rx_buffer_status(&g_can0)) {
            uint8_t ret = MSS_CAN_get_message(&g_can0, &pMsg);
            if(ret != CAN_VALID_MSG) break;
            
            rx_buf.id = pMsg.ID;
            rx_buf.dlc = pMsg.DLC;
            for (uint8_t i = 0; i < 8; i++)
            {
              rx_buf.data[i] = pMsg.DATA[i];
            }
            
            k++;
            //printf("R:%d,%08x\n", k, pMsg.ID);
            csp_can_rx_frame((can_frame_t *) &rx_buf, &xTaskWoken);
          }
          MSS_CAN_clear_int_status(&g_can0, CAN_INT_RX_MSG);
	}
        
        if( xTaskWoken )
	{
		taskYIELD ();
	}
}
