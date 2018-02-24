#ifndef APB3_REG_H
#define APB3_REG_H

#include "m2sxxx.h"
#include <stdint.h>


#ifdef __ICCARM__
#pragma pack(4)
typedef struct {
	__IO uint32_t FLAG;
	__IO uint32_t ID;
	__IO uint32_t MID;
	__IO uint32_t MAD;
	__IO uint32_t TM_LEN;
	__IO uint32_t TC_LEN;
	__IO uint32_t PTXC;
	__IO uint32_t PRXC;
	__IO uint32_t PREC;
	__IO uint32_t TX_FREN;
	__IO uint32_t RX_FREN;
	__IO uint32_t TIMEOUT;
	__IO uint32_t PTIMEOUT;
	__IO uint32_t ADV;
	__IO uint32_t RSTART;
} TTC_Typedef;
#pragma pack()
#elif __GNUC__
typedef struct __attribute__((aligned(4))){
	__IO uint32_t FLAG;
	__IO uint32_t ID;
	__IO uint32_t MID;
	__IO uint32_t MAD;
	__IO uint32_t TM_LEN;
	__IO uint32_t TC_LEN;
	__IO uint32_t PTXC;
	__IO uint32_t PRXC;
	__IO uint32_t PREC;
	__IO uint32_t TX_FREN;
	__IO uint32_t RX_FREN;
	__IO uint32_t TIMEOUT;
	__IO uint32_t PTIMEOUT;
	__IO uint32_t ADV;
	__IO uint32_t RSTART;
} TTC_Typedef;
#endif

#ifdef __ICCARM__
__packed typedef struct {
  __IO uint16_t id;
  __IO uint8_t version : 4;
  __IO uint8_t type : 4;
  __IO uint8_t data8[220];
  __IO uint8_t null;
} TTC_FIFO_Typedef;

__packed typedef struct {
  __IO uint16_t id;
  __IO uint8_t version : 4;
  __IO uint8_t type : 4;
  __IO uint8_t data8[25];
} TTC_Rx_FIFO_Typedef;
#elif __GNUC__

typedef struct __attribute__((__packed__)){
  __IO uint16_t id;
  __IO uint8_t version : 4;
  __IO uint8_t type : 4;
  __IO uint8_t data8[220];
  __IO uint8_t null;
} TTC_FIFO_Typedef;
#else
typedef struct {
  __IO uint16_t id;
  __IO uint8_t version : 4;
  __IO uint8_t type : 4;
  __IO uint8_t data8[220];
  __IO uint8_t null;
} TTC_FIFO_Typedef;
}
#endif

#define TTC_BASE 0x30000000
#define TX_FIFOA_BASE (TTC_BASE + 0x2000)
#define TX_FIFOB_BASE (TTC_BASE + 0x4000)
#define RX_FIFOA_BASE (TTC_BASE + 0x6000) //57*32Bit  0~56  0:not used
#define RX_FIFOB_BASE (TTC_BASE + 0x8000) //57*32Bit  0~56  0:not used

#define TTC ((TTC_Typedef *)TTC_BASE)

#define RF_RxA      (uint32_t)(1 << 2)
#define RF_RxB      (uint32_t)(1 << 3)
#define RF_TxA      (uint32_t)(1 << 0)
#define RF_TxB      (uint32_t)(1 << 1)
#define RF_TxSending (uint32_t)(1 << 5)
#define RF_RxRecing (uint32_t)(1 << 6)
#define RF_RxFLAG   (uint32_t)(1 << 7)

/*
#define TX_FIFOA		((__IO TTC_FIFO_Typedef *)TX_FIFOA_BASE)
#define TX_FIFOB		((__IO TTC_FIFO_Typedef *)TX_FIFOB_BASE)
#define RX_FIFOA		((__IO TTC_FIFO_Typedef *)RX_FIFOA_BASE)
#define RX_FIFOB		((__IO TTC_FIFO_Typedef *)RX_FIFOB_BASE)
*/

#endif
