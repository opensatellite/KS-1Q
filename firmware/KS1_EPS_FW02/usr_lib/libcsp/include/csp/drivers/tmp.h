#ifndef TMP_H
#define TMP_H

#include <stdint.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_semaphore.h>

typedef struct
{
	uint32_t tmp_fifo_addr;
	uint32_t tmp_fifo_index;
	uint32_t tmp_fifo_len;
	csp_bin_sem_handle_t csp_tmp_tc;
	csp_queue_handle_t csp_tmp_tx_queue;
}tmp_conf_typedef;

extern tmp_conf_typedef tmp_conf;

int tmp_init(uint32_t *fifo,uint32_t len);

void tmp_putc(char c);

__weak void csp_tmp_usr(tmp_conf_typedef *tmp_conf);

#endif