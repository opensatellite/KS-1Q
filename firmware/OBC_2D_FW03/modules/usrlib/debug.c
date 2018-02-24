#include "os.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include "led.h"

#define CMD_BUFFER_LEN  64

/*
#ifndef USE_SEMIHOSTED_DEBUG   
void kprintf(const TCHAR* fmt, ... )
{
    char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度自己定义吧
        
    va_list arg_ptr;
    va_start(arg_ptr, fmt);  
    vsnprintf(buffer, CMD_BUFFER_LEN+1, (char const *)fmt, arg_ptr);
    kputs(buffer);
    va_end(arg_ptr);
}
#endif
*/


void __hw_error(char *module,char *device,char *function,int error_number,char *file,uint32_t line)
{
    printf("Module:%s,Device:%s,Function:%s,Error:%d, at file %s line %d\n",
            module,device,function,error_number,file,line);
#ifdef REBOOT_ON_ERROR
    reboot();
#else
    exit(error_number);
#endif
}

void __sw_error(char *task,char *function,int error_number,char *file,uint32_t line)
{
    printf("Task:%s,Function:%s,Error:%d, at file %s line %d\n",
            task,function,error_number,file,line);
#ifdef REBOOT_ON_ERROR
    reboot();
#else
    exit(error_number);
#endif

}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
    printf("Task:%s,Stack Overflow!\n",pcTaskName);
    osThreadTerminate(xTask);
}

#ifdef USE_TELEMETRY_DEBUG

DEFINE_MFIFO(debug_msg_fifo, CMD_BUFFER_LEN);

int putchar(int x)
{
	while(mfifo_put(&debug_msg_fifo, x) < 1)
	{
		os_delay(1);
	}
	return x;
}
#endif

/*
void debug_log_start()
{

}

void debug_log_stop()
{

}
*/