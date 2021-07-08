#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UP 0
#define DOWN 1

extern s8 Step_Motor_Stop_Flag;
extern s8 motor_stop_flag;
extern s8 motor_start_flag;
extern s8 present_floor;
extern s8 target_floor;
extern u32 motor_cnt;
	

void Delay(uint32_t nCount);
void Step_Motor_Conf(void);
void Step_Motor_Off (u8 id);
u8 Step_Motor_Local(u8 id, s8 loc);
void Step_Motor_A (u8 id, u8 dir, s8 A, u32 speed);
void Step_Motor_Loop(u8 id, u8 dir, u32 speed, u32 loop);

#ifdef __cplusplus
}
#endif
