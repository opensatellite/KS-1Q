#include "spi.h"
#include "pin.h"

#define IS_SPI_DATABITS(BITS)   ((BITS == 8)||(BITS == 16))
#define IS_SPI_MODENUMBER(MODE) (MODE <= 3)

void __spi_init_pins(SPI_TypeDef *spi)
{
    if(spi == SPI1)
    {
        #ifdef SPI_INIT_CS    
        pinmodespd(PA4,PUSHPULL,pin_25MHz);
        pinhigh(PA4);
        #endif
        pinmodespd(PA5,AFPUSHPULL,pin_25MHz);
        pinmodespd(PA6,AFPULLUP,pin_25MHz);
        pinmodespd(PA7,AFPUSHPULL,pin_25MHz);
        
        pin_af_for(PA5,GPIO_AF_SPI1);
        pin_af_for(PA6,GPIO_AF_SPI1);
        pin_af_for(PA7,GPIO_AF_SPI1);
    }
    
    if(spi == SPI2)
    {
        #ifdef SPI_INIT_CS    
        pinmodespd(PB12,PUSHPULL,pin_25MHz);
        pinhigh(PB12);
        #endif
        pinmodespd(PB13,AFPUSHPULL,pin_25MHz);
        pinmodespd(PB14,AFPULLUP,pin_25MHz);
        pinmodespd(PB15,AFPUSHPULL,pin_25MHz);
        
        pin_af_for(PB13,GPIO_AF_SPI2);
        pin_af_for(PB14,GPIO_AF_SPI2);
        pin_af_for(PB15,GPIO_AF_SPI2);
    }
	
	if(spi == SPI3)
    {
        #ifdef SPI_INIT_CS    
        pinmodespd(PC9,PUSHPULL,pin_25MHz);
        pinhigh(PC9);
        #endif
        pinmodespd(PC10,AFPUSHPULL,pin_25MHz);
        pinmodespd(PC11,AFPULLUP,pin_25MHz);
        pinmodespd(PC12,AFPUSHPULL,pin_25MHz);
        
        pin_af_for(PC10,GPIO_AF_SPI3);
        pin_af_for(PC11,GPIO_AF_SPI3);
        pin_af_for(PC12,GPIO_AF_SPI3);
    }
}

void __spi_deinit_pins(SPI_TypeDef *spi)
{
    if(spi == SPI1)
    {
        #ifdef SPI_INIT_CS    
        pinmode(PA4,PULLDOWN);
        #endif
        pinmode(PA5,PULLDOWN);
        pinmode(PA6,PULLDOWN);
        pinmode(PA7,PULLDOWN);
    }
    
    if(spi == SPI2)
    {
        #ifdef SPI_INIT_CS    
        pinmode(PB12,PULLDOWN);
        pinhigh(PB12);
        #endif
        pinmode(PB13,PULLDOWN);
        pinmode(PB14,PULLDOWN);
        pinmode(PB15,PULLDOWN);
    }
	
	if(spi == SPI3)
    {
        #ifdef SPI_INIT_CS    
        pinmode(PC9,PULLDOWN);
        pinhigh(PC9);
        #endif
        pinmode(PC10,PULLDOWN);
        pinmode(PC11,PULLDOWN);
        pinmode(PC12,PULLDOWN);
    }
}

void __spi_clk_cmd(SPI_TypeDef *spi,FunctionalState x)
{
    if(spi == SPI1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, x);
    if(spi == SPI2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, x);
    if(spi == SPI3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, x);
}

uint16_t __spi_divider(int divider)
{
    switch(divider)
    {
    case 2:
        return SPI_BaudRatePrescaler_2;
    case 4:
        return SPI_BaudRatePrescaler_4;
    case 8:
        return SPI_BaudRatePrescaler_8;
    case 16:
        return SPI_BaudRatePrescaler_16;
    case 32:
        return SPI_BaudRatePrescaler_32;
    case 64:
        return SPI_BaudRatePrescaler_64;
    case 128:
        return SPI_BaudRatePrescaler_128;
    case 256:
        return SPI_BaudRatePrescaler_256;
    default:
        assert_param(1);//generate a error for this wrong spi divider
        return SPI_BaudRatePrescaler_256;
    }
}

void __spi_init(SPI_TypeDef *spi,
                    uint8_t mode_number,
                    uint8_t databits,
                    bool lsbfirst,
                    int divider,
                    bool master,
                    uint8_t lines,      //1 line or 2 lines
                    uint8_t direction)
{
    SPI_InitTypeDef  SPI_InitStructure;
    
    __spi_init_pins(spi);
    __spi_clk_cmd(spi,ENABLE);
    
    assert_param(IS_SPI_DATABITS(databits));
    assert_param(IS_SPI_MODENUMBER(mode_number));
        
    uint16_t datasize;
    if(databits == 8)
        datasize = SPI_DataSize_8b;
    else if(databits == 16)
        datasize = SPI_DataSize_16b;
    else assert_param(0);
    
    uint16_t cpol,cpha;
    cpol = (mode_number & 0x02) ? SPI_CPOL_High  : SPI_CPOL_Low;
    cpha = (mode_number & 0x01) ? SPI_CPHA_2Edge : SPI_CPHA_1Edge;
        
    SPI_I2S_DeInit(spi);
    uint16_t dir;
    if(lines == 1)
    {
        if(direction == SPI_RX)
            dir = SPI_Direction_1Line_Rx;
        else if(direction == SPI_TX)
            dir = SPI_Direction_1Line_Tx;
        else assert_param(0);
    }
    else if(lines == 2)
    {
        if(direction == SPI_RX)
            dir = SPI_Direction_2Lines_RxOnly;
        else if(direction == (SPI_RX | SPI_TX))
            dir = SPI_Direction_2Lines_FullDuplex;
        else assert_param(0);
    }
    else assert_param(0);
    SPI_InitStructure.SPI_Direction = dir;
    SPI_InitStructure.SPI_DataSize = datasize;
    SPI_InitStructure.SPI_CPOL = cpol;
    SPI_InitStructure.SPI_CPHA = cpha;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = __spi_divider(divider);
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = master ? SPI_Mode_Master : SPI_Mode_Slave;
    SPI_Init(spi, &SPI_InitStructure);
    
    SPI_Cmd(spi, ENABLE);
}

void spi_deinit(SPI_TypeDef *spi)
{
    //SPI_Cmd(spi,DISABLE);
    //SPI_I2S_DeInit(spi);
    __spi_clk_cmd(spi,DISABLE);
    __spi_deinit_pins(spi);
}

