#include "os.h"
#include "mark.h"
#include "sdcard.h"
#include "serial_svc.h"

int time_mark_id = 0;
uint64_t time_mark_seconds = 0;
uint32_t time_mark_micros = 0;
bool time_mark_pending = false;

void time_mark(int id)
{
	sys_noirq();
	time_mark_micros = micros();
	time_mark_seconds = seconds();
	sys_irq();
	time_mark_pending = true;
	
	serial_send_mark(id, time_mark_seconds, time_mark_micros, NULL);
	printf("[mark]id=%d,time=%llu.%06lus\r\n", id, time_mark_seconds, time_mark_micros % 1000000);
}
