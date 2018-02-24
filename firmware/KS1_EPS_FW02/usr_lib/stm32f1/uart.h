#ifndef _UART_H__
#define _UART_H__

//#include <sys/types.h>
//#include <fcntl.h>
#include "datatypes.h"
#include "stm32f10x_usart.h"

#ifdef __cplusplus
extern "C" {
#endif
  
#define uart_config_t USART_InitTypeDef
//For convenient, use USART original definition instead
//Hardware flow control CTS mode still have bug, use it carefully

//#define O_SYNC		0x0001    // this feature is not implemented
#define O_RDONLY	0x0002
#define O_WRONLY	0x0004
#define O_RDWR		0x0006

#define UART_PORTS	3
//3 of 5 port enabled, other port disabled by board design

//Unix standard API's
int  uart_open(int port, int oflag);
void uart_close(int port);
int uart_read(int port,uint8_t *buff,size_t nbytes);
int uart_write(int port,uint8_t *buff,size_t nbytes);
//sync(port) before writing a long data block
int  uart_sync(int port);
#define uart_flush uart_sync

//Custom API's
int  uart_init(int port, uart_config_t *);
int  uart_getc(int port);
void uart_putc(int port,int ch);
int  uart_peek(int port);
int  uart_find(int port,char c);
//read data until char c is appeared, return <0 for not found
void uart_clear(int port);
void uart_print(int port,const char *str);
size_t uart_available(int port);

#include "uart_custom_api.h"

#define IS_VALID_PORT(INT_PORT)	(INT_PORT>0 && INT_PORT<UART_PORTS)
#define IS_VALID_OFLAG(OFLAG)   (OFLAG &~ 0xfff8)

#ifdef __cplusplus
}
#endif

#endif
