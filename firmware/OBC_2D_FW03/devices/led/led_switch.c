#include "os.h"
#include "config.h"
#include "led_switch.h"

static uint8_t switch_count = 0;
static uint16_t green_blink_timer = 0;
static uint16_t red_blink_timer = 0;
static bool _red = false;
static bool _green = false;

#define green(X)	  {if(X) pinlow(LED_GREEN); else pinhigh(LED_GREEN);}
#define red(X)        {if(X) pinlow(LED_RED); else pinhigh(LED_RED);}

void led_switch_hook()
{
	switch_count++;
	if(switch_count > 50)
	{
		switch_count = 100;
	}	
	
	if(green_blink_timer)
	{
		green_blink_timer--;
		green(!_green);
	}
	else green(_green);
	
	if(red_blink_timer)
	{
		red_blink_timer--;
		red(!_red);
	}
	else red(_red);
}

void led_switch_init()
{
	pinmode(LED_GREEN,OPENDRAIN);
	pinhigh(LED_GREEN);
	pinmode(LED_RED,OPENDRAIN);
	pinhigh(LED_RED);
	systick_attach(led_switch_hook);
}

void led_green(bool en)
{
	_green = en;
}

void led_red(bool en)
{
	_red = en;
}

void led_red_blink(int ms)
{
	red_blink_timer = ms;
}
