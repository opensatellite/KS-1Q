#ifndef SI4463_H
#define SI4463_H

#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_spi/mss_spi.h"



#define RX_SDN	MSS_GPIO_0
#define TX_SDN	MSS_GPIO_1
#define RX_NIRQ	MSS_GPIO_2
#define RX_SPI0_SLAVE_MODE MSS_SPI_SLAVE_0
#define TX_SPI0_SLAVE_MODE MSS_SPI_SLAVE_1

#define pinhigh(gpio)	MSS_GPIO_set_output(gpio,1)
#define pinlow(gpio)	MSS_GPIO_set_output(gpio,0)



#define spi0	(&g_mss_spi0)

#define spi_rw(spix,data) 	MSS_SPI_transfer_frame(spix,data)
#define spi_cs_l(spix,MSS_SPI_SLAVE_x)		MSS_SPI_set_slave_select(spix,MSS_SPI_SLAVE_x)
#define spi_cs_h(spix,MSS_SPI_SLAVE_x)		MSS_SPI_clear_slave_select(spix,MSS_SPI_SLAVE_x)

void si4463_rst(mss_gpio_id_t pin);

uint8_t radio_comm_GetResp(uint8_t byteCount, uint8_t* pData,mss_spi_slave_t slave_mode );
void radio_comm_SendCmd(uint8_t byteCount, uint8_t *pData,mss_spi_slave_t slave_mode );
uint8_t si446x_configuration_init(const uint8_t* pSetPropCmd,mss_spi_slave_t slave_mode );
void si446x_get_int_status(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND,mss_spi_slave_t slave_mode);
void vRadio_Init(mss_spi_slave_t slave_mode);

void si446x_start_rx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN, uint8_t NEXT_STATE1, uint8_t NEXT_STATE2, uint8_t NEXT_STATE3,mss_spi_slave_t slave_mode);
void vRadio_StartRX(uint8_t channel,mss_spi_slave_t slave_mode);

void si446x_start_tx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN,mss_spi_slave_t slave_mode);
void vRadio_StartTX(uint8_t channel,mss_spi_slave_t slave_mode);


void si446x_get_modem_status(uint8_t MODEM_CLR_PEND,mss_spi_slave_t slave_mode );
void si446x_part_info(mss_spi_slave_t slave_mode);


void si446x_set_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP, uint8_t *data,mss_spi_slave_t slave_mode);
void si446x_get_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP,mss_spi_slave_t slave_mode);


extern uint8_t cmd[8];
extern uint8_t rssi_rep_data[8];
extern uint8_t property[16];


enum
{
	SI446X_SUCCESS,
	SI446X_NO_PATCH,
	SI446X_CTS_TIMEOUT,
	SI446X_PATCH_FAIL,
	SI446X_COMMAND_ERROR
};


void si4463_init(void);


#endif
