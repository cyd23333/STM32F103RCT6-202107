#include "stm32f10x.h"
#include "Step_Motor.h"

#define DIR_FORWARD 0                                                                                                 
#define DIR_BACKWARD 1

#define MOTOR_ENBALE 0
#define MOTOR_DISABLE 1

#define MOTOR1 1
#define MOTOR2 2
#define MOTOR3 3

#define A_P 0
#define A_P_B_P 1
#define B_P 2
#define A_M_B_P 3
#define A_M 4
#define A_M_B_M 5
#define B_M 6
#define A_P_B_M 7

#define STEP_MOTOR_PORT1 GPIOA
#define STEP_MOTOR_PORT2 GPIOA
#define STEP_MOTOR_PORT3 GPIOA

#define STEP_MOTOR1_A_PLUS GPIO_Pin_1
#define STEP_MOTOR1_B_PLUS GPIO_Pin_2
#define STEP_MOTOR1_A_MINUS GPIO_Pin_3
#define STEP_MOTOR1_B_MINUS GPIO_Pin_4

#define STEP_MOTOR2_A_PLUS GPIO_Pin_9
#define STEP_MOTOR2_B_PLUS GPIO_Pin_10
#define STEP_MOTOR2_A_MINUS GPIO_Pin_11
#define STEP_MOTOR2_B_MINUS GPIO_Pin_12

#define STEP_MOTOR_CLK GPIO_Pin_5
#define STEP_MOTOR_DIR GPIO_Pin_6
#define STEP_MOTOR_EN  GPIO_Pin_7

#define BEEP GPIO_Pin_15

#define STEP_MOTOR1_CHECK 
#define STEP_MOTOR2_CHECK 


s8 Step_Motor_Stop_Flag = 0;
s8 motor_start_flag = 0;
s8 motor_stop_flag = 0;
s8 present_floor = 0;
s8 target_floor = 0;
u32 motor_cnt = 0;


//=============================================================================
//函数名称：Delay
//功能概要：延时
//参数说明：nCount：延时长短
//函数返回：无
//=============================================================================

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}



void Step_Motor_Conf(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 	

  GPIO_InitStructure.GPIO_Pin = STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_PLUS | STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(STEP_MOTOR_PORT1, &GPIO_InitStructure);
	GPIO_ResetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_PLUS | STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS);
	
  GPIO_InitStructure.GPIO_Pin = STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_PLUS | STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(STEP_MOTOR_PORT2, &GPIO_InitStructure);
	GPIO_ResetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_PLUS | STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS);
	
	GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_CLK | STEP_MOTOR_DIR | STEP_MOTOR_EN | BEEP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(STEP_MOTOR_PORT3, &GPIO_InitStructure);
	GPIO_ResetBits(STEP_MOTOR_PORT3,STEP_MOTOR_CLK | STEP_MOTOR_DIR | STEP_MOTOR_EN);
	GPIO_SetBits(STEP_MOTOR_PORT3,BEEP);
}

void Beep_ON()
{
	GPIO_ResetBits(STEP_MOTOR_PORT3,BEEP);
}

void Beep_OFF()
{
	GPIO_SetBits(STEP_MOTOR_PORT3,BEEP);
}


//=============================================================================
//函数名称：Step_Motor_Off
//功能概要：电机断电
//参数说明：id：电机ID
//函数返回：无
//=============================================================================

