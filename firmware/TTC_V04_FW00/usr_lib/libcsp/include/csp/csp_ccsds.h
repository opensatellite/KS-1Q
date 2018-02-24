#ifndef CCSDS_H
#define CCSDS_H

#include <stdint.h>

#define KISS_MTU  220

typedef struct{
    uint8_t valid_map;
    uint8_t data[KISS_MTU];
}csp_kiss_packet_t;

typedef struct{
    uint8_t packet_num;
    uint8_t err;
    uint32_t packet_addr[0];
}csp_packetaddr_t;

int8_t ccsds_kiss_encode(csp_packet_t* packet,csp_kiss_packet_t* kiss_packet);
csp_packetaddr_t ccsds_kiss_decode(uint8_t *buf,void * pxTaskWoken);

#endif // !CCSDS_H
