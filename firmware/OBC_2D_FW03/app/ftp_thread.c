#include <os.h>

#include "ftp_thread.h"
#include "memory_thread.h"
#include "bsp.h"

void ftp_thread(void const *args)
{
	csp_socket_t *sock = csp_socket(CSP_SO_CRC32REQ);
	
	csp_bind(sock, 0x09);
	
	csp_listen(sock, 1);
	
	csp_conn_t *conn;
	csp_packet_t *packet;
	
	for(;;)
	{
		if ((conn = csp_accept(sock, 2000)) == NULL)
			continue;
		
		while ((packet = csp_read(conn, 100)) != NULL) 
		{
			if(csp_conn_dst(conn) == TNC_ADDR)
			{
				printf("Now runing ftp thread\n");
				
				if((dm_var.memavail == 1) && (dm_var.memstatus == 1))
				{
					char filename[20];
					FIL f;
					
					sprintf(filename,"%d",packet->data16[0]);
					
					if(f_open(&f,(const char*)filename,FA_READ) == FR_OK)
					{	
						f_lseek(&f,packet->data[3]);
						
						csp_packet_t *out_packet = csp_buffer_get(packet->data[4] + 20);
						
						if(f_read(&f,out_packet->data,packet->data[4],(UINT*)&out_packet->length))
						{
							csp_buffer_free(out_packet);
							csp_buffer_free(packet);
							f_close(&f);
							break;
						}
						f_close(&f);
						
						if(csp_send(conn,out_packet,1000) != 1)
						{
							csp_buffer_free(out_packet);
						}
					}
				}
			}
			csp_buffer_free(packet);
			break;
		}		
	}
	
}