void Step_Motor_Off (u8 id)
{
	switch(id)
	{
		case MOTOR1:
			GPIO_ResetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_PLUS | STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS);
			break;
		case MOTOR2:
			GPIO_ResetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_PLUS | STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS);
			break;
		case MOTOR3:
			GPIO_SetBits(STEP_MOTOR_PORT3,STEP_MOTOR_EN);
			break;
	}
}
//=============================================================================
//函数名称：Step_Motor_Local
//功能概要：电机位置切换
//参数说明：id：电机ID  loc：电机位置
//函数返回：loc
//=============================================================================
u8 Step_Motor_Local(u8 id, s8 loc)
{
	switch(loc)
	{
//		case A_P:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_PLUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_B_PLUS | STEP_MOTOR_A_MINUS | STEP_MOTOR_B_MINUS);
//			break;
//		case A_P_B_P:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_PLUS | STEP_MOTOR_B_PLUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_MINUS | STEP_MOTOR_B_MINUS);
//			break;
//		case B_P:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_B_PLUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_PLUS | STEP_MOTOR_A_MINUS | STEP_MOTOR_B_MINUS);
//			break;
//		case A_M_B_P:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_MINUS | STEP_MOTOR_B_PLUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_PLUS | STEP_MOTOR_B_MINUS);
//			break;
//		case A_M:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_MINUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_B_PLUS | STEP_MOTOR_A_PLUS | STEP_MOTOR_B_MINUS);
//			break;
//		case A_M_B_M:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_MINUS | STEP_MOTOR_B_MINUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_PLUS | STEP_MOTOR_B_PLUS);
//			break;
//		case B_M:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_B_MINUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_B_PLUS | STEP_MOTOR_A_MINUS | STEP_MOTOR_A_PLUS);
//			break;
//		case A_P_B_M:
//			GPIO_SetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_PLUS | STEP_MOTOR_B_MINUS);
//			GPIO_ResetBits(id == 1 ? STEP_MOTOR_PORT1 : STEP_MOTOR_PORT2,STEP_MOTOR_A_MINUS | STEP_MOTOR_B_PLUS);
//			break;
		case A_P:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_A_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_B_PLUS | STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_MINUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_A_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_B_PLUS | STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_MINUS);
			}
			break;
		case A_P_B_P:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_PLUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_PLUS);
			}
			break;
		case B_P:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_B_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_B_PLUS | STEP_MOTOR1_A_MINUS | STEP_MOTOR1_A_PLUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_B_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_B_PLUS | STEP_MOTOR2_A_MINUS | STEP_MOTOR2_A_PLUS);
			}
			break;
		case A_M_B_P:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_PLUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_MINUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_PLUS);
			}
			break;
		case A_M:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_A_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_B_PLUS | STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_A_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_B_PLUS | STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS);
			}
			break;
		case A_M_B_M:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS);
			}
			break;
		case B_M:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_B_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_PLUS | STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_MINUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_B_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_PLUS | STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_MINUS);
			}
			break;
		case A_P_B_M:
			if(id == 1)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT1, STEP_MOTOR1_A_MINUS | STEP_MOTOR1_B_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT1,STEP_MOTOR1_A_PLUS | STEP_MOTOR1_B_MINUS);
			}
			if(id == 2)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT2, STEP_MOTOR2_A_MINUS | STEP_MOTOR2_B_PLUS );
				GPIO_SetBits(STEP_MOTOR_PORT2,STEP_MOTOR2_A_PLUS | STEP_MOTOR2_B_MINUS);
			}
			break;
	}
	return loc;
}

//=============================================================================
//函数名称：Step_Motor_4A
//功能概要：电机单步4拍
//参数说明：id：电机ID  dir：运行方向   speed：电机速度  A：单步拍数
//函数返回：无
//=============================================================================

void Step_Motor_A (u8 id, u8 dir, s8 A, u32 speed)
{
	int i;
	s8 loc = A_P;
	switch(id)
	{	
		case MOTOR1:
		case MOTOR2:
			for(i = 0; i < 4; i++)
			{
				Step_Motor_Local(id, loc);
				Delay(speed);
				loc += ( DIR_FORWARD == dir ? 8/A : -8/A);
				if(loc < 0 )
					loc += 8;
				if(loc >=8)
					loc -=8;
			}
			break;

		case MOTOR3:

			GPIO_ResetBits(STEP_MOTOR_PORT3,STEP_MOTOR_EN);

			if(DIR_FORWARD == dir)
			{
				GPIO_ResetBits(STEP_MOTOR_PORT3,STEP_MOTOR_DIR);
			}
			else
			{
				GPIO_SetBits(STEP_MOTOR_PORT3,STEP_MOTOR_DIR);
			}
			for(i = 0; i < A; i++)
			{
				GPIO_SetBits(STEP_MOTOR_PORT3,STEP_MOTOR_CLK);
				Delay(speed*3/4);
				GPIO_ResetBits(STEP_MOTOR_PORT3,STEP_MOTOR_CLK);
				Delay(speed/4);
			}

			break;
	}
	Step_Motor_Off(id);
}

//=============================================================================
//函数名称：Step_Motor_Loop
//功能概要：电机单步4拍
//参数说明：id：电机ID  dir：运行方向   speed：电机速度  A：单步拍数
//函数返回：无
//=============================================================================

void Step_Motor_Loop(u8 id, u8 dir, u32 speed, u32 loop)
{
	int i;
	for(i = 0;i < loop; i++)
	{
		Step_Motor_A(id,dir,4,speed);
	}
}


