#include "m2sxxx.h"
#include "bsp.h"

#include <stdint.h>

#include <stdio.h>

#include "APB3_reg.h"

#include "pkg_cb.h"
#include "csp_task.h"
#include "rf_rx_task.h"
#include "rf_tx_task.h"
#include "adc_task.h"
#include "dm.h"

#include <mss_can/mss_can.h>
#include <mss_watchdog/mss_watchdog.h>

#include <csp/csp.h>

#include <csp/arch/csp_thread.h>

#include <csp/drivers/can.h>
#include <csp/drivers/tmp.h>

#include <csp/interfaces/csp_if_kiss.h>
#include <csp/interfaces/csp_if_can.h>

#include "FreeRTOS.h"
#include "StackMacros.h"
#include "task.h"
#include "queue.h"

struct csp_can_config can_conf =
{ .bitrate = CAN_SPEED_16M_1M, .ifc = "CAN" };

void libcsp_init()
{
	csp_buffer_init(10, 260);
	csp_init(TTC_ADDR);

	csp_can_init(CSP_CAN_PROMISC, &can_conf);

	tmp_init();

	csp_kiss_init(&csp_if_kiss, &csp_kiss_driver, tmp_putc, NULL, "KISS");

	csp_route_set(OBC_ADDR, &csp_if_can, CSP_NODE_MAC);
	csp_route_set(EPS_ADDR, &csp_if_can, CSP_NODE_MAC);
	csp_route_set(TNC_ADDR, &csp_if_kiss, CSP_NODE_MAC);
        csp_route_set(CSP_BROADCAST_ADDR, &csp_if_can, CSP_NODE_MAC);

        //csp_debug_toggle_level(0);
        //csp_debug_toggle_level(1);
        csp_debug_toggle_level(2);
        //csp_debug_toggle_level(3);
        csp_debug_toggle_level(4);
        csp_debug_toggle_level(5);
        //csp_debug_toggle_level(6);
        
	csp_route_start_task(512, ROUTE_PRIO);
}

CSP_DEFINE_TASK(sysmonitor)
{

	for(;;)
	{
          

		//vTaskDelay(1*1000);
		
		//printf("TTC_FLAG is %x\r\n\n",flag);

                //printf("Interfaces\r\n");
                //csp_route_print_interfaces();
                

                
                
                vTaskDelay(200);
		//csp_conn_print_table();
		//csp_route_print_table();
		//csp_route_print_interfaces();
	}
}

void beacon_task(void *pvParameters ){
    uint16_t adc_data[8] = {0,0,0,0,0,0,0,0};
    uint8_t channel_num = 0;
    TickType_t xLastWakeTime = 0, xNewTime = 0 , delayuntil = 0;
    uint32_t time_interval = 0;
    delayuntil = xTaskGetTickCount ();
    	
    csp_socket_t* sock = csp_socket(CSP_SO_CONN_LESS);
    csp_bind(sock, CSP_ANY);
  
  for(;;){
    

    printf("WDog feed!\n");
    MSS_WD_reload();
                
                //feed watchdog
    if(rx_active) {
       rx_active = 0;
    }
    
             //ADC MEASURE 
        for(channel_num=0;channel_num<8;channel_num++)
        {
           adc_data[channel_num]=tlv2548_rchannel((uint16_t)channel_num)>>4;
           while(MSS_GPIO_get_inputs()&0x0400000);
        }
                
         //can error count
        dm_var.canerrs = MSS_CAN_get_rx_error_count(&g_can0);
        
        dm_var.ipa = (uint16_t)IPA_CAL(adc_data[5]);
        dm_var.ittc = (uint16_t)ITTC_CAL(adc_data[0]);
        dm_var.mcutemp = ntc_cal_ttc(adc_data[2]);
        dm_var.patemp = ntc_cal_ttc(adc_data[4]);
                
        dm_var.resets = 0;
        dm_var.rxerrs = 0;

    	dm_var.rssi = si446x_get_modem_status(RF_Rx);
        
        
        
        //request beacon
        csp_packet_t *packet = csp_buffer_get(2);
        packet->length = 2;
        packet->data[0] = 0x0a;
        packet->data[1] = 0x00;
        if(csp_sendto(CSP_PRIO_NORM, CSP_BROADCAST_ADDR, dm_service_port, dm_service_port, 0,  packet, 100 ) != CSP_ERR_NONE){
          csp_buffer_free(packet);
        }
        
        vTaskDelay(10);
        
        printf("rssi is %d\r\n\n",dm_var.rssi);
        printf("Free Memory: %d\n", xPortGetFreeHeapSize());
        printf("Free Minimial: %d\n", xPortGetMinimumEverFreeHeapSize());

        dm_send_bcast(TNC_ADDR);
        
        xNewTime = xTaskGetTickCount ();
        
        time_interval = xNewTime - xLastWakeTime; 
        if(time_interval >= 200){ 
          xLastWakeTime = xNewTime;
          if(dm_var.btimeout > 0) {
            dm_var.btimeout--;
            if(dm_var.binterval < 100) {
              dm_var.binterval = 100;   //minimal interval 100ms
            }
          }
          else {
            dm_var.binterval = 10000;   //default interval 10s
          }
        }
        vTaskDelayUntil( &delayuntil, dm_var.binterval*configTICK_RATE_HZ/1000 );  
  }
  
  
  
}

