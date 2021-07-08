#include "Android.h"
#include "modbus.h"
#include "millisecondtimer.h"
#include "topics.h"
#include "config.h"

extern u8 Rx1_update_flag;
extern u8 Rx_APP_data[100];
extern u8 mode_choose;
//extern u8 Rx_mode_data[9];
u8 pre_mode;
u8 start_stop = 0;
u32 key_count = 0;
u32 android_tick = 0;

float g_x_rate = 3.5f;
float g_z_rate = 1.8f;
float g_motor1 = 0, g_motor2 = 0;
u8 SetAcc_flag = 0;

u8 turn_flag = 0;
u8 pre_turn_flag = 0;
float turn_angle = PI/180*30.0f;
int gear = 1;
extern int rotate_flag;


void KeyControl(void)
{
	key_count++;
    if(mode_choose==1)
    {
        if(Rx1_update_flag)
        {
            switch(Rx_APP_data[0])
            {
                case '1'://前

                    pre_turn_flag = 1;

                    _actuator.linear_joystick = 15 + 1.1 * gear;
                    _actuator.angular_joystick = 0;

                    Rx1_update_flag=0;
                    break;
                case '2'://前右
                    pre_turn_flag = 2;

                    _actuator.linear_joystick = 10 + gear;
                    _actuator.angular_joystick = 8;
                    Rx1_update_flag=0;
                    break;
                case '3'://右
                    pre_turn_flag = 3;

                    _actuator.linear_joystick = 5 + 0.05 * gear;
                    _actuator.angular_joystick = 10 + 0.1 * gear;
                    Rx1_update_flag=0;
                    break;

                case '5'://后
                    _actuator.linear_joystick = -15 - gear * 0.2f;
                    _actuator.angular_joystick = 0;
                    pre_turn_flag = 5;

                    Rx1_update_flag=0;
                    break;
                
                case '7'://左
                    pre_turn_flag = 7;
                
                    _actuator.linear_joystick = 5 + 0.05 * gear;
                    _actuator.angular_joystick = -10 - 0.1 * gear;

                    Rx1_update_flag=0;
                    break;

                case '8'://前左
                    pre_turn_flag = 8;

                    _actuator.linear_joystick = 10 + gear;
                    _actuator.angular_joystick = - 8;

                    Rx1_update_flag=0;

                    break;

                default:
                    Rx1_update_flag=0;
                    Stop(0x01,emergency);
                    Stop(0x02,emergency);
                    break;
            }
        }
    }
    else
    {
        _actuator.linear_joystick = 0;
        _actuator.angular_joystick = 0;
    }
//    if(     Rx_mode_data[0] == 'a'
//        &&  Rx_mode_data[1] == 'b'
//        &&  Rx_mode_data[2] == 'c'
//        &&  Rx_mode_data[3] == 'b'
//        &&  Rx_mode_data[4] == 'c'
//        &&  Rx_mode_data[5] == 'b'
//        &&  Rx_mode_data[6] == 'c'
//        &&  Rx_mode_data[7] == 'b'
//        &&  Rx_mode_data[8] == 'a')
//    {
//        _actuator.linear_joystick = 0;
//        _actuator.angular_joystick = -5;
//    }
}

float x_vel=0,z_vel=0,zz;
void gravity_control(void)
{
	uint8_t p=0;
	float x,z;
    if(mode_choose==2)
    {
        if(Rx1_update_flag)
        {
            if(Rx_APP_data[0]=='n')  //收到n则表示停止指示
            {
                start_stop=0;   //设置成stop状态
                _actuator.linear_gravity = 0;
                _actuator.angular_gravity = 0;
                Rx1_update_flag=0;
            }
            else if(Rx_APP_data[0]=='y')  //收到y则表示开始指示
            {
                start_stop=1;   //设置成start状态
                Rx1_update_flag=0;
            }
            if(start_stop)
            {
                while(Rx_APP_data[p]!=',')
                {
                  z*=10.0f;
                    z+=Rx_APP_data[p]-'0';
                    p++;
                }
                p++;  //跳过逗号
                while(Rx_APP_data[p]!='s')
                {
                    x*=10.0f;
                    x+=Rx_APP_data[p]-'0';
                    p++;
                }
                Rx1_update_flag=0; //标志位清零
                zz=z;
                x_vel = g_x_rate * ( x >= 10 ? (x - 10.0f) : (x > 7.0f ? 0 : x - 7.0f));
                z_vel = g_z_rate * (10.0f - z);
                 if (x_vel > 40)
                     x_vel = 40;
                 if (z_vel > 40)
                     z_vel = 40;
                 if (z_vel < -40)
                     z_vel = -40;
                _actuator.linear_gravity = x_vel;
                
                _actuator.angular_gravity = - z_vel;
            }
            
        }
    }
    else
    {
        _actuator.linear_gravity = 0;
        _actuator.angular_gravity = 0;
    }
}
void start_control(void)
{
  if(Rx1_update_flag)
	{
    if(Rx_APP_data[0]=='y')  //收到y则表示开始指示
		{
			start_stop=1;   //设置成start状态
			Rx1_update_flag=0;
		}
	}
}

