#include "uart.h"
#include "pin.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define U1_TXBUF_SIZE   0
#define U1_RXBUF_SIZE   0
#define U2_TXBUF_SIZE   0
#define U2_RXBUF_SIZE   0
#define U3_TXBUF_SIZE   512
#define U3_RXBUF_SIZE   32

#define TX_DMA		(1<<0)  //Send by DMA
#define TX_INTR		(1<<1)  //Send by Interrupt
//#define TX_POLLING	(1<<2)
#define RX_DMA		(1<<4)
//#define RX_INTR	(1<<5)
//#define RX_POLLING	(1<<6)

typedef struct {
    USART_TypeDef* usart;
    GPIO_TypeDef*  gpio;
    uint16_t       in_pin,out_pin;
    //RX,CTS is in, TX,RTS is out
    pin            cts_pin;
    
    uint8_t 	 mode;
    
    DMA_Channel_TypeDef* tx_dma;
    DMA_Channel_TypeDef* rx_dma;
    
    IRQn_Type      tx_dma_irq;
    IRQn_Type	 usart_irq;
    //IRQn_Type	 rx_irq;
    
    uint8_t        irq_preemption_pr;
    uint8_t	 irq_sub_pr;
}uart_hardware_t;

typedef struct {
    bool blocking;// this feature is not implemented
    volatile uint8_t *txBuf;
    volatile size_t txHead, txTail;
    size_t txSize;
    volatile uint8_t *rxBuf;
    volatile size_t rxHead, rxTail;
    size_t rxSize;
    volatile size_t rxPos;
}uart_var_t;

static const uart_hardware_t s_hw[UART_PORTS] = { {
    .usart = USART1,//GPS
    .gpio = GPIOA,
    .in_pin = GPIO_Pin_10,
    .out_pin = GPIO_Pin_9,
    .cts_pin = C_PA11,
    .mode = TX_INTR | RX_DMA,
    .tx_dma = DMA1_Channel4, 
    .rx_dma = DMA1_Channel5,
    .tx_dma_irq = DMA1_Channel4_IRQn,
    .usart_irq = USART1_IRQn,
    .irq_preemption_pr = 3,
    .irq_sub_pr = 0
    }, { 
    .usart = USART2,//RS232
    .gpio = GPIOA,
    .in_pin = GPIO_Pin_0 | GPIO_Pin_3,
    .out_pin = GPIO_Pin_2 | GPIO_Pin_1,
    .cts_pin = C_PA0,
    .mode = TX_INTR | RX_DMA,
    .tx_dma = DMA1_Channel7,
    .rx_dma = DMA1_Channel6,
    .tx_dma_irq = DMA1_Channel7_IRQn,
    .usart_irq = USART2_IRQn,
    .irq_preemption_pr = 3,
    .irq_sub_pr = 0
    }, { 
    .usart = USART3,//RS232
    .gpio = GPIOC,
    .in_pin = GPIO_Pin_11,
    .out_pin = GPIO_Pin_10,
    .cts_pin = C_PA0,
    .mode = TX_INTR | RX_DMA,
    .tx_dma = DMA1_Channel2,
    .rx_dma = DMA1_Channel3,
    .tx_dma_irq = DMA1_Channel2_IRQn,
    .usart_irq = USART3_IRQn,
    .irq_preemption_pr = 3,
    .irq_sub_pr = 0
    }
};

#if (U1_TXBUF_SIZE > 0)
static uint8_t __txbuf1_alloc[U1_TXBUF_SIZE];
#endif
#if (U1_RXBUF_SIZE > 0)  
static uint8_t __rxbuf1_alloc[U1_RXBUF_SIZE];
#endif

#if (U2_TXBUF_SIZE > 0)
static uint8_t __txbuf2_alloc[U2_TXBUF_SIZE];
#endif
#if (U2_RXBUF_SIZE > 0)  
static uint8_t __rxbuf2_alloc[U2_RXBUF_SIZE];
#endif

#if (U3_TXBUF_SIZE > 0)
static uint8_t __txbuf3_alloc[U3_TXBUF_SIZE];
#endif
#if (U3_RXBUF_SIZE > 0)  
static uint8_t __rxbuf3_alloc[U3_RXBUF_SIZE];
#endif

