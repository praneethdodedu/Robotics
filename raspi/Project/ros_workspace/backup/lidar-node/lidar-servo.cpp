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
#include<pigpio.h>
#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include <jsoncpp/json/json.h>

#define MAXPULSEWIDTH 2175
#define MINPULSEWIDTH 500


using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main (int argc, char **argv)
{

	//if(gpioInitialise() < 0 ){
		//return 1;
		//}


    Json::Value minimizer;
	Json::Value minParameters;
	std_msgs::String msg;

	ros::init(argc, argv, "lidar_node");
	ros::NodeHandle nH;
	ros::Publisher chatter_pub = nH.advertise<std_msgs::String>("lidar_topic", 1000);

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

		int angle = 0;
  		while(angle <= 180){
		int pw = ((2175 - 500)/180)*angle+500;
			
        while(1){
		serialFlush(fd) ;
		if(char(serialGetchar(fd))=='Y'){
			if(char(serialGetchar(fd))=='Y'){
				low = serialGetchar(fd);
				high = serialGetchar(fd);	
				distance = low + high * 256;
				minParameters["servo_angle"] = angle;
				minParameters["lidar_data"] = distance;
				std::string str = minParameters.toStyledString();
				msg.data = str;	
				cout << "Json string " << str << endl;
				chatter_pub.publish(msg);					
				angle = angle+5;
				gpioServo(23, pw);
				break;
			}

          }
			usleep(200 * 1000);
		}
			
			
     }
	//sleep(1);
	fflush (stdout) ;
	}
	return 0;
}