void stop_control(void)
{
  if(Rx1_update_flag)
	{
		if(Rx_APP_data[0]=='n')  //收到n则表示停止指示
		{
			start_stop=0;   //设置成stop状态
			_actuator.linear_gravity = 0;
			_actuator.angular_gravity = 0;
			Rx1_update_flag=0;
		}
	}
}

void rotate_control()
{
    if(rotate_flag)
        _actuator.angular_rotate = 2.0f;
    else
        _actuator.angular_rotate = 0;
//    Rx1_update_flag=0;
}

u16 voice_angular;
u16 angg;
u16 meter;
float initial_angle;
void voice_receive(void)
{
	int p=0;
	voice_angular=0;
	if(Rx1_update_flag)
	{
		if(Rx_APP_data[p]=='d')
		{
			p++;
			while(Rx_APP_data[p]>='0' && Rx_APP_data[p]<='9')
			{
				voice_angular*=10;
				voice_angular+=Rx_APP_data[p]-'0';
				p++;
			}
			switch(Rx_APP_data[p++])
			{
				case 'l':_pid.euler_sp -= voice_angular * PI/180;Rx1_update_flag=0;break;
				case 'r':_pid.euler_sp += voice_angular * PI/180;Rx1_update_flag=0;break;
				default:Rx1_update_flag=0;
			}
		}
		if(Rx_APP_data[p]=='m')
		{
			p++;
			while(Rx_APP_data[p]>='0' && Rx_APP_data[p]<='9')
			{
				meter = Rx_APP_data[p]-'0';
				p++;
			}
			switch(Rx_APP_data[p])
			{
				case 'f':Rx1_update_flag=0;break;
				case 'b':Rx1_update_flag=0;break;
				default:Rx1_update_flag=0;
			}
		}
	}
}
void reset_actuator(void)
{
	_actuator.linear_test = 0;
	_actuator.linear_joystick = 0;
	_actuator.angular_joystick = 0;
	_actuator.linear_test = 0;
	_actuator.linear_auto_x = 0;
	_actuator.linear_gravity = 0;
	_actuator.linear_voice = 0;
	_actuator.angular_auto_z = 0;
	_actuator.angular_euler = 0;
	_actuator.angular_gravity = 0;
    _actuator.angular_rotate = 0;
    rotate_flag = 0;
}

void Emergency_stop(void)
{
	if(Rx_APP_data[0] == 0x30)//急停
	{
		_actuator.linear_test = 0;
		_actuator.linear_joystick = 0;
		_actuator.angular_joystick = 0;
		_actuator.linear_test = 0;
		_actuator.linear_auto_x = 0;
		_actuator.linear_gravity = 0;
		_actuator.linear_voice = 0;
		_actuator.angular_auto_z = 0;
		_actuator.angular_euler = 0;
		_actuator.angular_gravity = 0;
        _actuator.angular_rotate = 0;
        rotate_flag = 0;
		Rx1_update_flag=0;
        meter = 0;
	}
}

