#include "systick.h"
#include "delay.h"

void delay_cycles(uint32_t c)
{
    while(c--);
}

void delay_ticks(uint32_t x)
{
    uint32_t s = ticks();
    volatile uint32_t d;
    while(1)
    {
        d = ticks() - s;
        if(d >= x) return;
    }
}

void delay_nms(uint32_t x)
{
  uint32_t s = millis();
  volatile uint32_t d;
  while(1)
  {
    d = millis() - s;
    if(d >= x) return;
  }
}

void delay_nus(uint32_t x)
{
  uint32_t s = micros();
  volatile uint32_t d;
  while(1)
  {
    d = micros() - s;
    if(d >= x) return;
  }
}
