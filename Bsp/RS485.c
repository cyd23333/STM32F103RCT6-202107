//#include "RS485.h"
//#include "timer.h"
//#include "modbus.h"

//extern u8 RS485_TX_CNT;//发送的数据长度
//extern u8 RS485_RX_CNT;//接收到的数据长度
//extern u8 RS485_RX_BUFF[64];
//extern u8 RS485_RxFlag;
//extern rx_data_frame_t Rx_data_frame1;
//extern rx_data_frame_t Rx_data_frame2;
//u8 err;
//u8 addr = 0;
//u8 addr_2 = 0;
//u8 _rpm[2] = { 0 };
//u8 _pos[4] = { 0 };
//u8 _crc[2] = { 0 };
//u8 _rpm_2[2] = { 0 };
//u8 _pos_2[4] = { 0 };
//u8 _crc_2[2] = { 0 };
//RxStruct_t RxStruct_data = { 0 };
//RxStruct_t RxStruct_data_2 = { 0 };
//void RxStruct_init(RxStruct_t* RxStruct, uint8_t *pool, uint16_t size)
//{
///* 初始化头 */
//RxStruct->index = 0;
///* 初始化缓冲区 */
//RxStruct->buffer_ptr = pool;
//RxStruct->buffer_size = size;
//}

////RS485发送len个字节.
////buf:发送区首地址
////len:发送的字节数
//void RS485_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
//  	for(t=0;t<len;t++)
//	{
//	    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET){};	
//		USART_SendData(USART3,buf[t]); //发送数据
//	}
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //等待发送结束		
//	RS485_RX_CNT=0;
//}

//void RS485_Send_Data_2(u8 *buf,u8 len)
//{
//	u8 t;
//  	for(t=0;t<len;t++)
//	{
//	    while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){};	
//		USART_SendData(UART4,buf[t]); //发送数据
//	}
//	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET); //等待发送结束		
//	RS485_RX_CNT=0;
//}


//void RS485_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef  NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	//初始化使能485芯片管脚
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//	GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 

//	USART_InitStructure.USART_BaudRate	 = 9600;				
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	
//	USART_InitStructure.USART_StopBits 	 = USART_StopBits_1;	
//	USART_InitStructure.USART_Parity	 = USART_Parity_No;		
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//	USART_Init(UART4, &USART_InitStructure); 				

//	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;       
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
//	NVIC_Init(&NVIC_InitStructure);						   
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

//	USART_Cmd(UART4, ENABLE);

//}

//u32 usart3_tick = 0;
//void USART3_IRQHandler(void)
//{
//	u8 res;
//	usart3_tick++;
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
//	{
////		if(USART_GetFlagStatus(USART3,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) {err=1;}//检测到噪音、帧错误或校验错误
////            else err=0;
//		if(err == 0)
//		{
//			res =USART_ReceiveData(USART3);//;读取接收到的数据
//			switch(RxStruct_data.index)
//			{
//			case 0://地址
//				if((res == 0x01 || res == 0x02))
//				{
//					addr = res;
//					RxStruct_data.index++;
//				}
//				break;
//			case 1://读
//				if(res == 0x03 )
//					RxStruct_data.index++;
//				else
//				{
//					RxStruct_data.index = 0;
//				}

