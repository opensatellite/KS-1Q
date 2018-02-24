#include "m2sxxx.h"
#include "mss_gpio.h"
#include "radio_tx_config_40ksps.h"
#include "radio_rx_config.h"
#include "si4463.h"
#include "mss_spi.h"

#include <string.h>
#include <stdio.h>

#define Flag_Register_Addr_0 0x30000000
#define Tx_Base_Addr_A       0x30002000
#define Tx_Base_Addr_B       0x30004000
#define Packet_Data_Num      225

 uint8_t cmd[8];
 uint8_t property[16];
 uint32_t data_buff[10][56];

 uint8_t temp=0;


 volatile uint32_t ADDR_A=Tx_Base_Addr_A;
 volatile uint32_t ADDR_B=Tx_Base_Addr_B;

uint8_t delay(uint32_t num)
{
	uint32_t count1=0,count2=0;
	for(count1=0;count1<num;count1++)
	{
		for(;count2<30000;count2++);
	}
	return 0;
}
uint8_t CPU_Send_Data( uint32_t *data)
{
	uint32_t flag=0,set_flag_a=0,set_flag_b=0;
	uint8_t i=1;
/*	uint32_t *addr=(uint32_t *)0x30000000;*/
	uint32_t *addr=(uint32_t *)0x30002000;
	memcpy(&flag,(uint32_t *)0x30000000,1);
	//flag=*((uint32_t *)0x30000000);
	set_flag_a=flag|0x00000001;
	set_flag_b=flag|0x00000002;
	if(!(flag&0x00000001))
	{
		memcpy((uint32_t *)0x30002000,data,224);
		*(uint32_t *)0x30000000=set_flag_a;
		flag=*(uint32_t *)0x30000000;
		return 1;
	}
	else if(!(flag&0x00000002))
	{
		memcpy((uint32_t *)0x30004000,data,224);
		*(uint32_t *)0x30000000=set_flag_b;
		flag=*(uint32_t *)0x30000000;
		return 1;
	}
	else
	{

		return 0;
	}
}
void data_gen()
{
	uint8_t x_count=0,y_count=0;
	uint32_t data=0;
	for(y_count=0;y_count<10;y_count++)
	{
		for(x_count=0;x_count<Packet_Data_Num;x_count+=4)
		{

			data |=y_count;
			data =data <<8;
			data |=y_count;
			data =data <<8;
			data |=y_count;
			data =data <<8;
			data |=y_count;
//		data =data<<8;
			data_buff[y_count][x_count/4]=data;
			data=0;
	}
	}


}



int main()
{


	uint8_t data_num=0;

	data_gen();

	while(1)
		{

		for(data_num=0;data_num<10;)
		{
			if(CPU_Send_Data(*(data_buff+data_num)))
				data_num++;
		}
	//	delay(*(*(data_buff+data_num)));
		}
		}
/*	MSS_GPIO_init();
	MSS_GPIO_config(RX_SDN, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(TX_SDN, MSS_GPIO_OUTPUT_MODE);

	MSS_SPI_init(&g_mss_spi0);
		MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 128, 8);
		MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_1, MSS_SPI_MODE0, 128, 8);

		vRadio_Init(TX_SPI0_SLAVE_MODE);
		si446x_part_info(TX_SPI0_SLAVE_MODE);
		vRadio_StartTX(0x5A,TX_SPI0_SLAVE_MODE);

	while( 1 )
	{
		vRadio_Init(TX_SPI0_SLAVE_MODE);
		si446x_part_info(TX_SPI0_SLAVE_MODE);
		vRadio_StartTX(0x5A,TX_SPI0_SLAVE_MODE);
		//if(!CPU_Send_Data(data_buff));
//		  i=i+1;
	}*/


