#include <stdio.h>
#include <pigpio.h>

int main(int argc, char *argv[])
{
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
   
   //forward right motor 
   gpioPWM(17, 200); 
   // forward left motor
   gpioPWM(27, 255); 
   start = time_time();
   while ((time_time() - start) < 10.0)
   {
      printf("inside :: \n");
      gpioWrite(18, 0); 
      time_sleep(0.25);
      gpioWrite(22, 0); 
      time_sleep(0.25);
   }
   gpioWrite(17, 0); 
   gpioWrite(27, 0); 
   gpioTerminate();

   return 0;
}
