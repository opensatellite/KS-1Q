
__packed struct dm_fixed_s {
	uint8_t null;
};

__packed struct dm_var_s {
	int16_t magx;
	int16_t magy;
	int16_t magz;
	int16_t magtemp;
	
	uint8_t resets;
	uint16_t wdt;
	uint16_t vin;
	uint16_t canerrs;
	
	uint8_t memstatus;
	/* Memory status
	   0 = no memory
	   1 = using sdcard
	   2 = using flash
	*/
	uint8_t formatdisk;
	/*
		write 0xd8 then 0x1c
		to clear all file
		go back to 0x00 after completed
	*/
	uint16_t memavail;
	uint16_t lastfile;
	
	uint8_t cammode;	
	/* camera mode
	   0=off, 
	   1=small pic + transmit now,
	   2=medium pic, 
	   3=large pic, 
	   4=timed shot, 
	   5=attitude best shot,
	   6=video
	*/ 
	uint8_t camcount;
	uint16_t camdelay;
	uint16_t caminterval;
	/* camera shotting interval in seconds
	   video recording length
	*/
	
	uint16_t pwrimu;
	/* imu running countdown
	   power off imu after going to zero
	   after each imuinterval, pwrimu = pwrimu - 1;
	*/
	uint8_t imuinterval;
	/* imu sample interval
	   1LSB = 1s
	   0 = invalid
	*/
	
	float gyrox;
	float gyroy;
	float gyroz;
	float accx;
	float accy;
	float accz;
	int8_t imutemp;
	float baro;
	int8_t barotemp;
};

__packed struct dm_cfg_s {
	uint16_t null;
};

static const int dm_var_bcast_size = sizeof(struct dm_var_s);
static const int dm_cfg_bcast_size = 0;

