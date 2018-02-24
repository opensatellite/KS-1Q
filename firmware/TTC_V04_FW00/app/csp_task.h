#ifndef CSP_TASK_H
#define CSP_TASK_H

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>

extern csp_thread_handle_t handle_client;

void task_server(void *pvParameters);
void task_test(void *pvParameters);


#endif
