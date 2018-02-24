#include <yfuns.h>

//#include <os.h>
#include "uart.h"
#include "bsp.h"
#ifdef PRINTF_REMAP_RTT
#include "SEGGER_RTT.h"
#endif

#pragma module_name = "?__write"

size_t __write(int handle, const unsigned char * buffer, size_t size);

size_t __write(int handle, const unsigned char * buffer, size_t size) {
  (void) handle;  /* Not used, avoid warning */

  
#ifdef PRINTF_REMAP_RTT
    SEGGER_RTT_Write(0, (const char*)buffer, size);
#else
    uart_write(CONSOLE,(uint8_t *)buffer,size);
#endif

  
  return size;
}

size_t __write_buffered(int handle, const unsigned char * buffer, size_t size) {
  (void) handle;  /* Not used, avoid warning */

#ifdef PRINTF_REMAP_RTT
    SEGGER_RTT_Write(0, (const char*)buffer, size);
#else
    uart_write(CONSOLE,(uint8_t *)buffer,size);
#endif

  return size;
}