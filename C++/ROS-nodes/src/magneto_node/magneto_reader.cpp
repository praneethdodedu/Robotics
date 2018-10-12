#include <iostream>
#include "pyembed.h"
#include<stdio.h>
#include<wiringPi.h>
#include <sstream>
#include <wiringPiI2C.h>
#include "ros/ros.h"
#include "std_msgs/String.h"

//GPIO  pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//GPIO  pin wiringPi 1, GPIO pin 12, BCM 18
#define MOTOR1_PIN2 1

//wiringPi 2, GPIO 13, BCM 27
#define MOTOR2_PIN1 2

//wiringPi 3, GPIO 15, BCM 22
#define MOTOR2_PIN2 3

float getMagnetoReading();
int read_reg16(int fd, int reg);

float FIXED_DEGREE;
int fd;

using namespace std;
int main(int argc, char** argv)
{

	ros::init(argc, argv, "magneto_node");
  	ros::NodeHandle n;
  	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("magneto_topic", 1000);
	ros::Rate loop_rate(10);
	
    fd = wiringPiI2CSetup(0x0d);
	while (ros::ok())
  	{
		wiringPiI2CWriteReg16 (fd, 11, 0b01110000) ;
		wiringPiI2CWriteReg16 (fd, 10, 0b00100000) ;
		wiringPiI2CWriteReg16 (fd, 9, 0b00011101) ;
		FIXED_DEGREE  = getMagnetoReading();
	    cout << "fixed degree : " << FIXED_DEGREE << endl;
		std_msgs::String msg;
		std::stringstream ss;
		ss << FIXED_DEGREE;
		msg.data = ss.str();
		ROS_INFO("%s", msg.data.c_str());
		chatter_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
   	return 0;
}


float getMagnetoReading(){

    float scale = 0.92;
    int x_offset = -10;
    int y_offset = 10;
    float x_out = (read_reg16(fd,0)- x_offset+2) * scale  ;
    float y_out = (read_reg16(fd,2)- y_offset+2)* scale;
    float z_out = read_reg16(fd, 4) * scale;
    //int bearing = math.atan2(y_out, x_out)+.48;

    float bearing = atan2(y_out, x_out)+.48;
      if(bearing < 0)
          bearing += 2* M_PI;

    float heading = (180 * bearing/M_PI);
    if(heading < 0)
     heading += 360;
return heading;
}

int read_reg16(int fd, int reg){
			int val = wiringPiI2CReadReg16(fd,reg);
			if (val>= 0x8000)
					return -((65535 - val)+1);
			else
					return val;
}
