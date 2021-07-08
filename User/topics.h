/***************************************************************************
 *
 *                          厦门大学海韵机器人队
 *
 * @2017 all rights reserved
 *
 * @file topics.h
 *
 * @author zwh <zwh@raaworks.com>
 *         hc <450801089@qq.com>
 *
 ***************************************************************************/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float accel_offset[3];
    float accel_scale[3];
    float gyro_offset[3];
    float gyro_scale[3];

    float velocity_p[4];
    float velocity_i[4];
    float velocity_d[4];

    float att_p;
	float att_i;
	float att_d;

} params_t;

typedef struct {
    uint64_t timestamp;
    float temperature;
    int16_t x_raw, y_raw, z_raw;
    float x, y, z;
} accel_t;

typedef struct {
    uint64_t timestamp;
    float temperature;
    int16_t x_raw, y_raw, z_raw;
    float x, y, z;
} gyro_t;

typedef struct {
    uint64_t timestamp;

    struct {
        int16_t ch0;
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;
        uint8_t s1;
        uint8_t s2;
    } rc;

    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;

    struct {
        uint16_t v;
    } key;

	uint32_t lastUpdate;

    float quality;
} radio_raw_t;

typedef struct {
    uint64_t timestamp;
    float x;
    float y;
    float pitch;
    float yaw;
    uint8_t mode;
    uint8_t shoot;
} radio_t;

typedef struct {
    uint64_t timestamp;
    float euler[3];
    float rates[3];
} attitude_t;

typedef struct {
    uint64_t timestamp;
    float linear_test;
	float linear_joystick;
	float linear_gravity;
	float linear_auto_x;
	float linear_voice;
	float angular_auto_z;
	float angular_euler;
	float angular_joystick;
	float angular_gravity;
	float angular_voice;
    float angular_rotate;
	
} actuator_t;

typedef struct {
    uint64_t timestamp;
    float wheel[4];
} wheel_velocity_t;

typedef struct {
    uint64_t timestamp;
    float trigger;
} tri_rates_t;

typedef struct {
    uint64_t timestamp;
    float wheel[4];
    float power_sum;
    float max_sum;
} wheel_current_t;

typedef struct {
    uint64_t timestamp;
    float gimbal[2];
} gimbal_angle_t;

typedef struct
{
    int16_t  speed_rpm;

    int16_t  given_current;
    uint8_t  hall;

    uint16_t offset_angle;
    int32_t round_cnt;
    uint32_t msg_cnt;
    int32_t total_ecd;
    int32_t total_angle;

    uint16_t angle; //abs angle range:[0,8191]
    uint16_t last_angle; //abs angle range:[0,8191]

} motor_t;


typedef struct {

    uint64_t timestamp;
    
	float euler_sp;
	float euler_prev;
	float euler_int;
	float err_now;
	float err_prev;
	float output_rate_sp;
	float output_angle;
	
} pid_t;

extern pid_t _pid;
extern gyro_t _gyro;
extern accel_t _accel;
extern radio_t _radio;
extern params_t _params;
extern actuator_t _actuator;
extern attitude_t _attitude;

extern uint32_t schedule_tick;
extern uint32_t pre_schedule_tick;

#ifdef __cplusplus
}
#endif