static uart_var_t s_var[UART_PORTS] = {
    {  
#if (U1_TXBUF_SIZE > 0)  
        .txBuf = __txbuf1_alloc,
        .txSize = sizeof(__txbuf1_alloc)/sizeof(__txbuf1_alloc[0]),
#else
        .txBuf = NULL,
        .txSize = 0,
#endif
#if (U1_RXBUF_SIZE > 0)  
        .rxBuf = __rxbuf1_alloc,
        .rxSize = sizeof(__rxbuf1_alloc)/sizeof(__rxbuf1_alloc[0])
#else
        .rxBuf = NULL,
        .rxSize = 0,
#endif
    }, {
#if (U2_TXBUF_SIZE > 0)  
        .txBuf = __txbuf2_alloc,
        .txSize = sizeof(__txbuf2_alloc)/sizeof(__txbuf2_alloc[0]),
#else
        .txBuf = NULL,
        .txSize = 0,
#endif
#if (U2_RXBUF_SIZE > 0)  
        .rxBuf = __rxbuf2_alloc,
        .rxSize = sizeof(__rxbuf2_alloc)/sizeof(__rxbuf2_alloc[0])
#else
        .rxBuf = NULL,
        .rxSize = 0,
#endif
    }, {
#if (U3_TXBUF_SIZE > 0)  
        .txBuf = __txbuf3_alloc,
        .txSize = sizeof(__txbuf3_alloc)/sizeof(__txbuf3_alloc[0]),
#else
        .txBuf = NULL,
        .txSize = 0,
#endif
#if (U3_RXBUF_SIZE > 0)     
        .rxBuf = __rxbuf3_alloc,
        .rxSize = sizeof(__rxbuf3_alloc)/sizeof(__rxbuf3_alloc[0])
#else
        .rxBuf = NULL,
        .rxSize = 0,
#endif
    }
};

static void uart_start_txdma(int port)
{
    s_hw[port].tx_dma->CMAR = (uint32_t)(s_var[port].txBuf+s_var[port].txTail);
    if (s_var[port].txHead > s_var[port].txTail) {
        s_hw[port].tx_dma->CNDTR = s_var[port].txHead - s_var[port].txTail;
        s_var[port].txTail = s_var[port].txHead;
    }
    else {
        s_hw[port].tx_dma->CNDTR = s_var[port].txSize - s_var[port].txTail;
        s_var[port].txTail = 0;
    }
    
    DMA_Cmd(s_hw[port].tx_dma, ENABLE);
}

static void uart_start_tx(int port)
{
    if((s_hw[port].usart)->CR3 & USART_HardwareFlowControl_CTS) 
        //CTS signal enable
    {
        //has flowctrl, enable cts interrupt
        if( s_hw[port].usart->CR3 & (1<<10) == 0) //test CTSIE bit
            USART_ITConfig(s_hw[port].usart,USART_IT_CTS,ENABLE);//write enable will trigger interrupt again ?
        //if(!pinread(s_hw[port].cts_pin)) return; //cts low, wait for interrupt
        return;//just wait for cts interrupt
    }//cts high, no interrupt will occure, start tx immediately
    if(s_hw[port].mode & TX_DMA)
    {
        //no flowctrl, enable dma directly
        if(!(s_hw[port].tx_dma->CCR & 1))
            uart_start_txdma(port);
    }
    if(s_hw[port].mode & TX_INTR)
    {
        //interrupt mode
        //USART_ITConfig(s_hw[port].usart, USART_IT_TXE, ENABLE);
        if(!(s_hw[port].usart->CR1 & (1<<7)))
            s_hw[port].usart->CR1 |= (1<<7);
    }
}

int  uart_open(int port, int oflag)
{
    assert_param(IS_VAILD_PORT(port));
    assert_param(IS_VALID_OFLAG(oflag));
    //s_var[port].blocking = (oflag | O_SYNC) ? true : false;
    
    uint16_t tmpreg;
    tmpreg = s_hw[port].usart->CR1;
    tmpreg &= 0xfffffff3;//clear TE and RE bit
    if(oflag & O_RDWR)
    {
        tmpreg |= (1<<3)|(1<<2);
    }
    else
    {
        if(oflag & O_RDONLY)
            tmpreg |= (1<<2);//RE enable
        if(oflag & O_WRONLY)
            tmpreg |= (1<<3);//TE enable
    }
    s_hw[port].usart->CR1 = (uint16_t)tmpreg;
    
    USART_Cmd(s_hw[port].usart,ENABLE);
    
    return 0;
}

void uart_close(int port)
{
    USART_Cmd(s_hw[port].usart,DISABLE);
}

int uart_read(int port,uint8_t *buff,size_t nbytes)
{
    size_t read = 0;
    uint32_t tmpreg = s_hw[port].rx_dma->CNDTR;
    while(read < nbytes && tmpreg != s_var[port].rxPos)
    {
        buff[read] = s_var[port].rxBuf[s_var[port].rxSize - s_var[port].rxPos];
        if (--s_var[port].rxPos == 0)
        s_var[port].rxPos = s_var[port].rxSize;
        read++;
    }
    return read;
}

