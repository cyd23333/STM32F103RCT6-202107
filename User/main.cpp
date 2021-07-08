#include <stdio.h>
#include "hardwareserial.h"
#include "gy85.h"
#include "motor.h"
#include "encoder.h"
#include "battery.h"
#include "led.h"
#include "PID.h"
#include "Kinematics.h"
#include <ros.h>
#include <riki_msgs/Velocities.h>
#include <geometry_msgs/Twist.h>
#include <riki_msgs/PID.h>
#include <riki_msgs/Imu.h>
#include <riki_msgs/Battery.h>
#include <geometry_msgs/Vector3.h>
#include <ros/time.h>
#include "interrupt.h"

#include "timer.h"
#include "RS485.h"

#include "modbus.h"
#include "Android.h"
#include "SPI.h"
#include "icm20600.h"
#include "estimator.h"
#include "topics.h"
#include "commander.h"
#include "host.h"
#include <math.h>
#include "control.h"
#include "scheduler.h"
#include "CRC.h"
//���ϵ�ͷ�ļ�������û�õ�������ɾ��������ɾ��֮����ܻ��б��������ø��˾�ûɾ
#include "scheduler.h"
#include "Step_Motor.h"
#include "usart3.h"

#define TOUCH_1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define TOUCH_2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define HALL_1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define HALL_2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)

/*�ɸĽ�֮����
1�����ݰ�ť�����������Բ����жϴ��������жϻص���������б�־λ���޸�
2�����ݻ��漰��һ��ws2812b�ƴ�������ָʾ��������״̬�������������һ��stm32f103c8t����������PWN+DMA����
3��������ƺ���ʱ����
*/


//void UART5_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��UART5��GPIOBʱ��
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��GPIOC/Dʱ��
//    USART_DeInit(UART5);  //��λ����5
//    //USART5_TX   PC12
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC12
//    //USART5_RX	  PD2
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//    GPIO_Init(GPIOD, &GPIO_InitStructure);  //��ʼ��PB11
// 
//    //Usart1 NVIC ����
//    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

//    //USART ��ʼ������
//    USART_InitStructure.USART_BaudRate = 9600;//һ������Ϊ;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//    USART_Init(UART5, &USART_InitStructure); //��ʼ������
// 
//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
//    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���
//}
//void UART5_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
//  	for(t=0;t<len;t++)
//	{
//	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET){};	
//		USART_SendData(UART5,buf[t]); //��������
//	}
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
//}
//void USART2_IRQHandler(void) 
//{
//	usart2_tick++;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//		Rx_APP_data[Rx_app_cnt++] = USART_ReceiveData(USART2);
//	}
//	if(Rx_APP_data[Rx_app_cnt-1]=='.')		// ����λ
//	{
//		Rx_APP_data[Rx_app_cnt-1] = '\0';	// ��'\r'����'\0'
//		Rx_app_cnt = 0;
//		Rx1_update_flag = 1;		// ��������
//		mode_election();
//	}
//	USART_ClearITPendingBit(USART2, USART_IT_RXNE);    

//}



//�ⲿ�ж�0�������
//void KEY_Init(void) //IO��ʼ��
//{ 
// 	GPIO_InitTypeDef GPIO_InitStructure;
//	//��ʼ��KEY1-->PA0��������
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PAʱ��

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PA0
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���ó���������
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��PA


//}
//void EXTIX_Init(void)
//{
// 
// 	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

//    KEY_Init();	 //	�����˿ڳ�ʼ��

//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

////  //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
////  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

////  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
////  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
////  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
////  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
////  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//   //PA0	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
// 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
//		EXTI_InitStructure.EXTI_Line=EXTI_Line11;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
// 	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure); 
//		NVIC_EnableIRQ(EXTI0_IRQn);


// 
//}
//void EXTI0_IRQHandler(void)
//{
//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//	USART_SendData(USART3,'1');
//	EXTI_ClearITPendingBit(EXTI_Line11); 
//}


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 	
//=============================================================================
//LED -> PA8
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
}

void Touch_Sensor1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void Touch_Sensor2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void Hall_Sensor1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void Hall_Sensor2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
u8 k=0;

int main(void)
{

	SystemInit();//ϵͳ��ʼ��
	initialise();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Step_Motor_Conf();//�����ʼ��
	LED_Init();
	
	Touch_Sensor1_Init();//�������س�ʼ��
	Touch_Sensor2_Init();
	
	Hall_Sensor1_Init();//���ݻ�����������ʼ��
	Hall_Sensor1_Init();
	
//	EXTIX_Init();
	USART3_Config();//ͨ�Ŵ�������

	

	
	while(1){
//		tttt = time();
		
		
	_schedule();
//		
//	if(TOUCH_1)Step_Motor_Loop(3, DOWN, 1500,25);
//	if(TOUCH_2)Step_Motor_Loop(3, UP, 1500,25);			
		
		
//UARTTest();
		
		
//	if(TOUCH_1&&HALL_1)
//	{
////		Step_Motor_Loop(1, UP, 30000, 160);
////		Step_Motor_Loop(1, DOWN, 30000, 160);
//		Step_Motor_Loop(3, DOWN, 1500,25);
//	}
//	if(TOUCH_2&&HALL_2)
//	{
////		Step_Motor_Loop(2, UP, 30000, 160);
////		Step_Motor_Loop(2, DOWN, 30000, 160);
//		Step_Motor_Loop(3, UP, 1500,25);
//	}		
	

		
		
//		k = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10);
//		if(k)
//		{
//			
//		}
//		else
//		{
//			GPIO_SetBits(GPIOA,GPIO_Pin_8);
//		}
		
  }

}

