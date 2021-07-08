#include "timer.h"
#include "scheduler.h"
#include "modbus.h"

extern u8 RS485_RxFlag;//����һ֡�������

void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��	
	TIM_TimeBaseStructure.TIM_Period = 50 - 1; //������50Ϊ5ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; //10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update|TIM_IT_Trigger, ENABLE);

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void timer7_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7ʱ��ʹ�� 

	//TIM7��ʼ������
	TIM_TimeBaseStructure.TIM_Period = 30-1;//4ms���жϣ��ж�Ϊһ֡
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //���ü���Ƶ��Ϊ10kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE );//TIM7��������ж�
	
//	TIM_Cmd(TIM7, ENABLE);  //ʹ��TIMx����
	//TIM7�жϷ�������
	NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}




//int tick4;
//void TIM4_IRQHandler(void)   //TIM4�ж�
//{
//	tick4++;
//	
//	if (TIM_GetITStatus(TIM4,TIM_IT_Update) == SET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//	{
//		modbus_rtu();
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ ;
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////

//�ö�ʱ��7�жϽ��տ���ʱ��,������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
//��ʱ��7�жϷ������       
int tick7;
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		tick7++;
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//����жϱ�־
		TIM_Cmd(TIM7,DISABLE);//ֹͣ��ʱ��
		GPIO_SetBits(GPIOD,GPIO_Pin_7);//�л�Ϊ����״̬
		ModbusRtuReceive();
	}
}