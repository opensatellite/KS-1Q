#include "si4463.h"
#include "radio_rx_config.h"
#include "radio_tx_config.h"
//#include "rx2_radio_config.h"

//uint8_t config_table[] = RADIO_CONFIGURATION_DATA_ARRAY;
uint8_t Pro2Cmd[16];
uint8_t ctsWentHigh;

uint8_t cmd[8] = { 0,0,0,0,0,0,0,0 };
uint8_t rssi_rep_data[8];
uint8_t property[16];

void si4463_rst()
{
	uint32_t temp;
	
	pinhigh(TX_SDN);
	for (temp = 50000; temp > 0; temp--);
	pinlow(TX_SDN);
	for (temp = 50000; temp > 0; temp--);
	spi_cs_h(RF_Tx);
	
	
	pinhigh(RX_SDN);
	for (temp = 50000; temp > 0; temp--);
	pinlow(RX_SDN);
	for (temp = 50000; temp > 0; temp--);
	spi_cs_h(RF_Rx);
}

uint8_t radio_comm_GetResp(mss_gpio_id_t slavex, uint8_t byteCount, uint8_t* pData)
{
	uint8_t ctsVal = 0;
	uint16_t	errCnt = 50000;
	
	
	while (errCnt != 0)
	{
		spi_cs_l(slavex);
		spi_rw(spi0, 0x44);
		ctsVal = spi_rw(spi0, 0x00);
		if (ctsVal == 0xff)
		{
			if (byteCount)
			{
				while (byteCount--)
				{
					*pData++ = spi_rw(spi0, 0x00);
				}
			}
			spi_cs_h(slavex);
			break;
		}
		spi_cs_h(slavex);
		errCnt--;
	}

	if (errCnt == 0)
	{
		NVIC_SystemReset();
	}

	if (ctsVal == 0xFF)
	{
		ctsWentHigh = 1;
	}
	return ctsVal;
}


void radio_comm_SendCmd(mss_gpio_id_t slavex, uint8_t byteCount, uint8_t *pData)
{
	radio_comm_GetResp(slavex,0, 0);
	spi_cs_l(slavex);
	while (byteCount--)
	{
		spi_rw(spi0,*pData++);
	}
	spi_cs_h(slavex);
}

uint8_t si446x_configuration_init(mss_gpio_id_t slavex,const uint8_t* pSetPropCmd)
{
	uint8_t col;
	uint8_t numOfBytes;


	while (*pSetPropCmd != 0x00)
	{
		numOfBytes = *pSetPropCmd++;

		if (numOfBytes > 16u)
		{

			return SI446X_COMMAND_ERROR;
		}

		for (col = 0u; col < numOfBytes; col++)
		{
			Pro2Cmd[col] = *pSetPropCmd;
			pSetPropCmd++;
		}
		radio_comm_SendCmd(slavex,numOfBytes, Pro2Cmd);

		//for(uint16_t i = 0xffff;i>0;i--);
		
		if (radio_comm_GetResp(slavex,0, 0) != 0xFF)
		{
			return SI446X_CTS_TIMEOUT;
		}
		
	}
		
	return SI446X_SUCCESS;
}

void vRadio_Init()
{
	si4463_rst();
	
	si446x_part_info(RF_Tx);
	si446x_part_info(RF_Rx);
	
	uint8_t txconfig_table[] = TX_RADIO_CONFIGURATION_DATA_ARRAY;
	si446x_configuration_init(RF_Tx,txconfig_table);
	si446x_get_int_status(RF_Tx,0, 0, 0);
	
        uint8_t rx_config_table[] = RADIO_CONFIGURATION_DATA_ARRAY; //RX_RADIO_CONFIGURATION_DATA_ARRAY;//RADIO_CONFIGURATION_DATA_ARRAY;//
	si446x_configuration_init(RF_Rx,rx_config_table);
	si446x_get_int_status(RF_Rx,0, 0, 0);
}

