#ifndef _DEBUG_H__
#define _DEBUG_H__

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "datatypes.h"

//#define USE_SEMIHOSTED_DEBUG     //in config.h
     
#ifdef NODEBUG
#define   kprintf 
#define   kputs
#define   kputc
#define   kputn
#endif     

#if ((defined USE_SEMIHOSTED_DEBUG) || (defined USE_TELEMETRY_DEBUG))
#define   kprintf		printf
#define   kputs(STR)	puts(STR)
#define   kputc(C)	    putchar(C)
#define   kputn(N)	    kprintf("%d",n)
#endif     
	 
#ifdef USE_TELEMETRY_DEBUG
#include "mfifo.h"
DECLARE_MFIFO(debug_msg_fifo);
#endif
     
#ifdef USE_BTSerial_DEBUG     
void kprintf(const TCHAR* fmt, ... );
#define   kputs(STR)	    BTserial_puts(STR)
#define   kputc(C)	    BTserial_putc(C)
#define   kputn(N)	    kprintf("%d",n)
#endif

void __hw_error(char *module,char *device,char *function,int error_number,char *file,uint32_t line);
#define hw_error(M,D,F,E) __hw_error(M,D,F,E,__FILE__,__LINE__)
void __sw_error(char *task,char *function,int error_number,char *file,uint32_t line);
#define sw_error(T,F,E) __sw_error(T,F,E,__FILE__,__LINE__)

//void debug_log_start();
//void debug_log_stop();

#ifdef __cplusplus
}
#endif

#endif
