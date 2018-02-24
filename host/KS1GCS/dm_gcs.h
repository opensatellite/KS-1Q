#pragma once

#define REGION_FIXED	0
#define REGION_VAR		1
#define REGION_CFG		2

#define DM_REQUEST	0x0A
#define DM_R_BASE	0x0B
#define DM_R_FIXED	(DM_R_BASE + REGION_FIXED)	//0B
#define DM_R_VAR	(DM_R_BASE + REGION_VAR)	//0C
#define DM_R_CFG	(DM_R_BASE + REGION_CFG)	//0D
#define DM_W_BASE	0x0D
#define DM_W_VAR	(DM_W_BASE + REGION_VAR)	//0E
#define DM_W_CFG	(DM_W_BASE + REGION_CFG)	//0F

#define OBC_ADDR 0x00
#define TTC_ADDR 0x01
#define EPS_ADDR 0x02
#define TNC_ADDR 0x09

const int dm_service_port = 7;
const int dm_bcast_port = 8;

#define __packed __attribute__((packed))

struct __packed dm_op_s {
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

	Retval: -1 = error, 0 -> OK, 1 = port not match
*/

