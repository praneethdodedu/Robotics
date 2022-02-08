#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h> 
#include<sstream>
#include<iostream>
#include<wiringPi.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include<cstring>
#include "sys/sysinfo.h"
#include <jsoncpp/json/json.h>

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

std::string status;
std::string cpuUsage;
std::string memoryUsed;
std::string id = "";
std::string status_msg;
struct sysinfo memInfo;

std::string getAck(int response);
std::string getId(std::string command);
std::string getStatus(std::string command);
int checkInternet();
void ReadXBytes(int socket, unsigned int x, void* buffer);
void getIPAddress(char* outStr);
double getCurrentValue();
void getCPUUsage();

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


void chatterCallbackAcknowledgment(const std_msgs::String::ConstPtr& ros_msg)
{
  ROS_INFO("Message from Acknowledgment topic: [%s]", ros_msg->data.c_str());
  status_msg = ros_msg->data.c_str();
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "robot_command_sender");
  ros::NodeHandle nH;
  ros::Publisher chatter_pub = nH.advertise<std_msgs::String>("robot_command", 1000);

  ros::Subscriber sub = nH.subscribe("acknowledgment_topic", 1000, chatterCallbackAcknowledgment);

  ros::Rate loop_rate(10);

  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

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
		
				std_msgs::String ros_str;
				ros_str.data = command;
				ROS_INFO("Hello World :: %s", ros_str.data.c_str());
				id = getId(command);
				chatter_pub.publish(ros_str);
				ros::spinOnce();
				while(true){
					if(status_msg != "" && id == getId(status_msg) && getStatus(status_msg) == "completed"){
						ack = getAck(1);
						n = write(sockfd,ack.c_str(),strlen(ack.c_str()));
						if (n < 0){ 
							 cout << "ERROR writing acknowledgement to socket " << endl;
							 error("ERROR writing to socket");
						}
						break;
					}else if(status_msg != "" && id == getId(status_msg) && getStatus(status_msg) == "failure"){
						ack = getAck(0);
						n = write(sockfd,ack.c_str(),strlen(ack.c_str()));
						if (n < 0) {
							 cout << "ERROR writing acknowledgement to socket " << endl;
							 error("ERROR writing to socket");
						}
						break;
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

std::string getAck(int response) {	
	std::string ack;
	getCPUUsage();	
	if(response){
		ack = "{\"id\":\""+id+"\",\"cpu\":\""+cpuUsage+"\",\"memory\":\""+memoryUsed+"\",\"temperature\":\"47\",\"battery\":\"12.2\",\"status\":\"completed\"}\n";
	}else{
		ack = "{\"id\":\""+id+"\",\"cpu\":\""+cpuUsage+"\",\"memory\":\""+memoryUsed+"\",\"temperature\":\"47\",\"battery\":\"12.2\",\"status\":\"failed\"}\n";
	}
	cout << "acknowledgement ::" << ack << endl;
	return ack;
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

std::string getId(std::string command) {
	Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( command, root );
    if ( !parsingSuccessful )
    {
        cout << "Error parsing the command" << endl;
    }
	return root["id"].asString();
}

string getStatus(std::string command) {
	Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( command, root );
    if ( !parsingSuccessful )
    {
        cout << "Error parsing the command" << endl;
    }
	return root["status"].asString();
}
