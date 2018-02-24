#include "bsp.h"
#include "console.h"
#include "main.h"
#include "sdcard.h"
#include "spi_flash.h"
#include "os.h"

extern void disk_timerproc_helper(void);

uart_config_t uart_rs232_config = {
  .USART_BaudRate = SERIAL_BAUD,
  .USART_WordLength = USART_WordLength_8b,
  .USART_StopBits = USART_StopBits_1,
  .USART_Parity = USART_Parity_No,
  .USART_Mode = USART_Mode_Tx | USART_Mode_Rx,
  .USART_HardwareFlowControl = USART_HardwareFlowControl_None
};

static void gpio_init()
{
	pinmodespd(CAN_Tx, AFPUSHPULL, pin_25MHz);
	pinmodespd(CAN_Rx, AFPULLUP, pin_25MHz);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	
	pinmodespd(PB11,PUSHPULL,pin_25MHz);
    pinhigh(PB11);
}

int sdcard_init()
{
	sdcard_reload();
	
	if(sdcard_detection())
	{
		sdcard_fsinfo();
		return 1;
	}
	return 0;
}

void bsp_init()
{
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08080000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1|
						   RCC_AHB1Periph_GPIOA|
							   RCC_AHB1Periph_GPIOB|
								   RCC_AHB1Periph_GPIOC|
									   RCC_AHB1Periph_GPIOD|
										   RCC_AHB1Periph_GPIOE,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|
                         RCC_APB2Periph_USART1|
                         RCC_APB2Periph_SPI1,
                         ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2|
                         RCC_APB1Periph_USART2|
							 RCC_APB1Periph_CAN1,
                         ENABLE);
	
	
	gpio_init();
	
	//console(CONSOLE_PORT, SERIAL_BAUD);
	
	spi_init(SPI_SD,0,8,256);
	spi_init(SPI_SENSOR,3,16,SPI_SENSOR_SPEED);
	
	uart_init(SERIAL_PORT, &uart_rs232_config);
    uart_open(SERIAL_PORT, O_RDWR);
	
	//timer_clock_cntmode(TIM2,1000,1000,);
	
	//vtimer_init();
	//systick_init();
}


