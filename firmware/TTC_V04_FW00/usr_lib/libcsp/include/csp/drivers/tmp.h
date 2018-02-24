#ifndef TMP_H
#define TMP_H

#include <stdint.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_semaphore.h>
#include "APB3_reg.h"

//#define TMP_FIFO_MASK   0x3fff
extern csp_queue_handle_t tmp_queue; 

int tmp_init();

void tmp_putc(char c);

void tmp_get_packet(uint8_t *src, int len);

int tmp_get_avail();
//return -1 = no data, 0 = partial packet, 1 = many packet

void tmp_wait();

#endif
