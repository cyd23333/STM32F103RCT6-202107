/***************************************************************************
 *
 *                          厦门大学海韵机器人队
 *
 * @2017 all rights reserved
 *
 * @file commander.c
 *
 * @author zwh <zwh@raaworks.com>
 *         hc <450801089@qq.com>
 *
 ***************************************************************************/

#include "commander.h"

#include <math.h>
#include <string.h>
#include <stdint.h>
#include "millisecondtimer.h"

#include "topics.h"

#ifndef M_ONE_G
#define M_ONE_G 9.80665f
#endif

static uint8_t _calibrate = 0;

float _acc_filter;
float _battery_filter;

static void calibrate()
{
    static int count = 0;
    static float accel_sample[3] = { 0.0f };
    static float gyro_sample[3] = { 0.0f };
	int i = 0;

    const int samples = 300;

    if (_calibrate) {
        if (!count) {
            gyro_sample[0] = _gyro.x;
            gyro_sample[1] = _gyro.y;
            gyro_sample[2] = _gyro.z;

            accel_sample[0] = _accel.x;
            accel_sample[1] = _accel.y;
            accel_sample[2] = _accel.z;

            count++;

        } else {
            
            gyro_sample[0] += _gyro.x;
            gyro_sample[1] += _gyro.y;
            gyro_sample[2] += _gyro.z;

            accel_sample[0] += _accel.x;
            accel_sample[1] += _accel.y;
            accel_sample[2] += _accel.z;

            count++;

            if (count == samples) {
                for (i = 0; i < 3; i++) {
                    gyro_sample[i] /= (float)samples;
                    accel_sample[i] /= (float)samples;
                }

                accel_sample[2] += M_ONE_G;

                _params.gyro_offset[0] += gyro_sample[0];
                _params.gyro_offset[1] += gyro_sample[1];
                _params.gyro_offset[2] += gyro_sample[2];

                _params.accel_offset[0] += accel_sample[0];
                _params.accel_offset[1] += accel_sample[1];
                _params.accel_offset[2] += accel_sample[2];

                count = 0;

                _calibrate = 0;

                // save parameters to flash
//                flash_save();
            }
        }
    }
}
u8 qqqq;
void commander()
{
    uint64_t t = millis();

    static uint64_t count = 0;
    
    count++;

    // calibration
    static uint64_t calibrate_timestamp = 0;

	if (!_calibrate && t == 80) {
            _calibrate = 1;
		qqqq = 1;
    } else {
        calibrate_timestamp = t;
    }

    calibrate();
}

void commander_task()
{
    commander();
}
