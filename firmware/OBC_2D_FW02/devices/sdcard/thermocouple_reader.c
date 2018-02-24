#include "os.h"
#include "thermocouple_reader.h"
#include "lib/thermocouple.h"

#include "adc_reader.h"

//ch 0-7 = adc1 diff0-7, ch 8-16 = adc2 diff0-7
char thermocouple_type[16];
int8_t thermocouple_ovf[16];
uint16_t thermocouple_disconnected = 0xffff;
float thermocouple_voltage[16] = {0};
float thermocouple_degree[16] = {0};
float thermocouple_filter_k = 1.0;

void thermocouple_reader_init(float tau)
{
	memset(thermocouple_type, 'x', sizeof(thermocouple_type));
	memset(thermocouple_ovf, 0, sizeof(thermocouple_ovf));
	memset(thermocouple_voltage, 0, sizeof(thermocouple_voltage));
	thermocouple_filter_k = 1.0 / tau;
}

int thermocouple_set(uint8_t n, char type)
{
	if(adc1_input_mode && n < 8)
		return -1;
	if(adc2_input_mode && n >= 8 && n < 16)
		return -2;
	if(n >= 16)
		return THERMOCOUPLE_NO_CHANNEL;
	thermocouple_type[n] = type;
	return 0;
}

void thermocouple_process(uint8_t ch, float v, bool adc_ovf)
{
	thermocouple_voltage[ch] = v * thermocouple_filter_k + thermocouple_voltage[ch] * (1.0 - thermocouple_filter_k);
	if(thermocouple_type[ch] == 'x' || adc_ovf)
	{
		thermocouple_degree[ch] = 0.0;
		setb(thermocouple_disconnected, ch);
		thermocouple_ovf[ch] = 0;
	}
	else
	{
		thermocouple_degree[ch] = thermocouple(thermocouple_voltage[ch], thermocouple_type[ch], &thermocouple_ovf[ch]);
		clr(thermocouple_disconnected, ch);
	}
}

void show_thermocouple()
{
	if(!adc1_input_mode)
	{
		printf("[thermocouple]on adc 1\r\n");
		int n = 0;
		for(int i = 0; i < 8; i++)
		{
			if(thermocouple_type[i] != 'x')
			{
				printf("|%2d| %c: ", i, toupper(thermocouple_type[i]));
				if(get(thermocouple_disconnected, i))
					printf("  OFFLINE ");
				else if(thermocouple_ovf[i])
					printf("%cINF      ",thermocouple_ovf[i] < 0 ? '-' : '+'); 
				else
					printf("%7.2f`C ", thermocouple_degree[i]);
				n++;
				if(n % 2 == 0) printf("\r\n");
			}
		}
		if(n == 0) printf("[thermocouple]no sensor on adc1\r\n");
		else if(n % 2 != 0) printf("\r\n");
		printf("\r\n");
	}
	
	if(!adc2_input_mode)
	{
		printf("[thermocouple]on adc 2\r\n");
		int n = 0;
		for(int i = 8; i < 16; i++)
		{
			if(thermocouple_type[i] != 'x')
			{
				printf("|%2d| %c: ", i-8, toupper(thermocouple_type[i]));
				if(get(thermocouple_disconnected, i))
					printf("  OFFLINE ");
				else if(thermocouple_ovf[i])
					printf("%cINF      ",thermocouple_ovf[i] < 0 ? '-' : '+'); 
				else
					printf("%7.2f`C ", thermocouple_degree[i]);
				n++;
				if(n % 2 == 0) printf("\r\n");
			}
		}
		if(n == 0)  printf("[thermocouple]no sensor on adc2\r\n");
		else if(n % 2 != 0) printf("\r\n");
		printf("\r\n");
	}
}