void dm_task(void *pvParameters) 
{
    csp_socket_t* sock = csp_socket(CSP_SO_CONN_LESS);
    csp_bind(sock, CSP_ANY);
  
    for(;;){
        csp_packet_t* packet = csp_recvfrom(sock, 1000);
	if(packet)
	{
          if(dm_reply(packet) == 1)
	  {
	     csp_buffer_free(packet);
	  }
	}
    }
}

/*
void csp_task_tx(void *parameters)
{
	
	
	for(;;)
	{
		
		csp_packet_t *packet = csp_buffer_get(100);
		
		for(uint8_t i = 0;i<8;i++)
		{
			packet->data[i] = i;
		}
		packet->length = 8;
		
		int result = csp_sendto(CSP_PRIO_HIGH,EPS_ADDR,10,10,CSP_O_NONE,packet,10);
		
		if(result == CSP_ERR_NONE)
		{
			printf("CSP_Send Succeed\n");
		}
		else printf("CSP_Send error\n");
        }
}
*/
/*
int putchar(int x) {
  return x;
}
*/

void can_loopback() {
  
  CAN_FILTEROBJECT pFilter;
  CAN_MSGOBJECT pMsg;
  CAN_MSGOBJECT rx_buf;
  
  pMsg.ID=0x120;
  pMsg.DATALOW = 0x1A2B3C4D;
  pMsg.DATAHIGH = 0xF5E6C7D9;
  pMsg.L =((1<<20)| 0x00080000);
  
  /* Configure for receive */
  pFilter.ACR.L=0x00000000 ;
  pFilter.AMR.L= 0xFFFFFFFF;
  pFilter.AMCR_D.MASK= 0xFFFF;
  pFilter.AMCR_D.CODE= 0x00;
  
  
  MSS_CAN_init(&g_can0,
               CAN_SPEED_16M_1M | CAN_AUTO_RESTART | CAN_LITTLE_ENDIAN,
               (PCAN_CONFIG_REG)0,
               32,
               32);
  
  MSS_CAN_set_mode(&g_can0, CANOP_MODE_EXT_LOOPBACK);
  
  MSS_CAN_start(&g_can0);
  MSS_CAN_clear_int_ebl(&g_can0, CAN_INT_RX_MSG);
  //NVIC_EnableIRQ(CAN_IRQn);
  MSS_CAN_config_buffer(&g_can0, &pFilter);
  
  MSS_CAN_send_message(&g_can0, &pMsg);
  
  while(CAN_VALID_MSG != MSS_CAN_get_message_av(&g_can0));
  MSS_CAN_get_message(&g_can0, &rx_buf);

}

void mscc_post_hw_cfg_init(void)
{
  MSS_WD_init();
}

void main()
{
        //MSS_WD_init();
        
        //can_loopback();
        
        bsp_init();
	libcsp_init();

	csp_thread_create(rf_rx_task,  "RF_RX",  256, NULL, RF_RX_PRIO,  NULL);
	csp_thread_create(rf_tx_task,  "RF_TX",  256, NULL, RF_TX_PRIO,  NULL);
	csp_thread_create(beacon_task, "BEACON", 256, NULL, BEACON_PRIO, NULL);
	csp_thread_create(dm_task,     "DM",     256, NULL, DM_PRIO,     NULL);
       
        //printf("start\n");
        
        dm_var.binterval = 10000;
	vTaskStartScheduler();

}
