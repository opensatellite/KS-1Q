#include "bsp.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "uart.h"
#include "stm32_adc.h"
#include "freertos.h"
#include "task.h"
#include "adxl345.h"
#include "systick.h"
#include "debug.h"
#include "mfifo.h"



#include "pin.h"
#include "delay.h"

#include "ntc.h"

#include "cmsis_os.h"

uint16_t radraw;
	static uint16_t cycle_reboot = 0;

static void loop_init()
{
	for(int i = 0; i < 16; i++)
        {
            adc_trig();
        }
        
        dm_var.solar1i = (uint16_t)P1_CS_CAL(adc_read(P1_CS));
        dm_var.solar2i = (uint16_t)P2_CS_CAL(adc_read(P2_CS));
        if(adxl345_dataready()) {
            int16_t d[3];
            adxl345_read(d);
            float ax = ADXL345_ACC_MILLIG(d[0]);
            float ay = ADXL345_ACC_MILLIG(d[1]);
            float az = ADXL345_ACC_MILLIG(d[2]);
            dm_var.accel = (uint16_t)sqrt(ax * ax + ay * ay + az * az);
        }
        dm_var.batvl    = (uint16_t)VL_DET_CAL(adc_read(VL_DET));
        dm_var.batvh    = (uint16_t)VH_DET_CAL(adc_read(VH_DET));
        dm_var.ipsb     = (uint16_t)PSB_CS_CAL(adc_read(PSB_CS));
        dm_var.iheater  = (uint16_t)HT_CS_CAL (adc_read(HT_CS));
        dm_var.ibus     = (uint16_t)BUS_CS_CAL(adc_read(BUS_CS));
        dm_var.iled     = (uint16_t)LED_CS_CAL(adc_read(LED_CS));
        dm_var.icam     = (uint16_t)CAM_CS_CAL(adc_read(CAM_CS));
#ifdef HEATER_PROT_INJECTION
        printf("HT TEST: BAT %d`C\n", NTC_THRU_CAL(adc_read(BAT_TMP)));
        dm_var.battemp = -15;
#else
        dm_var.battemp = NTC_THRU_CAL(adc_read(BAT_TMP));
#endif
        dm_var.camtemp  = NTC_THRU_CAL(adc_read(CAM_TMP));
        dm_var.temp1 = adc_read(CASE_TMP1);
        dm_var.temp2 = adc_read(CASE_TMP2);
        dm_var.temp3 = adc_read(CASE_TMP3);
        dm_var.temp4 = adc_read(CASE_TMP4);
        dm_var.radraw = adc_read(RAD_DET);
        dm_var.suna = adc_read(SUN_A);
        dm_var.sunb = adc_read(SUN_B);
        dm_var.sunc = adc_read(SUN_C);
        dm_var.sund = adc_read(SUN_D);
        dm_var.wdt = cycle_reboot;
        //eps_tlm.sunx;
        //eps_tlm.suny;
        //eps_tlm.mtqix;
        //eps_tlm.mtqiy;
        //eps_tlm.mtqiz;
        //eps_tlm.ebatvl;
}


void loop_thread(void const *args)
{
	static uint8_t cycle_wtd_ht = 0;
	static int8_t count_ht = 0;
	static int8_t battemp_bak;
        static uint8_t low_power_counter = 0;

	for (;;)
	{	
		loop_init();
		
		uint32_t WakeTimes = osKernelSysTick();
                
		if ((dm_var.batvh - dm_var.batvl < 3200) || (dm_var.batvl < 3200))
		{
                      low_power_counter++;
                      if(low_power_counter >= 10)
                      {
			CAM_POWER_OFF();
			TTC_POWER_OFF();
			OBC_POWER_OFF();
			
			//printf("CAM_POWER_OFF\n");
			//printf("TTC_POWER_OFF\n");
			//printf("OBC_POWER_OFF\n");
			
			OBC_RUN = 0;
#ifdef OBC_PRESERVE
			chang_cfg(CFG_OFF_OBCRUNFLAG,0);
#endif
                      }
		}
		else if ((dm_var.batvh - dm_var.batvl > 3400) && (dm_var.batvl > 3400))
		{
                        low_power_counter = 0;
			TTC_POWER_ON();
			//printf("TTC_POWER_ON\n");
		}

		if (OBC_RUN)
		{
			OBC_POWER_ON();
			//printf("OBC_POWER_ON\n");
		} 
		else
		{
			OBC_POWER_OFF();
			//printf("OBC_POWER_OFF\n");
		}

		if (CAM_RUN_FLAG == 0)
		{
			CAM_POWER_OFF();
			//printf("CAM_POWER_OFF\n");
		}
		else
		{
			CAM_RUN_FLAG--;
		}
		
		cycle_wtd_ht++;
		if (cycle_wtd_ht >= 8)
		{
			cycle_wtd_ht = 0;
			
			//printf("feeding the dog\n");
			
			WTD_IN_H();
			delay_cycles(10);
			WTD_IN_L();
			//printf("Feeding the dog\n");
                        printf("[%05d]WDog feed\n", cycle_reboot);
			
			if((dm_var.battemp < -10) && (dm_cfg.heat_valid))
			{
				static int8_t ht_first = 1;
				if ((dm_var.batvh - dm_var.batvl > 3500) && (dm_var.batvl > 3500))
				{
#ifndef HEATER_PROT_INJECTION
                                        HT_POWER_ON();
#else
                                        printf("HT TEST MODE: NOT ON\n");
#endif
        				printf("HT ON: LOW TEMP\n");
					if(ht_first)
					{
						battemp_bak = dm_var.battemp;
                                                count_ht = 30;
						ht_first = 0;
					}
					count_ht --;
				}
				else if((dm_var.batvh - dm_var.batvl < 3400) && (dm_var.batvl < 3400))
				{
					HT_POWER_OFF();
					ht_first = 1;
					count_ht = 30;  //8s * 30 = 240s
					printf("HT OFF: LOW PWR\n");
				}
                                
				printf("HT DeltaT: %d\n", dm_var.battemp - battemp_bak);
					
#ifdef HEATER_PROT_INJECTION
				if(count_ht <= 0)
#else
                                if(HT_ON_STATUS&&(count_ht <= 0))
#endif
				{
                                        if(dm_var.battemp - battemp_bak < 3)
                                          //temperature rise lower than 3`C
					{
						HT_POWER_OFF();
                                                printf("HT ERR: TEMP NOT RISING\n");
						//printf("Heating is prabably invaild,Now Power off\n");
						dm_var.heater = 0;
                                                chang_cfg(CFG_OFF_HEATERVALID,0);
					}
					else 
					{
						count_ht = 30;//8s * 30 = 240s
						battemp_bak = dm_var.battemp;
					}
				}	
			}
                        else
                        {
                          printf("HT OFF\n");
                            HT_POWER_OFF();
                        }
		}
		
		cycle_reboot ++;
		if(cycle_reboot > 9000)
		{
			printf("Sys is Rebooting Now\n");
			sys_reboot();
		}
		//osDelay(1000);
		osDelayUntil(&WakeTimes, 1000);
	}
}
