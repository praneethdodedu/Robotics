#include <iostream>
#include "pyembed.h"
#include<stdio.h>
#include<wiringPi.h>
#include <sstream>

//GPIO  pin wiringPi 0, GPIO pin 11 , BCM 17
#define MOTOR1_PIN1 0
//GPIO  pin wiringPi 1, GPIO pin 12, BCM 18
#define MOTOR1_PIN2 1

//wiringPi 2, GPIO 13, BCM 27
#define MOTOR2_PIN1 2

//wiringPi 3, GPIO 15, BCM 22
#define MOTOR2_PIN2 3

void applyBreak();
void moveRobotForward(int argc, char** argv);
std::string getMagnetoReading(int argc, char** argv);

int FIXED_DEGREE;
int CURRENT_DEGREE;
int chatter_count = 0 ;

using namespace pyembed; // for brevity
using namespace std;
int main(int argc, char** argv)
{
  	std::string reading  = getMagnetoReading(argc, argv);
	stringstream fixed_degree_str(reading);
    fixed_degree_str >> FIXED_DEGREE;
    CURRENT_DEGREE = FIXED_DEGREE;
	moveRobotForward(argc, argv);
  return 0;
}


std::string getMagnetoReading(int argc, char** argv){
try // always check for Python_exceptions
    {

      // Create an instance of the interpreter.
      Python py(argc, argv);
      // Load the test module into the interpreter
      py.load("magneto_reading");
      // value returned by python is stored here
      string ret;
      // our argument list that maps string values
      // to their python types
      Arg_map args;

      // fill up argument strings and their types.
      //args["10"] = Py_long;
      //["20"] = Py_long;

      // make the call
      py.call("magneto_reading", args, ret);

      // will print 200
      //std::cout << "Magento-Reading ::" << ret << '\n';

      return ret;
    }
  catch (Python_exception ex)
    {
	  cout << "Exception :: " << endl;
      std::cout << ex.what();
    }
}

void moveRobotForward(int argc, char** argv){

	wiringPiSetup();
	pinMode(MOTOR1_PIN1, OUTPUT);
	pinMode(MOTOR1_PIN2, OUTPUT);
	pinMode(MOTOR2_PIN1, OUTPUT);
	pinMode(MOTOR2_PIN2, OUTPUT);

	int count = 0;
	int degDiffAbs = 0;

	int timeSpent = 0;
  int direction = 0;
      do{
  			if(direction == 0)
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
        std::string reading  = getMagnetoReading(argc, argv);
  		  stringstream current_degree_str(reading);
      	current_degree_str >> CURRENT_DEGREE;

        //cout << "Reading :: " << reading << endl;

        degDiffAbs = std::abs(FIXED_DEGREE-CURRENT_DEGREE);
       if((CURRENT_DEGREE > FIXED_DEGREE) && (degDiffAbs>0.2))
         direction = 1;
       else if((CURRENT_DEGREE < FIXED_DEGREE) && (degDiffAbs>0.2))
         direction = -1;
       timeSpent ++;
       //cout << "timeSpent :: " << timeSpent << endl;
    }while(timeSpent<=200);
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
