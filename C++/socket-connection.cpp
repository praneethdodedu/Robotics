#include<ros/ros.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h> 
#include<std_msgs/String.h>
#include<sstream>
#include<iostream>
#include<wiringPi.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include<cstring>
#include "sys/sysinfo.h"

using namespace std;

std::string status;
std::string cpuUsage;
std::string memoryUsed;
struct sysinfo memInfo;

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

double getCurrentValue();
void getCPUUsage();

int isStatusRecieved = 0;

//GPIO  pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//GPIO  pin wiringPi 1, GPIO pin 12, BCM 18 
#define MOTOR1_PIN2 1

//wiringPi 2, GPIO 13, BCM 27
#define MOTOR2_PIN1 2 

//wiringPi 3, GPIO 15, BCM 22
#define MOTOR2_PIN2 3

int radianInt = -1;
int degreeInt = -1;
int gearInt = -1;
std::string direction = "";
std::string id = "";
std::string securitykey = "";

int parseCommand(std::string command);
std::string getAck(int response);
int runMotorForStreeingWheel(std::string command);

int checkInternet();
void ReadXBytes(int socket, unsigned int x, void* buffer);
void getIPAddress(char* outStr);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


int main1(){
//char[] robot_name;	
char ip_addr[15];
getIPAddress(ip_addr);
char *robot_name = "Robot-1:";
char * robot_name_with_id = new char[std::strlen(ip_addr)+std::strlen(robot_name)+1];
std::strcpy(robot_name_with_id,robot_name);
std::strcat(robot_name_with_id,ip_addr);
robot_name_with_id[strlen(robot_name_with_id)]='\n';

cout << "Robot-name :: "  << robot_name_with_id << endl;

}

int main(int argc, char *argv[])
{	
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

	wiringPiSetup();
    pinMode(MOTOR1_PIN1, OUTPUT);
    pinMode(MOTOR1_PIN2, OUTPUT);
    pinMode(MOTOR2_PIN1, OUTPUT);
    pinMode(MOTOR2_PIN2, OUTPUT);

	char ip_addr[15];
	getIPAddress(ip_addr);
	char *robot_name = "Robot-1:";
	char * robot_name_with_id = new char[std::strlen(ip_addr)+std::strlen(robot_name)+1];
	std::strcpy(robot_name_with_id,robot_name);
	std::strcat(robot_name_with_id,ip_addr);
	robot_name_with_id[strlen(robot_name_with_id)]='\n';
	std::string ack = "";
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	
	while(checkInternet()){
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
			cout << "Error connecting to socket server" << endl;
			continue;
		}
		n = write(sockfd,robot_name_with_id,strlen(robot_name_with_id));
		if (n < 0) {
			 cout << "ERROR writing to socket" << endl;
			 error("ERROR writing to socket");
		}
		while(1){
			unsigned int length = 0;
			char* buffer = 0;
			// we assume that sizeof(length) will return 4 here.
			ReadXBytes(sockfd, 4, (void*)(&length));
			cout << "Buffer Size ::" << length << endl; 
			buffer = new char[length];
			ReadXBytes(sockfd, length, (void*)buffer);

			printf("Buffer name after :  %s\n",buffer);
			if(length > 0){
				std::string command(buffer,length);
				if(runMotorForStreeingWheel(command)){
					ack = getAck(1);
					n = write(sockfd,ack.c_str(),strlen(ack.c_str()));
					if (n < 0){ 
						 cout << "ERROR writing acknowledgement to socket " << endl;
						 error("ERROR writing to socket");
					}
				}else{
					ack = getAck(0);
					n = write(sockfd,ack.c_str(),strlen(ack.c_str()));
					if (n < 0) {
						 cout << "ERROR writing acknowledgement to socket " << endl;
						 error("ERROR writing to socket");
					}
				}
			} 
		
		}
	}
    
    close(sockfd);
    return 0;
}

int checkInternet(){
	FILE *output;

	if(!(output = popen("/sbin/route -n | grep -c '^0\\.0\\.0\\.0'","r")))
	{
		return 1;
	}
	unsigned int i;
	fscanf(output,"%u",&i);
	if(i == 1){
		cerr<<"There is internet connection\n";
		return 1;
	} else if(i == 0){
		cerr<<"There is no internet connection\n";
		return 0;
	}
}

