#include "adc_task.h"


void adc_task(void *pvParameters)
{
  uint16_t adc_data[8]={0,0,0,0,0,0,0,0};
  uint8_t num=0;
  for(;;)
  {
    for(num=0;num<8;num++)
      {
      adc_data[num]=tlv2548_rchannel((uint16_t)num)>>4;
     while(MSS_GPIO_get_inputs()&0x0400000);
     vTaskDelay(1);
    }

  

}
}
