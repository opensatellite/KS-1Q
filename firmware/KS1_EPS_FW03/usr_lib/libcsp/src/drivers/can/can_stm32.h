#ifndef CAN_STM32_H
#define CAN_STM32_H

#include "stm32f10x.h"
#include "csp/drivers/can.h"

#include <stdio.h>
#include <inttypes.h>

#include <FreeRTOS.h>
#include <task.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_can.h>

int can_init(uint32_t id, uint32_t mask, struct  csp_can_config *conf);
int can_send(can_id_t id, uint8_t * data, uint8_t dlc);


#endif
