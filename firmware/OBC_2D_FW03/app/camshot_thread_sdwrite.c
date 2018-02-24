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
/*
uint8_t  stop =  0xff;

uint8_t sdcard_wcam()
{
	FIL f;
	char buff[512];
	uint32_t count;
	uint32_t file_count;
	uint8_t err;

	openfile(&f);

	while(uart_available(SERIAL_PORT) ==0);
	
	do
	{
		campwr_ctrl(100);
		count = uart_available(SERIAL_PORT);
		uart_read(SERIAL_PORT,buff,count);
		f_write(&f, buff, count, (UINT*)&file_count);
		if (file_count != count)
		{
			err = 0;
			f_close(&f);

			printf("cam data write to sdcard err\n");
			return err;
		}
		f_sync(&f);
	} while (count > 0);
	
	printf("cam data write to sdcard\n");

	f_close(&f);
	return 1;
}*/


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

			printf("Watting Camera Power\n");
			
			osDelay(15000);
			
			char camcmd[64];
			switch (dm_var.cammode)
			{
				case 0:
					goto exti;
				case 1:
				case 2:
					sprintf(camcmd, "\r\n\r\n\r\ncamshot %d %d\r\n", dm_var.caminterval, dm_var.camcount);
					break;
				case 3:
					sprintf(camcmd, "\r\n\r\n\r\ncamshot %d %d 5M\n", dm_var.caminterval, dm_var.camcount);
					break;
				case 6:
					sprintf(camcmd, "\r\n\r\n\r\ncamshot %d\r\n", dm_var.caminterval);
					break;
				case 7:
					sprintf(camcmd, "\r\n\r\n\r\ncamshot 1 1\r\n");
				default:
					break;
			}
			// send camshot cmd
			while(uart_getc(SERIAL_PORT)>=0);
			uart_write(SERIAL_PORT,camcmd,strlen(camcmd));
			printf("camshot_cmd is %s\n", camcmd);
			
			for (uint8_t n = 0; n < dm_var.camcount; n++)
			{
				campwr_ctrl(255);
				switch (dm_var.cammode)
				{
					case 0:
						goto exti;
					case 1:
						sprintf(camcmd, "sz /tmp/snap_%d.jpg\n", 1);
						break;
					case 2:
					case 3:
						sprintf(camcmd, "sz /tmp/snap_%d.jpg\n", 0);
						break;
					case 6:
						sprintf(camcmd, "sz /tmp/stream_ch%d.h265\n", 0);
						break;
					case 7:
						sprintf(camcmd, "cat /tmp/snap_0.jpg >/dev/ttyAMA0\n");
						break;
					default:
					break;
				}
				//clear recv buffer
				while(uart_getc(SERIAL_PORT)>=0);
				
				if(dm_var.cammode == 7)
				{
					uart_write(SERIAL_PORT,camcmd,strlen(camcmd));
					printf("camshot_cmd is %s\n", camcmd);
					
					osDelay(10);
					
					if(uart_available(SERIAL_PORT) == 0)
					{
						continue;
					}
					

					while(1)
					{
						if(uart_available(SERIAL_PORT) == 0)
						{
							printf("END CAMSHOT\n");
							break;
						}
						
						csp_packet_t *packet = csp_buffer_get(256);
						if(packet == NULL)	continue;
						packet->length = uart_read(SERIAL_PORT,(uint8_t *)packet->data,240);
												
						uint8_t buffer[256];
						//filebytes += uart_read(SERIAL_PORT,buffer,240);
						if(csp_send(conn,packet,1000) != 1)
						{
							csp_buffer_free(packet);
						}
						
						osDelay(10);
					}
				}
			}
		exti:	
			campwr_ctrl(0);		
			osDelayUntil(&WakeTime,dm_var.camdelay);
		}
		 	
		osDelay(1000);
	}
}