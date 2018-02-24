#ifndef _SPI_H__
#define _SPI_H__

#include "datatypes.h"
#include "stm32f4xx_spi.h"

#ifdef __cplusplus
extern "C" {
#endif
    
//#define SPI_INIT_CS     
    //Chipselect pins initialized by this driver    
    //and CS_*() api's are included
    
    typedef SPI_TypeDef spi_handle_t;
    
    //spi direction / interrupts
    #define SPI_RX      0x01
    #define SPI_TX      0x02
    
    //now only SPI1,SPI2 supported
    /*
    init spi as master
    mode   CPOL       CPHA
    0      Low(0)     1Edge(0)
    1      Low        2Edge(1)
    2      High(1)    1Edge
    3      High       2Edge
    databits can be 8 or 16
    divider can be 2^1 - 2^8
    */
    void __spi_init(SPI_TypeDef *spi,
                    uint8_t mode_number,
                    uint8_t databits,
                    bool lsbfirst,
                    int divider,
                    bool master,
                    uint8_t lines,      //1 line or 2 lines
                    uint8_t direction);
    //with master/slave and direction options
    #define spi_init(SPI,MODE,BITS,DIV) __spi_init(SPI,MODE,BITS,false,DIV,true,2,SPI_RX|SPI_TX)
    //default: 8bits, MSBFIRST, master, 2lines, RX+TX
    void spi_deinit(SPI_TypeDef *spi);
    
    //exchange a byte, return after finished
    static inline uint16_t spi_rw(SPI_TypeDef *SPIx,uint16_t x)
    {
        while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPIx,x);
        while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
        uint16_t d = SPI_I2S_ReceiveData(SPIx);
        return d;
    }
    
    #define SPI_IRQ_RX  SPI_I2S_IT_RXNE
    #define SPI_IRQ_TX  SPI_I2S_IT_TXE
    #define SPI_IRQ_ERR SPI_I2S_IT_ERR
    #define spi_setirq(SPI,IRQ)     __spi_irq(SPI,SPI_IRQ_##IRQ,ENABLE)
    #define spi_noirq(SPI,IRQ)      __spi_irq(SPI,SPI_IRQ_##IRQ,DISABLE)
    static inline void __spi_irq(SPI_TypeDef *SPIx,uint8_t it,FunctionalState cmd)
    {SPI_I2S_ITConfig(SPIx,it,cmd);}
        
#ifdef SPI_INIT_CS     
#define CS_HIGH(S)     CS_HIGH_##S()
#define CS_LOW(S)      CS_LOW_##S()
#define CS_HIGH_SPI1() pinhigh(PA4)
#define CS_LOW_SPI1()  pinlow(PA4)
#define CS_HIGH_SPI2() pinhigh(PB12)
#define CS_LOW_SPI2()  pinlow(PB12)
#endif
    
#define spi_r(S)       SPI_I2S_ReceiveData(S)
#define spi_w(S,x)     SPI_I2S_SendData(S,x)
#define spi_wait_r(S)  while(SPI_I2S_GetFlagStatus(S,SPI_I2S_FLAG_RXNE) == RESET) //wait for data available
#define spi_wait_w(S)  while(SPI_I2S_GetFlagStatus(S,SPI_I2S_FLAG_TXE) == RESET) //wait for transmit complete
#define spi_wait(S)    while(SPI_I2S_GetFlagStatus(S,SPI_I2S_FLAG_BSY) == SET)  //wait for communication finish
     
    uint16_t __spi_divider(int divider);
	
#ifdef __cplusplus
}
#endif

#endif