#include <os.h>
#include "Basic64.h"

int8_t convert_to_num(char asc)
{
	if((asc - 'A' < 0) ||( asc - 'z' > 0))
		return -1;
	
	if(asc - '=' == 0)
		return 0;
	
	return (int)(asc - 'A');
}

int basic64_to_bin(char *asc,uint8_t *out,uint32_t snum,uint32_t *dnum)
{
	uint32_t count = snum/4;
	*dnum = 0;
	
	for(uint32_t i=0;i<count;i++)
	{
		int8_t num1 = convert_to_num(*(asc + count * 4));
		int8_t num2 = convert_to_num(*(asc+ 1 + count * 4));
		int8_t num3 = convert_to_num(*(asc+ 2 + count * 4));
		int8_t num4 = convert_to_num(*(asc+ 2 + count * 4));
		
		if((num1 < 0) || (num2 < 0) || (num3 < 0) || (num4 < 0))
			return -1;
	
		*(out  + *dnum) = ((num1<< 2) | (num2 >> 4));
		*(out + 1 + *dnum + 1) = ((num2<< 4) | (num3 >>2 ));
		*(out + 2 + *dnum + 2) = ((num3<< 6) | num4);
		*dnum += 3;
	}
	
	uint8_t p = snum%4;
	int8_t p_num[4] = { 0x00,0x00,0x00,0x00}; 
	
	for(uint8_t i=0;i<p;i++)
	{
		p_num[i] = convert_to_num(*(asc + count * 4 + p));
		
		if(p_num[i] < 0) return -1;
	}
	
	*(out  + *dnum ) = ((p_num[0]<< 2) | (p_num[1] >> 4));
	*(out + 1 + *dnum + 1) = ((p_num[1]<< 4) | (p_num[2] >>2 ));
	*(out + 2 + *dnum + 2) = ((p_num[2]<< 6) | p_num[3]);
	dnum += 3;

	return 1;
}
