#include "stm32f10x.h"
#include "Step_Motor.h"
#include "usart3.h"

s8 USART3_RX_Data = '1';

void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART3 时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	USART_DeInit(USART3);
	/* USART3 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA
	
	 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
   NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 
	/* USART3 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART3, &USART_InitStructure);  //初始化USART3
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);// USART3使能
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

 /*发送一个字节数据*/
 void USART3SendByte(unsigned char SendData)
{	   
        USART_SendData(USART3,SendData);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	    
}  

/*接收一个字节数据*/
unsigned char USART3GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
        {  return 0;//没有收到数据 
		}
        *GetData = USART_ReceiveData(USART3); 
        return 1;//收到数据
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