//				break;
//			case 2://数目
//				if(res == 0x02 ) {
//					RxStruct_data.index++;
//					if(addr == 0x01)
//						Rx_data_frame1.total_bytes = res;
//					else if(addr == 0x02)
//						Rx_data_frame2.total_bytes = res;
//						
//				}
//				else
//				{
//					RxStruct_data.index = 0;
//					
//				}
//				
//				break;
//			case 3:
//					if(addr == 0x01)
//						Rx_data_frame1.addr = addr;
//					else if(addr == 0x02)
//						Rx_data_frame2.addr = addr;
//					if(addr == 0x01)
//						Rx_data_frame1.command = 0x03;
//					else if(addr == 0x02)
//						Rx_data_frame2.command = 0x03;
//					
//					_pos[0] = res;
//					RxStruct_data.index++;
//				break;
//            case 4:
//					if(addr == 0x01)
//						Rx_data_frame1.addr = addr;
//					else if(addr == 0x02)
//						Rx_data_frame2.addr = addr;
//					if(addr == 0x01)
//						Rx_data_frame1.command = 0x03;
//					else if(addr == 0x02)
//						Rx_data_frame2.command = 0x03;
//					
//					_pos[1] = res;
//					RxStruct_data.index++;
//				break;
//            case 5:
//					if(addr == 0x01)
//						Rx_data_frame1.addr = addr;
//					else if(addr == 0x02)
//						Rx_data_frame2.addr = addr;
//					if(addr == 0x01)
//						Rx_data_frame1.command = 0x03;
//					else if(addr == 0x02)
//						Rx_data_frame2.command = 0x03;
//					
//					_pos[2] = res;
//					RxStruct_data.index++;
//				break;
//			case 6://值
//					_pos[3] = res;
//					if(addr == 0x01)
//						Rx_data_frame1.pos = (s32)((u32)_pos[3] | (((u32)_pos[2]) << 8)| (((u32)_pos[1]) << 16)| (((u32)_pos[0]) << 24));
//					else if(addr == 0x02)
//						Rx_data_frame2.pos = (s32)((u32)_pos[3] | (((u32)_pos[2]) << 8)| (((u32)_pos[1]) << 16)| (((u32)_pos[0]) << 24));
//					RxStruct_data.index++;
//				break;
//			case 7:
//					_crc[0] = res;
//					RxStruct_data.index++;
//				break;
//			case 8:
//					_crc[1] = res;
//					RxStruct_data.index = 0;
//				break;
//			default:break;
//			}

//		}
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}
//u32 usart4_tick = 0;
////void USART4_IRQHandler(void)
////{
////	u8 res;
////	usart4_tick++;
////	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
////	{
//////		if(USART_GetFlagStatus(UART4,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) {err=1;}//检测到噪音、帧错误或校验错误
//////            else err=0;
////		if(err == 0)
////		{
////			res =USART_ReceiveData(UART4);//;读取接收到的数据
////			switch(RxStruct_data.index)
////			{
////			case 0://地址
////				if((res == 0x01 || res == 0x02))
////				{
////					addr = res;
////					RxStruct_data.index++;
////				}
////				break;
////			case 1://读
////				if(res == 0x03 )
////					RxStruct_data.index++;
////				else
////				{
////					RxStruct_data.index = 0;
////				}

////				break;
////			case 2://数目
////				if(res == 0x04 ) {
////					RxStruct_data.index++;
////					if(addr == 0x01)
////						Rx_data_frame1.total_bytes = res;
////					else if(addr == 0x02)
////						Rx_data_frame2.total_bytes = res;
////						
////				}
////				else
////				{
////					RxStruct_data.index = 0;
////					
////				}
////				
////				break;
////			case 3:
////					if(addr == 0x01)
////						Rx_data_frame1.addr = addr;
////					else if(addr == 0x02)
////						Rx_data_frame2.addr = addr;
////					if(addr == 0x01)
////						Rx_data_frame1.command = 0x03;
////					else if(addr == 0x02)
////						Rx_data_frame2.command = 0x03;
////					
////					_pos[0] = res;
////					RxStruct_data.index++;
////				break;
////            case 4:
////					if(addr == 0x01)
////						Rx_data_frame1.addr = addr;
////					else if(addr == 0x02)
////						Rx_data_frame2.addr = addr;
////					if(addr == 0x01)
////						Rx_data_frame1.command = 0x03;
////					else if(addr == 0x02)
////						Rx_data_frame2.command = 0x03;
////					
////					_pos[1] = res;
////					RxStruct_data.index++;
////				break;
////            case 5:
////					if(addr == 0x01)
////						Rx_data_frame1.addr = addr;
////					else if(addr == 0x02)
////						Rx_data_frame2.addr = addr;
////					if(addr == 0x01)
////						Rx_data_frame1.command = 0x03;
////					else if(addr == 0x02)
////						Rx_data_frame2.command = 0x03;
////					
////					_pos[2] = res;
////					RxStruct_data.index++;
////				break;
////			case 6://值
////					_pos[3] = res;
////					if(addr == 0x01)
////						Rx_data_frame1.pos = (s32)((u32)_pos[3] | (((u32)_pos[2]) << 8)| (((u32)_pos[1]) << 16)| (((u32)_pos[0]) << 24));
////					else if(addr == 0x02)
////						Rx_data_frame2.pos = (s32)((u32)_pos[3] | (((u32)_pos[2]) << 8)| (((u32)_pos[1]) << 16)| (((u32)_pos[0]) << 24));
////					RxStruct_data.index++;
////				break;
////			case 7:
////					_crc[0] = res;
////					RxStruct_data.index++;
////				break;
////			case 8:
////					_crc[1] = res;
////					RxStruct_data.index = 0;
////				break;
////			default:break;
////			}

