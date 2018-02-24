#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <csp/csp.h>
#include <csp/csp_ccsds.h>
#include <csp/csp_crc32.h>
#include <csp/csp_endian.h>
#include <csp/interfaces/csp_if_kiss.h>

#define FEND  					0xC0
#define FESC  					0xDB
#define TFEND 					0xDC
#define TFESC 					0xDD

#define TNC_DATA				0x00
#define TNC_SET_HARDWARE		0x06
#define TNC_RETURN				0xFF

int8_t ccsds_kiss_encode(csp_packet_t* packet,csp_kiss_packet_t* kiss_packet)
{
	uint8_t j = KISS_MTU - kiss_packet->valid_map;

    if(kiss_packet->valid_map < (packet->length + sizeof(packet->id.ext) +1 ))
    {
        while(j<KISS_MTU)
        {
        	kiss_packet->data[j++] = FEND;
        }
        kiss_packet->valid_map = 0;

        return CSP_ERR_NOBUFS;
    }

    csp_crc32_append(packet, false);

    packet->id.ext = csp_hton32(packet->id.ext);
	packet->length += sizeof(packet->id.ext);

    kiss_packet->data[j++] = FEND;
    kiss_packet->data[j++] = TNC_DATA;

    for (unsigned int i = 0; i < packet->length; i++) {
		if (((unsigned char *) &packet->id.ext)[i] == FEND) {
			((unsigned char *) &packet->id.ext)[i] = TFEND;
			kiss_packet->data[j++] = FESC;
		} else if (((unsigned char *) &packet->id.ext)[i] == FESC) {
			((unsigned char *) &packet->id.ext)[i] = TFESC;
			kiss_packet->data[j++] = FESC;
		}
		kiss_packet->data[j++] = ((unsigned char *) &packet->id.ext)[i];
	}
    kiss_packet->data[j++] = FEND;

    kiss_packet->valid_map -= packet->length;

    csp_buffer_free(packet);

    return CSP_ERR_NONE;
}

csp_packetaddr_t ccsds_kiss_decode(uint8_t *buf,void * pxTaskWoken)
{
    uint8_t i = KISS_MTU;
    //uint8_t err = 0;
    uint8_t *packet;
    uint8_t packet_len = 0;
    uint8_t temp1 = 0x00,temp2 = 0x00;

    csp_packetaddr_t csp_packetaddr ={
        .packet_num = 0,
        .err = 0
    };

    kiss_mode_e mode = KISS_MODE_STARTED;

   for(i=0;i<KISS_MTU;i++)
   {
       temp1 = temp2;
       temp2 = *buf++;

       switch (mode)
       {
           case KISS_MODE_STARTED:
                if((temp1 == FEND )&&(temp2 == TNC_DATA))
                {
                    if(packet == NULL)
                    {
                        packet = csp_buffer_get(256);
                    }

                    if(packet != NULL)
                    {
                        csp_packetaddr.packet_addr[csp_packetaddr.packet_num] = (uint32_t)&packet;
                        mode = KISS_MODE_NOT_STARTED;
                        packet_len = 0;
                    }
                    else
                    {
                        mode = KISS_MODE_SKIP_FRAME;
                    }
                }
                break;
            case KISS_MODE_NOT_STARTED:
                if(temp2 == FESC)
                {
                    mode = KISS_MODE_ESCAPED;
                    break;
                }

                if(temp2 == FEND)
                {
                    if(packet_len < CSP_HEADER_LENGTH + 4)
                    {
                        csp_packetaddr.err ++;
                        break;
                    }
                    ((csp_packet_t *)packet) -> length = packet_len - CSP_HEADER_LENGTH;
                    ((csp_packet_t *)packet)->id.ext = csp_ntoh32(((csp_packet_t *)packet)->id.ext);

                    if (csp_crc32_verify((csp_packet_t *)packet, false) != CSP_ERR_NONE) {
						csp_log_warn("KISS invalid crc frame skipped, len: %u", ((csp_packet_t *)packet) -> length);
						csp_packetaddr.err ++;
						mode = KISS_MODE_STARTED;
						break;
					}

                    packet = NULL;
                    csp_packetaddr.packet_num++;
                    mode = KISS_MODE_STARTED;
					break;
                }

                ((char*)(((csp_packet_t *)packet)->id.ext))[packet_len++] = temp2;
                break;
            case KISS_MODE_ESCAPED:
                if(temp2 == TFESC)
                {
                	((char*)(((csp_packet_t *)packet)->id.ext))[packet_len++] = FESC;
                    break;
                }   

                if(temp2 == TFEND)
                {
                	((char*)(((csp_packet_t *)packet)->id.ext))[packet_len++] = FEND;
                    break;
                }

                ((char*)(((csp_packet_t *)packet)->id.ext))[packet_len++] = FESC;
                break;

            case KISS_MODE_SKIP_FRAME:
                if (temp2 == FEND)
                {
                    mode = KISS_MODE_NOT_STARTED;
                }
                break;
        }
   }
   
   return csp_packetaddr;
}


