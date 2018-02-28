//virtual timer
//vtimer.c

//#include "platform.h"
#include "systick.h"
#include "delay.h"
#include "vtimer.h"

//内部静态变量 
static volatile uint32_t vtimer_ovft[VTIMERS_NUM]    = {0};
static volatile uint8_t  vtimer_state[VTIMERS_NUM]   = {0};
#define RUN   0x01
#define USING 0x02
#define OVF   0x04

//功能函数 
bool vtimer_init(void)
{
  uint8_t i;
  for(i=0;i<VTIMERS_NUM;i++)
  {
    vtimer_state[i] |= OVF;
    vtimer_state[i] &= ~RUN;
    vtimer_state[i] &= ~USING;
  }
  return true;
}

bool vtimer_deinit(void) 
{
  return true;
}

void vtimer_service(void)
{
  uint8_t i;
  for(i=0;i<VTIMERS_NUM;i++)
  {
    if((vtimer_state[i]&RUN)&&(vtimer_ovft[i]==millis())) 
    {
      vtimer_state[i] |= OVF;
      vtimer_state[i] &= ~RUN;
    }
  }
}

void vtimer_set(uint8_t n,uint32_t t)//开定时器,n为定时器号,t为时间(ms) 
{
  if(n==VTIMER_NULL) return;
  if(t==0) 
  {
    vtimer_state[n] |= OVF;
    return;
  }
  vtimer_ovft[n]=millis()+t;
  vtimer_state[n] &= ~OVF;
  vtimer_state[n] |= RUN;
}

bool vtimer_ovf(uint8_t n)
{
  return (bool)(vtimer_state[n] & OVF);
}

uint8_t vtimer_alloc(void)
{
  uint8_t i;
  
  for(i=0;i<VTIMERS_NUM;i++)
  {
    if(!(vtimer_state[i] & USING))
    {
      vtimer_state[i] |= USING;
      return i;
    }
  }
  return VTIMER_NULL;
}

void vtimer_free(uint8_t n)
{
  vtimer_state[n] &= ~USING;
  vtimer_state[n] |= OVF;
  vtimer_state[n] &= ~RUN;
  vtimer_state[n] &= ~USING;
}

uint32_t vtimer_timeleft(uint8_t n)
{
  if((!(vtimer_state[n] & USING)) || (vtimer_state[n] & OVF))
    return 0;
  else
    return (vtimer_ovft[n]-millis());
}