void si446x_start_rx(mss_gpio_id_t slavex,uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN, uint8_t NEXT_STATE1, uint8_t NEXT_STATE2, uint8_t NEXT_STATE3)
{
	Pro2Cmd[0] = 0x32;// SI446X_CMD_ID_START_RX;
	Pro2Cmd[1] = CHANNEL;
	Pro2Cmd[2] = CONDITION;
	Pro2Cmd[3] = (uint8_t)(RX_LEN >> 8);
	Pro2Cmd[4] = (uint8_t)(RX_LEN);
	Pro2Cmd[5] = NEXT_STATE1;
	Pro2Cmd[6] = NEXT_STATE2;
	Pro2Cmd[7] = NEXT_STATE3;
	radio_comm_SendCmd(slavex,8, Pro2Cmd);
}

void vRadio_StartRX(mss_gpio_id_t slavex,uint8_t channel)
{
	// Read ITs, clear pending ones
	si446x_get_int_status(slavex,0u, 0u, 0u);

	/* Start Receiving packet, channel 0, START immediately, Packet off  */
	si446x_start_rx(slavex,channel, 0u, 0u, 0, 8, 8);
	/*   channel, 0u, 0u  SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
	SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_RX,
	SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX );
	*/
}

void si446x_start_tx(mss_gpio_id_t slavex,uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN)
{
	  Pro2Cmd[0] =0x31;// SI446X_CMD_ID_START_RX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (uint8_t)(RX_LEN >> 8);
    Pro2Cmd[4] = (uint8_t)(RX_LEN);
    Pro2Cmd[5] = 0x00;
    Pro2Cmd[6] = 0x00;
	 radio_comm_SendCmd(slavex, 7, Pro2Cmd);
}

