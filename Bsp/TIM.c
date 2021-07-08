#include "timer.h"
#include "scheduler.h"
#include "modbus.h"

extern u8 RS485_RxFlag;//接收一帧结束标记

void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能	
	TIM_TimeBaseStructure.TIM_Period = 50 - 1; //计数到50为5ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; //10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update|TIM_IT_Trigger, ENABLE);

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void timer7_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7时钟使能 

	//TIM7初始化设置
	TIM_TimeBaseStructure.TIM_Period = 30-1;//4ms进中断，判断为一帧
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置计数频率为10kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE );//TIM7允许更新中断
	
//	TIM_Cmd(TIM7, ENABLE);  //使能TIMx外设
	//TIM7中断分组配置
	NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}




//int tick4;
//void TIM4_IRQHandler(void)   //TIM4中断
//{
//	tick4++;
//	
//	if (TIM_GetITStatus(TIM4,TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		modbus_rtu();
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 ;
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////

//用定时器7判断接收空闲时间,当空闲时间大于指定时间，认为一帧结束
//定时器7中断服务程序       
int tick7;
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		tick7++;
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除中断标志
		TIM_Cmd(TIM7,DISABLE);//停止定时器
		GPIO_SetBits(GPIOD,GPIO_Pin_7);//切换为发送状态
		ModbusRtuReceive();
	}
}