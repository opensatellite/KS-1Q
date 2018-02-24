#ifndef RF_RX_TASK_H
#define RF_RX_TASK_H

#include "APB3_reg.h"

#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>

#include <csp/interfaces/csp_if_kiss.h>

#define RF_RX_CMP 0x01

extern QueueHandle_t rf_rx_queue;
extern csp_iface_t csp_if_kiss;
extern csp_kiss_handle_t csp_kiss_driver;

void rf_rx_task(void *pvParameters);

#endif
