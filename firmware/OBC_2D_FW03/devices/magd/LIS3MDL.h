#ifndef LIS3MDL_H
#define LIS3MDL_H

#define ID		0x0f
#define CTRL	0x20
#define STATUS	0x27
#define OUT_X	0x28
#define OUT_Y	0x2A
#define OUT_Z	0x2C
#define OUT_TEMP	0x2e

/*	CTRL1:	0xe0
	TEMP_EN:7	1		Temp_sensor_en
	OM[6:5]		11		Ultrl_high_performance
	DO[4:2]		000		0.625Hz
	FAST_ODR:1	0		FAST_ODR_disable
	ST:0		0		Self_Test_disable
*/

/*	CTRL2:	0x00
	FS[6:5]		00		¡À4gauss
	REBOOT:3	0		normal_mode
	SOFT_RST:2	0		not_reset
*/

/*	CTRL3:	0x00
	LP:5		0		low_power_mode_off
	SIM:2		0		4-wire-interface
	MD[1:0]		00		continuous-conversion_mode
*/

/*	CTRL4:	0x0C
	OMZ[3:2]	11		ultra-high-performance mode for Z-axis
	BLE:1		0		littl-endian
*/

/*	CTRL5:	0x00
	FAST_READ:7	0		allow read the low part of DATA_OUT
	BDU	:6		0		continuous update the DATA_REG
*/

#define MAGD_CFG	{0xe0,0x00,0x00,0x0c,0x00}


uint8_t LIS3MDL_init();
uint8_t magd_rdata();

#endif