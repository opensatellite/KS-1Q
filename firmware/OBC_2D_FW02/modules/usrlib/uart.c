#include "uart.h"
#include "stm32f4xx.h"
#include "pin.h"
#include "dma.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "config.h"

#define TX_DMA		(1<<0)  //Send by DMA
#define TX_INTR		(1<<1)  //Send by Interrupt
#define TX_POLLING	(1<<2)
#define RX_DMA		(1<<4)
#define RX_INTR  	(1<<5)
#define RX_POLLING	(1<<6)

typedef struct {
    USART_TypeDef* usart;
    GPIO_TypeDef*  gpio;
    pin   rx_pin,tx_pin;
    //RX is in, TX is out
    
    uint8_t 	 mode;
    
    DMA_Stream_TypeDef* tx_dma;
    uint32_t tx_dma_ch;
    DMA_Stream_TypeDef* rx_dma;
    uint32_t rx_dma_ch;
    
    IRQn_Type      tx_dma_irq;
    IRQn_Type	 usart_irq;
    
    uint8_t  irq_preemption_pr;
    uint8_t	 irq_sub_pr;
}uart_hardware_t;

typedef struct {
    bool blocking;// this feature is not implemented
    uint8_t *txBuf;
    volatile size_t txHead, txTail, txPos;
    size_t txSize;
    volatile bool tx_dma_running;
    uint8_t *rxBuf;
    volatile size_t rxHead, rxTail, rxPos;
    size_t rxSize;
}uart_var_t;

static const uart_hardware_t s_hw[UART_PORTS] = { 
	{
		.usart = USART1,
		.rx_pin = C_PA10,
		.tx_pin = C_PA9,
		.mode = TX_INTR | RX_DMA,
		.tx_dma = DMA1_Stream7,
		.tx_dma_ch = DMA_Channel_4,
		.rx_dma = DMA1_Stream2,
		.rx_dma_ch = DMA_Channel_4,
		.tx_dma_irq = DMA1_Stream7_IRQn,
		.usart_irq = USART1_IRQn,
		.irq_preemption_pr = 2,
		.irq_sub_pr = 0
	},
	{
		.usart = USART2,
		.rx_pin = C_PA3,
		.tx_pin = C_PA2,
		.mode = TX_POLLING | RX_DMA,
		.tx_dma = DMA1_Stream6,
		.tx_dma_ch = DMA_Channel_4,
		.rx_dma = DMA1_Stream5,
		.rx_dma_ch = DMA_Channel_4,
		.tx_dma_irq = DMA1_Stream6_IRQn,
		.usart_irq = USART2_IRQn,
		.irq_preemption_pr = 1,
		.irq_sub_pr = 0
	},
	{ 
		.usart = USART3,
		.rx_pin = C_PB11,
                .tx_pin = C_PB10,
                .mode = TX_INTR | RX_DMA,
                .tx_dma = DMA1_Stream3,
                .tx_dma_ch = DMA_Channel_4,
                .rx_dma = DMA1_Stream1,
                .rx_dma_ch = DMA_Channel_4,
                .tx_dma_irq = DMA1_Stream3_IRQn,
                .usart_irq = USART3_IRQn,
                .irq_preemption_pr = 1,
        .irq_sub_pr = 0
    }
};

static uint8_t __txbuf1_alloc[MODBUS_TXBUF_SIZE];
static uint8_t __rxbuf1_alloc[MODBUS_RXBUF_SIZE];
static uint8_t __txbuf2_alloc[CONSOLE_TXBUF_SIZE];
static uint8_t __rxbuf2_alloc[CONSOLE_RXBUF_SIZE];
static uint8_t __txbuf3_alloc[SERIAL_TXBUF_SIZE];
static uint8_t __rxbuf3_alloc[SERIAL_RXBUF_SIZE];

static uart_var_t s_var[UART_PORTS] = {
    { 
        .txBuf = __txbuf1_alloc,
        .txSize = sizeof(__txbuf1_alloc)/sizeof(__txbuf1_alloc[0]),
        .rxBuf = __rxbuf1_alloc,
        .rxSize = sizeof(__rxbuf1_alloc)/sizeof(__rxbuf1_alloc[0])
    },
    {
        .txBuf = __txbuf2_alloc,
        .txSize = sizeof(__txbuf2_alloc)/sizeof(__txbuf2_alloc[0]),
        .rxBuf = __rxbuf2_alloc,
        .rxSize = sizeof(__rxbuf2_alloc)/sizeof(__rxbuf2_alloc[0])
    },
    {
        .txBuf = __txbuf3_alloc,
        .txSize = sizeof(__txbuf3_alloc)/sizeof(__txbuf3_alloc[0]),
        .rxBuf = __rxbuf3_alloc,
        .rxSize = sizeof(__rxbuf3_alloc)/sizeof(__rxbuf3_alloc[0])
    }
};

