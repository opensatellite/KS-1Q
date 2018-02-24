
#ifndef _ADC_TASK_H_
#define _ADC_TASK_H_

#include "bsp.h"
#include "si4463.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "tlv2548.h"
#include "m2sxxx.h"

void adc_task(void *pvParameters);

#endif

