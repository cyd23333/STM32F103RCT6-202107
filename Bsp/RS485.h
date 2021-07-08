#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
	
typedef struct RxStruct
{
uint16_t index;
uint8_t *buffer_ptr;
uint16_t buffer_size;
}RxStruct_t;

extern RxStruct_t RxStruct_data;
extern void RS485_Init(void);
extern void RS485_Send_Data(u8 *buf,u8 len);
extern void RS485_Send_Data_2(u8 *buf,u8 len);
extern void UART5_init(void);
//extern void RxStruct_init(RxStruct_t* RxStruct, uint8_t *pool, uint16_t size);


#ifdef __cplusplus
}
#endif