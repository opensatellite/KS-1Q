
struct __packed dm_eps_fixed_s {

};

struct __packed dm_eps_var_s {
	uint16_t solar1i;
	uint16_t solar2i;
	uint16_t accel;
	uint8_t  heater;
	int8_t   deploy;//pwrobc
	uint8_t  pwrcam;
	uint16_t batvh;
	uint16_t batvl;
	int16_t  ibat;
	uint16_t ipsb;
	uint16_t iheater;
	uint16_t ibus;
	uint16_t iled;
	uint16_t icam;
	int8_t   battemp;
	int8_t   camtemp;
	int8_t   temp1;
	int8_t   temp2;
	int8_t   temp3;
	int8_t   temp4;
	uint16_t radraw;
	uint32_t datetime;
	uint16_t suna;
	uint16_t sunb;
	uint16_t sunc;
	uint16_t sund;
	int16_t  ebatvl;
	uint8_t  resets;
	uint16_t wdt;
};

struct __packed dm_eps_cfg_s {
	uint16_t eps_run_wait;
	uint16_t heat_valid;
	uint16_t obc_run_flag;
};

static const int dm_eps_var_bcast_size = sizeof(struct dm_eps_var_s);
static const int dm_eps_cfg_bcast_size = sizeof(struct dm_eps_cfg_s);

static const int eps_led_port = 9;
