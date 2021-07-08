#include "modbus.h"
#include "RS485.h"
#include "CRC.h"
#include "millisecondtimer.h"

u16 pole = 2;//�������
u16 reduction_ratio = 45;//���ٱ�

u8 RS485_TX_BUFF[20];//���ͻ�����
u8 RS485_TX_BUFF_2[20];//���ͻ�����
u8 RS485_TX_CNT=0;//���͵����ݳ���
u8 RS485_TxFlag=0;//����һ֡�������

u8 RS485_RX_BUFF[200];//���ջ�����
u8 RS485_RX_CNT=0;//���յ������ݳ���
u8 RS485_RxFlag=0;//����һ֡�������

u8 raw_receive_data[64] = { 0 };

tx_data_frame_t Tx_data_frame1 = { 0 };
tx_data_frame_t Tx_data_frame2 = { 0 };
rx_data_frame_t Rx_data_frame1 = { 0 };
rx_data_frame_t Rx_data_frame2 = { 0 };
err_frame_t _err_frame = { 0 };
realtime_status_t _realtime_status = { 0 };
extern u32 schedule_tick;
u8 modbus_init_finish = 0;

void SendCommand(u8 addr,u8 command, u16 srart_register, s16 value)
{
	u16 calCRC;

	RS485_TX_BUFF[0] = addr;
    RS485_TX_BUFF[1] = command;
    RS485_TX_BUFF[2] = HI(srart_register);
    RS485_TX_BUFF[3] = LOW(srart_register); 
    RS485_TX_BUFF[4] = HI(value);
    RS485_TX_BUFF[5] = LOW(value);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_Send_Data(RS485_TX_BUFF,8);
}

