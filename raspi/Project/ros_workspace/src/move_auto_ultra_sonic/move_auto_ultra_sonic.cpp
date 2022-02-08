#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
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
#define TRUE 1
#define TRIG 5
#define ECHO 6

void move_robot();
void stop_robot();
void move_robot_left();
void move_robot_right();
void move_robot_rev();
void parseString(std::string text);
int getCM();

int DISTANCE;
using namespace std;

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

void setup() {
  wiringPiSetup();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  //TRIG pin must start LOW
  digitalWrite(TRIG, LOW);
  delay(30);
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
	int i_straight = 0;
	bool isMoveMotorStraight = true;
	bool isMoveMotorLeft = true;
	bool isMoveMotorRight = true;
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(text, root)) {
	  std::cout << reader.getFormattedErrorMessages() << std::endl;
	}
	Json::Value data = root["data"];
	int lidar_values[60];
	int lidar_values_left[60];
	int i_left = 0;
	int lidar_values_right[60];
	int i_right = 0;


	for( int index =0; index < data.size(); index++ ){
		int lidar_data = data[index]["lidar_data"].asInt();
		int lidar_angle = data[index]["servo_angle"].asInt();
		cout << "lidar_data " << lidar_data << " and angle is " << lidar_angle <<  endl;
		if(lidar_angle >= 70 && lidar_angle <= 110){
			lidar_values[i_straight] = lidar_data;
			i_straight++;
		}else if(lidar_angle > 130){
			lidar_values_left[i_left] = lidar_data;
			i_left++;
	   }else if(lidar_angle < 50 && lidar_angle > 20){
			lidar_values_right[i_right] = lidar_data;
			i_right++;
		}

	}
	for(int index = 0; index < i_straight;index++){
		if(lidar_values[index] < 100){
			isMoveMotorStraight = false;
		}
	}


	cout << "isMoveMotorStraight "<< isMoveMotorStraight << endl;

	if(isMoveMotorStraight){
		move_robot();
	} else {
		for(int index = 0; index < i_left;index++){
			if(lidar_values_left[index] < 90){
				isMoveMotorLeft = false;
			}
		}
		cout << "isMoveMotorLeft "<< isMoveMotorLeft << endl;

		if(isMoveMotorLeft) {
			move_robot_left();
			stop_robot();
		} else {
			for(int index = 0; index < i_right;index++){
				if(lidar_values_right[index] < 90){
					isMoveMotorRight = false;
				}
			}
			cout << "isMoveMotorRight "<< isMoveMotorRight << endl;
			if(isMoveMotorRight) {
				move_robot_right();
				stop_robot();
			} else {
				stop_robot();
				move_robot_rev();
				stop_robot();
			}
		}
	}
}

int main(int argc, char *argv[])
{
    //Added code
	Json::Value root;
	Json::Value minParameters;
	std_msgs::String msg;

	int distance;
	int low;
	int high;

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
  		while(angle <= 200){
			int pw = ((2220 - 500)/180)*angle+500;  //2220
		    while(1){
				  distance = getCM();
				  minParameters["servo_angle"] = angle;
					minParameters["lidar_data"] = distance;
					root["data"].append(minParameters);
					angle = angle-5;
					gpioServo(23, pw);
			  }
      }

	std::string str = root.toStyledString();
  parseString(str);
	fflush (stdout) ;


 	//Added code for rotating from 180-0
	root["data"] = Json::Value(Json::arrayValue);
 	while(angle >= 0){
			int pw = ((2000 - 500)/180)*angle+500;
	    while(1){
			  distance = getCM();
			  minParameters["servo_angle"] = angle;
				minParameters["lidar_data"] = distance;
				root["data"].append(minParameters);
				angle = angle-5;
				gpioServo(23, pw);
		  }
  }

		std::string str_reverse = root.toStyledString();
		parseString(str_reverse);

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
   printf("inside move robot straight:: \n");
   //forward left motor
   gpioPWM(17, 140);
   // forward right motor
   gpioPWM(27, 140);
   gpioWrite(18, 0);
   gpioWrite(22, 0);
}

void move_robot_rev(){
   printf("inside move robot reverse:: \n");
   double start;
   // right reverse motor
   gpioPWM(18, 255);
   // left reverse motor
   gpioPWM(22, 202);
   start = time_time();
   while ((time_time() - start) < 1.0)
   {
      gpioWrite(17, 0);
      time_sleep(0.5);
      gpioWrite(27, 0);
      time_sleep(0.5);
   }
}

void move_robot_left(){
   double start;
   // left motor reverse
    gpioPWM(27, 190);
   // right motor forward
   gpioPWM(18, 190);
   start = time_time();
   while ((time_time() - start) < 1.0)
   {
      gpioWrite(22, 0);
      time_sleep(0.5);
      gpioWrite(17, 0);
      time_sleep(0.5);
   }
}

void move_robot_right(){
      double start;
   // left motor forward
   gpioPWM(22, 255);
   // right motor reverse
   gpioPWM(17, 255);
   start = time_time();
   while ((time_time() - start) < 1.0)
   {
      gpioWrite(18, 0);
      time_sleep(0.5);
      gpioWrite(27, 0);
      time_sleep(0.5);
   }
}

void stop_robot(){
   printf("inside stop motor :: \n");
   gpioWrite(17, 0);
   gpioWrite(18, 0);
   gpioWrite(22, 0);
   gpioWrite(27, 0);
}

int getCM() {
    //Send trig pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);

    //Wait for echo start
    while(digitalRead(ECHO) == LOW);

    //Wait for echo end
    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;

    //Get distance in cm
    int distance = travelTime / 58;

    return distance;
}
