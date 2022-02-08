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
#include <fstream>



#define MAXPULSEWIDTH 2175
#define MINPULSEWIDTH 500





void move_robot();
void stop_robot();
void move_robot_left();
void move_robot_right();
void move_robot_rev();
int DISTANCE;

using namespace std;
void parseString(std::string text);
void writeToFile(std::string text, int count);

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


void writeToFile(std::string text,int count){

	cout<<"Count value "<< count << endl;

	ofstream outfile;
	std::string fileName = "servo_data";

	fileName += std::to_string(count);
	fileName += ".txt";
    outfile.open(fileName);

	int n = text.length(); 
   char char_array[n + 1]; 
   strcpy(char_array, text.c_str()); 
 	outfile << char_array << endl;
	outfile.close();

	//move_robot();
	stop_robot();
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
 int fileAppendcount=1;
	while (ros::ok())
	{
	   
		int angle = 0;
		root["data"] = Json::Value(Json::arrayValue);
  		while(angle <= 200){
			int pw = ((2220 - 500)/180)*angle+500;  //2220
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
						angle = angle+1;
						gpioServo(23, pw);

						break;
					}
				  }
				usleep(10 * 1000);  //10 * 1000
			   }
     		}

	std::string str = root.toStyledString();
	cout << "Servo Data "<< str <<endl;	
	writeToFile(str,fileAppendcount);
	fileAppendcount++;
        //parseString(str);
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
   printf("inside move robot straight:: \n");
	start = time_time();
	 while ((time_time() - start) < 1.0)
   {
      //forward left motor 
   gpioPWM(17, 140); 
   // forward right motor
   gpioPWM(27, 140); 
   gpioWrite(18, 0); 
   gpioWrite(22, 0); 
   } 

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
