#include "scheduler.h"
#include "control.h"
#include "topics.h"
#include "icm20600.h"
#include "estimator.h"
#include "Android.h"
#include "commander.h"
#include "modbus.h"
#include "RS485.h"


#include "Step_Motor.h"
#include "usart3.h"
//=============================================================================
//函数名称：Signal_Deal
//功能概要：电梯信号处理
//参数说明：无
//函数返回：无
//=============================================================================
void Signal_Deal()
{
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))//一层的传感器
	{
		present_floor = 1;
//		target_floor = 0;
	}
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))//二层的传感器
	{
		present_floor = 2;
//		target_floor = 0;
	}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11))//摁下二楼按钮
	{
		target_floor = 1;
	}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))//按下一楼按钮
	{
		target_floor = 2;
	}
}
//=============================================================================
//函数名称：Elevator_Call
//功能概要：电梯运行
//参数说明：无
//函数返回：无
//=============================================================================
void Elevator_Call()
{
	if(motor_stop_flag)
	{
		Step_Motor_Off(1);
		Step_Motor_Off(2);
		Step_Motor_Off(3);
		return;
	}
	
	while(present_floor!=target_floor)//判断电梯是否在按钮位置
	{
		Step_Motor_Loop(3, target_floor == 1 ? DOWN : UP, 1500,25);
		Signal_Deal();
	}
	Delay(10000000);

	Step_Motor_Loop(present_floor, UP, 30000, 160);

	USART3SendByte('0'); //摄像头开始检测
//	Delay (10000000);
//	Beep_ON();
//	Delay (30000000);
//	Beep_OFF();
//	Delay (30000000);
//	Beep_ON();
//	Delay (30000000);
//	Beep_OFF();
//	Delay (10000000);

	  Delay(40000000);
	while(Step_Motor_Stop_Flag)		//检测到电动车
	{
		//USART3SendByte('6');
		Step_Motor_Stop_Flag = 0;
		Step_Motor_Off(1);
		//	Beep_ON();//报警
		Delay(40000000);//一段时间后再次检测
		//	Beep_OFF();
	}
	
	USART3SendByte('1');//关闭摄像头

	Step_Motor_Loop(present_floor, DOWN, 30000, 160);
	Delay (10000000);
	
	while(present_floor==target_floor)//判断电梯是否到达
	{
		Step_Motor_Loop(3, target_floor == 1 ? UP : DOWN, 1500, 25);
		Signal_Deal();
	}
	Delay (10000000);
	Step_Motor_Loop(present_floor, UP, 30000, 160);
	Delay (10000000);
	Step_Motor_Loop(present_floor, DOWN, 30000, 160);
	Delay (10000000);

	target_floor = 0;
}




  void _schedule(void)
{
	schedule_tick++;

	unsigned char i = 0;
	
	Signal_Deal();
	if(target_floor != 0)
	{
		Elevator_Call();
	}
	
	
	
	
//	if(motor_start_flag)
//	{
////	Step_Motor_Loop(3, DOWN, 1500,22500);
////	Delay (10000000);
//	Step_Motor_Loop(1, UP, 30000, 160);
//		
//	USART3SendByte('0');
////	Delay (10000000);
////	Beep_ON();
////	Delay (30000000);
////	Beep_OFF();
////	Delay (30000000);
////	Beep_ON();
////	Delay (30000000);
////	Beep_OFF();
////	Delay (10000000);
//		
//	  Delay(10000000);Delay(10000000);Delay(10000000);Delay(10000000);
//	while(Step_Motor_Stop_Flag)
//	{
//		USART3SendByte('6');
//		Step_Motor_Stop_Flag = 0;
//		Step_Motor_Off(1);
//		//	Beep_ON();
//		Delay(6000000);Delay(10000000);Delay(10000000);Delay(10000000);
//		//	Beep_OFF();
//	}
//	
//	
//	USART3SendByte('1');
//	

//	Step_Motor_Loop(1, DOWN, 30000, 160);
////	Delay (10000000);
////	Step_Motor_Loop(3, UP, 1500, 22500);
////	Delay (10000000);
////	Step_Motor_Loop(2, UP, 30000, 160);
////	Delay (10000000);
////	Step_Motor_Loop(2, DOWN, 30000, 160);
////	Delay (10000000);
////	Step_Motor_Loop(3, DOWN, 1500,22500);
////		
////		
////		
//	
//	motor_start_flag = 0;
//	}	
	
	
	
	
	
	

}