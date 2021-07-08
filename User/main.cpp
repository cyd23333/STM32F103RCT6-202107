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
//以上的头文件基本都没用到，可以删掉，但是删掉之后可能会有报错，我懒得改了就没删
#include "scheduler.h"
#include "Step_Motor.h"
#include "usart3.h"

#define TOUCH_1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define TOUCH_2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define HALL_1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define HALL_2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)

/*可改进之处：
1、电梯按钮、传感器可以采用中断触发，在中断回调函数里进行标志位的修改
2、电梯还涉及到一个ws2812b灯带，用来指示电梯运行状态，这个可以用另一块stm32f103c8t处理，或是用PWN+DMA控制
3、电机控制和延时函数
*/


//void UART5_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能UART5，GPIOB时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//使能GPIOC/D时钟
//    USART_DeInit(UART5);  //复位串口5
//    //USART5_TX   PC12
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PC12
//    //USART5_RX	  PD2
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//上拉输入
//    GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化PB11
// 
//    //Usart1 NVIC 配置
//    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

//    //USART 初始化设置
//    USART_InitStructure.USART_BaudRate = 9600;//一般设置为;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//    USART_Init(UART5, &USART_InitStructure); //初始化串口
// 
//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
//    USART_Cmd(UART5, ENABLE);                    //使能串口
//}
//void UART5_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
//  	for(t=0;t<len;t++)
//	{
//	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET){};	
//		USART_SendData(UART5,buf[t]); //发送数据
//	}
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //等待发送结束		
//}
//void USART2_IRQHandler(void) 
//{
//	usart2_tick++;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//		Rx_APP_data[Rx_app_cnt++] = USART_ReceiveData(USART2);
//	}
//	if(Rx_APP_data[Rx_app_cnt-1]=='.')		// 结束位
//	{
//		Rx_APP_data[Rx_app_cnt-1] = '\0';	// 把'\r'换成'\0'
//		Rx_app_cnt = 0;
//		Rx1_update_flag = 1;		// 更新数据
//		mode_election();
//	}
//	USART_ClearITPendingBit(USART2, USART_IT_RXNE);    

//}



//外部中断0服务程序
//void KEY_Init(void) //IO初始化
//{ 
// 	GPIO_InitTypeDef GPIO_InitStructure;
//	//初始化KEY1-->PA0上拉输入
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PA时钟

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;//PA0
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化PA


//}
//void EXTIX_Init(void)
//{
// 
// 	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

//    KEY_Init();	 //	按键端口初始化

//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

////  //GPIOE.2 中断线以及中断初始化配置   下降沿触发
////  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

////  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
////  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
////  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
////  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
////  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//   //PA0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
// 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
//		EXTI_InitStructure.EXTI_Line=EXTI_Line11;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
// 	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
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

	SystemInit();//系统初始化
	initialise();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Step_Motor_Conf();//电机初始化
	LED_Init();
	
	Touch_Sensor1_Init();//触摸开关初始化
	Touch_Sensor2_Init();
	
	Hall_Sensor1_Init();//电梯霍尔传感器初始化
	Hall_Sensor1_Init();
	
//	EXTIX_Init();
	USART3_Config();//通信串口设置

	

	
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