void __uart_clock_cmd(int port,bool cmd)
{
    if(s_hw[port].usart == USART1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,cmd==true?ENABLE:DISABLE);
    else if(s_hw[port].usart == USART2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,cmd==true?ENABLE:DISABLE);
    else if(s_hw[port].usart == USART3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,cmd==true?ENABLE:DISABLE);
    else if(s_hw[port].usart == UART4)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,cmd==true?ENABLE:DISABLE);
    else if(s_hw[port].usart == UART5)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,cmd==true?ENABLE:DISABLE);
    else if(s_hw[port].usart == USART6)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,cmd==true?ENABLE:DISABLE);
    else;
}

uint8_t __uart_af(int port)
{
    if(s_hw[port].usart == USART1)
        return GPIO_AF_USART1;
    else if(s_hw[port].usart == USART2)
        return GPIO_AF_USART2;
    else if(s_hw[port].usart == USART3)
        return GPIO_AF_USART3;
    else if(s_hw[port].usart == UART4)
        return GPIO_AF_UART4;
    else if(s_hw[port].usart == UART5)
        return GPIO_AF_UART5;
    else if(s_hw[port].usart == USART6)
        return GPIO_AF_USART6;
    else;
    return 0xff;
}

/*!
 \brief flush tx buffer using dma
 \param port uart port id
*/
static void uart_start_txdma(int port)  
{
    size_t head = s_var[port].txHead;
    ssize_t size;
    
    if(!s_var[port].tx_dma_running && head != s_var[port].txTail)
    {
        if(head > s_var[port].txTail)
        {
            size = head - s_var[port].txTail;
            s_var[port].txPos = head;
        }
        else
        {
            size = s_var[port].txSize - s_var[port].txTail;
            s_var[port].txPos = 0;
        }
        
        s_var[port].tx_dma_running = true;
        s_hw[port].tx_dma->M0AR = (uint32_t)&(s_var[port].txBuf[s_var[port].txTail]);
        s_hw[port].tx_dma->NDTR = size;
        DMA_Cmd(s_hw[port].tx_dma, ENABLE);
        //s_hw[port].tx_dma->CR |= 1;      
    }
}

int  uart_open(int port, int oflag)
{
    assert_param(IS_VALID_PORT(port));
    assert_param(IS_VALID_OFLAG(oflag));
    //s_var[port].blocking = (oflag | O_SYNC) ? true : false;
    
    uint16_t tmpreg;
    tmpreg = s_hw[port].usart->CR1;
    tmpreg &=~(USART_CR1_TE|USART_CR1_RE);//clear TE and RE bit
    if(oflag & O_RDWR)
    {
        tmpreg |= USART_CR1_TE|USART_CR1_RE;
    }
    else
    {
        if(oflag & O_RDONLY)
            tmpreg |= USART_CR1_RE;//RE enable
        if(oflag & O_WRONLY)
            tmpreg |= USART_CR1_TE;//TE enable
    }
    s_hw[port].usart->CR1 = (uint16_t)tmpreg;
    
    USART_Cmd(s_hw[port].usart,ENABLE);
    
    return 0;
}

void uart_close(int port)
{
    USART_Cmd(s_hw[port].usart,DISABLE);
}

void uart_deinit(int port)
{
    __uart_clock_cmd(port,false);
    __dma_clock_cmd(s_hw[port].tx_dma,false);
    __dma_clock_cmd(s_hw[port].rx_dma,false);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    if(s_hw[port].mode & TX_DMA) 
    {
        NVIC_InitStructure.NVIC_IRQChannel = s_hw[port].tx_dma_irq;
        NVIC_Init(&NVIC_InitStructure);
    }
    else if(s_hw[port].mode & TX_INTR)
    {
        NVIC_InitStructure.NVIC_IRQChannel = s_hw[port].usart_irq;
        NVIC_Init(&NVIC_InitStructure);
    }
}

