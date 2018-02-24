#include "bsp.h"
#include "macros.h"
#include "stm32_adc.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "vtimer.h"
#include "stm32_pwm.h"

#include <string.h>

adc_channel_t adc_channels[] = {
  {VL_DET,  ADC_SampleTime_239Cycles5},
  {VH_DET,  ADC_SampleTime_239Cycles5},
  {HT_CS,   ADC_SampleTime_239Cycles5},
  {RAD_DET, ADC_SampleTime_239Cycles5},
  {SUN_A,   ADC_SampleTime_239Cycles5},
  {SUN_B,   ADC_SampleTime_239Cycles5},
  {SUN_C,   ADC_SampleTime_239Cycles5},
  {SUN_D,   ADC_SampleTime_239Cycles5},
  {CAM_TMP, ADC_SampleTime_239Cycles5},
  {BUS_CS,  ADC_SampleTime_239Cycles5},
  {P2_CS,   ADC_SampleTime_239Cycles5},
  {P1_CS,   ADC_SampleTime_239Cycles5},
  {LED_CS,  ADC_SampleTime_239Cycles5},
  {PSB_CS,  ADC_SampleTime_239Cycles5},
  {BAT_TMP, ADC_SampleTime_239Cycles5},
  {CAM_CS,  ADC_SampleTime_239Cycles5}
};

uart_config_t uart_config = {
  .USART_BaudRate = 115200,
  .USART_WordLength = USART_WordLength_8b,
  .USART_StopBits = USART_StopBits_1,
  .USART_Parity = USART_Parity_No,
  .USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
  .USART_HardwareFlowControl = USART_HardwareFlowControl_None
};

void rcc_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

void gpio_init(void)
{
  pinmode(VL_DET_PIN,  AIN);
  pinmode(VH_DET_PIN,  AIN);
  pinmode(RAD_DET_PIN, AIN);
  pinmode(SUN_A_PIN,   AIN); 
  pinmode(SUN_B_PIN,   AIN);
  pinmode(SUN_C_PIN,   AIN);
  pinmode(SUN_D_PIN,   AIN);
  pinmode(P1_CS_PIN,   AIN); 
  pinmode(P2_CS_PIN,   AIN);
  pinmode(CAM_TMP_PIN, AIN);
  pinmode(BAT_TMP_PIN, AIN);
  pinmode(HT_CS_PIN,   AIN);
  pinmode(BUS_CS_PIN,  AIN);
  pinmode(RTC_CS_PIN,  AIN);
  pinmode(LED_CS_PIN,  AIN);
  pinmode(PSB_CS_PIN,  AIN);
  
  pinmode(X_F_PWM_PIN, AFPUSHPULL);
  pinmode(X_R_PWM_PIN, AFPUSHPULL);
  pinmode(Y_F_PWM_PIN, AFPUSHPULL);
  pinmode(Y_R_PWM_PIN, AFPUSHPULL);
  pinmode(Z_F_PWM_PIN, AFPUSHPULL);
  pinmode(Z_R_PWM_PIN, AFPUSHPULL);
  
  pinmode(EX_IO1_PIN,  PULLUP);
  pinmode(EX_IO2_PIN,  PULLUP);
  
  pinmode(HT_CS_PIN,   AIN);
  pinmode(BUS_CS_PIN,  AIN);
  pinmode(LED_CS_PIN,  AIN);
  pinmode(PSB_CS_PIN,  AIN);
  pinmode(CAM_CS_PIN,  AIN);
  
  pinmode(ADC_PWRON_PIN, OUTPUT);
  pinmode(CAM_ON_PIN, OUTPUT);
  pinmode(BUS_ON_PIN, OUTPUT);
  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  pinmode(LED_ON_PIN, OUTPUT);
  pinlow(LED_ON_PIN);
  
  pinmode(MTQ_ON_PIN, OUTPUT);
  pinlow(MTQ_ON_PIN);
  pinmode(HT_ON_PIN, OUTPUT);
  pinlow(HT_ON_PIN);
  pinmode(DEPLOY_PIN, OUTPUT);
  pinlow(DEPLOY_PIN);
  
  pinmode(MOSI_PIN, AFPUSHPULL);
  pinmode(SCLK_PIN, AFPUSHPULL);
  pinmode(MISO_PIN, PULLDOWN);
  //pinmode(ADC_CS_PIN, OUTPUT);
  pinhigh(ADC_CS_PIN);
  pinmode(ADC_EOC_PIN, INPUT);
  
  pinmode(AX3S_CS_PIN, OUTPUT);
  //pinhigh(AX3S_CS_PIN);
  pinmode(RTC_CS_PIN, OUTPUT);
  //pinhigh(RTC_CS_PIN);
  pinmode(CAM_TX_PIN, AFPUSHPULL);
  pinmode(CAM_RX_PIN, INPUT);
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
  pinmode(WTD_IN_PIN, INPUT);
  pinmode(CAN_TX_PIN, AFPUSHPULL);
  pinmode(CAN_RX_PIN, PULLUP);
  pinmode(CAN_LBK_PIN,PUSHPULL);
  pinlow(CAN_LBK_PIN);
  
  pinmode(TTC_ON_PIN,PUSHPULL);
  pinlow(TTC_ON_PIN);
  
  pinmode(ADC_PWRON_PIN,PUSHPULL);
  pinhigh(ADC_PWRON_PIN);
}

