#include <stdint.h>
#include "csp/csp.h"
//#include "datatypes.h"
#include "dm.h"
#include "bsp.h"

const struct dm_fixed_s dm_fixed;

struct dm_var_s dm_var;

__no_init struct dm_cfg_s dm_cfg@0x0801FC00;

const struct dm_cfg_s dm_cfg_default;

extern const struct dm_cfg_s dm_cfg_default;
#define TIMEOUT	(1000)

int dm_var_write_callback(uint16_t offset, int size) {
	
	return 1;
}

int dm_cfg_write_callback(uint16_t offset, int size) {
	
	
	return 1;
}

void dm_cfg_save() {


	dm_cfg_notsave = false;
}

void dm_cfg_load() {


	dm_cfg_notsave = false;
}

void dm_cfg_restore(uint16_t offset, int size) {
	memcpy(&dm_cfg + offset, &dm_cfg_default + offset, size);
	dm_cfg_save();
}

void dm_send_bcast(uint8_t dest) {
	csp_packet_t *r = csp_buffer_get(dm_bcast_size);
	dm_fill_bcast(r->data);
	r->length = dm_bcast_size;
	csp_sendto(CSP_PRIO_NORM, dest, dm_bcast_port, dm_bcast_port, CSP_O_NONE, r, TIMEOUT); 
}

int dm_reply(csp_packet_t *p)
{
	if(!p) return -1;
	if(p->id.dport == dm_bcast_port) {
		//request telemetry
		if(p->length == 0)
		{
			uint8_t src = p->id.src;
			dm_send_bcast(src);
			csp_buffer_free(p);
			return 0;
		}
		return 1;
	}
	if(p->id.dport != dm_service_port) {
		return 1;
	}
	if(p->length < 1) {
		csp_buffer_free(p);
		return -1;
	}
	struct dm_op_s *d = (struct dm_op_s *)p->data;
	switch(d->op) {
		case DM_REQUEST:
		{
			uint8_t x = p->data[1];
			csp_buffer_free(p);
			if(p->length < 2) return -1;
			if(x) dm_send_bcast(CSP_BROADCAST_ADDR);
			else dm_send_bcast(TNC_ADDR);
			return 0;
		}
		case DM_R_FIXED:
		case DM_R_VAR:
		case DM_R_CFG:
		{
			if(p->length < 4) {
				csp_buffer_free(p);
				return -1;
			}
			csp_packet_t *r = csp_buffer_get(d->size);
			if(!r) {
				csp_buffer_free(p);
				return -1;
			}
			r->data[0] = d->op;
			r->length = d->size + 1;
			int ret = dm_read(d->op - DM_R_BASE, d->offset, r->data + 1, r->length);
			if(ret >= 0) {
				csp_sendto_reply(p, r, CSP_O_NONE, TIMEOUT);
			}
			csp_buffer_free(p);
			return 0;
		}
		case DM_W_VAR:
		case DM_W_CFG:
		{
			if(p->length < 4) {
				csp_buffer_free(p);
				return -1;
			}
			csp_packet_t *r = csp_buffer_get(2);
			if(!r) {
				csp_buffer_free(p);
				return -1;
			}
			r->data[0] = d->op;
			r->length = 1;
			int ret = dm_write(d->op - DM_W_BASE, d->offset, d->data, d->size);
			if(ret < 0) {
				r->data[1] = 0xff;
				r->length = 2;
			}
			csp_sendto_reply(p, r, CSP_O_NONE, TIMEOUT);
			csp_buffer_free(p);
			return 0;
		}
		default:
			csp_buffer_free(p);
			return 0;
	}
}
