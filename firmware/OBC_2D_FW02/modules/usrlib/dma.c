#include "stm32f4xx.h"
#include "dma.h"

#define IS_DMA1_PERIPH(PERIPH) (((PERIPH) == DMA1_Stream0) || \
                                ((PERIPH) == DMA1_Stream1) || \
                                ((PERIPH) == DMA1_Stream2) || \
                                ((PERIPH) == DMA1_Stream3) || \
                                ((PERIPH) == DMA1_Stream4) || \
                                ((PERIPH) == DMA1_Stream5) || \
                                ((PERIPH) == DMA1_Stream6) || \
                                ((PERIPH) == DMA1_Stream7))

#define IS_DMA2_PERIPH(PERIPH) (((PERIPH) == DMA2_Stream0) || \
                                ((PERIPH) == DMA2_Stream1) || \
                                ((PERIPH) == DMA2_Stream2) || \
                                ((PERIPH) == DMA2_Stream3) || \
                                ((PERIPH) == DMA2_Stream4) || \
                                ((PERIPH) == DMA2_Stream5) || \
                                ((PERIPH) == DMA2_Stream6) || \
                                ((PERIPH) == DMA2_Stream7))

void __dma_clock_cmd(DMA_Stream_TypeDef *dma,bool cmd)
{
  if(IS_DMA1_PERIPH(dma))
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,cmd==true?ENABLE:DISABLE);
  else if(IS_DMA2_PERIPH(dma))
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,cmd==true?ENABLE:DISABLE);
  else;
}