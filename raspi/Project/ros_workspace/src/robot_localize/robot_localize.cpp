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
#include<unistd.h>
#include <jsoncpp/json/json.h>
#include<iterator>
#include <fstream>
#include<bits/stdc++.h>



#define MAXPULSEWIDTH 2175
#define MINPULSEWIDTH 500





void move_robot();
void stop_robot();
void move_robot_left();
void move_robot_right();
void move_robot_rev();
float localizeRobot(std::string fileName);
int DISTANCE;
int row[202];
int getLeastValue(float arr[]);

using namespace std;

//Added code
void error(const char *msg)
{
    perror(msg);
    exit(0);
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

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }
		int fileAppendcount = 1;
      std::ofstream myfile;
	while (ros::ok())
	{
 		std::string fileName = "lidar-data";
		fileName += std::to_string(fileAppendcount);
		fileName += ".csv";
	        myfile.open (fileName);
                myfile << "servo angle,lidar data\n";
		int angle = 0;
		root["data"] = Json::Value(Json::arrayValue);
  		while(angle <= 200){
			int pw = ((2000 - 500)/180)*angle+500;  //2220
		    while(1){
				serialFlush(fd) ;
				gpioServo(23, pw);
				if(char(serialGetchar(fd))=='Y'){
					if(char(serialGetchar(fd))=='Y'){
						low = serialGetchar(fd);
						high = serialGetchar(fd);	
						distance = low + high * 256;
						minParameters["servo_angle"] = angle;
						minParameters["lidar_data"] = distance;
						myfile << angle << "," << distance << "\n";
						root["data"].append(minParameters);
						angle = angle+1;
						break;
					}
				  }
				usleep(10 * 1000);  //10 * 1000
			   }
     		}

	std::string str = root.toStyledString();
	// cout << "Servo Data "<< str <<endl;	
	fflush (stdout) ;
	myfile.close();

	fstream fin; 
	fin.open(fileName, ios::in);
	std::string word, temp; 
    double sum=0, sqrtSum = 0;
  
    while (fin >> temp) {   
	int i=0;
	cout << "opening file " << endl;
	while (getline(fin, word, '\n')) { 
		stringstream sFirst(word.substr(word.find(",") + 1)); 
		int firstData = 0; 
		sFirst >> firstData;
		row[i]=firstData; 
		i++;
	}  
    } 
	fin.close();
	float values[4];
	values[0] = localizeRobot("lidar-data-C1.csv");
	values[1] = localizeRobot("lidar-data-C2.csv");
	values[2] = localizeRobot("lidar-data-C3.csv");
	values[3] = localizeRobot("lidar-data-C4.csv");

	int index = getLeastValue(values);
	cout << "the posotion of the robot is in C" << index << " cell" << endl;

	fileAppendcount++;
	}

	//Servo code ends here


   /* Set GPIO modes */
   gpioSetMode(17, PI_OUTPUT);
   gpioSetMode(18, PI_OUTPUT);
   gpioSetMode(27, PI_OUTPUT);
   gpioSetMode(22, PI_OUTPUT);
   
   //ros::MultiThreadedSpinner spinner(4); // Use 4 threads
   //spinner.spin(); // spin() will not return until the node has been shutdown
}

int getLeastValue(float arr[]){

   int temp = arr[0], index = 0;
   for(int i=0; i< 4; i++) {
      if(temp>arr[i]) {
         temp=arr[i];
	 index = i;
      }
   }
return index;

}


float localizeRobot(std::string fileName){
    fstream fin; 
    fin.open(fileName, ios::in); 
    int secondRow[202]; 
    string word, temp; 
    double sum = 0, sqrtSum = 0;

    while (fin >> temp) { 
	int i = 0;
	while (getline(fin, word, '\n')) { 
		stringstream sFirst(word.substr(word.find(",") + 1)); 
		int firstData = 0; 
		sFirst >> firstData;
		secondRow[i]=firstData; 
		i++;
	}  
    } 

    int sizeOfRow = sizeof(row)/sizeof(row[0]);   
    int sizeOfSecondRow = sizeof(secondRow)/sizeof(secondRow[0]);
 
    for(int i=1; i < sizeOfRow;i++) {
	if(row[i] > 1000 || secondRow[i] > 1000)
	  continue;

	// cout << "firstData :" << row[i]  << " secondData :" << secondRow[i] << " pow : " << pow((row[i]-secondRow[i]), 2) << endl;
	sum = sum + pow((row[i]-secondRow[i]), 2);
    }
	sqrtSum = sqrt(sum);
	fin.close();
	cout << "sqrtSum :" << sqrtSum << endl;
   return sqrtSum;
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
