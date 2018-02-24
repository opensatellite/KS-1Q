
struct __packed dm_ttc_fixed_s {
//  uint32_t txfreq;
//  uint32_t rxfreq;
};

struct __packed dm_ttc_var_s {
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

struct __packed dm_ttc_cfg_s {
//	uint8_t txch;
//  uint8_t rxch;
};

static const int dm_ttc_var_bcast_size = sizeof(struct dm_ttc_var_s);
static const int dm_ttc_cfg_bcast_size = 0;

