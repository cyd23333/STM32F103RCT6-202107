#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define Err_Max PI/180*50                   //角度误差限幅70°
#define Integrate_Max 20                     //积分限幅
#define Integrate_Separation_Err 6          //积分误差限幅
#define Control_OutPut_Limit 35             //总输出限幅
#define ABS(X)  (((X)>0)?(X):-(X))
	
extern float auto_rpm_x;
extern float auto_rpm_z;
extern float test_speed;
extern void speed_test(void);
extern void rs485_write(float linear_vel_x, float angular_vel_z);
extern void control_task(void);
extern void reset_actuator(void);

#ifdef __cplusplus
}
#endif