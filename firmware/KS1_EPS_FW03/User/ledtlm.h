#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "cmsis_os.h"

struct ledtlm_s {
	int ptr;
	int num;
	uint8_t data[256];
};

extern struct ledtlm_s ledtlm_d;

void ledtlm_start();

#endif