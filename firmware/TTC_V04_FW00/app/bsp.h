#ifndef BSP_H
#define BSP_H

#include "mss_gpio/mss_gpio.h"
#include "mss_spi/mss_spi.h"
//#include "mss_spi/mss_spi.h"


//#include "user_drivers/si4463.h"
/*
 #define RX_SDN		MSS_GPIO_0
 #define TX_SDN		MSS_GPIO_1
 #define RF_Rx		MSS_GPIO_2
 #define RF_Tx		MSS_GPIO_3
 #define RF_Tx_PA	MSS_GPIO_5
 */


#define SW          MSS_GPIO_0
#define FABRIC_RST      MSS_GPIO_1
#define ADC_CS		MSS_GPIO_8
#define ADC_FS		MSS_GPIO_9
#define ADC_PWDN	MSS_GPIO_10
#define L4V_EN		MSS_GPIO_12
#define ANT_SW1		MSS_GPIO_13
//#define ANT_SW2		MSS_GPIO_14
#define PA4V_ON		MSS_GPIO_15
#define L3V3_EN2	MSS_GPIO_16
#define L3V3_EN1	MSS_GPIO_17
#define RF_Rx		MSS_GPIO_18
#define RX_SDN		MSS_GPIO_19
#define RF_Tx		MSS_GPIO_20
#define TX_SDN		MSS_GPIO_21
#define A3V3_EN		MSS_GPIO_22
#define FRAM_CS		MSS_GPIO_25
#define ADC_INT         MSS_GPIO_26
#define ADC_CSTART      MSS_GPIO_29


#define ANT1CN200_RX() pinhigh(ANT_SW1);pinlow(PA4V_ON)
#define ANT1CN200_TX() pinlow(ANT_SW1);pinhigh(PA4V_ON)
#define ANT2CN201_RX() pinlow(ANT_SW1);pinlow(PA4V_ON)
#define ANT2CN201_TX() pinlow(ANT_SW1);pinhigh(PA4V_ON)

#define ADC0_CS_L()     pinlow(ADC_CS);MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 128, 16);MSS_SPI_set_slave_select(&g_mss_spi0,MSS_SPI_SLAVE_0)
#define ADC0_CS_H()     pinhigh(ADC_CS)

#define OSC_SW1()  pinhigh(L3V3_EN1);pinlow(L3V3_EN2);pinhigh(SW)
#define OSC_SW2()  pinlow(L3V3_EN1);pinhigh(L3V3_EN2);pinlow(SW)


#define pinhigh(gpio)	GPIO->GPIO_OUT|=1<<((uint8_t)gpio)
#define pinlow(gpio)	GPIO->GPIO_OUT&=~(1<<((uint8_t)gpio))

#define OBC_ADDR	0x00
#define TTC_ADDR	0x01
#define EPS_ADDR	0x02
#define TNC_ADDR	0x09
   
#define SPACECRAFT_ID           1
#define CCSDS_TYPE_DOWN_IP      5
#define CCSDS_VERSION           0

#define VREF    2000.0
#define ITTC_CAL(X)  ((X)*VREF/4095.0*2.0)
#define IPA_CAL(X)   ((X)*VREF/4095.0*2.0)

#define CAN_RX_PRIO 6
#define ROUTE_PRIO  5
#define RF_RX_PRIO  4
#define RF_TX_PRIO  3
#define DM_PRIO     2
#define BEACON_PRIO 1

void bsp_init(void);
void memcpy32(uint32_t* src, uint32_t* dst, uint32_t len);

extern uint8_t rx_active;

#endif
