#include <stdio.h>
#include <pigpio.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include<iostream>

void move_robot();
void stop_robot();
int DISTANCE;

using namespace std;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from lidar: [%s]", msg->data.c_str());
  stringstream distance_str(msg->data.c_str());
  distance_str >> DISTANCE;
  if(DISTANCE > 30){
	move_robot();
  } else {
	stop_robot();
  }
}

int main(int argc, char *argv[])
{

   ros::init(argc, argv, "move_forward");
   ros::NodeHandle n;
   ros::Subscriber sub = n.subscribe("lidar_topic", 10, chatterCallback);

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   /* Set GPIO modes */
   gpioSetMode(17, PI_OUTPUT);
   gpioSetMode(18, PI_OUTPUT);
   gpioSetMode(27, PI_OUTPUT);
   gpioSetMode(22, PI_OUTPUT);
   
   ros::MultiThreadedSpinner spinner(4); // Use 4 threads
   spinner.spin(); // spin() will not return until the node has been shutdown
}

void move_robot(){
   double start;
   //forward right motor 
   gpioPWM(17, 140); 
   // forward left motor
   gpioPWM(27, 192); 
   start = time_time();
	printf("inside :: \n");
	gpioWrite(18, 0); 
	time_sleep(0.25);
	gpioWrite(22, 0); 
	time_sleep(0.25);
}

void stop_robot(){
   gpioWrite(17, 0); 
   gpioWrite(27, 0); 
//   gpioTerminate();
}
