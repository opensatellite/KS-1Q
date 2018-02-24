#include "si4463.h"
#include "radio_rx_config.h"
#include "radio_tx_config.h"

uint8_t rx_config_table[] = RX_RADIO_CONFIGURATION_DATA_ARRAY;
uint8_t tx_config_table[] = TX_RADIO_CONFIGURATION_DATA_ARRAY;
uint8_t Pro2Cmd[16];
uint8_t ctsWentHigh;

uint8_t cmd[8] = { 0,0,0,0,0,0,0,0 };
uint8_t rssi_rep_data[8];
uint8_t property[16];

void si4463_rst(mss_gpio_id_t pin)
{
	uint32_t temp,cnt=0,del=0;
	pinhigh(pin);
	for (temp = 500; temp > 0; temp--);
	pinlow(pin);
	for (temp = 500; temp > 0; temp--);
	ctsWentHigh = 0;
	if(pin==TX_SDN)del=TX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET;
	else if(pin==RX_SDN)del=RX_RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET;
	for(cnt=0;cnt<del;cnt++);
}

uint8_t radio_comm_GetResp(uint8_t byteCount, uint8_t* pData,mss_spi_slave_t slave_mode )
{
	uint8_t ctsVal = 0;
	uint16_t	errCnt = 50000;
	while (errCnt != 0)
	{
		spi_cs_l(spi0,slave_mode);
		spi_rw(spi0, 0x44);
		ctsVal = spi_rw(spi0, 0xff);
               
		if (ctsVal == 0xff)
		{
			if (byteCount)
			{
				while (byteCount--)
				{
					*pData++ = spi_rw(spi0, 0xff);
				}
			}
			spi_cs_h(spi0,slave_mode);
			break;
		}
		spi_cs_h(spi0,slave_mode);
		errCnt--;
	}

	if (errCnt == 0)
	{
		while (1); /* ERROR!!!!  CTS should never take this long. */
	}

	if (ctsVal == 0xFF)
	{
		ctsWentHigh = 1;
	}
	return ctsVal;
}


void radio_comm_SendCmd(uint8_t byteCount, uint8_t *pData,mss_spi_slave_t slave_mode )
{
	while (!ctsWentHigh)
	{
		radio_comm_GetResp(0,0,slave_mode);
	}
	spi_cs_l(spi0,slave_mode);
	while (byteCount--)
	{
		spi_rw(spi0,*pData++);
	}
	spi_cs_h(spi0,slave_mode);
	ctsWentHigh = 0;
}

uint8_t si446x_configuration_init(const uint8_t* pSetPropCmd,mss_spi_slave_t slave_mode )
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
		radio_comm_SendCmd(numOfBytes, Pro2Cmd,slave_mode);

		if (radio_comm_GetResp(0, 0,slave_mode) != 0xFF)
		{
			return SI446X_CTS_TIMEOUT;
		}
	}
	return SI446X_SUCCESS;
}

void vRadio_Init(mss_spi_slave_t slave_mode)
{
	uint8_t tep = 1;
        uint8_t *config_table;
	//	 uint16_t wDelay;
        
        if(slave_mode==RX_SPI0_SLAVE_MODE){
          config_table=rx_config_table;
	      si4463_rst(RX_SDN);
        }
        else if(slave_mode==TX_SPI0_SLAVE_MODE){
          config_table=tx_config_table;
          si4463_rst(TX_SDN);
        }
        
        
	while (tep)
	{
		if (tep)
		{
			tep = 0;
			si446x_configuration_init(config_table,slave_mode);
		}
		else
         
        if(slave_mode==RX_SPI0_SLAVE_MODE)
	  si4463_rst(RX_SDN);
        else if(slave_mode==TX_SPI0_SLAVE_MODE)
          si4463_rst(TX_SDN); 
	}
	si446x_get_int_status(0, 0, 0,slave_mode);
}

void si446x_start_rx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN, uint8_t NEXT_STATE1, uint8_t NEXT_STATE2, uint8_t NEXT_STATE3,mss_spi_slave_t slave_mode)
{
	Pro2Cmd[0] = 0x32;// SI446X_CMD_ID_START_RX;
	Pro2Cmd[1] = CHANNEL;
	Pro2Cmd[2] = CONDITION;
	Pro2Cmd[3] = (uint8_t)(RX_LEN >> 8);
	Pro2Cmd[4] = (uint8_t)(RX_LEN);
	Pro2Cmd[5] = NEXT_STATE1;
	Pro2Cmd[6] = NEXT_STATE2;
	Pro2Cmd[7] = NEXT_STATE3;
	radio_comm_SendCmd(8, Pro2Cmd,slave_mode);
}


