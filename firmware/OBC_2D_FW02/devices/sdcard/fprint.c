#include <os.h>
#include "fprint.h"
#include "sdcard.h"
//#include "led_switch.h"

void halt()
{
	printf("[sys]system halted at %lld.%ld\r\n", seconds(), micros());
	//uart_flush(SERIAL_PORT);
	/*
	//close sdcard log files
	sdcard_off();
	
	//bool last = true;
	while(1)
	{
		/*
		bool now = runstop_state();
		if((!last) && now) //catch rising edge
		{
			printf("[sys]rebooting...\r\n\r\n");
			uart_flush(SERIAL_PORT);
			delay_nms(5);
			reboot();
		}
		last = now;
		if(uart_can_readline(SERIAL_PORT))
		{
			char buf[80];
			uart_readline(SERIAL_PORT,buf,sizeof(buf));
			if(strstr(buf,"reboot"))
			{
				printf("[sys]rebooting...\r\n\r\n");
				uart_flush(SERIAL_PORT);
				delay_nms(5);
				reboot();
			}
		}
	}
	*/
}

int error(const char *format, ...)
{
	//led_error();
	
	int n;
	va_list args;
  	va_start(args, format);
  	n = vprintf(format, args);
  	va_end(args);
	
#ifdef AUTO_REBOOT
	printf("[sys]error at %ld.%ld, rebooting...\r\n", seconds(), micros());
	
	//close sdcard log files
	sdcard_logger();
	sdcard_off();
	
	delay_nms(5);
	reboot();
#else
	halt();
#endif
	//never reach here
	return n;
}

void print_hex(uint8_t *ptr, uint32_t begin, uint32_t size)
{
	ptr += begin;
	for(uint32_t i = 0; i < size; i++)
	{
		printf("%02x", ptr[i]);
	}
}

void f_printresult (FRESULT rc)
{
	const char *str =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
		"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
	for (UINT i = 0; i != (UINT)rc && *str; i++) {
		while (*str++) ;
	}
	printf("FR_%s", str);
}