void ReadXBytes(int socket, unsigned int x, void* buffer)
{
    int bytesRead = 0;
    int result;
    while (bytesRead < x)
    {
        result = read(socket, buffer + bytesRead, x - bytesRead);
//		cout << buffer + bytesRead << endl;
        if (result < 1 )
        {
            //cerr << "Error ::" << endl;
        }

        bytesRead += result;
    }
//	cout << buffer + bytesRead << endl;
}


int runMotorForStreeingWheel(std::string command)
{
	//command = "{\"id\":\"822453680e284262ba0f73dea1d0a5b8\",\"direction\":\"up\",\"degree\":\"96.69404589067692\",\"radian\":\"1.6876294678668051\",\"gear\":\"1\",\"status\":\"initialised\",\"securitykey\":\"f50b51935fd14fe0be894a388ed8b5eb\"}";

	cout << "Robot is Moving" << endl;
	command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());
	cout << "command is : " << command << endl;

	if(parseCommand(command)) {
		int delaySeconds,  degreeTobeRotated;
		for(int run_time = 0; run_time <= 50; run_time++)
		{ 
			if(75 < degreeInt && degreeInt <  115)
			{
				//moving forward
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 1); //ON
				digitalWrite(MOTOR1_PIN2, 0); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 1); //ON
				digitalWrite(MOTOR2_PIN2, 0); //OFF
				degreeTobeRotated = 0;
				delay(gearInt * 15 * 1);
			}
			else if(-10 < degreeInt && degreeInt <  75)
			{
				//moving left forward
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 1); //ON
				digitalWrite(MOTOR1_PIN2, 0); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 0); //ON
				digitalWrite(MOTOR2_PIN2, 0); //OFF

				degreeTobeRotated = 90 - ((degreeInt < -1) ? 0 : degreeInt); 
				delaySeconds = 15 * degreeTobeRotated / 90; 
				delay(gearInt * delaySeconds);
			}
			else if(-170 > degreeInt ||  degreeInt >  115)
			{
				//moving right forward
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 0); //ON
				digitalWrite(MOTOR1_PIN2, 0); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 1); //ON
				digitalWrite(MOTOR2_PIN2, 0); //OFF

				degreeTobeRotated = ((degreeInt < -1) ? 180 : degreeInt) - 90; 
				delaySeconds = 15 * degreeTobeRotated / 90; 
				delay(gearInt * delaySeconds);


			}
			else if(-75 < degreeInt &&  degreeInt <  -10)
			{
				//moving left backward
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 0); //ON
				digitalWrite(MOTOR1_PIN2, 1); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 0); //ON
				digitalWrite(MOTOR2_PIN2, 0); //OFF

				degreeTobeRotated = 90 + (degreeInt); 
				delaySeconds = 15 * degreeTobeRotated / 90; 				
				delay(gearInt * delaySeconds);
			}
			else if(-170 < degreeInt && degreeInt <  -115)
			{
				//moving right backward
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 0); //ON
				digitalWrite(MOTOR1_PIN2, 0); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 0); //ON
				digitalWrite(MOTOR2_PIN2, 1); //OFF

				degreeTobeRotated = (-degreeInt) - 90 ; 
				delaySeconds = 15 * degreeTobeRotated / 90; 				
				delay(gearInt * delaySeconds);
			}
			else if(-115 < degreeInt && degreeInt <  -75)
			{
				//moving backward
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 0); //ON
				digitalWrite(MOTOR1_PIN2, 1); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 0); //ON
				digitalWrite(MOTOR2_PIN2, 1); //OFF
				degreeTobeRotated = 0;
				delay(gearInt * 15 * 1);
			}
		}

		cout << "Robot is Moving on degree :: " << degreeInt << endl;
		cout << "Robot is Moving on direction :: " << direction << endl;
		
		digitalWrite(MOTOR1_PIN1, 0);
		digitalWrite(MOTOR1_PIN2, 0);
		digitalWrite(MOTOR2_PIN1, 0);
		digitalWrite(MOTOR2_PIN2, 0);
		
		return 1;
	} else {	
		return 0;
	}
	
}