void wdog_feed() {
  WTD_IN_H();
  delay_cycles(10);
  WTD_IN_L();
}

void load_cfg() {
  dm_var.heater = dm_cfg.heat_valid;
#ifdef OBC_PRESERVE
  dm_var.deploy = dm_cfg.obc_run_flag;
#endif
}

void reset_cfg() {
  FLASH_UnlockBank1();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR | FLASH_FLAG_PGERR);
  FLASH_ErasePage(user_set_addr);
  FLASH_LockBank1();
}

int chang_cfg(int offset,uint16_t val)
{
  if(offset < 0 || offset > CFG_OFF_MAX) return 0;
  
  uint16_t backup[3];
  memcpy(backup, &dm_cfg, sizeof(struct dm_cfg_s));
  backup[offset] = val;
         
  __disable_irq();
 
  FLASH_UnlockBank1(); 
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR | FLASH_FLAG_PGERR );
  if(val) {
    FLASH_ErasePage(user_set_addr);  
    FLASH_ProgramHalfWord(user_set_addr,     backup[0]);
    FLASH_ProgramHalfWord(user_set_addr + 2, backup[1]);
    FLASH_ProgramHalfWord(user_set_addr + 4, backup[2]);
  }
  else FLASH_ProgramHalfWord(user_set_addr + offset * 2, 0);
  FLASH_LockBank1();
  
  __enable_irq();
    
  return 1;
}

void boot_check()
{
  static int32_t timer = 0;
  
  delay_cycles(1000);
  bool last_read = pinread(EX_IO2_PIN);
  while(last_read != pinread(EX_IO2_PIN)) {
    last_read = pinread(EX_IO2_PIN);
  }

  if (!last_read)
  {
    printf("EX_IO2_PIN low\n");
    printf("Reset to default config\n");
    reset_cfg();
  }
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  timer_clock(TIM2 , 32000, 500);
  
  if(dm_cfg.eps_run_wait)
  {
    printf("Firstrun... Waiting for 100 min\n");
  }
  
  int bypass_state = 0;
  while (dm_cfg.eps_run_wait)
  {
    if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update))
    {
      TIM_ClearFlag(TIM2, TIM_FLAG_Update);
      timer++;
      
      WTD_IN_H();
      delay_cycles(10);
      WTD_IN_L();
      printf("+%ds,", timer);
      /*
      while(uart_available(CONSOLE) > 0) {
        char c = uart_getc(CONSOLE);
        switch(bypass_state) {
        case 0: if(c == 'k') bypass_state = 1;break;
        case 1: if(c == 's') bypass_state = 2;break;
        case 2: if(c == '1') bypass_state = 3;break;
        case 3: if(c == 'q') bypass_state = 4;break;
        case 4: if(c == '\r' || c == '\n') {
            printf("Bypass, clear eps_run_wait flag\n");
            chang_cfg( CFG_OFF_EPSRUNWAIT, 0 );
        }
        default: bypass_state = 0;
        }
      }
      */
    }
    if (timer >= 60 * 100)
    {
      printf("Timeout, clear eps_run_wait flag\n");
      chang_cfg( CFG_OFF_EPSRUNWAIT, 0 );
    }
  
  }
  TIM_Cmd(TIM2, DISABLE);
  
  //__WFI();
}

void bsp_init()
{
  rcc_init();
  gpio_init();
  spi_init(SPI2, 3, 8, 16);
  
  systick_init();
  vtimer_init();
  
  uart_init(CONSOLE, &uart_config);
  uart_open(CONSOLE, O_RDWR);
  
  //flash_test();
  boot_check();
  load_cfg();
  
  adc_init(adc_channels, NUMBER_OF(adc_channels), false);
  
  //set tlv2548 low power mode
  pinlow(ADC_CS_PIN);
  spi_rw(SPI2,0x80);
  //spirw(0x80);
  pinhigh(ADC_CS_PIN);
  
}
