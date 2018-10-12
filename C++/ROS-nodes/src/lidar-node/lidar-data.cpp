#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<iostream>
#include <wiringSerial.h>
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace std;

int main (int argc, char **argv)
{
	ros::init(argc, argv, "lidar_node");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("lidar_topic", 1000);

	int fd;
	int distance;
	int low;
	int high;
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	while (ros::ok())
	{
		if(char(serialGetchar(fd))=='Y'){
			if(char(serialGetchar(fd))=='Y'){
				low = serialGetchar(fd);
				high = serialGetchar(fd);	
				distance = low + high * 256;
				cout << 	"Distance :: " << distance << endl;
				std_msgs::String msg;
				std::stringstream ss;
				ss << distance;
				msg.data = ss.str();
				ROS_INFO("%s", msg.data.c_str());
				chatter_pub.publish(msg);
				ros::spinOnce();
				
			}
		}
	fflush (stdout) ;
	}
	return 0;
}
