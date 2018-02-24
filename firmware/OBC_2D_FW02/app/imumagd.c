#include "os.h"
#include "imumagd.h"
#include "adis16375.h"
//#include "hmc5883l.h"
#include "fdr.h"
#include "sdcard.h"
#include "memory_thread.h"
//#define IMUD_PRIO  1       //thread priority

imu_data_t imu_readout;
mag_data_t mag_readout;

#define MAX_RETRY       3

const adis16375_config_t adis16375_cfg = {
    .spi = SPI_SENSOR,
    .cs = PIN_IMU_CS,
    .sync = PIN_IMU_SYNC,
    .rst = PIN_IMU_RST,
    .irq = NULL,
    .sync_pwmchannel = NULL,
    .decim = IMUD_SENSOR_DECIM,
};       

void imumagd_start()
{
    //void* args = 0;
    printf("Starting task imud... ");
    
    //init sensor
    printf("Init ADIS16375 sensor... ");
	
	
	adis16375_init((adis16375_config_t*)&adis16375_cfg);
	
    uint8_t filter_banks[6]=IMU_FILTER_BANKS;
    adis16375_filter(IMU_FILTER_ENABLE,filter_banks);
    adis16375_start();
    printf("OK\n");
}

void imumagd_stop()
{
    //adis16375_stop();
    kprintf("IMU STOP\n");
}

void imumagd_thread(void  *args)
{
	//FIL f;
	
	LIS3MDL_init();
	
	//openfile(&f);
	//systick_attach(adis16375_process);
	for(;;)
	{
		printf("Now runing imu thread\n");
		
		int16_t rawdata[4];
		magd_rdata(rawdata);
		
		dm_var.magx = rawdata[0];
		dm_var.magy = rawdata[1];
		dm_var.magz = rawdata[2];
		dm_var.magtemp = (int8_t)(rawdata[3] / 8 + 25);
		
		if(dm_var.pwrimu)
		{
			uint32_t WakeTime =  osKernelSysTick();
			
			dm_var.pwrimu --;
			imumagd_start();
				
			imu_data_t m;
			
			while(pinread(PB5));
			
   	 		adis16375_get_output(&(m.body_acc),&(m.gyro),&(m.temp));
			
			dm_var.accx = m.body_acc.x;
			dm_var.accy = m.body_acc.y;
			dm_var.accz = m.body_acc.z;
			
			dm_var.gyrox = m.gyro.x;
			dm_var.gyroy = m.gyro.y;
			dm_var.gyroz = m.gyro.z;
			
			dm_var.imutemp = (int)m.temp;
			
			imumagd_stop();
			/*
			if((dm_var.memstatus == 1) && (dm_var.memavail == 1))
			{
				sdcard_wlog(&f,(uint8_t*)&dm_var.gyrox,25);
			}
			*/
			
			osDelayUntil(&WakeTime,dm_var.imuinterval * 1000);
		
			printf("Now runing imu thread\n");
		}
		osDelay(1000);
	}
}




