#include "rf_rx_task.h"
#include "APB3_reg.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "StackMacros.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "dm.h"

#include "m2sxxx.h"

csp_iface_t csp_if_kiss;
csp_kiss_handle_t csp_kiss_driver;
TTC_Rx_FIFO_Typedef TTC_RxFIFO;
uint8_t rx_active = 0;

void FabricIrq0_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(FabricIrq0_IRQn);
	
	
}

uint32_t FLAG;

void rf_rx_task(void *pvParameters)
{
	uint8_t msg;

	for (;;)
	{
		if (TTC->FLAG & RF_RxA )
		{
                        memcpy32((uint32_t *) RX_FIFOA_BASE, (uint32_t *)&TTC_RxFIFO, sizeof(TTC_Rx_FIFO_Typedef) / 4);
			csp_kiss_rx(&csp_if_kiss, (uint8_t *)TTC_RxFIFO.data8, 25, NULL);
			TTC->FLAG &= ~RF_RxA;
                        dm_var.rxpkts++;
		}
		if (TTC->FLAG & RF_RxB  )
		{
			memcpy32((uint32_t *) RX_FIFOB_BASE, (uint32_t *)&TTC_RxFIFO, sizeof(TTC_Rx_FIFO_Typedef) / 4);
			csp_kiss_rx(&csp_if_kiss, (uint8_t *)TTC_RxFIFO.data8, 25, NULL);
			TTC->FLAG &= ~RF_RxB;
                        dm_var.rxpkts++;
		}
		vTaskDelay(10);
                rx_active = 1;
	}
}
