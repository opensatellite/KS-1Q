#pragma once

#define PACKET_SYSINFO	0

/*
differential
|timestamp(us)|Data0|...|Data7 |   OVF  |
|uint32_t     |float|...|float | uint8_t|
Size=37
single-ended
|timestamp(us)|Data0|...|Data15|   OVF  |
|uint32_t     |float|...|float |uint16_t|
Size=70
*/
#define PACKET_ADC_BASE 0
#define PACKET_ADC1		1
#define PACKET_ADC2		2

/*
transfer all updated channels
|timestamp(us)|Update  |Type|  Ovf | Temp1|Type|  Ovf | Temp2|...|Type|  Ovf |Temp16|
|uint32_t     |uint16_t|char|int8_t| float|char|int8_t| float|...|char|int8_t| float|
Size=6+6*n, n<=16, Size<=102
*/
#define PACKET_TC		64

/*
mark id, time and appendix
|   id  | seconds|microseconds|appendix...|
|int16_t|uint64_t|  uint32_t  |    char   |
Size=14+strlen(appendix) < 128
string end '\0' NOT included!
*/
#define PACKET_MARK		128

