#ifndef RF_TX_TASK_H
#define RF_TX_TASK_H

#include "APB3_reg.h"

#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>

#include <csp/interfaces/csp_if_kiss.h>
#include <csp/drivers/tmp.h>

//#define RF_TX_CMP	0x01

//extern QueueHandle_t rf_tx_queue;

//extern TTC_FIFO_Typedef TTC_Tx_FIFO;

void rf_tx_task(void *pvParameters);
extern uint8_t tx_on;



#endif
