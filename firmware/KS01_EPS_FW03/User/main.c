#include "bsp.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "uart.h"
#include "stm32_adc.h"
#include "freertos.h"
#include "task.h"
#include "adxl345.h"
#include "systick.h"
#include "debug.h"
#include "mfifo.h"

#include "dm.h"
#include "loop.h"
#include "ledtlm.h"

#include "csp/csp.h"
#include "csp/interfaces/csp_if_can.h"
#include <csp/arch/csp_thread.h>

#include "cmsis_os.h"

void vApplicationIdleHook( void )
{
  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
  to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
  task.  It is essential that code added to this hook function never attempts
  to block in any way (for example, call xQueueReceive() with a block time
  specified, or call vTaskDelay()).  If the application makes use of the
  vTaskDelete() API function (as this demo application does) then it is also
  important that vApplicationIdleHook() is permitted to return to its calling
  function, because it is the responsibility of the idle task to clean up
  memory allocated by the kernel to any task that has since been deleted. */
  __WFI();
  
}

static void libcsp_init()
{
  struct csp_can_config can_conf = { .bitrate = 1000000, .ifc = "CAN" };
  
  csp_buffer_init(10, 300);
  csp_init(EPS_ADDR);
  
  csp_can_init(CSP_CAN_PROMISC, &can_conf);
  
  csp_route_set(OBC_ADDR, &csp_if_can, CSP_NODE_MAC);
  csp_route_set(TTC_ADDR, &csp_if_can, CSP_NODE_MAC);
  csp_route_set(TNC_ADDR, &csp_if_can, CSP_NODE_MAC);
  csp_route_set(CSP_BROADCAST_ADDR, &csp_if_can, CSP_NODE_MAC);
  
  csp_route_start_task(768, 2);
  
  csp_debug_toggle_level(2);
  csp_debug_toggle_level(4);
  csp_debug_toggle_level(5);
}

void dm_thread(void const*args)
{
  csp_socket_t* sock = csp_socket(CSP_SO_CONN_LESS);
  
  csp_bind(sock, CSP_ANY);
  
  for(;;)
  {
    csp_packet_t* packet = csp_recvfrom(sock,1000);
    if(packet == NULL)
    {
      continue;
    }
    
    if(dm_reply(packet) == 1)
    {
      //9 for led
      if(packet->id.dport == 9)
      {
        memcpy(ledtlm_d.data,packet->data,packet->length);	
        ledtlm_d.num = packet->length;
        ledtlm_start();
      }
      csp_buffer_free(packet);
    }
  }
}

int main(int argc, char * argv[])
{	
  //printf("System is booting\n");
  bsp_init();
  libcsp_init();
  
  osThreadDef(loop, loop_thread, 5, 1, 512);
  osThreadId loop = osThreadCreate(osThread(loop), NULL);
  
  osThreadDef(dm, dm_thread, 2, 1, 512);
  osThreadId dm = osThreadCreate(osThread(dm), NULL);
  
  printf("system is running\n");
  osKernelStart();
  //for(;;);
}
