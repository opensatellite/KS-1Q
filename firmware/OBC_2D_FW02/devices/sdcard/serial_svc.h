#pragma once

int serial(int port, uint32_t baud);
void serial_rate(int rate);//adcdata rate limit
void serial_off();
void serial_info();

void serial_send_adcdata(uint32_t timestamp, int adc, int n, uint16_t ovf, float data[]);
void serial_send_thermocouple(uint32_t timestamp, uint16_t mask, int n, char type[], int8_t ovf[], float temp[]);
void serial_send_mark(int id, uint64_t s, uint32_t us, char str[]);
