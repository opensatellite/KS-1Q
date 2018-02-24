#pragma once

#define THERMOCOUPLE_NO_CHANNEL -100

extern char thermocouple_type[16];
extern int8_t thermocouple_ovf[16];
extern float thermocouple_degree[16];

void thermocouple_reader_init(float tau);
int thermocouple_set(uint8_t n, char type);
void thermocouple_process(uint8_t ch, float v, bool adc_ovf);
void show_thermocouple();