int uart_write(int port,uint8_t *buff,size_t nbytes)
{
    size_t written = 0;
    size_t tmphead;
    
    written = 0;
    while(written < nbytes)
    {
        tmphead = (s_var[port].txHead + 1) % s_var[port].txSize;
        while ( tmphead == s_var[port].txTail );
        s_var[port].txBuf[s_var[port].txHead] = buff[written];
        s_var[port].txHead = tmphead;
        written++;
        uart_start_tx(port);
    }
    
    return written; 
}

int  uart_sync(int port)
{
    if( (s_hw[port].usart->CR1) & ((1<<15)|(1<<3)) == ((1<<15)|(1<<3)))
        //test usart enable and tx enable bit
    {
        while(s_var[port].txHead != s_var[port].txTail);
        return 0;
    }
    else
        //    	return -EIO; // I/O error
        return -1;
}

//Custom API's
int  uart_init(int port, uart_config_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    // alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = s_hw[port].out_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(s_hw[port].gpio, &GPIO_InitStructure);
    
    // input floating w/ pull ups
    GPIO_InitStructure.GPIO_Pin = s_hw[port].in_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //RX and CTS pin pullup to prevent data send when voltage low
    GPIO_Init(s_hw[port].gpio, &GPIO_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = s_hw[port].irq_preemption_pr;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = s_hw[port].irq_sub_pr;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    if(s_hw[port].mode & TX_DMA) 
    {
        NVIC_InitStructure.NVIC_IRQChannel = s_hw[port].tx_dma_irq;
        NVIC_Init(&NVIC_InitStructure);
    }
    if((s_hw[port].mode & TX_INTR) || 
       (cfg->USART_HardwareFlowControl & USART_HardwareFlowControl_CTS))
    {
        NVIC_InitStructure.NVIC_IRQChannel = s_hw[port].usart_irq;
        NVIC_Init(&NVIC_InitStructure);
    }
    
    s_var[port].rxHead = s_var[port].rxTail = 0;
    s_var[port].txHead = s_var[port].txTail = 0;
    
    // Enable the USART OverSampling by 8 
    //  USART_OverSampling8Cmd(s_hw[port].usart, ENABLE);  
    USART_Init(s_hw[port].usart, cfg);
    
    if(cfg->USART_HardwareFlowControl & USART_HardwareFlowControl_CTS)
    {
        USART_ITConfig(s_hw[port].usart,USART_IT_CTS,ENABLE);
    }
    
    // Configure DMA for rx
    if(s_hw[port].mode & RX_DMA) {
        DMA_DeInit(s_hw[port].rx_dma);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)s_hw[port].usart + 0x04;
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)s_var[port].rxBuf;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize = s_var[port].rxSize;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;
        DMA_Init(s_hw[port].rx_dma, &DMA_InitStructure);
        
        DMA_Cmd(s_hw[port].rx_dma, ENABLE);
        
        USART_DMACmd(s_hw[port].usart, USART_DMAReq_Rx, ENABLE);
        s_var[port].rxPos = DMA_GetCurrDataCounter(s_hw[port].rx_dma);
    }
    
    // Configure DMA for tx
    if(s_hw[port].mode & TX_DMA) {
        DMA_DeInit(s_hw[port].tx_dma);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(s_hw[port].usart) + 0x04;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
        DMA_Init(s_hw[port].tx_dma, &DMA_InitStructure);
        DMA_ITConfig(s_hw[port].tx_dma, DMA_IT_TC, ENABLE);
        s_hw[port].tx_dma->CNDTR = 0;
        
        USART_DMACmd(s_hw[port].usart, USART_DMAReq_Tx, ENABLE);
    }
    
    USART_Cmd(s_hw[port].usart, ENABLE);
    
    return 0;
}

int  uart_getc(int port)
{
    int ch;
    
    size_t tail = s_var[port].rxSize - s_var[port].rxPos;
    ch = s_var[port].rxBuf[tail];
    if (--s_var[port].rxPos == 0)
        s_var[port].rxPos = s_var[port].rxSize;
    
    return ch;
}

void uart_putc(int port,int ch)
{
    size_t tmphead = (s_var[port].txHead + 1) % s_var[port].txSize;
    while ( tmphead == s_var[port].txTail );
    s_var[port].txBuf[s_var[port].txHead] = ch;
    s_var[port].txHead = tmphead;
    uart_start_tx(port);
}