////		}
////		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
////	}
////}
//u32 uart4_tick = 0;
//u8 ressss;
//void UART4_IRQHandler(void)
//{
//	u8 res;
//	uart4_tick++;
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
//	{
////		if(USART_GetFlagStatus(UART4,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) {err=1;}//检测到噪音、帧错误或校验错误
////            else err=0;
//		if(err == 0)
//		{
//			res =USART_ReceiveData(UART4);//;读取接收到的数据
//			ressss = res;
//			switch(RxStruct_data_2.index)
//			{
//			case 0://地址
//				if((res == 0x01 || res == 0x02))
//				{
//					addr_2 = res;
//					RxStruct_data_2.index++;
//				}
//				break;
//			case 1://读
//				if(res == 0x03 )
//					RxStruct_data_2.index++;
//				else
//				{
//					RxStruct_data_2.index = 0;
//				}

//				break;
//			case 2://数目
//				if(res == 0x02 ) {
//					RxStruct_data_2.index++;
//					if(addr_2 == 0x01)
//						Rx_data_frame1.total_bytes = res;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.total_bytes = res;
//						
//				}
//				else
//				{
//					RxStruct_data_2.index = 0;
//					
//				}
//				
//				break;
//            case 3:
//					if(addr_2 == 0x01)
//						Rx_data_frame1.addr = addr_2;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.addr = addr_2;
//					if(addr_2 == 0x01)
//						Rx_data_frame1.command = 0x03;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.command = 0x03;
//					
//					_pos_2[0] = res;
//					RxStruct_data_2.index++;
//				break;
//            case 4:
//					if(addr_2 == 0x01)
//						Rx_data_frame1.addr = addr_2;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.addr = addr_2;
//					if(addr_2 == 0x01)
//						Rx_data_frame1.command = 0x03;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.command = 0x03;
//					
//					_pos_2[1] = res;
//					RxStruct_data_2.index++;
//				break;
//            case 5:
//					if(addr_2 == 0x01)
//						Rx_data_frame1.addr = addr_2;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.addr = addr_2;
//					if(addr_2 == 0x01)
//						Rx_data_frame1.command = 0x03;
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.command = 0x03;
//					
//					_pos_2[2] = res;
//					RxStruct_data_2.index++;
//				break;
//			case 6://值
//					_pos_2[3] = res;
//					if(addr_2 == 0x01)
//						Rx_data_frame1.pos = (s32)((u32)_pos_2[3] | (((u32)_pos_2[2]) << 8)| (((u32)_pos_2[1]) << 16)| (((u32)_pos_2[0]) << 24));
//					else if(addr_2 == 0x02)
//						Rx_data_frame2.pos = (s32)((u32)_pos_2[3] | (((u32)_pos_2[2]) << 8)| (((u32)_pos_2[1]) << 16)| (((u32)_pos_2[0]) << 24));
//					RxStruct_data_2.index++;
//				break;
//			case 7:
//					_crc_2[0] = res;
//					RxStruct_data_2.index++;
//				break;
//			case 8:
//					_crc_2[1] = res;
//					RxStruct_data_2.index = 0;
//				break;
//			default:break;
//			}

//		}
//		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
//	}
//}