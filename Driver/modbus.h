#pragma once

#ifdef __cplusplus

extern "C" {
#endif

#include "stm32f10x.h"
#define HI(n) ((n)>>8)
#define LOW(n) ((n)&0xff)
	

typedef struct {

	u8 addr;
	u8 command;
	u8 first_register[2];
	u16 value;
	u8 crc[2];
	u8 len;
	u8 err;

} tx_data_frame_t;

typedef struct {

	u8 index;
	u8 addr;
	u8 command;
	u8 total_bytes;
	u16 rpm;
    s32 pos;
	u8 crc[2];
	u8 len;
	u8 err;

} rx_data_frame_t;

typedef struct {
	u8 addr;
	u8 command;
	u8 crc[2];
	u8 err; //0:正常
			//0x01:非法功能码
			//0x02:非法数据地址
			//0x03:非法数据值
			//0x04:从站设备故障
			//0x05:请求已被确认，但需要较长时间来处理请求
			//0x06:从设备忙
			//0x08:存储奇偶性差错
			//0x0A:不可用的网关
			//0x0B:网关目标设备响应失败
			//0x40:禁止操作
			//0x60:尚未学习电机相序
			//0xFF:未定义错误
} err_frame_t;

typedef struct {
	u16 plugging;
	u16 pwm;
	float current;
	float speed;
	float voltage1;
	float voltage2;
	float voltage3;
}realtime_status_t;

typedef enum {
	normal = 0,
	emergency = 1,
	_free = 2
}stop_t;

typedef enum {
	up = 0,
	down = 1,
}acceleration_t;

extern rx_data_frame_t Rx_data_frame1;
extern rx_data_frame_t Rx_data_frame2;
extern err_frame_t _err_frame;
extern realtime_status_t _realtime_status;
extern stop_t _stop;
extern acceleration_t _acceleration;
extern void modbus_rtu(void);
extern void SendCommand(u8 addr,u8 command, u16 srart_register, s16 value);
extern void SendCommand_2(u8 addr,u8 command, u16 srart_register, s16 value);
void SetAcceleration(u8 addr, acceleration_t mode, float acc);
extern void ModbusRtuReceive();
extern void Stop(u8 addr, stop_t mode);
extern void SetRPM(u8 addr, float rpm);


#ifdef __cplusplus
}
#endif