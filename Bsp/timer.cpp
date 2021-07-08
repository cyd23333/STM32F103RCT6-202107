#include "timer.h"
#include "scheduler.h"

void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��	
	TIM_TimeBaseStructure.TIM_Period = 200; //������200Ϊ20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; //10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM4, TIM_IT_Update|TIM_IT_Trigger, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
}

void timer7_init(void)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7ʱ��ʹ�� 

//	//TIM7��ʼ������
//	TIM_TimeBaseStructure.TIM_Period = 40;//4ms���жϣ��ж�Ϊһ֡
//	TIM_TimeBaseStructure.TIM_Prescaler =7199; //���ü���Ƶ��Ϊ10kHz
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
//	
//	//TIM7�жϷ�������
//	NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE );//TIM7��������ж�



	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* PE5/PE6 */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	/// Step 1 : Open TIM6&TIM7 Clock
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

//	/// Step 2 : Set basic settings
//	// Input CLOCK = 72MHz
//	TIM_TimeBaseStructure.TIM_Period = 10000 - 1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

//	TIM_ARRPreloadConfig(TIM6, ENABLE);

//	TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Repetitive);

//	TIM_UpdateRequestConfig(TIM6, TIM_UpdateSource_Global);

//	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

//	TIM_Cmd(TIM6, ENABLE);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;               //2k�ļ���Ƶ��
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_Period = 500-1;                  // ������500����һ�������¼�
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);         //��������ж�
TIM_Cmd(TIM3, ENABLE); //ʹ��TIM3
}




int tick4;
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	tick4++;
	
	if (TIM_GetITStatus(TIM4,TIM_IT_Update) == SET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ ;
		scheduler();
	}
}

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
//		GPIO_SetBits(GPIOD,GPIO_Pin_7);//�л�Ϊ����״̬
//		RS485_RxFlag=1;//��λ֡�������
//		errpace=1;
	}
}
int tick3;
void TIM3_IRQHandler(void)
{
	tick3++;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    }
}