void vRadio_StartTX(mss_gpio_id_t slavex,uint8_t channel)
{
  si446x_get_int_status(slavex,0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet off  */
  si446x_start_tx(slavex,channel, 0u, 0u);
}

void vRadio_ReadyTX(mss_gpio_id_t slavex,uint8_t channel)
{
	Pro2Cmd[0] =0x31;// SI446X_CMD_ID_START_RX;
	Pro2Cmd[1] = channel;
	Pro2Cmd[2] = 0x01;
	Pro2Cmd[3] = 0;
	Pro2Cmd[4] = 0;
	Pro2Cmd[5] = 0x00;
	Pro2Cmd[6] = 0x00;
	 radio_comm_SendCmd(slavex, 7, Pro2Cmd);
}

void si446x_get_int_status(mss_gpio_id_t slavex, uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND)
{
	Pro2Cmd[0] = 0x20;//SI446X_CMD_ID_GET_INT_STATUS;
	Pro2Cmd[1] = PH_CLR_PEND;
	Pro2Cmd[2] = MODEM_CLR_PEND;
	Pro2Cmd[3] = CHIP_CLR_PEND;

	radio_comm_SendCmd(slavex,4, Pro2Cmd);

	radio_comm_GetResp(slavex,8, Pro2Cmd);

}

uint8_t si446x_get_modem_status(mss_gpio_id_t slavex)
{
	rssi_rep_data[0] = 0x22;// SI446X_CMD_ID_GET_MODEM_STATUS;

	radio_comm_SendCmd(slavex,1, rssi_rep_data);

	radio_comm_GetResp(slavex,8, rssi_rep_data);

	return rssi_rep_data[2];
	//    Si446xCmd.GET_MODEM_STATUS.MODEM_PEND   = Pro2Cmd[0];
	//    Si446xCmd.GET_MODEM_STATUS.MODEM_STATUS = Pro2Cmd[1];
	//    Si446xCmd.GET_MODEM_STATUS.CURR_RSSI    = Pro2Cmd[2];
	//    Si446xCmd.GET_MODEM_STATUS.LATCH_RSSI   = Pro2Cmd[3];
	//    Si446xCmd.GET_MODEM_STATUS.ANT1_RSSI    = Pro2Cmd[4];
	//    Si446xCmd.GET_MODEM_STATUS.ANT2_RSSI    = Pro2Cmd[5];
	//    Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET =  ((uint16_t)Pro2Cmd[6] << 8) & 0xFF00;
	//    Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET |= (uint16_t)Pro2Cmd[7] & 0x00FF;
}

void si446x_part_info(mss_gpio_id_t slavex)
{
	cmd[0] = 0x01;//SI446X_CMD_ID_PART_INFO;

	radio_comm_SendCmd(slavex,1, cmd);
	
	radio_comm_GetResp(slavex,8, cmd);
	
	cmd[0] = 0;
	//    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PART_INFO,
	//                              Pro2Cmd,
	//                              SI446X_CMD_REPLY_COUNT_PART_INFO,
	//                              Pro2Cmd );

	//    Si446xCmd.PART_INFO.CHIPREV         = Pro2Cmd[0];
	//    Si446xCmd.PART_INFO.PART            = ((uint16_t)Pro2Cmd[1] << 8) & 0xFF00;
	//    Si446xCmd.PART_INFO.PART           |= (uint16_t)Pro2Cmd[2] & 0x00FF;
	//    Si446xCmd.PART_INFO.PBUILD          = Pro2Cmd[3];
	//    Si446xCmd.PART_INFO.ID              = ((uint16_t)Pro2Cmd[4] << 8) & 0xFF00;
	//    Si446xCmd.PART_INFO.ID             |= (uint16_t)Pro2Cmd[5] & 0x00FF;
	//    Si446xCmd.PART_INFO.CUSTOMER        = Pro2Cmd[6];
	//    Si446xCmd.PART_INFO.ROMID           = Pro2Cmd[7];
}

void si446x__set_property(mss_gpio_id_t slavex,uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP, uint8_t *data)
{
	uint8_t i = 0;
	property[0] = 0x11; //SI446X_CMD_ID_GET_PROPERTY;
	property[1] = GROUP;
	property[2] = NUM_PROPS;
	property[3] = START_PROP;
	while (NUM_PROPS--)
	{
		property[4 + i] = *data;
		i += 1;
	}
	radio_comm_SendCmd(slavex,5, property);
}

void si446x_get_property(mss_gpio_id_t slavex,uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP)
{
	property[0] = 0x12; //SI446X_CMD_ID_GET_PROPERTY;
	property[1] = GROUP;
	property[2] = NUM_PROPS;
	property[3] = START_PROP;

	radio_comm_SendCmd(slavex, 4, property);

	radio_comm_GetResp(slavex, 16, property);

	//    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_PROPERTY,
	//                              Pro2Cmd,
	//                              Pro2Cmd[2],
	//                              Pro2Cmd );

	//    Si446xCmd.GET_PROPERTY.DATA[0 ]   = Pro2Cmd[0];
	//    Si446xCmd.GET_PROPERTY.DATA[1 ]   = Pro2Cmd[1];
	//    Si446xCmd.GET_PROPERTY.DATA[2 ]   = Pro2Cmd[2];
	//    Si446xCmd.GET_PROPERTY.DATA[3 ]   = Pro2Cmd[3];
	//    Si446xCmd.GET_PROPERTY.DATA[4 ]   = Pro2Cmd[4];
	//    Si446xCmd.GET_PROPERTY.DATA[5 ]   = Pro2Cmd[5];
	//    Si446xCmd.GET_PROPERTY.DATA[6 ]   = Pro2Cmd[6];
	//    Si446xCmd.GET_PROPERTY.DATA[7 ]   = Pro2Cmd[7];
	//    Si446xCmd.GET_PROPERTY.DATA[8 ]   = Pro2Cmd[8];
	//    Si446xCmd.GET_PROPERTY.DATA[9 ]   = Pro2Cmd[9];
	//    Si446xCmd.GET_PROPERTY.DATA[10]   = Pro2Cmd[10];
	//    Si446xCmd.GET_PROPERTY.DATA[11]   = Pro2Cmd[11];
	//    Si446xCmd.GET_PROPERTY.DATA[12]   = Pro2Cmd[12];
	//    Si446xCmd.GET_PROPERTY.DATA[13]   = Pro2Cmd[13];
	//    Si446xCmd.GET_PROPERTY.DATA[14]   = Pro2Cmd[14];
	//    Si446xCmd.GET_PROPERTY.DATA[15]   = Pro2Cmd[15];
}


