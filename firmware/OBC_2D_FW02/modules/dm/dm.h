#pragma once
#include "dm_data_obc.h"
#include "csp/csp.h"

extern const struct dm_fixed_s dm_fixed;
extern struct dm_var_s dm_var;
extern struct dm_cfg_s dm_cfg;
extern bool dm_cfg_notsave;

#define REGION_FIXED	0
#define REGION_VAR		1
#define REGION_CFG		2

/*
	Retval: 0 - n -> bytes read or written
	       <0 -> action failed
*/
int dm_read(uint8_t region, uint16_t offset, uint8_t *data, uint16_t size);
int dm_write(uint8_t region, uint16_t offset, uint8_t *data, uint16_t size);

#define DM_REQUEST	0x0A
#define DM_R_BASE	0x0B
#define DM_R_FIXED	(DM_R_BASE + REGION_FIXED)	//0B
#define DM_R_VAR	(DM_R_BASE + REGION_VAR)	//0C
#define DM_R_CFG	(DM_R_BASE + REGION_CFG)	//0D
#define DM_W_BASE	0x0D
#define DM_W_VAR	(DM_W_BASE + REGION_VAR)	//0E
#define DM_W_CFG	(DM_W_BASE + REGION_CFG)	//0F

static const int dm_service_port = 7;
static const int dm_bcast_port = 8;

__packed struct dm_op_s {
	uint8_t op;
	uint16_t offset;
	uint8_t size;
	uint8_t data[0];
};

/*
	Port 7 Service protocol

	RX, 0x0A 0x00, from any source, port 7: Request beacon
	Reply, bcast, to TNC_ADDR, port 8: Broadcast packet

	RX, 0x0A 0x01, from any source, port 7: Request broadcast
	Reply: bcast, to BROADCAST_ADDR, port 8: Broadcast packet

	RX, empty, from any source, port 8: Request telemetry
	Reply: bcast, to source addr, port 8: Broadcast packet

	offset = uint16, size = uint8

	RX, 0x0B offset size, from any source, port 7: Read DM region FIXED
	Reply, 0x0B, to source addr, port 7: DM readout

	RX, 0x0C offset size, from any source, port 7: Read DM region VAR
	Reply, 0x0C, to source addr, port 7: DM readout

	RX, 0x0D offset size, from any source, port 7: Read DM region CFG
	Reply, 0x0D, to source addr, port 7: DM readout

	RX, 0x0E offset size data..., from any source, port 7: Write DM region VAR
	Reply, 0x0E, to source addr, port 7: Success
	Reply, 0x0E 0xff, to source addr, port 7: Failed

	RX, 0x0F offset size data..., from any source, port 7: Write DM region CFG
	Reply, 0x0F, to source addr, port 7: Success
	Reply, 0x0F 0xff, to source addr, port 7: Failed

	Retval: -1 = error, 0 -> OK, 1 = packet not match
*/
int dm_reply(csp_packet_t *);

/*
	Prepare broadcast data

	Retval: >=0 -> bytes to send
	         <0 -> outlimit too small
*/
#define dm_bcast_size (dm_var_bcast_size + dm_cfg_bcast_size)
void dm_fill_bcast(uint8_t* out);
void dm_send_bcast(uint8_t dest);

/*
	Called after dm_write
	Implemented by user
	Retval: 0 - n -> bytes written
	       <0 -> callback action failed
*/
int dm_var_write_callback(uint16_t offset, int size);
int dm_cfg_write_callback(uint16_t offset, int size);

void dm_cfg_save();
void dm_cfg_load();

/*
	Restore settings to default
*/
void dm_cfg_restore(uint16_t offset, int size);
