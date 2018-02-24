#include "pkg_cb.h"
#include "stdio.h"

int csp_pkg_callback(int dport, csp_packet_t *packet)
{
	switch (dport)
	{
		case 0x10:
			printf("Packet received on MY_PORT: %s\r\n", (char *) packet->data);
			csp_buffer_free(packet);
			break;
		default:
			return 0;
	}
	return 1;
}
