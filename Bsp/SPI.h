#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

void SPI_INIT();
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
extern int read(char *path, uint8_t *buf, int len);
extern int write(char *paht, uint8_t *buf, int len);
#ifdef __cplusplus
}
#endif