
__packed struct dm_fixed_s {
  uint32_t null;
};

__packed struct dm_var_s {
	uint16_t txpkts;
	uint16_t rxpkts;
	uint16_t rxerrs;
	uint16_t canerrs;
	int8_t   mcutemp;
	int8_t   patemp;
        uint16_t ittc;
	uint16_t ipa;
//	uint8_t  status;
	uint8_t  resets;
//	uint16_t  wdt;
	uint8_t  rssi;
	uint16_t  binterval;
	/* Beacon interval
	   1LSB = 1ms
	*/
	uint8_t btimeout;
	/* Beacon interval
	   1LSB = 1s
	*/
//	uint8_t  txrate;
//  uint8_t  rxrate;
//  uint8_t  rtimeout;
};

__packed struct dm_cfg_s {
  uint16_t null;
};

static const int dm_var_bcast_size = sizeof(struct dm_var_s);
static const int dm_cfg_bcast_size = 0;