void AndroidTask(void)
{
	android_tick++;

	if(mode_choose != 4 && pre_mode == 4)//切换手动控制模式时加速度
	{
			SetAcceleration(0x01,up,100);
			SetAcceleration(0x02,up,100);
			SetAcceleration(0x01,down,700);
			SetAcceleration(0x02,down,700);
	}

	if(mode_choose == 4 && pre_mode != 4)//切换自动导航模式时加速度
	{
			SetAcceleration(0x01,up,200);
			SetAcceleration(0x02,up,200);
			SetAcceleration(0x01,down,30);
			SetAcceleration(0x02,down,30);
	}
	if(mode_choose == 2 && pre_mode != 2)//切换重力感应模式时加速度
	{
			SetAcceleration(0x01,up,200);
			SetAcceleration(0x02,up,200);
			SetAcceleration(0x01,down,200);
			SetAcceleration(0x02,down,200);
	}

	if(mode_choose==1)//按键控制
	{
		if(pre_mode != 1)
		{
			_pid.euler_sp = _attitude.euler[2];//防止模式切换突转
			
			//切换模式急停
			_actuator.linear_test = 0;
			_actuator.linear_joystick = 0;
			_actuator.angular_joystick = 0;
			_actuator.linear_test = 0;
			_actuator.linear_auto_x = 0;
			_actuator.linear_gravity = 0;
			_actuator.linear_voice = 0;
			_actuator.angular_auto_z = 0;
			_actuator.angular_euler = 0;
			_actuator.angular_gravity = 0;
            _actuator.angular_rotate = 0;
            rotate_flag = 0;
		}

		KeyControl();
		pre_mode = 1;
	}
	else if(mode_choose==2)//重力感应控制
	{
		if(pre_mode != 2)
		{
			_pid.euler_sp = _attitude.euler[2];//防止模式切换突转

			//切换模式急停
			_actuator.linear_test = 0;
			_actuator.linear_joystick = 0;
			_actuator.angular_joystick = 0;
			_actuator.linear_test = 0;
			_actuator.linear_auto_x = 0;
			_actuator.linear_gravity = 0;
			_actuator.linear_voice = 0;
			_actuator.angular_auto_z = 0;
			_actuator.angular_euler = 0;
			_actuator.angular_gravity = 0;
            _actuator.angular_rotate = 0;
            rotate_flag = 0;
		}


		gravity_control();

		pre_mode = 2;
	}
	else if(mode_choose==3)//语音控制
	{
		if(pre_mode != 3)
		{
			_pid.euler_sp = _attitude.euler[2];//防止模式切换突转

			//切换模式急停
			_actuator.linear_test = 0;
			_actuator.linear_joystick = 0;
			_actuator.angular_joystick = 0;
			_actuator.linear_test = 0;
			_actuator.linear_auto_x = 0;
			_actuator.linear_gravity = 0;
			_actuator.linear_voice = 0;
			_actuator.angular_auto_z = 0;
			_actuator.angular_euler = 0;
			_actuator.angular_gravity = 0;
            _actuator.angular_rotate = 0;
            meter = 0;
            initial_angle = _attitude.euler[2];
            rotate_flag = 0;
		}
		pre_mode = 3;
		voice_receive();
	}
	else if(mode_choose == 4)//自动导航
	{
		if(pre_mode != 4)
		{
			_pid.euler_sp = _attitude.euler[2];//防止模式切换突转

			//切换模式急停
			_actuator.linear_test = 0;
			_actuator.linear_joystick = 0;
			_actuator.angular_joystick = 0;
			_actuator.linear_test = 0;
			_actuator.linear_auto_x = 0;
			_actuator.linear_gravity = 0;
			_actuator.linear_voice = 0;
			_actuator.angular_auto_z = 0;
			_actuator.angular_euler = 0;
			_actuator.angular_gravity = 0;
            _actuator.angular_rotate = 0;
            rotate_flag = 0;
		}
		
		pre_mode = 4;
	}
                  rotate_control();  
//    else if(mode_choose ==5)//自动旋转
//    {
//        if(pre_mode != 5)
//		{
//			_pid.euler_sp = _attitude.euler[2];//防止模式切换突转

//			//切换模式急停
//			_actuator.linear_test = 0;
//			_actuator.linear_joystick = 0;
//			_actuator.angular_joystick = 0;
//			_actuator.linear_test = 0;
//			_actuator.linear_auto_x = 0;
//			_actuator.linear_gravity = 0;
//			_actuator.linear_voice = 0;
//			_actuator.angular_auto_z = 0;
//			_actuator.angular_euler = 0;
//			_actuator.angular_gravity = 0;
//            _actuator.angular_rotate = 0;
//		}
//        pre_mode = 5;
//    }
}
