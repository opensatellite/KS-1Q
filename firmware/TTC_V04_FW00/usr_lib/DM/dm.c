#include <stdint.h>
//#include "datatypes.h"
#include "dm.h"

bool dm_cfg_changed = false;

int dm_read(uint8_t region, uint16_t offset, uint8_t *data, uint16_t size)
{
	switch(region) {
		case REGION_FIXED:
		if(offset + size >= sizeof(struct dm_fixed_s))
			size = sizeof(struct dm_fixed_s) - offset;
		memcpy(data, ((uint8_t*)&dm_fixed)+offset, size);
		return size;

		case REGION_VAR:
		if(offset + size >= sizeof(struct dm_var_s))
			size = sizeof(struct dm_var_s) - offset;
		memcpy(data, ((uint8_t*)&dm_var)+offset, size);
		return size;

		case REGION_CFG:
		if(offset + size >= sizeof(struct dm_cfg_s))
			size = sizeof(struct dm_cfg_s) - offset;
		memcpy(data, ((uint8_t*)&dm_cfg)+offset, size);
		return size;

		default: return -1;
	}
}

int dm_write(uint8_t region, uint16_t offset, uint8_t *data, uint16_t size)
{
	switch(region) {
		case REGION_VAR:
		if(offset + size >= sizeof(struct dm_var_s))
			size = sizeof(struct dm_var_s) - offset;
		memcpy(((uint8_t*)&dm_var)+offset, data, size);
		if(dm_var_write_callback(offset, size) < 0) {
			return -1;
		}
		return size;

		case REGION_CFG:
		if(offset + size >= sizeof(struct dm_cfg_s))
			size = sizeof(struct dm_cfg_s) - offset;
		memcpy(((uint8_t*)&dm_cfg)+offset, data, size);
		dm_cfg_changed = true;
		if(dm_cfg_write_callback(offset, size) < 0) {
			return -1;
		}
		return size;

		default: return -2;
	}
}

void dm_fill_bcast(uint8_t* out) {
	memcpy(out, &dm_var, dm_var_bcast_size);
	memcpy(out + dm_var_bcast_size, &dm_cfg, dm_cfg_bcast_size);
}
