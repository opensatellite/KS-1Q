#include "os.h"
#include "ctype.h"
#include "thermocouple.h"

//calculate v^i for i = 1 to n
static inline void _pow(float v, float vpow[], int n)
{
	if(n <= 0) return;
	vpow[0] = v;
	for(int i = 1; i < n; i++)
	{
		vpow[i] = vpow[i-1] * v; 
	}
}

static inline float _tc_b(float mv, int8_t *ovf)
{
	float vpow[9];
	float voff0 = 9.8423321E+01;
	float vcoeff0[] = {
		 6.9971500E+02,
		-8.4765304E+02,
		 1.0052644E+03,
		-8.3345952E+02,
		 4.5508542E+02,
		-1.5523037E+02,
		 2.9886750E+01,
		-2.4742860E+00,
	};// 8-order
	float voff1 = 2.1315071E+02;
	float vcoeff1[] = {
		 2.8510504E+02,
		-5.2742887E+01,
		 9.9160804E+00,
		-1.2965303E+00,
		 1.1195870E-01,
		-6.0625199E-03,
		 1.8661696E-04,
		-2.4878585E-06
	};// 8-order
	
	float temp = 0;	
	if(ovf) *ovf = 0;
	if(mv < 2.431)
	{
		if(mv < 0.291)
		{
			if(ovf) *ovf = -1;
		}
		temp += voff0;
		_pow(mv, vpow, NUMBER_OF(vcoeff0));
		for(int i = 0; i < NUMBER_OF(vcoeff0); i++)
			temp += vpow[i] * vcoeff0[i];
	}
	else
	{
		if(mv > 13.820)
		{
			if(ovf) *ovf = +1;
		}
		_pow(mv, vpow, NUMBER_OF(vcoeff1));
		temp += voff1;
		for(int i = 0; i < NUMBER_OF(vcoeff1); i++)
			temp += vpow[i] * vcoeff1[i];
	}
	return temp;
}

static inline float _tc_k(float mv, int8_t *ovf)
{
	float vpow[9];
	//float voff0 = 0.0000000E+00,	 
	float vcoeff0[] = {
		 2.5173462E+01,
		-1.1662878E+00,
		-1.0833638E+00,
		-8.9773540E-01,
		-3.7342377E-01,
		-8.6632643E-02,
		-1.0450598E-02,
		-5.1920577E-04,
	};// 8-order
	//float voff1 = 0.0000000E+00,
	float vcoeff1[] = {
		 //0.0000000E+00,
		 2.508355E+01,
		 7.860106E-02,
		-2.503131E-01,
		 8.315270E-02,
		-1.228034E-02,
		 9.804036E-04,
		-4.413030E-05,
		 1.057734E-06,
		-1.052755E-08
	};// 9-order
	float voff2 = -1.318058E+02;
	float vcoeff2[] = {
		 4.830222E+01,
		-1.646031E+00,
		 5.464731E-02,
		-9.650715E-04,
		 8.802193E-06,
		-3.110810E-08,
		//0.0000000E+00,
		//0.0000000E+00,
		//0.0000000E+00,
	};// 6-order
	
	float temp = 0;	
	if(ovf) *ovf = 0;
	if(mv < 0.0)
	{
		if(mv < -5.891)
		{
			if(ovf) *ovf = -1;
		}
		_pow(mv, vpow, NUMBER_OF(vcoeff0));
		for(int i = 0; i < NUMBER_OF(vcoeff0); i++)
			temp += vpow[i] * vcoeff0[i];
	}
	else if (mv < 20.644)
	{
		_pow(mv, vpow, NUMBER_OF(vcoeff1));
		for(int i = 0; i < NUMBER_OF(vcoeff1); i++)
			temp += vpow[i] * vcoeff1[i];
	}
	else
	{
		if(mv > 54.886)
		{
			if(ovf) *ovf = +1;
		}
		_pow(mv, vpow, NUMBER_OF(vcoeff2));
		temp += voff2;
		for(int i = 0; i < NUMBER_OF(vcoeff2); i++)
			temp += vpow[i] * vcoeff2[i];
	}
	return temp;
}

void thermocouple_algo_test()
{
	int count;
	uint32_t time_min;
	uint32_t time_max;
	float time_avg;
	
	count = 0;
	time_min = 0xffffffff;
	time_max = 0;
	time_avg = 0;
	printf("Thermocouple Type K Voltage (-10mV:0.1mV:60mV)\r\n");
	for(float v = -10e-3;v < 65e-3; v += 1e-4)
	{
		float t;
		uint32_t start = micros();
		t = thermocouple(v, 'k', NULL);
		uint32_t dt = micros() - start;
		time_avg += dt;
		if(dt < time_min) time_min = dt;
		if(dt > time_max) time_max = dt;
		count++;
		printf("%f,%f\r\n", v, t);
	}
	time_avg /= (float)count;
	printf("max: %ldus, min: %ldus, avg: %.3fus\r\n", time_max, time_min, time_avg);
	
	count = 0;
	time_min = 0xffffffff;
	time_max = 0;
	time_avg = 0;	
	printf("Thermocouple Type B Voltage (0:0.1mV:15mV)\r\n");
	for(float v = 0.0;v < 15e-3; v += 1e-4)
	{
		float t;
		uint32_t start = micros();
		t = thermocouple(v, 'b', NULL);
		uint32_t dt = micros() - start;
		time_avg += dt;
		if(dt < time_min) time_min = dt;
		if(dt > time_max) time_max = dt;
		count++;
		printf("%f,%f\r\n", v, t);
	}
	time_avg /= (float)count;
	printf("max: %ldus, min: %ldus, avg: %.3fus\r\n", time_max, time_min, time_avg);
}

float thermocouple(float voltage, char type, int8_t *ovf)
{
	voltage *= 1e3;//convert to mv
	type = tolower(type);
	switch(type)
	{
	case 'x':	//channel is off
		*ovf = 0;
		return FP_NAN;
	case 'b':
		return _tc_b(voltage, ovf);
	case 'k':
		return _tc_k(voltage, ovf);
	default:
		*ovf = 0;
		return FP_NAN;
	}
}
