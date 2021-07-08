#include "stm32f10x.h"
#include "Step_Motor.h"
#include "usart3.h"

s8 USART3_RX_Data = '1';

void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART3 ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	USART_DeInit(USART3);
	/* USART3 ʹ��IO�˿����� */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOA
	
	 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
   NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	 
	/* USART3 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);  //��ʼ��USART3
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);// USART3ʹ��
}

void USART3_IRQHandler(void) 
{

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART3_RX_Data = USART_ReceiveData(USART3);
	}
	if(USART3_RX_Data == 'q')
	{
		motor_start_flag = 1;
	}
	
	if(USART3_RX_Data == '0')
	{
		Step_Motor_Stop_Flag = '1';
	}
	
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);    

}

 /*����һ���ֽ�����*/
 void USART3SendByte(unsigned char SendData)
{	   
        USART_SendData(USART3,SendData);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char USART3GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
        {  return 0;//û���յ����� 
		}
        *GetData = USART_ReceiveData(USART3); 
        return 1;//�յ�����
}
void UARTTest(void)
{
       unsigned char i = 0;

       while(1)
       {    
		 while(USART3GetByte(&i))
        {
         USART_SendData(USART3,i);
        }      
       }     
}