#pragma once

/*!
 * \file led_switch.h
 * Status LED
 * - LED_RUN Green
 * - LED_STOP Red
 * Switch
 * - RUN/STOP
 * 
 * LED Modes:
 * + SOLID LIGHT
 * - RUN 		          Green
 * - STOP 		          None
 * - ERROR                Red	(critical error)
 * + BLINK
 * - NO SDCARD   		  Red short blink (10ms)
 * - WRITE FAIL  		  Red short blink (10ms)
 * - MODBUS ERROR         Red long blink (200ms)
 */

/*!
 * \brief Status LED and RUN/STOP switch
 */

void led_switch_init();
bool runstop_state();

void led_green(bool en);
void led_red(bool en);
void led_green_blink(int ms);
void led_red_blink(int ms);

static inline void led_run()
{
	led_green(true);
	led_red(false);
}

static inline void led_stop()
{
	led_green(false);
	led_red(false);
}

static inline void led_error()		   
{
	led_green(false);
	led_red(true);
}

#define led_no_sdcard()		led_red_blink(10)
#define led_write_fail()  	led_red_blink(10)
#define led_modbus_error() 	led_red_blink(200)

