#ifndef SI4463_H
#define SI4463_H

#include "mss_gpio/mss_gpio.h"
#include "mss_spi/mss_spi.h"
#include "bsp.h"

#define spi0	(&g_mss_spi0)

#define spi_rw(spix,data) 	MSS_SPI_transfer_frame(spix,data)
//#define spi_cs_l(spix,slavex)		MSS_SPI_set_slave_select(spix,(mss_spi_slave_t)(slavex))
#define spi_cs_l(spi_slave)		pinlow(spi_slave);MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_0, MSS_SPI_MODE1, 64, 8);MSS_SPI_set_slave_select(&g_mss_spi0,MSS_SPI_SLAVE_0)
#define spi_cs_h(spi_slave)		pinhigh(spi_slave)

//#define spi_cs_l(spi_slave)		//pinlow(spi_slave)
//#define spi_cs_h(spi_slave)		//pinhigh(spi_slave)
//#define spi_cs_h(spix,slavex)		MSS_SPI_clear_slave_select(spix,(mss_spi_slave_t)(slavex))

void si4463_reset(void);

uint8_t radio_comm_GetResp(mss_gpio_id_t slavex, uint8_t byteCount,
		uint8_t* pData);
void radio_comm_SendCmd(mss_gpio_id_t slaves, uint8_t byteCount, uint8_t *pData);
uint8_t si446x_configuration_init(mss_gpio_id_t slavex,
		const uint8_t* pSetPropCmd);
void si446x_get_int_status(mss_gpio_id_t slavex, uint8_t PH_CLR_PEND,
		uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND);
void vRadio_Init();

void si446x_start_rx(mss_gpio_id_t slaves, uint8_t CHANNEL, uint8_t CONDITION,
		uint16_t RX_LEN, uint8_t NEXT_STATE1, uint8_t NEXT_STATE2,
		uint8_t NEXT_STATE3);
void vRadio_StartRX(mss_gpio_id_t slavex, uint8_t channel);
void vRadio_ReadyTX(mss_gpio_id_t slavex,uint8_t channel);
void vRadio_StartTX(mss_gpio_id_t slavex, uint8_t channel);
void si446x_start_tx(mss_gpio_id_t slavex, uint8_t CHANNEL, uint8_t CONDITION,
		uint16_t RX_LEN);

uint8_t si446x_get_modem_status(mss_gpio_id_t slaves);
void si446x_part_info(mss_gpio_id_t slaves);

void si446x__set_property(mss_gpio_id_t slaves, uint8_t GROUP,
		uint8_t NUM_PROPS, uint8_t START_PROP, uint8_t *data);
void si446x_get_property(mss_gpio_id_t slaves, uint8_t GROUP, uint8_t NUM_PROPS,
		uint8_t START_PROP);

//void si4463_repro(uint16_t temp,uint8_t NUM_PROPS);

//void get_prio(void);

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
