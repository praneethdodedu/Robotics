#include <iostream>
#include "pyembed.h"
#include<stdio.h>
#include<wiringPi.h>
#include <sstream>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

//GPIO  pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//GPIO  pin wiringPi 1, GPIO pin 12, BCM 18
#define MOTOR1_PIN2 1

//wiringPi 2, GPIO 13, BCM 27
#define MOTOR2_PIN1 2

//wiringPi 3, GPIO 15, BCM 22
#define MOTOR2_PIN2 3

void applyBreak();
void moveRobotForward();
float getMagnetoReading();
int getLidarData();
int read_reg16(int fd, int reg);

float FIXED_DEGREE;
float CURRENT_DEGREE;
int chatter_count = 0 ;
int fd;
int fdSerial;

using namespace std;
int main(int argc, char** argv)
{
	if ((fdSerial = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

    fd = wiringPiI2CSetup(0x0d);
    wiringPiI2CWriteReg16 (fd, 11, 0b01110000) ;
    wiringPiI2CWriteReg16 (fd, 10, 0b00100000) ;
    wiringPiI2CWriteReg16 (fd, 9, 0b00011101) ;

  	FIXED_DEGREE  = getMagnetoReading();
    cout << "fixed degree : " << FIXED_DEGREE << endl;
    CURRENT_DEGREE = FIXED_DEGREE;
	  moveRobotForward();
  return 0;
}


float getMagnetoReading(){

    float scale = 0.92;
    int x_offset = -10;
    int y_offset = 10;
    float x_out = (read_reg16(fd,0)- x_offset+2) * scale  ;
    float y_out = (read_reg16(fd,2)- y_offset+2)* scale;
    float z_out = read_reg16(fd, 4) * scale;
    //int bearing = math.atan2(y_out, x_out)+.48;

    float bearing = atan2(y_out, x_out)+.48;
      if(bearing < 0)
          bearing += 2* M_PI;

    float heading = (180 * bearing/M_PI);
    if(heading < 0)
     heading += 360;
return heading;
}

void moveRobotForward(){

	wiringPiSetup();
	pinMode(MOTOR1_PIN1, OUTPUT);
	pinMode(MOTOR1_PIN2, OUTPUT);
	pinMode(MOTOR2_PIN1, OUTPUT);
	pinMode(MOTOR2_PIN2, OUTPUT);

	int count = 0;
	float degDiffAbs = 0;

	int timeSpent = 0;
  int direction = 0;
      do{
			int distance = getLidarData();
			if(distance < 50){
				// stop the motor
				digitalWrite(MOTOR1_PIN1, 0); //ON
				digitalWrite(MOTOR1_PIN2, 0); //OFF
				//Right Motor
				digitalWrite(MOTOR2_PIN1, 0); //ON
				digitalWrite(MOTOR2_PIN2, 0); //OFF
				cout << "stopping ::" << endl;
			}
			else if(direction == 0)
  			{
  				//Left Motor
  				digitalWrite(MOTOR1_PIN1, 1); //ON
  				digitalWrite(MOTOR1_PIN2, 0); //OFF
  				//Right Motor
  				digitalWrite(MOTOR2_PIN1, 1); //ON
  				digitalWrite(MOTOR2_PIN2, 0); //OFF
          //delay(10000);
  			}
  			else if(direction == 1)
  			{
          //moving right forward
  				//Left Motor
  				digitalWrite(MOTOR1_PIN1, 0); //ON
  				digitalWrite(MOTOR1_PIN2, 0); //OFF
  				//Right Motor
  				digitalWrite(MOTOR2_PIN1, 1); //ON
  				digitalWrite(MOTOR2_PIN2, 0); //OFF
  			}
  			else if(direction == -1)
  			{
          //moving left forward
  				//Left Motor
  				digitalWrite(MOTOR1_PIN1, 1); //ON
  				digitalWrite(MOTOR1_PIN2, 0); //OFF
  				//Right Motor
  				digitalWrite(MOTOR2_PIN1, 0); //ON
  				digitalWrite(MOTOR2_PIN2, 0); //OFF
  			}
        CURRENT_DEGREE  = getMagnetoReading();
        //cout << "Reading :: " << reading << endl;
        cout << "fixed degree : " << CURRENT_DEGREE << endl;

        degDiffAbs = fabs(FIXED_DEGREE-CURRENT_DEGREE);
        cout << "Difference ::" << degDiffAbs << endl;
       if((CURRENT_DEGREE > FIXED_DEGREE) && (degDiffAbs>0.5))
         direction = 1;
       else if((CURRENT_DEGREE < FIXED_DEGREE) && (degDiffAbs>0.5))
         direction = -1;
       timeSpent ++;
       //cout << "timeSpent :: " << timeSpent << endl;
    }while(timeSpent<=700);
      digitalWrite(MOTOR1_PIN1, 0);
      digitalWrite(MOTOR1_PIN2, 0);
      digitalWrite(MOTOR2_PIN1, 0);
      digitalWrite(MOTOR2_PIN2, 0);
      delay(1);
      //applyBreak();
		}


void applyBreak(){
  wiringPiSetup();
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  for(int run_time = 0; run_time <= 10; run_time++)
  {
    //Left Motor
    digitalWrite(MOTOR1_PIN1, 0); //ON
    digitalWrite(MOTOR1_PIN2, 1); //OFF
    //Right Motor
    digitalWrite(MOTOR2_PIN1, 0); //ON
    digitalWrite(MOTOR2_PIN2, 1); //OFF
    delay(10);
   }
  digitalWrite(MOTOR1_PIN1, 0);
  digitalWrite(MOTOR1_PIN2, 0);
  digitalWrite(MOTOR2_PIN1, 0);
  digitalWrite(MOTOR2_PIN2, 0);
}

int read_reg16(int fd, int reg){
			int val = wiringPiI2CReadReg16(fd,reg);
			if (val>= 0x8000)
					return -((65535 - val)+1);
			else
					return val;
}

int getLidarData(){
	int distance;
	int low;
	int high;
	while (true)
	{
		if(char(serialGetchar(fdSerial))=='Y'){
			if(char(serialGetchar(fdSerial))=='Y'){
				low = serialGetchar(fdSerial);
				high = serialGetchar(fdSerial);
				distance = low + high * 256;
				cout << 	"Distance :: " << distance << endl;
				break;
			}
		}
	fflush (stdout) ;
	}
	return distance;
}
