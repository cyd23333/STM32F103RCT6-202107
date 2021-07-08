#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

extern s8 USART3_RX_Data;
	
void USART3_Config(void);
 void USART3SendByte(unsigned char SendData);
 u8 USART3GetByte(unsigned char* GetData);
 void UARTTest(void);

#ifdef __cplusplus
}
#endif
