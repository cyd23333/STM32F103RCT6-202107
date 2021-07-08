/***************************************************************************
 *
 *                          厦门大学海韵机器人队
 *
 * @2017 all rights reserved
 *
 * @file topics.c
 *
 * @author zwh <zwh@raaworks.com>
 *         hc <450801089@qq.com>
 *
 ***************************************************************************/

#include "topics.h"

params_t _params = {
    .accel_offset[0] = 0.0983303189f,
    .accel_offset[1] = 0.0885278136f,
    .accel_offset[2] = -1.74590206f,
    .accel_scale[1] = 1.0f,
    .accel_scale[0] = 1.0f,
    .accel_scale[2] = 1.0f,

    .gyro_offset[0] = -0.0025610663f,
    .gyro_offset[1] = -0.02166491f,
    .gyro_offset[2] = 0.0204420667f,
    .gyro_scale[0] = 1.0f,
    .gyro_scale[1] = 1.0f,
    .gyro_scale[2] = 1.0f,

	.att_p = 20.0f,
	.att_i = 0,
	.att_d = 2
};

//加速度
accel_t _accel = { 0 };

//陀螺仪
gyro_t _gyro = { 0 };

//遥控器原始接收数据
radio_raw_t _radio_raw = { 0 };

//遥控器最终数据
radio_t _radio = { 0 };

//姿态最终数据
attitude_t _attitude = { 0 };

//激励值
actuator_t _actuator = { 0 };

//pid
pid_t _pid = { 0 };

uint32_t schedule_tick = 0;

uint32_t pre_schedule_tick = 0;

