//This file will rotate the motor forward 
#include<stdio.h>
#include<wiringPi.h>

//GPIO  pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//GPIO  pin wiringPi 1, GPIO pin 12, BCM 18 
#define MOTOR1_PIN2 1

//wiringPi 2, GPIO 13, BCM 27
#define MOTOR2_PIN1 2 

//wiringPi 3, GPIO 15, BCM 22
#define MOTOR2_PIN2 3

int main(void)
{
  printf("Starting\r\n");
  
  wiringPiSetup();

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  for(int run_time = 0; run_time <= 50; run_time++)
  { 
    //Left Motor
    digitalWrite(MOTOR1_PIN1, 1); //ON
    digitalWrite(MOTOR1_PIN2, 0); //OFF
    //Right Motor
    digitalWrite(MOTOR2_PIN1, 1); //ON
    digitalWrite(MOTOR2_PIN2, 0); //OFF
    delay(10);
     
    //Left Motor
    digitalWrite(MOTOR1_PIN1, 0); //OFF
    digitalWrite(MOTOR1_PIN2, 0); //OFF
    //Right Motor
    digitalWrite(MOTOR2_PIN1, 0); //OFF
    digitalWrite(MOTOR2_PIN2, 0); //OFF
    delay(10);

  }

  return 0;
}
