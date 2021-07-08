#include "bluetooth.h"
#include "modbus.h"
#include "topics.h"

u16 usart2_tick;
u16 pre_usart2_tick;
u8 Rx_APP_data[16] = {0};
u8 Rx1_update_flag = 0;
u8 Rx_app_cnt;
u8 mode_choose = 4;

float pause_flag = 1.0f;
//u8 Rx_mode_data[9];
u8 i=0;
int rotate_flag = -1;
extern int gear;
void mode_election(void)
{
  if(Rx1_update_flag)
	{
		switch(Rx_APP_data[0])
		{
			case 'v':
				mode_choose=3;
				pre_usart2_tick = usart2_tick;
				Rx1_update_flag=0;
				break;
			case 'g':
				mode_choose=2;
				Rx1_update_flag=0;
				break;
			case 'k':
				mode_choose=1;
				Rx1_update_flag=0;
				break;
			case 'o':
				mode_choose=4;
				Rx1_update_flag=0;
				break;
			case 'r':
				mode_choose=4;
				Rx1_update_flag=0;
				break;
			case 'a':
				gear = 5;
				Rx1_update_flag=0;
				break;
			case 'b':
				gear = 10;
				Rx1_update_flag=0;
				break;
			case 'c':
				gear = 20;
				Rx1_update_flag=0;
				break;
			case 'D':
				gear = 35;
				Rx1_update_flag=0;
				break;
			case 'e':
				gear = 50;
				Rx1_update_flag=0;
				break;
            case 'z':
                rotate_flag = rotate_flag == 0 ? 1 : 0;
                Rx1_update_flag=0;
            case 'p':
                pause_flag = pause_flag == 0 ? 1 : 0;
                Rx1_update_flag=0;
			default:

				break;
		}
//        if(Rx_mode_data[0] != 'a')
//        {
//            i=8;
//            Rx_mode_data[0]=0;
//        }
//        if(Rx_APP_data[0] != Rx_mode_data[i])
//        {
//            i = i==8 ? 0 : i+1;
//            Rx_mode_data[i] = Rx_APP_data[0];
//        }
	}
}
void USART2_IRQHandler(void) 
{
	usart2_tick++;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		Rx_APP_data[Rx_app_cnt++] = USART_ReceiveData(USART2);
	}
	if(Rx_APP_data[Rx_app_cnt-1]=='.')		// 结束位
	{
		Rx_APP_data[Rx_app_cnt-1] = '\0';	// 把'\r'换成'\0'
		Rx_app_cnt = 0;
		Rx1_update_flag = 1;		// 更新数据
		mode_election();
	}
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);    

}