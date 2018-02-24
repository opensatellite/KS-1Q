/*
 * File      : zdevice.c
 * the implemention of zmodem protocol.
 * Change Logs:
 * Date           Author       Notes
 * 2011-03-29     itspy       
 */

#include "zdef.h"

										  
uint32_t Line_left  = 0;		     /* left number of data in the read line buffer*/ 
uint32_t Left_sizes = 0;			 /* left file sizes */
uint32_t Baudrate   = BITRATE; 	 /* console baudrate */
 


uint32_t get_device_baud(void)
{
    return(Baudrate);
}

uint32_t get_sys_time(void)
{
    return(0L);
}

void zsend_byte(uint16_t ch)
{
    uart_putc(SERIAL_PORT,ch);

    return;
}

void zsend_line(uint16_t c)
{
    uint16_t ch;

	ch = (c & 0377);
    uart_putc(SERIAL_PORT,ch);  

    return;
}

int16_t zread_line(uint16_t timeout)
{
	char *str;	 
	static char buf[10];

	if (Line_left > 0)
	{
	    Line_left -= 1;
		return (*str++ & 0377);
	}
	Line_left = 0;
	timeout/=5;
	while (1)
	{
		int c = uart_getc(SERIAL_PORT);
		Line_left = c <= 0 ? 0 : c;
		if (Line_left <= 0)
		{
		     Line_left = Line_left;
		     str = buf;
			 break;
		}
	}
	if (Line_left < 1) return TIMEOUT;
	Line_left -=1;

	return (*str++ & 0377);
}

/*
 * send a string to the modem, processing for \336 (sleep 1 sec)
 *   and \335 (break signal)
 */
void zsend_break(char *cmd)
{
   
	while (*cmd++) 
	{
		switch (*cmd) 
		{
		case '\336':
			 continue;
		case '\335':
		     osDelay(1000);
			 continue;
		default:
			 zsend_line(*cmd);
			 break;
		}
	}
}
/* send cancel string to get the other end to shut up */
void zsend_can(void)
{
	static char cmd[] = {24,24,24,24,24,24,24,24,24,24,0};

	zsend_break(cmd);
	printf("\x0d");
	Line_left=0;	       /* clear Line_left */

	return;
}

/* end of zdevice.c */
