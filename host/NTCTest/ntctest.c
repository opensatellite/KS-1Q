#include <stdio.h>
#include <stdint.h>
#include "ntc.h"

int main(int argc, char *argv[]) {
	printf("ADC,SMD,THRU\n");
	for(uint16_t x = 0; x < 4096; x++) {
		printf("%d,%d,%d,\n", x, ntc_cal_smd(x), ntc_cal_thru(x));
	}
	
	return 0;
}
