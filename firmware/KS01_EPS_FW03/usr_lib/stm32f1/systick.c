#include "datatypes.h"
#include "stm32f10x.h"
#include "systick.h"
#include "vtimer.h"
#include <stdlib.h>
#include "debug.h"

#include "freertos.h"

//malloc and free functions
//#include "os_api.h"
//#define os_malloc malloc
//#define os_free free

typedef struct {
    void (*jobfunc)(void);
    void *nextjob;
} job_t;

static uint16_t __ticks_per_us = 72;
static __IO uint32_t sys_millis = 0;
static job_t* p_job_chain = 0;

//void systick_isr(void)
void vApplicationTickHook(void)
{
  sys_millis++;
  vtimer_service();
  job_t* p = p_job_chain;
  while(p)
  {
    (p->jobfunc)();//do job
    p = p->nextjob;//seek next
  }
}

void systick_init()
{
  RCC_ClocksTypeDef RCC_Clocks;

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */  

  // SysTick end of count event each 1ms 
  RCC_GetClocksFreq(&RCC_Clocks);
  
  //SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 1000);
  
  __ticks_per_us = RCC_Clocks.SYSCLK_Frequency / 1000 / configTICK_RATE_HZ;
}

void systick_attach(void (*job)(void))
{
  //os_enter_critical();
  job_t* p = p_job_chain;
  void* prev = 0;
  if(p)
  {
      while(p->nextjob)
          p = p->nextjob;
      prev = &(p->nextjob);
  }
  else
  {
      //allocate first 
    prev = &p_job_chain;
  }
  p = (job_t*)pvPortMalloc(sizeof(job_t));
  if( p == 0 )
      sw_error("systick","attach_nomem",(uint32_t)job);
  p->jobfunc = job;
  p->nextjob = 0;
  *(job_t**)prev = p;
  //os_leave_critical();
}

void systick_detach(void (*job)(void)) //not tested
{
  //os_enter_critical();
  job_t* p = p_job_chain;
  job_t* prevjob = p_job_chain;
  while(p)
  {
    if(p->jobfunc == job)
    {
        //remove this job
        if(p->nextjob) //prevjob ---> p ---> p->nextjob
            vPortFree(p);
        prevjob->nextjob = 0;
        return;
    }
    prevjob = p;
    p = p->nextjob;//seek next
  }
  //os_leave_critical();
}

void systick_deinit()
{
    SysTick->CTRL &= ~((1<<SysTick_CTRL_ENABLE) | (1<<SysTick_CTRL_TICKINT));
}

uint32_t millis()
{
  return sys_millis;
}

uint32_t micros()
{
    //os_enter_critical();
  uint32_t load = SysTick->LOAD;
  uint32_t tick,freeze_millis;
  do {
    tick = SysTick->VAL;
    freeze_millis = sys_millis;
  }
  //while(freeze_millis != sys_millis);
  while(SysTick->VAL < tick);
  uint32_t us = (load + 1 - tick ) / __ticks_per_us + freeze_millis * 1000;
  //os_leave_critical();
  return us;
}

uint32_t ticks()
{
  //os_enter_critical();
  uint32_t load = SysTick->LOAD;
  uint32_t tick,freeze_millis;
  do {
    tick = SysTick->VAL;
    freeze_millis = sys_millis;
  }//while(freeze_millis != sys_millis);
  while(SysTick->VAL < tick);
  uint32_t t = (load + 1 - tick ) + freeze_millis * 1000 * __ticks_per_us;
  //os_leave_critical();
  return t;
}

uint16_t ticks_per_us()
{
  return __ticks_per_us;
}
