#ifndef _DMA_H__
#define _DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_dma.h"  
#include "datatypes.h"
    
void __dma_clock_cmd(DMA_Stream_TypeDef *dma,bool cmd); 
    
#ifdef __cplusplus
}
#endif
    

#endif