int  uart_getc(int port)
{    
    char ch;
    if(s_hw[port].mode & RX_DMA)
    {
        size_t n = s_hw[port].rx_dma->NDTR;
        if(n == s_var[port].rxPos) return -1;
        ch = s_var[port].rxBuf[s_var[port].rxSize - s_var[port].rxPos];
        if (--s_var[port].rxPos == 0)
            s_var[port].rxPos = s_var[port].rxSize;
    }
    else if(s_hw[port].mode & RX_INTR)
    {
        size_t head = s_var[port].rxHead;
        if(s_var[port].rxTail == head) return -1;
        ch = s_var[port].rxBuf[s_var[port].rxTail];
        s_var[port].rxTail = (s_var[port].rxTail + 1) % s_var[port].rxSize;
    }    
    else if(s_hw[port].mode & RX_POLLING)
    {
        if(USART_GetFlagStatus(s_hw[port].usart,USART_FLAG_RXNE) == SET)
            ch = USART_ReceiveData(s_hw[port].usart);
        else
            ch = -1;
    } 
    return ch;
}

void uart_putc(int port,int ch)
{
    if(s_hw[port].mode & TX_DMA)
    {
        size_t head = s_var[port].txHead;
        while ( (head + 1) % s_var[port].txSize == s_var[port].txTail );
        //wait for buffer
        s_var[port].txBuf[head] = ch;
        s_var[port].txHead = (head + 1) % s_var[port].txSize;  
        uart_start_txdma(port);
    }
    else if(s_hw[port].mode & TX_INTR)
    {
        size_t head = s_var[port].txHead;
        while ( (head + 1) % s_var[port].txSize == s_var[port].txTail );
        //wait for buffer
        s_var[port].txBuf[head] = ch;
        s_var[port].txHead = (head + 1) % s_var[port].txSize;  
        USART_ITConfig(s_hw[port].usart,USART_IT_TXE,ENABLE);
    }    
    else if(s_hw[port].mode & TX_POLLING)
    {
        while(USART_GetFlagStatus(s_hw[port].usart,USART_FLAG_TXE) != SET);
        USART_SendData(s_hw[port].usart, ch);
    }
}

int  uart_peek(int port)
{
    int ch = -1;
    if(s_hw[port].mode & RX_DMA)
    {
        ch = s_var[port].rxBuf[s_var[port].rxSize - s_var[port].rxPos];
    }
    else if(s_hw[port].mode & RX_INTR)
    {
        ch = s_var[port].rxBuf[s_var[port].rxTail];
    }
    else if(s_hw[port].mode & RX_POLLING)
    {
        ch = s_hw[port].usart->DR;
    }
    return ch;
}

ssize_t uart_read(int port,uint8_t *buff,size_t nbytes)
{
    size_t read = 0;
    while(read < nbytes)
    {
         int ch = uart_getc(port);
         if(ch < 0) break;
         buff[read++] = ch;
    }
    return read;
}

ssize_t uart_write(int port,uint8_t *buff,size_t nbytes)
{
    size_t written = 0;
    
    while(written < nbytes)
    {
        uart_putc(port,buff[written++]);
    }
    
    return written; 
}

int  uart_sync(int port)
{
    if( ((s_hw[port].usart->CR1) & (USART_CR1_UE|USART_CR1_TE)) == (USART_CR1_UE|USART_CR1_TE))
        //test usart enable and tx enable bit
    {
        size_t tail = s_var[port].txTail; 
        while(s_var[port].txHead != tail)
            tail = s_var[port].txTail; 
        return 0;
    }
    else
        //    	return -EIO; // I/O error
        return -1;
}

