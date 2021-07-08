//#include "control.h"

//double required_angular_vel = 0;
//double required_linear_vel = 0;
//uint32_t previous_command_time = 0;

//Kinematics kinematics(MAX_RPM, WHEEL_DIAMETER, FR_WHEELS_DISTANCE, LR_WHEELS_DISTANCE);


//void command_callback( const geometry_msgs::Twist& cmd_msg)
//{
//    required_linear_vel = cmd_msg.linear.x;
//    required_angular_vel = cmd_msg.angular.z;

//    previous_command_time = millis();
//}

//void move_base()
//{
//	Kinematics::rpm req_rpm = kinematics.getRPM(required_linear_vel, 0, required_angular_vel);
//	
////	int current_rpm1 = encoder1.getRPM();
////    int current_rpm2 = encoder2.getRPM();
////	motor1.spin(motor1_pid.compute(req_rpm.motor1, current_rpm1));
////    motor2.spin(motor2_pid.compute(req_rpm.motor2, current_rpm2));
////	
////	Kinematics::velocities current_vel;
////	current_vel = kinematics.getVelocities(current_rpm1, current_rpm2);
////	
////	 //fill in the object
////    raw_vel_msg.linear_x = current_vel.linear_x;
////    raw_vel_msg.linear_y = 0.0;
////    raw_vel_msg.angular_z = current_vel.angular_z;

////    //publish raw_vel_msg object to ROS
////    raw_vel_pub.publish(&raw_vel_msg);

//}