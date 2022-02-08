#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<iostream>
#include <wiringSerial.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h> 
#include<sstream>
#include<wiringPi.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include<cstring>
#include "sys/sysinfo.h"

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main (int argc, char **argv)
{
	ros::init(argc, argv, "lidar_node");
	ros::NodeHandle nH;
	ros::Publisher chatter_pub = nH.advertise<std_msgs::String>("lidar_topic", 1000);
	ros::Rate loop_rate(70);

	int fd;
	int distance;
	int low;
	int high;
	fd = serialOpen ("/dev/ttyAMA0", 115200);
	while (ros::ok())
	{
		if (fd < 0)
		{
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
			return 1 ;
		}
		char firstByte = char(serialGetchar(fd));
		char secondByte = char(serialGetchar(fd)); 
		if(firstByte == 'Y'){
			if(secondByte =='Y'){
				low = serialGetchar(fd);
				high = serialGetchar(fd);	
				distance = low + high * 256;
				cout << 	"Distance :: " << distance <<  endl;
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
