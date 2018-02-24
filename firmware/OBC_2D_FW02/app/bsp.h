#ifndef BSP_H
#define BSP_H

#include <os.h>

#define OBC_ADDR	0x00
#define TTC_ADDR	0x01
#define EPS_ADDR	0x02
#define TNC_ADDR	0x09
#define User_ADDR   0x0A

void bsp_init(void);
void modules_init();
int sdcard_init();

CSP_DEFINE_TASK(task_client);

#endif