void vRadio_StartRX(uint8_t channel,mss_spi_slave_t slave_mode)
{
	// Read ITs, clear pending ones
	si446x_get_int_status(0u, 0u, 0u,slave_mode);

	/* Start Receiving packet, channel 0, START immediately, Packet off  */
	si446x_start_rx(channel, 0u, 0u, 0, 8, 8,slave_mode);
	/*   channel, 0u, 0u  SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
	SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_RX,
	SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX );
	*/
}


void si446x_start_tx(uint8_t CHANNEL, uint8_t CONDITION, uint16_t RX_LEN,mss_spi_slave_t slave_mode)
{
	  Pro2Cmd[0] =0x31;// SI446X_CMD_ID_START_RX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (uint8_t)(RX_LEN >> 8);
    Pro2Cmd[4] = (uint8_t)(RX_LEN);
    Pro2Cmd[5] = 0x00;
    Pro2Cmd[6] = 0x00;
	 radio_comm_SendCmd( 7, Pro2Cmd ,slave_mode);
}

void vRadio_StartTX(uint8_t channel,mss_spi_slave_t slave_mode)
{
  si446x_get_int_status(0u, 0u, 0u,slave_mode);

  /* Start Receiving packet, channel 0, START immediately, Packet off  */
  si446x_start_tx(channel, 0u, 0u,slave_mode);
}


void si446x_get_int_status(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND,mss_spi_slave_t slave_mode)
{
	Pro2Cmd[0] = 0x20;//SI446X_CMD_ID_GET_INT_STATUS;
	Pro2Cmd[1] = PH_CLR_PEND;
	Pro2Cmd[2] = MODEM_CLR_PEND;
	Pro2Cmd[3] = CHIP_CLR_PEND;

	radio_comm_SendCmd(4, Pro2Cmd,slave_mode);

	radio_comm_GetResp(8, Pro2Cmd,slave_mode);

}

void si446x_get_modem_status(uint8_t MODEM_CLR_PEND,mss_spi_slave_t slave_mode )
{
	rssi_rep_data[0] = 0x22;// SI446X_CMD_ID_GET_MODEM_STATUS;
	rssi_rep_data[1] = MODEM_CLR_PEND;

	radio_comm_SendCmd(1, rssi_rep_data,slave_mode);

	radio_comm_GetResp(8, rssi_rep_data,slave_mode);


	//    Si446xCmd.GET_MODEM_STATUS.MODEM_PEND   = Pro2Cmd[0];
	//    Si446xCmd.GET_MODEM_STATUS.MODEM_STATUS = Pro2Cmd[1];
	//    Si446xCmd.GET_MODEM_STATUS.CURR_RSSI    = Pro2Cmd[2];
	//    Si446xCmd.GET_MODEM_STATUS.LATCH_RSSI   = Pro2Cmd[3];
	//    Si446xCmd.GET_MODEM_STATUS.ANT1_RSSI    = Pro2Cmd[4];
	//    Si446xCmd.GET_MODEM_STATUS.ANT2_RSSI    = Pro2Cmd[5];
	//    Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET =  ((uint16_t)Pro2Cmd[6] << 8) & 0xFF00;
	//    Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET |= (uint16_t)Pro2Cmd[7] & 0x00FF;
}

void si446x_part_info(mss_spi_slave_t slave_mode)
{
	cmd[0] = 0x01;//SI446X_CMD_ID_PART_INFO;

	radio_comm_SendCmd(1, cmd,slave_mode);

	radio_comm_GetResp(8, cmd,slave_mode);
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

void si446x_set_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP, uint8_t *data,mss_spi_slave_t slave_mode)
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
	radio_comm_SendCmd(5, property,slave_mode);
}

void si446x_get_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP,mss_spi_slave_t slave_mode)
{
	property[0] = 0x12; //SI446X_CMD_ID_GET_PROPERTY;
	property[1] = GROUP;
	property[2] = NUM_PROPS;
	property[3] = START_PROP;

	radio_comm_SendCmd(4, property,slave_mode);

	radio_comm_GetResp(16, property,slave_mode);

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


void si4463_sendtest()
{
	vRadio_Init(RX_SPI0_SLAVE_MODE);
	si446x_part_info(RX_SPI0_SLAVE_MODE);
	
	vRadio_StartRX(0,RX_SPI0_SLAVE_MODE);
	
	si446x_get_modem_status( 0x00,RX_SPI0_SLAVE_MODE );
}
