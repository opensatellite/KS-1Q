#include "bsp.h"
#include "stdio.h"
#include "si4463.h"
#include "tlv2548.h"
static void gpio_init()
{
	MSS_GPIO_init();
        
	MSS_GPIO_config(FABRIC_RST, MSS_GPIO_OUTPUT_MODE);
        pinhigh(FABRIC_RST);
        pinlow(FABRIC_RST);
        pinhigh(FABRIC_RST);
        
	MSS_GPIO_config(RX_SDN, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(TX_SDN, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(RF_Tx, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(RF_Rx, MSS_GPIO_OUTPUT_MODE);
        
	MSS_GPIO_config(ADC_CS,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(FRAM_CS,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(ADC_FS,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(ADC_PWDN,MSS_GPIO_OUTPUT_MODE);
        MSS_GPIO_config(ADC_CSTART, MSS_GPIO_OUTPUT_MODE);
        
        MSS_GPIO_config(ADC_INT, MSS_GPIO_INPUT_MODE);

	MSS_GPIO_config(ANT_SW1,MSS_GPIO_OUTPUT_MODE);
	//MSS_GPIO_config(ANT_SW1,MSS_GPIO_OUTPUT_MODE);

	MSS_GPIO_config(L4V_EN,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(PA4V_ON,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(A3V3_EN,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(L3V3_EN2,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(L3V3_EN1,MSS_GPIO_OUTPUT_MODE);

	MSS_GPIO_config(SW, MSS_GPIO_OUTPUT_MODE);

	pinhigh(RF_Tx);
	pinhigh(RF_Rx);

	pinhigh(L4V_EN);
	pinhigh(A3V3_EN);
	//OSC_SW1();
	OSC_SW2();
	ANT1CN200_RX();
	pinhigh(ADC_CS);
        pinhigh(ADC_CSTART);
	pinhigh(FRAM_CS);
        pinhigh(ADC_FS);
	//MSS_GPIO_config(RX_NIRQ, MSS_GPIO_INOUT_MODE | MSS_GPIO_IRQ_LEVEL_LOW);

}

static void spi_init()
{
	MSS_SPI_init(&g_mss_spi0);
	MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_0, MSS_SPI_MODE1, 64, 8);
	MSS_SPI_set_slave_select(&g_mss_spi0,MSS_SPI_SLAVE_0);
	//MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_1, MSS_SPI_MODE0, 64, 8);
}

static void rf_init()
{
	vRadio_Init();

	//vRadio_StartTX(RF_Tx,0x00);
	vRadio_StartRX(RF_Rx,0x00);
}

void bsp_init()
{
	//NVIC_SetPriorityGrouping(3);
	NVIC_SetPriority(CAN_IRQn, 11);
	//NVIC_EnableIRQ(CAN_IRQn);
        
        gpio_init();
	//uart_init();
	spi_init();
	tlv2548_init();
	rf_init();
        
        
}

void memcpy32(uint32_t* src, uint32_t* dst, uint32_t len)
{
	while (len--)
	{
		*dst++ = *src++;
	}
}
