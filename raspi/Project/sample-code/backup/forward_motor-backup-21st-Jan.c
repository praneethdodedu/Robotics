//This file will rotate the motor forward
#include<stdio.h>
#include<wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

//GPIO  pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//GPIO  pin wiringPi 1, GPIO pin 12, BCM 18
#define MOTOR1_PIN2 1

//wiringPi 2, GPIO 13, BCM 27
#define MOTOR2_PIN1 2

//wiringPi 3, GPIO 15, BCM 22
#define MOTOR2_PIN2 3

void applyBreak(int count);

int main(int argc, char *argv[])
{
  printf("Starting\r\n");

//Server code starts
 int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
    while(1){
		bzero(buffer,256);
 		n = read(newsockfd,buffer,255);
       		if (n < 0) error("ERROR reading from socket");
		if(strcmp(buffer,"")==0){
                  n = read(newsockfd,buffer,255);	
		}
                printf("Here is the message: %s\n",buffer);
		n = write(newsockfd,"success",7);
		if (n < 0) error("ERROR writing to socket");	
		if(strstr(buffer, "forward") != NULL){
		  printf("Forward\n");
		  wiringPiSetup();

		  pinMode(MOTOR1_PIN1, OUTPUT);
		  pinMode(MOTOR1_PIN2, OUTPUT);
		  pinMode(MOTOR2_PIN1, OUTPUT);
		  pinMode(MOTOR2_PIN2, OUTPUT);

		  for(int run_time = 0; run_time <= 2; run_time++)
		  {
			// Left Motor
			digitalWrite(MOTOR1_PIN1, 1); //ON
			digitalWrite(MOTOR1_PIN2, 0); //OFF
			// Right Motor
			digitalWrite(MOTOR2_PIN1, 1); //ON
			digitalWrite(MOTOR2_PIN2, 0); //OFF
			delay(150);

			digitalWrite(MOTOR1_PIN1, 0);
			digitalWrite(MOTOR1_PIN2, 0);
			digitalWrite(MOTOR2_PIN1, 0);
			digitalWrite(MOTOR2_PIN2, 0);
			delay(350);
		  }
		  digitalWrite(MOTOR1_PIN1, 0);
		  digitalWrite(MOTOR1_PIN2, 0);
		  digitalWrite(MOTOR2_PIN1, 0);
		  digitalWrite(MOTOR2_PIN2, 0);
		  continue;		

		}

		if(strstr(buffer, "reverse") != NULL){
       		  printf("Reverse\n");
			  wiringPiSetup();

			  pinMode(MOTOR1_PIN1, OUTPUT);
			  pinMode(MOTOR1_PIN2, OUTPUT);
			  pinMode(MOTOR2_PIN1, OUTPUT);
			  pinMode(MOTOR2_PIN2, OUTPUT);

			  for(int run_time = 0; run_time <= 2; run_time++)
			  {
				//Left Motor
				digitalWrite(MOTOR1_PIN1, 0); //ON
				digitalWrite(MOTOR1_PIN2, 1); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 0); //ON
				digitalWrite(MOTOR2_PIN2, 1); //OFF
				delay(150);

				digitalWrite(MOTOR1_PIN1, 0);
				digitalWrite(MOTOR1_PIN2, 0);
				digitalWrite(MOTOR2_PIN1, 0);
				digitalWrite(MOTOR2_PIN2, 0);
				delay(350);
			  }
			  digitalWrite(MOTOR1_PIN1, 0);
			  digitalWrite(MOTOR1_PIN2, 0);
			  digitalWrite(MOTOR2_PIN1, 0);
			  digitalWrite(MOTOR2_PIN2, 0);
			
		}
     }

//applyBreak(count);
 return 0;
}

void applyBreak(int count){

    wiringPiSetup();

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  for(int run_time = 0; run_time <= 20; run_time++)
  {
    //Left Motor
    digitalWrite(MOTOR1_PIN1, 0); //ON
    digitalWrite(MOTOR1_PIN2, 1); //OFF
    //Right Motor
    digitalWrite(MOTOR2_PIN1, 0); //ON
    digitalWrite(MOTOR2_PIN2, 1); //OFF
    delay(10);

    //digitalWrite(MOTOR1_PIN1, 0);
    //digitalWrite(MOTOR1_PIN2, 0);
    //digitalWrite(MOTOR2_PIN1, 0);
    //digitalWrite(MOTOR2_PIN2, 0);
    //delay(10);

  }
  digitalWrite(MOTOR1_PIN1, 0);
  digitalWrite(MOTOR1_PIN2, 0);
  digitalWrite(MOTOR2_PIN1, 0);
  digitalWrite(MOTOR2_PIN2, 0);
}
