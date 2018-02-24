#ifndef _UART_H__
#define _UART_H__

//#include <sys/types.h>
//#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "datatypes.h"
#include "stm32f4xx_usart.h"
  
#define uart_config_t USART_InitTypeDef
//For convenient, use USART original definition instead
//Hardware flow control CTS mode still have bug, use it carefully

//#define O_SYNC		0x0001    // this feature is not implemented
#define O_RDONLY	0x0002
#define O_WRONLY	0x0004
#define O_RDWR		0x0006

#define UART_PORTS	3
//1 of 6 port enabled, other port disabled by board design
//use USART2 only

//Unix standard API's
int  uart_open(int port, int oflag);
void uart_close(int port);
ssize_t uart_read(int port,uint8_t *buff,size_t nbytes);
ssize_t uart_write(int port,uint8_t *buff,size_t nbytes);
#define uart_flush	uart_sync
int  uart_sync(int port);

//Custom API's
int  uart_init(int port, uart_config_t *);
void uart_deinit(int port);
int  uart_getc(int port);
void uart_putc(int port,int ch);
int  uart_peek(int port);

/*!
    \brief Read data until the char is appeared
    \param c The terminating char
    \return Return the char that was found, <0 when not found
    You can use this function skipping unwanted chars
*/
int  uart_find(int port,char c);

void uart_print(int port,const char *str);
size_t uart_available(int port);

/*!
    \brief  Test if the char is in receive buffer
    \param port uart port id
    \param c The char to find
    \return True = found, False = not found
*/
bool uart_recved(int port, char c);

/*!
    \brief  Test if the char is in receive buffer
    \param port uart port id
    \param c The char to find
    \return True = found, False = not found
*/
bool uart_recved_s(int port, char s[]);

/*!
    \brief Test if there is an \r or \r\n in buffer
    \param port uart port id
    \return True = new line in buffer, False = no new line
*/
bool uart_can_readline(int port);

/*!
    \brief Read bytes to buffer until char c is meet or buffer is full
    \param port uart port id
    \param c terminating char
    \param buff pointer to destination buffer
    \param nbytes size of the buffer
    \return Number of bytes in buffer
    If char \a c not met in rx stream, function will read all content in rx stream
    End-Of-Line = \r\n
*/
ssize_t uart_read_until(int port,char c,uint8_t *buff,size_t nbytes);

/*!
    \brief Read a line from buffer
    \param port uart port id
    \param buff pointer to destination buffer
    \param nbytes size of the buffer
    \return Length of the string in buffer
    If there is no new line in rx stream, function will read all content in rx stream
    End-Of-Line = \r\n
*/
ssize_t uart_readline(int port,char buff[], size_t nbytes);

#include "uart_custom_api.h"

#define IS_VALID_PORT(INT_PORT)	(INT_PORT>=0 && INT_PORT<UART_PORTS)
#define IS_VALID_OFLAG(OFLAG)   (OFLAG &~ 0xfff8)

uint8_t __uart_af(int port);//convert port to GPIO_AF variable
void __uart_clock_cmd(int port,bool cmd);

#ifdef __cplusplus
}
#endif

#endif