void SendCommand_2(u8 addr,u8 command, u16 srart_register, s16 value)
{
	u16 calCRC;

	RS485_TX_BUFF_2[0] = addr;
    RS485_TX_BUFF_2[1] = command;
    RS485_TX_BUFF_2[2] = HI(srart_register);
    RS485_TX_BUFF_2[3] = LOW(srart_register); 
    RS485_TX_BUFF_2[4] = HI(value);
    RS485_TX_BUFF_2[5] = LOW(value);
    calCRC=CRC_Compute(RS485_TX_BUFF_2,6);
	RS485_TX_BUFF_2[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF_2[7]=(calCRC)&0xFF;
	RS485_Send_Data_2(RS485_TX_BUFF_2,8);
}

void ReadEncoder(u8 addr,u8 command, u16 srart_register, s16 value)
{
    u16 calCRC;

	RS485_TX_BUFF[0] = addr;
    RS485_TX_BUFF[1] = command;
    RS485_TX_BUFF[2] = HI(srart_register);
    RS485_TX_BUFF[3] = LOW(srart_register); 
    RS485_TX_BUFF[4] = HI(value);
    RS485_TX_BUFF[5] = LOW(value);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_Send_Data(RS485_TX_BUFF,8);
    
}

void ReadEncoder2(u8 addr,u8 command, u16 srart_register, s16 value)
{
	u16 calCRC;

	RS485_TX_BUFF_2[0] = addr;
    RS485_TX_BUFF_2[1] = command;
    RS485_TX_BUFF_2[2] = HI(srart_register);
    RS485_TX_BUFF_2[3] = LOW(srart_register); 
    RS485_TX_BUFF_2[4] = HI(value);
    RS485_TX_BUFF_2[5] = LOW(value);
    calCRC=CRC_Compute(RS485_TX_BUFF_2,6);
	RS485_TX_BUFF_2[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF_2[7]=(calCRC)&0xFF;
	RS485_Send_Data_2(RS485_TX_BUFF_2,8);
}

void Stop(u8 addr, stop_t mode)
{
	SendCommand(addr,0x06,0x0040,mode);
	SendCommand(addr,0x06,0x0040,mode);
}

u16 left;
u16 right;
void SetRPM(u8 addr, float rpm)
{
	s16 steering_freq;

	if(addr == 0x01)
	{
		steering_freq = (s16)(rpm / 0.02222222f);
		SendCommand(addr,0x06,0x0043,steering_freq);
		left++;
	}
	else if (addr == 0x02)
	{
		steering_freq = (s16)(rpm / 0.02222222f);
		SendCommand_2(addr,0x06,0x0043,-steering_freq);
		right++;
	}
}


//void Set_RPM(u8 addr, float rpm1, float rpm2)
//{
//	s16 steering_freq1;
//	s16 steering_freq2;
//	
//	u16 calCRC;

//	RS485_TX_BUFF[0] = addr;
//    RS485_TX_BUFF[1] = command;
//    RS485_TX_BUFF[2] = HI(srart_register);
//    RS485_TX_BUFF[3] = LOW(srart_register); 
//    RS485_TX_BUFF[4] = HI(value);
//    RS485_TX_BUFF[5] = LOW(value);
//    calCRC=CRC_Compute(RS485_TX_BUFF,6);
//	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
//    RS485_TX_BUFF[7]=(calCRC)&0xFF;
//	RS485_Send_Data(RS485_TX_BUFF,8);

//	if(addr == 0x01)
//	{
//		steering_freq1 = (s16)(rpm1 / 0.02222222f);
//		SendCommand(addr,0x06,0x0043,steering_freq);
//	}
//	else if (addr == 0x02)
//	{
//		steering_freq2 = (s16)(rpm2 / 0.02222222f);
//		SendCommand(addr,0x06,0x0043,-steering_freq);
//	}
//}
void SetAcceleration(u8 addr, acceleration_t mode, float acc)
{
	u16 acceleration;
	u16 calCRC;
	u16 srart_register;
	acceleration = (u16)acc * 10;

	if(mode == up)
	{
		srart_register = 0x0052;
	}
	else if(mode == down)
	{
		srart_register = 0x0053;
	}
	if(addr == 0x01)
	{
		RS485_TX_BUFF[0] = addr;
		RS485_TX_BUFF[1] = 0x06;
		RS485_TX_BUFF[2] = HI(srart_register);
		RS485_TX_BUFF[3] = LOW(srart_register); 
		RS485_TX_BUFF[4] = HI(acceleration);
		RS485_TX_BUFF[5] = LOW(acceleration);
		calCRC=CRC_Compute(RS485_TX_BUFF,6);
		RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
		RS485_TX_BUFF[7]=(calCRC)&0xFF;
		RS485_Send_Data(RS485_TX_BUFF,8);
	}
	else if(addr == 0x02)
	{
		RS485_TX_BUFF[0] = addr;
		RS485_TX_BUFF[1] = 0x06;
		RS485_TX_BUFF[2] = HI(srart_register);
		RS485_TX_BUFF[3] = LOW(srart_register); 
		RS485_TX_BUFF[4] = HI(acceleration);
		RS485_TX_BUFF[5] = LOW(acceleration);
		calCRC=CRC_Compute(RS485_TX_BUFF,6);
		RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
		RS485_TX_BUFF[7]=(calCRC)&0xFF;
		RS485_Send_Data_2(RS485_TX_BUFF,8);
	}

	
}


//void SetPID(u8 addr, float p, float i, float d)
//{
//	SendCommand(addr,0x06,0x00c0,(s16)(p << 16));
//	delay(100);
//	SendCommand(addr,0x06,0x0043,i);
//	delay(100);
//	SendCommand(addr,0x06,0x0043,d);
//	delay(100);
//}


//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
u16 right_TX_tick;
u16 right_RX_tick;
u8 ttt,rrr;
void ModbusRtuReceive()
{
	u16 calCRC;
	u16 recCRC;
	u8 rxlen=RS485_RX_CNT - 1;
	u8 addr = RS485_RX_BUFF[0];
	u8 command = RS485_RX_BUFF[1];
	recCRC = RS485_RX_BUFF[rxlen - 1] | (((u16)RS485_RX_BUFF[rxlen - 2]) << 8);

	if(rxlen == 5)//�쳣��֡
		{
			if(addr == 0x01 || addr == 0x02)
			{
				calCRC = CRC_Compute(RS485_RX_BUFF,rxlen - 2);//�������������ݵ�CRC
				if(calCRC == recCRC)
				{
					_err_frame.addr = addr;
					_err_frame.crc[0] = RS485_RX_BUFF[rxlen - 2];
					_err_frame.crc[1] = RS485_RX_BUFF[rxlen - 1];
					_err_frame.command = command;
					_err_frame.err = RS485_RX_BUFF[2];
				}
			}
		}
	if(addr == 0x01)//����1����
	{
		if(command == 0x06)//����ָ���
		{
			calCRC = CRC_Compute(RS485_RX_BUFF,rxlen - 2);//�������������ݵ�CRC
			if(calCRC == recCRC)
			{
				right_TX_tick++;
				Tx_data_frame1.err = 0;
				Tx_data_frame1.len = rxlen;
				Tx_data_frame1.addr = addr;
				Tx_data_frame1.command = command;
				Tx_data_frame1.first_register[0] = RS485_RX_BUFF[2];
				Tx_data_frame1.first_register[1] = RS485_RX_BUFF[3];
				Tx_data_frame1.value = RS485_RX_BUFF[5] | (((u16)RS485_RX_BUFF[4]) << 8);
				Tx_data_frame1.crc[0] = RS485_RX_BUFF[rxlen - 2];
				Tx_data_frame1.crc[1] = RS485_RX_BUFF[rxlen - 1];
			}
		}
		else if(command == 0x03)//����ָ���
		{
			calCRC = CRC_Compute(RS485_RX_BUFF,rxlen - 2);//�������������ݵ�CRC
			if(calCRC == recCRC)
			{
				right_RX_tick++;
				Rx_data_frame1.err = 0;
				Rx_data_frame1.len = rxlen;
				Rx_data_frame1.addr = addr;
				Rx_data_frame1.command = command;
				Rx_data_frame1.total_bytes = RS485_RX_BUFF[2];
				Rx_data_frame1.rpm = RS485_RX_BUFF[4] | (((u16)RS485_RX_BUFF[3]) << 8);
				Rx_data_frame1.crc[0] = RS485_RX_BUFF[rxlen - 2];
				Rx_data_frame1.crc[1] = RS485_RX_BUFF[rxlen - 1];
			}
		}
	}
	else if(addr == 0x02)//����2����
	{
		if(command == 0x06)//����ָ���
		{
			calCRC = CRC_Compute(RS485_RX_BUFF,rxlen - 2);//�������������ݵ�CRC
			if(calCRC == recCRC)
			{
				right_TX_tick++;
				Tx_data_frame2.err = 0;
				Tx_data_frame2.len = rxlen;
				Tx_data_frame2.addr = addr;
				Tx_data_frame2.command = command;
				Tx_data_frame2.first_register[0] = RS485_RX_BUFF[2];
				Tx_data_frame2.first_register[1] = RS485_RX_BUFF[3];
				Tx_data_frame2.value = RS485_RX_BUFF[5] | (((u16)RS485_RX_BUFF[4]) << 8);
				Tx_data_frame2.crc[0] = RS485_RX_BUFF[rxlen - 2];
				Tx_data_frame2.crc[1] = RS485_RX_BUFF[rxlen - 1];
			}
		}
		else if(command == 0x03)//����ָ���
		{
			calCRC = CRC_Compute(RS485_RX_BUFF,rxlen - 2);//�������������ݵ�CRC
			if(calCRC == recCRC)
			{
				right_RX_tick++;
				Rx_data_frame2.err = 0;
				Rx_data_frame2.len = rxlen;
				Rx_data_frame2.addr = addr;
				Rx_data_frame2.command = command;
				Rx_data_frame2.total_bytes = RS485_RX_BUFF[2];
				Rx_data_frame2.rpm = RS485_RX_BUFF[4] | (((u16)RS485_RX_BUFF[3]) << 8);
				Rx_data_frame2.crc[0] = RS485_RX_BUFF[rxlen - 2];
				Rx_data_frame2.crc[1] = RS485_RX_BUFF[rxlen - 1];
			}
		}
	}
	RS485_RX_CNT=0;//����
	RS485_RxFlag=0;//�ֿ��Խ�����
}

void ModbusRtuTransmit(void)
{
	if(schedule_tick == 1)
	{
		SendCommand(0x01,0x06,0x0073,pole);//���õ������Ϊ4
	}
	else if(schedule_tick == 3)
	{
		SendCommand(0x01,0x06,0x0074,reduction_ratio * 10);//��ֵ���� 0.1 Ϊ������ٱ�
	}
	else if(schedule_tick == 5)
	{
		SendCommand(0x02,0x06,0x0073,pole);//���õ������Ϊ4
	}
	else if(schedule_tick == 7)
	{
		SendCommand(0x02,0x06,0x0074,reduction_ratio * 10);//��ֵ���� 0.1 Ϊ������ٱ�
		modbus_init_finish = 1;
		TIM_Cmd(TIM4,DISABLE);
	}
//	if(schedule_tick > 200)
//	{
//		if(schedule_tick % 4 == 0)
//			Stop(0x01,emergency);
//		else if(schedule_tick % 4 == 2)
//			Stop(0x02,emergency);
//	}
//	else if(schedule_tick > 10)
//	{
//		if(schedule_tick % 4 == 0) {//ÿ180msдһ��
//			SetRPM(0x01,55);
//		}
//		else if(schedule_tick % 4 == 1) {//ÿ180msдһ��
//			SetRPM(0x02,65);
//		}
//		else if(schedule_tick % 4 == 2) {//ÿ210ms��һ��
//			SendCommand(0x01,0x03,0x0034,0x0001);//���ٶ�
//		}
//		else if(schedule_tick % 4 == 3) {//ÿ210ms��һ��
//			SendCommand(0x02,0x03,0x0034,0x0001);//���ٶ�
//		}
//	}
//	SendCommand(0x01,0x03,0x0020,0x0017);//������ʵʱ״̬
}

void modbus_rtu(void)
{
	ModbusRtuTransmit();
}


