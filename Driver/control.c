//#include "topics.h"
//#include "control.h"
//#include "config.h"
//#include "modbus.h"
//#include "host.h"



//u8 Err_Limit_Flag = 0;//偏差限幅度标志位
//u8 Integrate_Separation_Flag = 0;//积分分离标志位
//u8 Integrate_Limit_Flag = 0;//积分限幅标志位


//float a,b=0;

//void rs485_write(float linear_vel_x, float angular_vel_z)
//{
//    float _vel_1 = 0; 
//    float _vel_2 = 0;
//    _vel_1 = linear_vel_x + angular_vel_z;
//    _vel_2 = linear_vel_x - angular_vel_z;
//    if( _vel_2 > 1.0f && _vel_1 <0.1f && _vel_1 >= 0)
//        _vel_1 -= 10.0f;
//    if( _vel_1 > 1.0f && _vel_2 <0.1f && _vel_2 >= 0)
//        _vel_2 -= 10.0f;
//	SetRPM(0x01,_vel_1);
//	SetRPM(0x02,_vel_2);
//}

////void reset_actuator(void)
////{
////	_actuator.linear_test = 0;
////	_actuator.linear_joystick = 0;
////	_actuator.angular_joystick = 0;
////	_actuator.linear_test = 0;
////	_actuator.linear_auto_x = 0;
////	_actuator.linear_gravity = 0;
////	_actuator.linear_voice = 0;
////	_actuator.angular_auto_z = 0;
////	_actuator.angular_euler = 0;
////	_actuator.angular_gravity = 0;
////}

//void angle_control(void)
//{
///************************偏差计算*********************/
//	_pid.err_prev = _pid.err_now;//保存上次偏差
//	_pid.err_now = _pid.euler_sp - _attitude.euler[2];//期望减去反馈得到偏差
///***********************偏航角偏差超过+-180处理*****************************/
//	if(_pid.err_now < -PI)  _pid.err_now = _pid.err_now + PI*2;
//	if(_pid.err_now > PI)  _pid.err_now = _pid.err_now - PI*2;

//	if(Err_Limit_Flag==1)//偏差限幅度标志位
//	{
//		if(_pid.err_now >=  Err_Max)   _pid.err_now =  Err_Max;
//		if(_pid.err_now <= -Err_Max)   _pid.err_now = -Err_Max;
//	}

//	/*******积分计算*********************/
//	if(Integrate_Separation_Flag==1)//积分分离标志位
//	{
//		if(ABS(_pid.err_now) <= Integrate_Separation_Err)
//		  _pid.euler_int += _params.att_i * _pid.err_now;
//	}
//	else
//	{
//	_pid.euler_int += _params.att_i * _pid.err_now;
//	}
//	/*******积分限幅*********************/
//	if(Integrate_Limit_Flag==1)//积分限制幅度标志
//	{
//		if(_pid.euler_int >= Integrate_Max)
//		  _pid.euler_int = Integrate_Max;
//		if(_pid.euler_int <= -Integrate_Max)
//		  _pid.euler_int = -Integrate_Max ;
//	}
//	/*******总输出计算*********************/

//	_pid.output_angle =   _params.att_p * _pid.err_now//比例
////	                 + _pid.euler_int//积分
//	                 + _params.att_d * (_pid.err_now - _pid.err_prev);//微分

//	/*******总输出限幅*********************/
//	if(_pid.output_angle >=  Control_OutPut_Limit)
//		_pid.output_angle =  Control_OutPut_Limit;
//	if(_pid.output_angle <= -Control_OutPut_Limit)
//		_pid.output_angle = -Control_OutPut_Limit;
//	
//	_actuator.angular_euler = _pid.output_angle;
//}

//float _value;
//float test_speed = 0;
//void speed_test(void)
//{
//	if(schedule_tick %4 == 0)//20ms
//	{
//		test_speed += _value;
//		if(test_speed >= 120)
//			_value = -2.7f;
//		else if(test_speed <= 0)
//			_value = 2.7f;
//	}
//	_actuator.linear_test = test_speed;
//}

//extern double required_angular_vel;
//extern double required_linear_vel;
//extern u16 rec_time;
//extern u16 pre_rec_time;
//float auto_rpm_x,auto_rpm_z;
//u8 send_actuator[6];
//float speed_ex = 2.15f;
//void auto_control(void)
//{
//    //calculate for the target motor RPM and direction
//    //front-left motor
//    auto_rpm_x = required_linear_vel * 80.0f * speed_ex;
//	if(auto_rpm_x >= 40 * speed_ex)
//		auto_rpm_x = 40 * speed_ex;

//    //front-right motor


//    auto_rpm_z = - required_angular_vel * 15.95f * 1.2f * speed_ex;
////    auto_rpm_z = - required_angular_vel * 20.0f * 1.4f;

//	_actuator.linear_auto_x = auto_rpm_x;
//	_actuator.angular_auto_z = auto_rpm_z;
//    
////    auto_rpm_x = 1.0f;
////    auto_rpm_z = 1.0f;
////    send_actuator[0]= auto_rpm_x > 0 ? 2 : 0;
////    send_actuator[1]= HI((s16)(auto_rpm_x));
////    send_actuator[2]= LOW((s16)(auto_rpm_x));
////    send_actuator[3]= auto_rpm_z > 0 ? 2 : 0;
////    send_actuator[4]= HI((s16)(auto_rpm_z));
////    send_actuator[5]= LOW((s16)(auto_rpm_z));
////    UART5_Send_Data(send_actuator,6);
//    
//}
//u32 voice_tick;
//extern u8 mode_choose;
//extern u16 meter;
//float meter_rate=1.0f;
//extern float initial_angle;
//float err_ang;
//int i_voice;
//int voice_choose_flag = 1;
//u8 voice_flag;
//extern u16 usart2_tick;
//extern u16 pre_usart2_tick;
//void voice_control(void)
//{
//    
//    meter_rate = meter == 1 ? 1.15 : 1; 
//		if(i_voice == 0 && voice_flag == 1)//开始旋转
//		{
//			i_voice++;
//		}
//		if(ABS(_pid.err_now) <= 5*PI/180 && i_voice == 1)//旋转停止
//		{
//			i_voice++;
//		}
//		if(i_voice == 2)
//		{
//			voice_tick++;//开始计数
//			if(voice_tick >= 120)//开始前进
//			{
//				_actuator.linear_voice = 18;
//			}
//		}
//		if(voice_tick >= 450 * meter * meter_rate)//停止前进
//		{
//			_actuator.linear_voice = 0;
//			if(voice_tick >= 520 * meter * meter_rate)
//			{
//				voice_tick = 0;
//				i_voice = 0;
//				meter = 0;
//				initial_angle = _attitude.euler[2];//记录初始角度
//				voice_flag = 0;
//				pre_usart2_tick = usart2_tick;
//			}
//		}
//}

//void control_task(void)
//{
//	auto_control();

//	if(mode_choose == 3)
//	{
//		if(usart2_tick != pre_usart2_tick)
//			voice_flag = 1;

//		angle_control();
//		voice_control();
//	}
//}

