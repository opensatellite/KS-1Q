#pragma once

#define debug   kprintf

//-----------------------info-------------------------------
#define PRODUCT_NAME    "KCNAV2C INS/FDR     " //string 23 char
#define BUILD_NUMBER    __DATE__               //integer 6 char

#define USE_FULL_ASSERT

//---------------uart ports---------------------
#define SERIAL_PORT			1
#define SERIAL_BAUD			115200

#define Z_BAUD				115200

#define CONSOLE_PORT		1


#define MODBUS_TXBUF_SIZE	1
#define MODBUS_RXBUF_SIZE	1

#define CONSOLE_TXBUF_SIZE	256
#define CONSOLE_RXBUF_SIZE	16*1024

#define SERIAL_TXBUF_SIZE	1
#define SERIAL_RXBUF_SIZE	1

//---------------adc----------------------------

#define ADC1_SPI			SPI3
#define ADC1_CS				C_PC8
#define ADC1_START			C_PD8
#define ADC1_PWDN			C_PD9
#define ADC1_DRDY			C_PD10
#define ADC1_CLK_PIN		C_PD12
#define ADC1_DEFAULT_MODE	adc_input_diff
#define ADC1_DELAY			0
#define ADC1_RATE			1

#define ADC2_SPI			SPI3
#define ADC2_CS				C_PC9
#define ADC2_START			C_PD11
#define ADC2_PWDN			C_PD14
#define ADC2_DRDY			C_PD15
#define ADC2_CLK_PIN		C_PD13
#define ADC2_DEFAULT_MODE	adc_input_single
#define ADC2_DELAY			3
#define ADC2_RATE			2

#define ADC_CHIPS			2
#define ADC_CHANNELS		16

#define ADC_TEMP_SCALE		0.000563
#define ADC_CLK_FREQ		14000000
#define ADC_SPI_DIV			16				//spi freq < clk freq / 2

#define ADC_MIN_RATE		20

#define THERMO_FILTER_TAU	25				//tau (samples)
#define THERMO_PERIOD		200000			//transmit period / us

//---------------imu sensors adis16375 and hmc5883----------
//set adis16375_DRDY_ISR function in stm32f10x_it.c
//set adis16375_SPI_ISR function in stm32f10x_it.c
#define PIN_IMU_SYNC            C_PA8
#define PIN_IMU_RST             C_PB4
#define PIN_IMU_IRQ             C_PB5         
#define PIN_IMU_CS              C_PA4
#define SPI_SENSOR              SPI1        //sensor spi bus
#define SPI_SENSOR_SPEED        4
#define PWM_IMU_SYNC            C_T1C1

#define IMU_FILTER_ENABLE       (ADIS16375_FILTER_GYROX|ADIS16375_FILTER_GYROY|ADIS16375_FILTER_GYROZ)
#define IMU_FILTER_BANKS        {0,0,0,0,0,0}

#define COMPASS_I2C             I2C_SENSOR
#define COMPASS_DELAY           20      // >=10ms
#define COMPASS_AVERAGING       1    

#define PIN_MAGD_CS				C_PB10
#define MAGD_SPI				SPI1
#define PIN_MAGD_DRDY			C_PB1


//---------------sdcard control-----------------
#define SD_PWR_SWITCHABLE       0       //0 or 1
#define PIN_SD_PWR              0       
#define SD_HAS_WP               0       //0 or 1
#define PIN_SD_WP               0
#define SD_HAS_CD               0       //0 or 1
#define PIN_SD_CD               0

//---------------spi bus, sdcard----------------
#define MS5611_CS				PB11
#define SPI_MS5611				SPI1


#define PIN_SD_CS               C_PC9
#define PIN_FLASH_CS			C_PA15
#define SPI_SD                  SPI3        //sdcard spi bus
#define SPI_SD_SPEED            4
#define SDCARD_CHECK_INTERVAL	3000
#define SDCARD_ERROR_TOLERANCE	12

/*--------------can bus-----------------------*/
#define CAN_Tx					PB13	
#define CAN_Rx					PB12

/*--------------led-----------------------*/
#define LED_GREEN				PC5
#define LED_RED					PC4

//-----------------------barometer task---------------------
#define BAROMETER_PRIO          osPriorityAboveNormal       //thread priority
#define PRES_READ_DELAY         20      //pressure read period in ms
#define PRES_TEMP_INTERLEAVING  10      //PRES:TEMP = N:1

//-----------------------cli interface----------------------
#define CLI_PRIO                osPriorityHigh

//-----------------------navd task--------------------------
#define NAVD_PRIO               osPriorityRealtime

//-----------------------system task------------------------
#define SYSTEM_PRIO             osPriorityNormal

//-----------------------telemetry task---------------------
#define TELEMETRY_PRIO          osPriorityLow
#define RS232_TXBUF_SIZE        256     //32,64,128,256,512,1024...
#define RS232_RXBUF_SIZE        16
#define BAUD_RS232              9600    //telemetry port
#define RADIO_CFG_PIN           PA0

//-----------------------imumagd task-----------------
#define IMUD_SENSOR_SAMPLERATE  1200.0 //700Hz-2.25kHz
//sensor raw data samples per second, depending on sensor
#define IMUD_SENSOR_DECIM       492
#define MAGD_PRIO               osPriorityNormal        //thread priority

//-----------------------fdr task---------------------------
#define FDR_WRITE_PRIO          osPriorityLow
#define FDR_BUFSIZE             8192
#define FDR_BLOCK_SIZE          4096
#define FDR_MAX_FILEID          999
#define FDR_FILENAME            "fdr%d.bin"
#define FDR_REPORT_BYTES        0   //bytes written report interval

//-----------------------gpsd task--------------------------
#define GPSD_PRIO               osPriorityNormal  //thread priority
#define GPS_TXBUF_SIZE          16          //32,64,128,256,512,1024...
#define GPS_RXBUF_SIZE          256         //32,64,128,256,512,1024... 
#define BAUD_GPS                38400
#define GPS_INTERVAL            40          //gps checking interval


#define os_min_stack            configMINIMAL_STACK_SIZE
#define os_tick_period			configTICK_RATE_HZ

//-----------------------control task-----------------------
#define CONTROL_PRIO            osPriorityHigh

//-----------------------canbusd task-----------------------
#define CANBUSD_PRIO            osPriorityAboveNormal


/*-----------------------Debug----------------------*/
#define kprintf					printf

//---------------options------------------------
//#define AUTO_REBOOT
#define CREATE_NEW_ON_START

