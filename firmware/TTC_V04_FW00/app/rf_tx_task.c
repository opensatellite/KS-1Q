#include "rf_tx_task.h"
#include "bsp.h"
#include "APB3_reg.h"
#include "si4463.h"

#include "FreeRTOS.h"
#include "StackMacros.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "dm.h"

#include "m2sxxx.h"

#include "csp/drivers/tmp.h"

TTC_FIFO_Typedef TTC_Tx_FIFO;

uint8_t tx_on = 0;
uint8_t last_is_partial = 0;

uint16_t PA_CURRENT = 0;

const int tx_prepare_time = 1;
const int pktsize = 220;
const int check_interval = 1;
const int tx_tail_time = 1;
const int partial_check_thresh = 10;

static void delay(int tx_post_delay){
  while(tx_post_delay--) __NOP();
}

static void start_tx()
{
  if(tx_on == 0) {	
      printf("start tx\n");	
    vRadio_StartTX(RF_Tx,0x00);
    ANT1CN200_TX();
    tx_on = 1;
    vTaskDelay(tx_prepare_time);
  }
}

static int read_pacurrent()
{
   tlv2548_rchannel((uint16_t)4);
   while(MSS_GPIO_get_inputs()&0x0400000);
   PA_CURRENT = tlv2548_rchannel((uint16_t)0)>>4;
   while(MSS_GPIO_get_inputs()&0x0400000);   
   return PA_CURRENT;
}


static void stop_tx()
{
  if(tx_on && ((TTC->FLAG & RF_TxSending) == 0) && 
     ((TTC->FLAG & RF_TxA) == 0) && ((TTC->FLAG & RF_TxB) == 0)) {	
      //printf("stop tx\n");
    //delay(tx_post_delay);
    vTaskDelay(tx_tail_time);
    ANT1CN200_RX();
    vRadio_ReadyTX(RF_Tx,0x00);
    tx_on = 0;	
  }
}

static void send_a_packet()
{
  uint32_t flag=TTC->FLAG;
  //if ((TTC->FLAG & RF_TxA == 0)&&(TTC->FLAG & RF_RxRecing==0))
  if ((flag & RF_TxA) == 0)
  {
    //printf("load tx a\n");
    //printf("tx flag: %h \n", flag);

    tmp_get_packet((uint8_t*)TTC_Tx_FIFO.data8, pktsize);
    TTC_Tx_FIFO.id = SPACECRAFT_ID;
    TTC_Tx_FIFO.type = CCSDS_TYPE_DOWN_IP;
    TTC_Tx_FIFO.version = CCSDS_VERSION;
    memcpy32((uint32_t*) &TTC_Tx_FIFO, (uint32_t*) TX_FIFOA_BASE,
             sizeof(TTC_Tx_FIFO) / 4);
    TTC->FLAG |= RF_TxA;
    dm_var.txpkts++;
    //printf("tx packet num: %d \n", dm_var.txpkts);
  }
  //else if ((TTC->FLAG & RF_TxB ==0)&&(TTC->FLAG & RF_RxRecing==0))
  else if ((flag & RF_TxB) ==0)
  {
    //printf("load tx b\n");
    //printf("tx flag: %h \n", flag);
    tmp_get_packet((uint8_t*)TTC_Tx_FIFO.data8, pktsize);
    TTC_Tx_FIFO.id = SPACECRAFT_ID;
    TTC_Tx_FIFO.type = CCSDS_TYPE_DOWN_IP;
    TTC_Tx_FIFO.version = CCSDS_VERSION;
    memcpy32((uint32_t*)&TTC_Tx_FIFO, (uint32_t*) TX_FIFOB_BASE,
             sizeof(TTC_Tx_FIFO) / 4);
    TTC->FLAG |= RF_TxB;
    dm_var.txpkts++;
    //printf("tx packet num: %d \n", dm_var.txpkts);
  }
  else 
    printf("all tx busy\n");
}

void rf_tx_task(void *pvParameters)
{
  for (;;)
  {
    if(!tx_on) {
      //printf("wait for data\n");
      tmp_wait();
    } 
    
    vTaskDelay(check_interval);
        
    int stat = tmp_get_avail(pktsize);
    if(stat == -1) { //there is no data
      last_is_partial = 0;
      //stop si4463
    }
    else if(stat == 0) {
      //have some data, only a partial of packet
      if(last_is_partial >= partial_check_thresh) {
        //last time also get partial
        start_tx();
        send_a_packet();//flush it now
        //read_pacurrent();
        last_is_partial = 0;
        continue;
      }
      else {
        last_is_partial++;
        //printf("wait for more\n");
      }
    }
    else
    {
      start_tx();
      send_a_packet();
      //read_pacurrent();
      last_is_partial = 0;
      continue;
    }
    stop_tx();
    
  }
}
