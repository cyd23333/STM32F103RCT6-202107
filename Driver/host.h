#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
	

extern void UART5_init(void);
extern void UART5_Send_Data(u8 *buf,u8 len);


#ifdef __cplusplus
}
#endif