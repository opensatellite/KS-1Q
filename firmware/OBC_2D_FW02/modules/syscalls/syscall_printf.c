#include <yfuns.h>

#include <os.h>

#define NODEBUG

#ifdef PRINTF_REMAP_RTT
#include "SEGGER_RTT.h"
#endif

#pragma module_name = "?__write"

size_t __write(int handle, const unsigned char * buffer, size_t size);

size_t __write(int handle, const unsigned char * buffer, size_t size) {
  (void) handle;  /* Not used, avoid warning */

#ifdef PRINTF_REMAP_RTT
    SEGGER_RTT_Write(0, (const char*)buffer, size);
#elifdef	NODEBUG
	
#else
    //uart_write(1,(uint8_t *)buffer,size);
#endif

  
  return size;
}

size_t __write_buffered(int handle, const unsigned char * buffer, size_t size) {
  (void) handle;  /* Not used, avoid warning */

#ifdef PRINTF_REMAP_RTT
    SEGGER_RTT_Write(0, (const char*)buffer, size);
#elifdef	NODEBUG
	
#else
    //uart_write(1,(uint8_t *)buffer,size);
#endif

  return size;
}