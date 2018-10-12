//This file will test the GPIO pin 11 (BCM 17) and wiringPi 0
#include<stdio.h>
#include<wiringPi.h>

//Test pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//Test pin wiringPi 1, GPIO pin 12, BCM 18 
#define MOTOR1_PIN2 1

int main(void)
{
  printf("Starting\r\n");
  
  wiringPiSetup();

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);

  for(;;)
  {
    digitalWrite(MOTOR1_PIN1, 1); //ON
    digitalWrite(MOTOR1_PIN2, 0); //OFF
    while(1);
    delay(1);
    digitalWrite(MOTOR1_PIN1, 0); //OFF
    digitalWrite(MOTOR1_PIN2, 0); //OFF
    delay(1);
    //while(1);
    //delay(1500);
    //digitalWrite(MOTOR1_PIN1, 0); //OFF
    //delay(1500);
  }

  return 0;
}
