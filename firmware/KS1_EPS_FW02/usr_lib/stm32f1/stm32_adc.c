#include "stm32f10x.h"
#include "stm32_adc.h"

static uint16_t buffer[16];
float adc_gain = 3.3/4096;// V/LSB
#define ADC1_DR_ADDR ((uint32_t)0x4001244c)

void adc_init(adc_channel_t cfg[], int n, bool continuous)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);
    DMA_InitTypeDef dma;
    dma.DMA_PeripheralBaseAddr = ADC1_DR_ADDR;
    dma.DMA_MemoryBaseAddr = (uint32_t)buffer;
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = n;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_Medium;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &dma);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);//24 / 2
    
    ADC_InitTypeDef adc;
    adc.ADC_Mode = ADC_Mode_Independent;
    adc.ADC_ScanConvMode = ENABLE;
    adc.ADC_ContinuousConvMode = continuous ? ENABLE : DISABLE;
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = n;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_Init(ADC1,&adc);
    
    for(int i = 0; i < n; i++)
    {
    	ADC_RegularChannelConfig(ADC1, cfg[i].ch, i+1, cfg[i].sample_time);
    }
    
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1,ENABLE);
    
    adc_calibration();
    if(continuous) ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void adc_trig()
{
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void adc_deinit()
{
    ADC_Cmd(ADC1,DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,DISABLE);
}

uint16_t adc_read(int id)
{
    return buffer[id];
}

void adc_calibration()
{
    //stop ADC
    ADC_ExternalTrigConvCmd(ADC1, DISABLE);
    //ADC_ExternalTrigConvCmd(ADC2, DISABLE);
    
    ADC_ResetCalibration(ADC1);
    //ADC_ResetCalibration(ADC2);
    while(ADC_GetResetCalibrationStatus(ADC1));
    //while(ADC_GetResetCalibrationStatus(ADC2));
    ADC_StartCalibration(ADC1);
    //ADC_StartCalibration(ADC2);
    while(ADC_GetCalibrationStatus(ADC1));
    //while(ADC_GetCalibrationStatus(ADC2));
    
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    //ADC_ExternalTrigConvCmd(ADC2, ENABLE);
    
    /*
    ADC_TempSensorVrefintCmd(ENABLE);
    ADC_RegularChannelConfig(ADC1,17,0,ADC_SampleTime_71Cycles5);
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
    adc_gain = 1.20 / (float)ADC_GetConversionValue(ADC1);
    ADC_TempSensorVrefintCmd(DISABLE);
    */
}
