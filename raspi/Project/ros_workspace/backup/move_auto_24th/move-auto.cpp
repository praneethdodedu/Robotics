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
#include<iterator>


#define MAXPULSEWIDTH 2175
#define MINPULSEWIDTH 500





void move_robot();
void stop_robot();
int DISTANCE;

using namespace std;
void parseString(std::string text);

//Added code
void error(const char *msg)
{
    perror(msg);
    exit(0);
}



void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from lidar: [%s]", msg->data.c_str());
  stringstream distance_str(msg->data.c_str());
  parseString(msg->data.c_str());
  /*distance_str >> DISTANCE;
  if(DISTANCE > 30){
	move_robot();
  } else {
	stop_robot();
  }*/
}

void parseString(std::string text){
	int i = 0;
	bool isMoveMotor = true;
	Json::Reader reader;
	Json::Value root;  
	if(!reader.parse(text, root)) {
	  std::cout << reader.getFormattedErrorMessages() << std::endl;
	}
	Json::Value data = root["data"];
	int lidar_values[60];
	for( int index =0; index < data.size(); index++ ){
		int lidar_data = data[index]["lidar_data"].asInt();
		int lidar_angle = data[index]["servo_angle"].asInt();
		if(lidar_angle >= 60 && lidar_angle <= 120){
			cout << "lidar_data " << lidar_data << endl;
			lidar_values[i] = lidar_data;
			i++;
		}
	}
	for(int index = 0; index < i;index++){
		cout << "lidar_data second " << lidar_values[index] << endl;
		if(lidar_values[index] < 100){
			isMoveMotor = false;
		}
	}
	cout << " isMoveMotor  " << isMoveMotor << endl;
	if(isMoveMotor){
		move_robot();
	} else {
		stop_robot();
	}
}

int main(int argc, char *argv[])
{
    //Added code
	Json::Value root;
	Json::Value minParameters;
	std_msgs::String msg;

	int fd;
	int distance;
	int low;
	int high;
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

   ros::init(argc, argv, "move_forward");
   ros::NodeHandle n;
   ros::Subscriber sub = n.subscribe("lidar_topic", 10, chatterCallback);

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }



   //Added servo code

	while (ros::ok())
	{

		int angle = 0;
		root["data"] = Json::Value(Json::arrayValue);
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
						root["data"].append(minParameters);
						angle = angle+5;
						gpioServo(23, pw);
						break;
					}
				  }
				usleep(20 * 1000);
			}
     	}
		std::string str = root.toStyledString();
		parseString(str);
	
	//sleep(1);
	fflush (stdout) ;
	}

	//Servo code ends here


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
   gpioWrite(18, 0); 
   gpioWrite(22, 0); 
}

void stop_robot(){
   gpioWrite(17, 0); 
   gpioWrite(27, 0); 
}