//Custom API's
int  uart_init(int port, uart_config_t *cfg)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)s_hw[port].usart + 0x04;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    
    __uart_clock_cmd(port,true);
    __dma_clock_cmd(s_hw[port].tx_dma,true);
    __dma_clock_cmd(s_hw[port].rx_dma,true);
    
    // input floating w/ pull ups
    pinmode(s_hw[port].rx_pin,AFPULLUP);
    pin_af_for(s_hw[port].rx_pin,__uart_af(port));
    //RX pin pullup to prevent data send when voltage low
    
    // alternate function push-pull
    __pin_periphial_clock(s_hw[port].tx_pin,ENABLE);
    pin_af_for(s_hw[port].tx_pin,__uart_af(port));
    pinmode(s_hw[port].tx_pin,AFPUSHPULL);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = s_hw[port].irq_preemption_pr;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = s_hw[port].irq_sub_pr;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    if(s_hw[port].mode & TX_DMA) 
    {
        NVIC_InitStructure.NVIC_IRQChannel = s_hw[port].tx_dma_irq;
        NVIC_Init(&NVIC_InitStructure);
    }
    if(s_hw[port].mode & TX_INTR)
    {
        NVIC_InitStructure.NVIC_IRQChannel = s_hw[port].usart_irq;
        NVIC_Init(&NVIC_InitStructure);
    }
    
    s_var[port].rxHead = s_var[port].rxTail = 0;
    s_var[port].txHead = s_var[port].txTail = 0;
    s_var[port].tx_dma_running = false;
    
    // Enable the USART OverSampling by 8 
    USART_OverSampling8Cmd(s_hw[port].usart, ENABLE);  
    USART_Init(s_hw[port].usart, cfg);
    
    // Configure DMA for rx
    if(s_hw[port].mode & RX_DMA) {
        DMA_DeInit(s_hw[port].rx_dma);   
        DMA_InitStructure.DMA_Channel = s_hw[port].rx_dma_ch;
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)s_var[port].rxBuf;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
        DMA_InitStructure.DMA_BufferSize = s_var[port].rxSize;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(s_hw[port].rx_dma, &DMA_InitStructure);
        DMA_Cmd(s_hw[port].rx_dma, ENABLE);
        
        USART_DMACmd(s_hw[port].usart, USART_DMAReq_Rx, ENABLE);
        s_var[port].rxPos = DMA_GetCurrDataCounter(s_hw[port].rx_dma);
    }
    
    // Configure DMA for tx
    if(s_hw[port].mode & TX_DMA) {
        DMA_DeInit(s_hw[port].tx_dma);       
        DMA_InitStructure.DMA_Channel = s_hw[port].tx_dma_ch;
        DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; 
        DMA_InitStructure.DMA_BufferSize = (s_var[port].txSize != 0) ? s_var[port].txSize : 16;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(s_hw[port].tx_dma, &DMA_InitStructure);
        
        s_hw[port].tx_dma->NDTR = 0;
        DMA_ITConfig(s_hw[port].tx_dma, DMA_IT_TC, ENABLE);
        
        USART_DMACmd(s_hw[port].usart, USART_DMAReq_Tx, ENABLE);
    }
    
    USART_Cmd(s_hw[port].usart, ENABLE);
    
    return 0;
}

int  uart_find(int port,char c)
{
    while(uart_available(port) > 0)
    {
        if(uart_getc(port) == c)
            return c;
    }
    return -1;
}

void uart_print(int port,const char *str)
{
    while (*str) uart_putc(port,*str);
}

size_t uart_available(int port)
{
    if(s_hw[port].mode & RX_DMA)
    {
        uint32_t n = s_hw[port].rx_dma->NDTR;
        return (n - s_var[port].rxPos) % s_var[port].rxSize;
    }
    else if(s_hw[port].mode & RX_INTR)
    {
        size_t tail = s_var[port].rxTail;
        return (tail - s_var[port].rxHead) % s_var[port].rxSize;
    }
    else if(s_hw[port].mode & RX_POLLING)
    {
        return (USART_GetFlagStatus(s_hw[port].usart,USART_FLAG_RXNE) == SET) ? 1 : 0;
    }
    return 0;
}

bool uart_recved(int port, char c)
{
    char ch;
    if(s_hw[port].mode & RX_DMA)
    {
        size_t pos = s_var[port].rxPos;
        while(s_hw[port].rx_dma->NDTR != pos)
        {
            ch = s_var[port].rxBuf[s_var[port].rxSize - pos];
            if(ch == c) return true;
            if (--pos == 0) pos = s_var[port].rxSize;
        }    
    }
    else if(s_hw[port].mode & RX_INTR)
    {
        size_t tail = s_var[port].rxTail;
        while(s_var[port].rxHead != tail)
        {
            ch = s_var[port].rxBuf[tail];
            if(ch == c) return true;
            tail = (s_var[port].rxTail + 1) % s_var[port].rxSize;
        }
    }    
    else if(s_hw[port].mode & RX_POLLING)
    {
        ch = USART_ReceiveData(s_hw[port].usart);
        if(ch == c) return true;
    }        
    return false;
}

