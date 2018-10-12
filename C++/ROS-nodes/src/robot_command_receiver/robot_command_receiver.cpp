#include "ros/ros.h"
#include "std_msgs/String.h"
#include<iostream>
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

void applyBreak();
void moveRobot();

using namespace std;

int FIXED_DEGREE;
int CURRENT_DEGREE;
int chatter_count = 0 ;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from Magneto-meter: [%s]", msg->data.c_str());
  char * pEnd;
  if(chatter_count == 0){
	stringstream fixed_degree_str(msg->data.c_str());
    fixed_degree_str >> FIXED_DEGREE;
    CURRENT_DEGREE = FIXED_DEGREE;
  } else {
	stringstream current_degree_str(msg->data.c_str());
    current_degree_str >> CURRENT_DEGREE;
  }
  cout << CURRENT_DEGREE << endl;
  cout << FIXED_DEGREE << endl;
 chatter_count++;
}

void chatterCallbackSecond(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Message from Robot: [%s]", msg->data.c_str());
  //for(int i=0;i<1000;i++){
  //  cout << "i am inside the loop :: " << endl;
  //  sleep(1);
  //}
  moveRobot();
}

int main2(int argc, char **argv)
{

  cout << "This is the start" << endl;
  ros::init(argc, argv, "robot_command_receiver");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("magneto_topic", 1000, chatterCallback);

  for(int i = 0;i<3;i++)
    moveRobot();
  return 0;
}

int main(int argc, char **argv)
{
  cout << "This is the start" << endl;
  ros::init(argc, argv, "robot_command_receiver");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("magneto_topic", 1000, chatterCallback);

  ros::NodeHandle n2;

  ros::Subscriber sub_2 = n.subscribe("robot_command", 1000, chatterCallbackSecond);

  cout << "This is the end" << endl;
  ros::MultiThreadedSpinner spinner(4); // Use 4 threads
  spinner.spin(); // spin() will not return until the node has been shutdown
  return 0;
}

void moveRobot(){

	wiringPiSetup();
	pinMode(MOTOR1_PIN1, OUTPUT);
	pinMode(MOTOR1_PIN2, OUTPUT);
	pinMode(MOTOR2_PIN1, OUTPUT);
	pinMode(MOTOR2_PIN2, OUTPUT);

	int count = 0;
	int degDiffAbs = 0;
	int ocsillationLevel = 1;

	int timeSpent = 0;
  int direction = 0;
      do
    	{

      for(int run_time = 0; run_time <= 50; run_time++)
      {

        degDiffAbs = std::abs(FIXED_DEGREE-CURRENT_DEGREE);
       if((CURRENT_DEGREE > FIXED_DEGREE) && (degDiffAbs>5))
         direction = 1;
       else if((CURRENT_DEGREE < FIXED_DEGREE) && (degDiffAbs>5))
         direction = -1;

       cout << "FIXED_DEGREE :: " << FIXED_DEGREE << endl;
       cout << "CURRENT_DEGREE :: " << CURRENT_DEGREE << endl;
       cout << "direction is ::" << direction << endl;
       cout << "degDiffAbs is ::" << degDiffAbs << endl;

  			if(direction == 0)
  			{
  				//Left Motor
  				digitalWrite(MOTOR1_PIN1, 1); //ON
  				digitalWrite(MOTOR1_PIN2, 0); //OFF
  				//Right Motor
  				digitalWrite(MOTOR2_PIN1, 1); //ON
  				digitalWrite(MOTOR2_PIN2, 0); //OFF
          delay(8);

          digitalWrite(MOTOR1_PIN1, 0);
    			digitalWrite(MOTOR1_PIN2, 0);
    			digitalWrite(MOTOR2_PIN1, 0);
    			digitalWrite(MOTOR2_PIN2, 0);
    			delay(5);
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
          delay(8);

          digitalWrite(MOTOR1_PIN1, 0);
    			digitalWrite(MOTOR1_PIN2, 0);
    			digitalWrite(MOTOR2_PIN1, 0);
    			digitalWrite(MOTOR2_PIN2, 0);
    			delay(1);
  			}
  			else
  			{
          //moving left forward
  				//Left Motor
  				digitalWrite(MOTOR1_PIN1, 1); //ON
  				digitalWrite(MOTOR1_PIN2, 0); //OFF
  				//Right Motor
  				digitalWrite(MOTOR2_PIN1, 0); //ON
  				digitalWrite(MOTOR2_PIN2, 0); //OFF
          delay(9);

          digitalWrite(MOTOR1_PIN1, 0);
    			digitalWrite(MOTOR1_PIN2, 0);
    			digitalWrite(MOTOR2_PIN1, 0);
    			digitalWrite(MOTOR2_PIN2, 0);
    			delay(1);
  			}

  			int delaySeconds = 15 * degDiffAbs / 90;
  			delaySeconds = (delaySeconds/ocsillationLevel<5)?5:delaySeconds/ocsillationLevel;
  			//delay(gearInt * delaySeconds);
        cout << "delaySeconds ::" << delaySeconds << endl;

  			timeSpent += (delaySeconds +5);
        cout << "timeSpent ::" << timeSpent << endl;

      }
    	}while(timeSpent<=5000);
       degDiffAbs = std::abs(FIXED_DEGREE-CURRENT_DEGREE);
      if((CURRENT_DEGREE > FIXED_DEGREE) && (degDiffAbs>5))
        direction = 1;
      else if((CURRENT_DEGREE < FIXED_DEGREE) && (degDiffAbs>5))
        direction = -1;
        cout << "After command ::" << endl;
      cout << "direction is ::" << direction << endl;
      cout << "degDiffAbs is ::" << degDiffAbs << endl;
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
