#include "bsp.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "uart.h"
#include "stm32_adc.h"
#include "cbinaryparser.h"
#include "freertos.h"
#include "task.h"
#include "adxl345.h"
#include "systick.h"
#include "debug.h"
#include "mfifo.h"

struct binaryParser parser;
/*
struct eps_tlm_t {
    uint32_t timestamp;
    float solar1v;
    float solar1i;
    float solar2v;
    float solar2i;
    float accel;
    float vpsb;
    float batvl;
    float ipsb;
    float iht;
    float ibus;
    float iled;
    float icam;
    float batttemp;
    float camtemp;
    float temp1;
    float temp2;
    float temp3;
    float temp4;
    float radraw;
    float sunva;
    float sunvb;
    float sunvc;
    float sunvd;
    float sunx;
    float suny;
    float mtqix;
    float mtqiy;
    float mtqiz;
    float ebatvl;
}eps_tlm;

void sendbyte(uint8_t x)
{
    uart_putc(CONSOLE, x);
}
*/
void thread_init(void *parameters)
{
    /*
    int parser_bufsize = 256;
    uint8_t* parser_buf = pvPortMalloc(parser_bufsize);
    binaryParser_init(&parser, formatRadio, parser_bufsize, parser_buf);
    parser.sendbyte = sendbyte;
    uart_open(CONSOLE, O_RDWR);
    */
    while(!adxl345_init(0x0a));
    
    while(1)
    {
        TickType_t xLastWakeTime = xTaskGetTickCount ();
        for(int i = 0; i < 16; i++)
        {
            adc_trig();
        }
        /*
        eps_tlm.solar1v = 0;
        eps_tlm.solar2v = 0;
        eps_tlm.solar1i = P1_CS_CAL(adc_read(P1_CS));
        eps_tlm.solar2i = P2_CS_CAL(adc_read(P2_CS));
        if(adxl345_dataready()) {
            int16_t d[3];
            adxl345_read(d);
            float ax = ADXL345_ACC_MILLIG(d[0]);
            float ay = ADXL345_ACC_MILLIG(d[1]);
            float az = ADXL345_ACC_MILLIG(d[2]);
            eps_tlm.accel = sqrt(ax * ax + ay * ay + az * az) / 1000.0f;
        }
        eps_tlm.batvl = VL_DET_CAL(adc_read(VL_DET));
        eps_tlm.vpsb = VH_DET_CAL(adc_read(VH_DET));
        eps_tlm.ipsb  = PSB_CS_CAL(adc_read(PSB_CS));
        eps_tlm.iht   = HT_CS_CAL (adc_read(HT_CS));
        eps_tlm.ibus  = BUS_CS_CAL(adc_read(BUS_CS));
        eps_tlm.iled  = LED_CS_CAL(adc_read(LED_CS));
        eps_tlm.icam  = CAM_CS_CAL(adc_read(CAM_CS));
        eps_tlm.batttemp = adc_read(BAT_TMP);
        eps_tlm.camtemp  = adc_read(CAM_TMP);
        //eps_tlm.temp1 = adc_read(CASE_TMP1);
        //eps_tlm.temp2 = adc_read(CASE_TMP2);
        //eps_tlm.temp3 = adc_read(CASE_TMP3);
        //eps_tlm.temp4 = adc_read(CASE_TMP4);
        eps_tlm.radraw = adc_read(RAD_DET);
        eps_tlm.sunva = adc_read(SUN_A);
        eps_tlm.sunvb = adc_read(SUN_B);
        eps_tlm.sunvc = adc_read(SUN_C);
        eps_tlm.sunvd = adc_read(SUN_D);
        //eps_tlm.sunx;
        //eps_tlm.suny;
        //eps_tlm.mtqix;
        //eps_tlm.mtqiy;
        //eps_tlm.mtqiz;
        //eps_tlm.ebatvl;
        eps_tlm.timestamp = micros();
        sendPacket(&parser, TELEMETRY_EPSTLM, sizeof(eps_tlm), (unsigned char*)&eps_tlm);
        */
        
        solar1i = (uint16_t)P1_CS_CAL(adc_read(P1_CS));
        solar2i = (uint16_t))P2_CS_CAL(adc_read(P2_CS));
        if(adxl345_dataready()) {
            int16_t d[3];
            adxl345_read(d);
            float ax = ADXL345_ACC_MILLIG(d[0]);
            float ay = ADXL345_ACC_MILLIG(d[1]);
            float az = ADXL345_ACC_MILLIG(d[2]);
            accel = (uint16_t)sqrt(ax * ax + ay * ay + az * az);
        }
        batvl    = (uint16_t)VL_DET_CAL(adc_read(VL_DET));
        vpsb     = (uint16_t)VH_DET_CAL(adc_read(VH_DET));
        ipsb     = (uint16_t)PSB_CS_CAL(adc_read(PSB_CS));
        iheater  = (uint16_t)HT_CS_CAL (adc_read(HT_CS));
        ibus     = (uint16_t)BUS_CS_CAL(adc_read(BUS_CS));
        iled     = (uint16_t)LED_CS_CAL(adc_read(LED_CS));
        icam     = (uint16_t)CAM_CS_CAL(adc_read(CAM_CS));
        batttemp = adc_read(BAT_TMP);
        camtemp  = adc_read(CAM_TMP);
        //eps_tlm.temp1 = adc_read(CASE_TMP1);
        //eps_tlm.temp2 = adc_read(CASE_TMP2);
        //eps_tlm.temp3 = adc_read(CASE_TMP3);
        //eps_tlm.temp4 = adc_read(CASE_TMP4);
        radraw = adc_read(RAD_DET);
        sunva = adc_read(SUN_A);
        sunvb = adc_read(SUN_B);
        sunvc = adc_read(SUN_C);
        sunvd = adc_read(SUN_D);
        //eps_tlm.sunx;
        //eps_tlm.suny;
        //eps_tlm.mtqix;
        //eps_tlm.mtqiy;
        //eps_tlm.mtqiz;
        //eps_tlm.ebatvl;
        
        vTaskDelayUntil( &xLastWakeTime, 100 );
    }
}

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
    __WFI();

}

void tlm_send_debug()
{
    uint8_t buffer[68];
#ifdef USE_TELEMETRY_DEBUG
    int len = mfifo_len(&debug_msg_fifo);
    if(len > 0)
    {
        uint32_t timestamp = micros();
        memcpy(buffer, &timestamp, 4);
        for(uint8_t i = 4; i < len + 4; i++)
        {
            uint8_t x;
            mfifo_get(&debug_msg_fifo, &x);
            buffer[i] = x;
        }
        sendPacket(&parser, TELEMETRY_DEBUG, len + 4, buffer);
    }
#endif
}

int main()
{	 
    bsp_init();
    xTaskCreate(thread_init, "init", 256, NULL, configMAX_PRIORITIES - 1, 0);
    vTaskStartScheduler();
    for(;;);
}