bool uart_recved_s(int port, char s[])
{
    char ch;
    if(s == 0) return false;
    
    //unmatch & not end => false
    //match & not end => next
    //match & end => true
    //unmatch & end => prev state => first one must not be '\0' => prev must be true    
    if(s_hw[port].mode & RX_DMA)
    {
        if(*s == '\0') return uart_peek(port) == '\0' ? true : false;
        size_t pos = s_var[port].rxPos;
        while(s_hw[port].rx_dma->NDTR != pos)
        {
            ch = s_var[port].rxBuf[s_var[port].rxSize - pos];
            if(*s == '\0') return true;
            if(*s++ != ch) return false;
            if (--pos == 0) pos = s_var[port].rxSize;
        }    
        if(*s == '\0') return true;
    }
    else if(s_hw[port].mode & RX_INTR)
    {
        if(*s == '\0') return uart_peek(port) == '\0' ? true : false;
        size_t tail = s_var[port].rxTail;
        while(s_var[port].rxHead != tail)
        {
            ch = s_var[port].rxBuf[tail];
            if(*s == '\0') return true;
            if(*s++ != ch) return false;
            tail = (s_var[port].rxTail + 1) % s_var[port].rxSize;
        }
        if(*s == '\0') return true;
    }    
    else if(s_hw[port].mode & RX_POLLING)
    {
        //match \0 or ch\0
        ch = USART_ReceiveData(s_hw[port].usart);
        if(*s == '\0')
        {
            if(ch == '\0') return true;
        }
        else
        {
            if(ch == *s && *(s+1) == '\0') return true;
        }
        return false;
    }        
    return false;
}

bool uart_can_readline(int port)
{
    if(uart_recved(port,'\n'))
        return true;
    if(uart_recved(port,'\r'))
        return true;
    return false;
}

ssize_t uart_read_until(int port,char c,uint8_t *buff,size_t nbytes)
{
    size_t read = 0;
    while(read < nbytes)
    {
         int ch = uart_getc(port);
         if(ch < 0) break;
         buff[read++] = ch;
		 if(ch == c) break;
    }
    return read;
}

ssize_t uart_readline(int port,char buff[],size_t nbytes)
{
    ssize_t s = uart_read_until(port,'\n',(uint8_t*)buff,nbytes-1);
    buff[s] = '\0';
    return s;
}

inline static void uart_txdma_handle(int port)
{
    s_var[port].tx_dma_running = false;
    s_var[port].txTail = s_var[port].txPos;
    uart_start_txdma(port);
}

inline static void uart_irq_handle(int port)
{
    uint16_t SR = s_hw[port].usart->SR;
    
    //if TX empty, test whether we should stop
    if(SR & USART_FLAG_TXE)
    {
        size_t tail = s_var[port].txTail; 
        if ( s_var[port].txHead != tail )
            //not empty
        {
                USART_SendData(s_hw[port].usart,s_var[port].txBuf[s_var[port].txTail]); 
                s_var[port].txTail = ( s_var[port].txTail + 1 ) % s_var[port].txSize;
        }
        else //is empty, end of transmission
        {
            USART_ITConfig(s_hw[port].usart, USART_IT_TXE, DISABLE);
        }
    }
    if(SR & USART_FLAG_RXNE)
    {
        s_var[port].rxBuf[s_var[port].rxHead] = USART_ReceiveData(s_hw[port].usart);
        s_var[port].rxHead = ( s_var[port].rxHead + 1 ) % s_var[port].rxSize;
    }
}

void USART1_IRQHandler()
{
	const int port = 0;
	assert_param(s_hw[port].usart == USART1);
	uart_irq_handle(port);
}

void USART2_IRQHandler()
{
    const int port = 1;
    assert_param(s_hw[port].usart == USART2);
    uart_irq_handle(port);
}

void USART3_IRQHandler()
{
    const int port = 2;
    assert_param(s_hw[port].usart == USART3);
    uart_irq_handle(port);
}

// USART1 tx DMA IRQ
void DMA1_Stream7_IRQHandler(void) {
	DMA_ClearITPendingBit(DMA1_Stream7,DMA_IT_TCIF7);
	//DMA_Cmd(DMA1_Stream7, DISABLE);
	assert_param(s_hw[0].tx_dma == DMA1_Stream7);
	uart_txdma_handle(0);//uart(1)
}

// USART2 tx DMA IRQ
void DMA1_Stream6_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
    //DMA_Cmd(DMA1_Stream6, DISABLE);
    assert_param(s_hw[1].tx_dma == DMA1_Stream6);
    uart_txdma_handle(1);//uart(1)
}

//USART3 tx DMA IRQ
void DMA1_Stream3_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_Stream3,DMA_IT_TCIF3);
    //DMA_Cmd(DMA1_Stream3, DISABLE);
    assert_param(s_hw[2].tx_dma == DMA1_Stream3);
    uart_txdma_handle(2);//uart(0)
}
