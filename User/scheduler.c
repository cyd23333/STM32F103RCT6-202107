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
//�������ƣ�Signal_Deal
//���ܸ�Ҫ�������źŴ���
//����˵������
//�������أ���
//=============================================================================
void Signal_Deal()
{
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))//һ��Ĵ�����
	{
		present_floor = 1;
//		target_floor = 0;
	}
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))//����Ĵ�����
	{
		present_floor = 2;
//		target_floor = 0;
	}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11))//���¶�¥��ť
	{
		target_floor = 1;
	}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12))//����һ¥��ť
	{
		target_floor = 2;
	}
}
//=============================================================================
//�������ƣ�Elevator_Call
//���ܸ�Ҫ����������
//����˵������
//�������أ���
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
	
	while(present_floor!=target_floor)//�жϵ����Ƿ��ڰ�ťλ��
	{
		Step_Motor_Loop(3, target_floor == 1 ? DOWN : UP, 1500,25);
		Signal_Deal();
	}
	Delay(10000000);

	Step_Motor_Loop(present_floor, UP, 30000, 160);

	USART3SendByte('0'); //����ͷ��ʼ���
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
	while(Step_Motor_Stop_Flag)		//��⵽�綯��
	{
		//USART3SendByte('6');
		Step_Motor_Stop_Flag = 0;
		Step_Motor_Off(1);
		//	Beep_ON();//����
		Delay(40000000);//һ��ʱ����ٴμ��
		//	Beep_OFF();
	}
	
	USART3SendByte('1');//�ر�����ͷ

	Step_Motor_Loop(present_floor, DOWN, 30000, 160);
	Delay (10000000);
	
	while(present_floor==target_floor)//�жϵ����Ƿ񵽴�
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