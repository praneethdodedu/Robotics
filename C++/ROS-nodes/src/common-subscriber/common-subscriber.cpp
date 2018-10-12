#include "ros/ros.h"
#include "std_msgs/String.h"
#include<iostream>
#include<stdio.h>

using namespace std;

int FIXED_DEGREE;
int CURRENT_DEGREE;
int chatter_count = 0 ;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from Magneto-meter: [%s]", msg->data.c_str());
}

void chatterCallbackRobotCommand(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from Robot: [%s]", msg->data.c_str());
}

void chatterCallbacklidar(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from Lidar: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  cout << "This is the start" << endl;
  ros::init(argc, argv, "robot_command_receiver");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("magneto_topic", 1000, chatterCallback);

  ros::Subscriber sub_2 = n.subscribe("robot_command", 1000, chatterCallbackRobotCommand);

  ros::Subscriber sub_3 = n.subscribe("lidar_topic", 1000, chatterCallbacklidar);

  cout << "This is the end" << endl;
  ros::MultiThreadedSpinner spinner(4); // Use 4 threads
  spinner.spin(); // spin() will not return until the node has been shutdown
  return 0;
}