int  uart_peek(int port)
{
    int ch;
    
    ch = s_var[port].rxBuf[s_var[port].rxSize - s_var[port].rxPos];
    
    return ch;
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
    while (*str)
        uart_putc(port,*(str++));
}

size_t uart_available(int port)
{
    return (s_var[port].rxPos - s_hw[port].rx_dma->CNDTR) % s_var[port].rxSize;
}

inline static void uart_txdma_handle(int port)
{
    if (s_var[port].txHead != s_var[port].txTail)
    {
        uart_start_txdma(port);
    }
    else
    {
        //no data to transmit, dma already auto disabled
        //    if(s_hw[port].usart->CR3 & USART_HardwareFlowControl_CTS)
        USART_ITConfig(s_hw[port].usart,USART_IT_CTS,DISABLE);
    }
}

void uart_clear(int port)
{
    s_hw[port].rx_dma->CNDTR = s_var[port].rxPos;
}

inline static void uart_irq_handle(int port)
{
    //if cts is low, enable transmission
    if(USART_GetITStatus(s_hw[port].usart,USART_IT_CTS) )
    {
        USART_ClearITPendingBit(s_hw[port].usart,USART_IT_CTS);
        if (s_var[port].txHead != s_var[port].txTail && !pinread(s_hw[port].cts_pin))
            //has data and cts falling
        {
            /*
            if( s_hw[port].mode & TX_DMA)
            uart_start_txdma(port); 
            if( s_hw[port].mode & TX_INTR)
            {
            //interrupt mode
            //USART_ITConfig(s_hw[port].usart, USART_IT_TXE, ENABLE);
            if(!(s_hw[port].usart->CR1 & (1<<7)))
            s_hw[port].usart->CR1 |= (1<<7);
        }*/
            uart_start_tx(port);
        }
    }
    //if TX empty, test whether we should stop
    if(USART_GetITStatus(s_hw[port].usart,USART_IT_TXE))
    {
        if ( s_var[port].txHead != s_var[port].txTail )
            //not empty
        {
            if(s_hw[port].usart->CR3 & USART_HardwareFlowControl_CTS && pinread(s_hw[port].cts_pin))
            {//cts enabled, cts high, abort transmission
                USART_ITConfig(s_hw[port].usart, USART_IT_TXE, DISABLE);
            }
            else
            {//cts disabled or cts low
                size_t tmp = s_var[port].txTail;
                s_var[port].txTail = ( tmp + 1 ) % s_var[port].txSize; 
                USART_SendData(s_hw[port].usart,s_var[port].txBuf[tmp]); 
            }
        }
        else //is empty
        {
            if(s_hw[port].usart->CR3 & USART_HardwareFlowControl_CTS && pinread(s_hw[port].cts_pin))
                //cts enabled, cts high, disable cts interrupt ( no falling else )
                USART_ITConfig(s_hw[port].usart, USART_IT_CTS, DISABLE);
            USART_ITConfig(s_hw[port].usart, USART_IT_TXE, DISABLE);
        }
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
void DMAChannel4_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);
    assert_param(s_hw[0].tx_dma == DMA1_Channel4);
    uart_txdma_handle(0);//uart(1)
}

// USART2 tx DMA IRQ
void DMAChannel7_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_IT_TC7);
    DMA_Cmd(DMA1_Channel7, DISABLE);
    assert_param(s_hw[1].tx_dma == DMA1_Channel7);
    uart_txdma_handle(1);//uart(0)
}

void DMAChannel2_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC2);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    assert_param(s_hw[2].tx_dma == DMA1_Channel2);
    uart_txdma_handle(2);//uart(0)
}

/*
TX sequence: 
write buff -> move ptr -> start_tx -> 1 or 3
1. start_tx:   +CTS: INTR: enable CTS intr -> 2.
2. start_tx: NO CTS: INTR: enable TX empty interrupt -> 5.
3. start_tx:   +CTS:  DMA: enable CTS intr -> 4.
4. start_tx: NO CTS:  DMA: enable dma tx -> 5.

5. transmission -> tx complete -> stop_tx

abort:
transmission abort -> auto stop -> CTS_falling -> continue

stop:
tx empty: usart_irq -> 6-11
6. still have data and cts high: disable TXE
7. still have data and cts disabled: write next
8. still have data and cts low: write next

9.  no data and cts high: disable TXE & CTS
10. no data and cts low: disable TXE 
11. no data and cts disabled: disable TXE

stop:
dma_complete: tx_dma_irq -> 12-13
12. still have data: continue
13. no data or cts high: disable DMA & CTS

continue:
CTS_falling_irq: INTR: -> send a byte -> enable tx irq
CTS_falling_irq:  DMA: -> tx_dma_enable
*/