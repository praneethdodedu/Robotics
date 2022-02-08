//This file will rotate the motor forward
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pigpio.h>
#include <iostream> 
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
  printf("Starting\r\n");
  double start;
  if (gpioInitialise() < 0)
  {
    fprintf(stderr, "pigpio initialisation failed\n");
    return 1;
  }
  /* Set GPIO modes */
  gpioSetMode(17, PI_OUTPUT);
  gpioSetMode(18, PI_OUTPUT);
  gpioSetMode(27, PI_OUTPUT);
  gpioSetMode(22, PI_OUTPUT);
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
    cerr <<"ERROR opening socket" << endl;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      cerr << "ERROR on binding" << endl;
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0)
    cerr << "ERROR on accept" << endl;
  while(1){
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) cerr << "ERROR reading from socket" << endl;
    if(strcmp(buffer,"")==0){
      n = read(newsockfd,buffer,255);
    }
    printf("Here is the message: %s\n",buffer);
    if(strstr(buffer, "left") != NULL){
      printf("Moving left\n");
      string str(buffer);
      int left_factor = 0; 
      stringstream geek(str.substr(str.find(":")+1, str.find(",")));
      geek >> left_factor; 
      cout << "geek left::" << left_factor << endl;	
      /* Start 75% dutycycle PWM on GPIO17 */
      gpioPWM(17, 50+left_factor/10); /* 192/255 = 75% */
      gpioPWM(27, 0); /* 192/255 = 75% */
      start = time_time();
      while ((time_time() - start) < 1.0)
      {
        gpioWrite(18, 0);
        time_sleep(0.5);
        gpioWrite(22, 0);
        time_sleep(0.5);
      }
      gpioPWM(17, 0);
      gpioPWM(27, 0);
    } else if(strstr(buffer, "right") != NULL){
      printf("Moving Right\n");
      string str(buffer);
      int right_factor = 0; 
      stringstream geek(str.substr(str.find(":")+1, str.find(",")));
      geek >> right_factor; 
      cout << "geek right::" << right_factor << endl;
      /* Start 75% dutycycle PWM on GPIO17 */
      gpioPWM(17, 0); /* 192/255 = 75% */
      gpioPWM(27, 50+right_factor/10); /* 192/255 = 75% */
      start = time_time();
      while ((time_time() - start) < 1.0)
      {
        gpioWrite(18, 0);
        time_sleep(0.5);
        gpioWrite(22, 0);
        time_sleep(0.5);
      }
      gpioPWM(17, 0);
      gpioPWM(27, 0);
    }
    if(strstr(buffer, "forward") != NULL){
      printf("Moving forward\n");
      gpioPWM(17, 140); /* 192/255 = 75% */
      gpioPWM(27, 192); /* 192/255 = 75% */
      start = time_time();
      while ((time_time() - start) < 1.0)
      {
        gpioWrite(18, 0);
        time_sleep(0.5);
        gpioWrite(22, 0);
        time_sleep(0.5);
      }
      gpioPWM(17, 0);
      gpioPWM(27, 0);
    } else if(strstr(buffer, "reverse") != NULL){
      printf("Moving Reverse\n");
      /* Start 75% dutycycle PWM on GPIO17 */
      gpioPWM(18, 192); /* 192/255 = 75% */
      gpioPWM(22, 202); /* 192/255 = 75% */
      start = time_time();
      while ((time_time() - start) < 1.0)
      {
        gpioWrite(17, 0);
        time_sleep(0.5);
        gpioWrite(27, 0);
        time_sleep(0.5);
      }
      gpioWrite(18, 0);
      gpioWrite(22, 0);
    }
    n = write(newsockfd,"success",7);
    if (n < 0) {
	cerr << "ERROR writing to socket" << endl;
	break;
    }
  }
  gpioTerminate();
  return 0;
}
