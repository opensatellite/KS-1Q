#include <os.h>

#include <stdio.h>
#include <string.h>

#include <cmsis_os.h>
#include "bsp.h"

#include <sfud.h>
#include "spiffs.h"

#include "spi_flash.h"

#include "ff.h"
#include "sdcard.h"
#include "dm.h"

#include "imumagd.h"
#include "memory_thread.h"
#include "camshot_thread.h"
#include "ftp_thread.h"

#include "LIS3MDL.h"

osMutexId mutex_rom;

osThreadId thread_mem;
osThreadId thread_dm;
osThreadId thread_imu;
osThreadId thread_cam;
osThreadId thread_ftp;

void libcsp_init()
{
	struct csp_can_config can_conf = { .bitrate = 1000000, .ifc = "CAN" };
	
	csp_buffer_init(10, 300);
	csp_init(OBC_ADDR);

	csp_can_init(CSP_CAN_PROMISC, &can_conf);
	
	csp_route_set(TTC_ADDR, &csp_if_can, CSP_NODE_MAC);
	csp_route_set(TNC_ADDR, &csp_if_can, CSP_NODE_MAC);
	csp_route_set(EPS_ADDR, &csp_if_can, CSP_NODE_MAC);
	csp_route_set(CSP_BROADCAST_ADDR, &csp_if_can, CSP_NODE_MAC);
	
	csp_route_start_task(512, 5);
	
	csp_debug_toggle_level(0);
	csp_debug_toggle_level(1);
	csp_debug_toggle_level(2);
	csp_debug_toggle_level(3);
	csp_debug_toggle_level(4);
	csp_debug_toggle_level(5);
	csp_debug_toggle_level(6);
}

//FIL csp_log;

void dm_thread(void *args)
{
	
	//openfile(&csp_log);
	
	csp_socket_t* sock = csp_socket(CSP_SO_CONN_LESS);
	
	csp_bind(sock, CSP_ANY);
	
	for(;;)
	{
		csp_packet_t* packet = csp_recvfrom(sock,1000);
		if(packet == NULL)
		{
			continue;
		}
		/*
		if((dm_var.memstatus == 1) && (dm_var.memavail == 1))
		{
			sdcard_wlog(&csp_log,packet->data,packet->length);
		}	
		*/
		
		if(dm_reply(packet) == 1)
		{
			csp_buffer_free(packet);
		}
		osDelay(100);
	}
}

int main(int argc, char * argv[]) {
	
	bsp_init();
	
	
	libcsp_init();
	
	//osMutexDef(Mutex_ROM);
	//mutex_rom = osMutexCreate(osMutex(Mutex_ROM));
	
	 //mem_init();
	
	//osThreadDef(MEM,mem_thread,osPriorityNormal,osPriorityHigh,512);
	//thread_mem = osThreadCreate (osThread(MEM), NULL);
	
	//osThreadDef(IMU,imumagd_thread,1,1,512);
	//thread_imu = osThreadCreate (osThread(IMU), NULL);
	
	//osThreadDef(DM,dm_thread,3,1,512);
	//thread_dm = osThreadCreate (osThread(DM), NULL);
	
	//osThreadDef(CAM,camshot_thread,2,1,512);
	//thread_cam = osThreadCreate (osThread(CAM), NULL);
	
	//osThreadDef(FTP,ftp_thread,osPriorityLow,osPriorityHigh,512);
	//thread_cam = osThreadCreate (osThread(FTP), NULL);
	
	xTaskCreate(imumagd_thread,"IMU",512,NULL,1,NULL);
	xTaskCreate(dm_thread,"DM",512,NULL,3,NULL);
	xTaskCreate(camshot_thread,"CAM",512,NULL,2,NULL);
	
	kprintf("Starting RTOS... ");
    osKernelStart();	
	

	return 0;
}

void vApplicationIdleHook()
{
	//__WFI();
}

void vApplicationTickHook() 
{
	//disk_timerproc();
}

