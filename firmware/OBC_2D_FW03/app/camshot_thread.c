#include <os.h>
#include "bsp.h"
#include "camshot_thread.h"
#include "sdcard.h"
#include "memory_thread.h"
#include "main.h"
#include "cmsis_os.h"

void campwr_ctrl(uint8_t data)
{
	csp_packet_t* packet = csp_buffer_get(20);
	struct dm_op_s * camshot_cmd = (struct dm_op_s *)packet->data;

	camshot_cmd->op = DM_W_VAR;
	camshot_cmd->offset = 8;
	camshot_cmd->size = 1;
	camshot_cmd->data[0] = data;
	
	packet->length = 5;

	csp_sendto(CSP_PRIO_NORM, EPS_ADDR, 0x07, 0x07, CSP_SO_NONE, packet, 1000);
}

void camshot_thread(void  *args)
{		
	csp_conn_t* conn = csp_connect(CSP_PRIO_NORM, TNC_ADDR, 0x10, 1000, CSP_O_NONE);
	
	for (;;)
	{
		if(dm_var.cammode != 0)
		{
			uint32_t WakeTime = osKernelSysTick();
			
			printf("Now run camshot thread\n");
			
			campwr_ctrl(255);

			printf("Waiting Camera Power\n");
			
			osDelay(15000);
			
			char camcmd[64];
			sprintf(camcmd, "\r\n\r\n\r\ncamshot 1 1\r\n");
			
			// send camshot cmd
			while(uart_getc(SERIAL_PORT)>=0);	//clear read buffer
			
			uart_write(SERIAL_PORT,camcmd,strlen(camcmd));
			printf("camshot_cmd is %s\n", camcmd);
			
			// send fetch data cmd
			sprintf(camcmd, "cat /tmp/snap_0.jpg >/dev/ttyAMA0\n");
			uart_write(SERIAL_PORT,camcmd,strlen(camcmd));
			printf("camshot_cmd is %s\n", camcmd);
					
			int pktcount = 0;
			while(1)
			{
				osDelay(10);
				if(uart_available(SERIAL_PORT) == 0 || )
				{
					printf("END CAMSHOT\n");
					break;
				}
						
				csp_packet_t *packet = csp_buffer_get(256);
				if(packet == NULL)	continue;
			
				packet->length = uart_read(SERIAL_PORT,(uint8_t *)packet->data,240);
				pktcount++;
												
				uint8_t buffer[256];
				//filebytes += uart_read(SERIAL_PORT,buffer,240);
				if(csp_send(conn,packet,1000) != 1)
				{
					csp_buffer_free(packet);
				}
		}
		 	
		osDelay(10);
	}
}
