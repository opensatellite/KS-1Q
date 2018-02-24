#include <csp/csp.h>
#include <stdlib.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_semaphore.h>

#include "csp/drivers/tmp.h"
#include "APB3_reg.h"

//static unsigned long head, tail;
//static uint8_t buf[TMP_FIFO_LEN];

#define KISS_FEND	0xc0

csp_queue_handle_t tmp_queue = NULL;
#define TMP_TIMEOUT     400
#define TMP_FIFO_LEN    8*1024

int tmp_init()
{
  tmp_queue = csp_queue_create(TMP_FIFO_LEN, 1);
  if(!tmp_queue) return -1;
  else return 0;
}

void tmp_putc(char c) {
  csp_queue_enqueue(tmp_queue, &c, TMP_TIMEOUT);
}

void tmp_get_packet(uint8_t *src, int len)
{
  if(src == NULL)
    return;
  
  for(int i = 0; i < len; i++) {
    uint8_t c;
    int ret = csp_queue_dequeue(tmp_queue, &c, 0);
    if(ret != CSP_QUEUE_OK) {
      *src++ = KISS_FEND;
    } else {
      *src++ = c;
    }
  }
}

int tmp_get_avail(int len)
{
  int sz = csp_queue_size(tmp_queue);
  if(sz == 0) return -1;
  else if(sz < len) return 0;
  else return 1;
}

void tmp_wait()
{
  char c;
  while(xQueuePeek( tmp_queue, &c, ( TickType_t ) 100 ) != pdTRUE);
}

/*
int tmp_init()
{
	
	head = 0;
	tail = 0;

	return CSP_ERR_NONE;
}

void tmp_putc(char c)
{
	unsigned long tmptail = (tail + 1) & TMP_FIFO_MASK;
	if(tmptail == head) return;

    buf[tail] = c;
	tail = tmptail;
}

void tmp_get_packet(uint8_t *src, int len)
{
	if(src == NULL)
		return;

	for(int i = 0; i < len; i++) {
		if(head == tail) {
			*src++ = KISS_FEND;
		} else {
                        *src++ = buf[head];
                        head = (head + 1) & TMP_FIFO_MASK;;
                }
	}
}

int tmp_get_avail(int len)
{
	unsigned long avail = (tail - head) & TMP_FIFO_MASK;
	if(avail == 0) return -1;
	else if(avail < 220) return 0;
	else return 1;
}
*/