std::string getAck(int response) {	
	std::string ack;

	getCPUUsage();	

	if(response){
		ack = "{\"id\":\""+id+"\",\"cpu\":\""+cpuUsage+"\",\"memory\":\""+memoryUsed+"\",\"temperature\":\"47\",\"battery\":\"12.2\",\"status\":\"completed\"}\n";
	}else{
		ack = "{\"id\":\""+id+"\",\"cpu\":\""+cpuUsage+"\",\"memory\":\""+memoryUsed+"\",\"temperature\":\"47\",\"battery\":\"12.2\",\"status\":\"failed\"}\n";
	}
	cout << "ack ::" << ack << endl;
	return ack;
}

int parseCommand(std::string command) {
	command = command.substr(1,command.length()-2);		
	cout << "Command is :: " << command << endl;
	std::string remaining = command;
	std::string degree = "";
	std::string radian = "";
	std::string gear = "";
	int count = 0;
	while(remaining.length()>0)	{
		int firstCommaLocation =  remaining.find(",");
		std::string item = "";
		if(firstCommaLocation > 0) {
			item = remaining.substr(0,firstCommaLocation-1);
			remaining = remaining.substr(firstCommaLocation+1, remaining.length());
		} else if (firstCommaLocation== -1)	{
			item = remaining;
			remaining = "";
		}

		int colanLocation =  item.find(":");
		if (colanLocation >0) {
			if(item.substr(1,colanLocation-2) == "direction")
				direction = item.substr(colanLocation+2,item.length());
			if(item.substr(1,colanLocation-2) == "id")
				id = item.substr(colanLocation+2,item.length());
			if(item.substr(1,colanLocation-2) == "securitykey"){
				cout << "security item ::" << item << endl;
				securitykey = item.substr(colanLocation+2,item.length()-3);
			}
			if(item.substr(1,colanLocation-2) == "radian"){
				cout << "radian item ::" << item << endl;
				radian = item.substr(colanLocation+2,item.length());
				cout << "radian  ::" << radian << endl;
				stringstream convInt(radian);
				convInt >> radianInt;
			}
			if(item.substr(1,colanLocation-2) == "degree"){
				cout << "item :: " << item << endl;				
				degree = item.substr(colanLocation+2,item.length());
				cout << "degree :: " << degree << endl;
				stringstream convInt(degree);
				convInt >> degreeInt;
			}
			if(item.substr(1,colanLocation-2) == "gear"){
				cout << "gear gear ::" << item << endl;
				gear = item.substr(colanLocation+2,item.length());
				cout << "gear ::" << gear << endl;
				stringstream convInt(gear);
				convInt >> gearInt;
			}
		}
	}

	cout << "ID :"<< id << endl;
	cout << "Security :"<< securitykey << endl;
	cout << "degreeInt :"<< degreeInt << endl;
	cout << "gearInt :"<< gearInt << endl;
	cout << "radianInt :"<< radianInt << endl;
return 1;
}

void getIPAddress(char* outStr){
	char ip_address[15];
    int fd;
    struct ifreq ifr;
    
    /*AF_INET - to define network interface IPv4*/
    /*Creating soket for it.*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;
    
    /*eth0 - define the ifr_name - port name
    where network attached.*/
    memcpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    
    /*Accessing network interface information by
    passing address using ioctl.*/
    ioctl(fd, SIOCGIFADDR, &ifr);
    /*closing fd*/
    close(fd);
    
    /*Extract IP Address*/
    strcpy(ip_address,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    
    printf("System IP Address is: %s\n",ip_address);
    	
	for(int i=0; i < 15; ++i){
		outStr[i] = ip_address[i];
	}
}

void getCPUUsage(){

	sysinfo (&memInfo);

	long long totalPhysMem = memInfo.totalram;
	//Multiply in next statement to avoid int overflow on right hand side...
	totalPhysMem *= memInfo.mem_unit;

	long long physMemUsed = memInfo.totalram - memInfo.freeram;
	//Multiply in next statement to avoid int overflow on right hand side...
	physMemUsed *= memInfo.mem_unit;

	cout << "memory usage :: " << (physMemUsed*100)/(totalPhysMem) << endl;

	std::stringstream cpuUse;
	cpuUse << getCurrentValue();
	cpuUse >> cpuUsage;

	std::stringstream memUsed;
	memUsed << (physMemUsed*100)/(totalPhysMem);
	memUsed >> memoryUsed;

	cout << "CPU Usage ::" << getCurrentValue() << endl;

}

double getCurrentValue(){
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}

