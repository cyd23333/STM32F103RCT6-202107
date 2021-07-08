#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f10x.h"
extern u16 CRC_Compute(u8 *puchMsg, u16 usDataLen);

#ifdef __cplusplus
}
#endif