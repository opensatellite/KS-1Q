#include <csp/csp.h>
#include <stdlib.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_semaphore.h>

#include "csp/drivers/tmp.h"

tmp_conf_typedef tmp_conf;

int tmp_init(uint32_t *fifo,uint32_t len)
{
	tmp_conf.tmp_fifo_addr = (uint32_t)fifo;
	tmp_conf.tmp_fifo_index = 0;
	tmp_conf.tmp_fifo_len = len;

	tmp_conf.csp_tmp_tx_queue = csp_queue_create(1,1);
	if (!tmp_conf.csp_tmp_tx_queue)
	{
		csp_log_error("Fail to init TMP TX queue");
		return CSP_ERR_NOMEM;
	}

	if (csp_mutex_create(&tmp_conf.csp_tmp_tc)!= CSP_MUTEX_OK)
	{
		csp_log_error("Fail to Creat CSP TEMP Tx Complete Lock");
		return CSP_ERR_NOMEM;
	}

	return CSP_ERR_NONE;
}

void tmp_putc(char c)
{
	if (csp_mutex_lock(&tmp_conf.csp_tmp_tc, 100) == CSP_SEMAPHORE_OK)
	{
		if (tmp_conf.tmp_fifo_addr != NULL)
		{
			*((uint8_t *)(tmp_conf.tmp_fifo_addr + tmp_conf.tmp_fifo_index)) = c;
			if (tmp_conf.tmp_fifo_index++ == tmp_conf.tmp_fifo_len)
			{
				csp_queue_enqueue(tmp_conf.csp_tmp_tx_queue,NULL, 100);
				tmp_conf.tmp_fifo_index = 0;
			}
		}
		csp_mutex_unlock(&tmp_conf.csp_tmp_tc);
	}
}
