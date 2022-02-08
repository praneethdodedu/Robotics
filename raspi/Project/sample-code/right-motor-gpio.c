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
   
   // left motor reverse
   gpioPWM(22, 255); 
   // right motor forward
   gpioPWM(17, 255); 
   start = time_time();
   while ((time_time() - start) < 1.0)
   {
      printf("inside :: \n");
      gpioWrite(18, 0); 
      time_sleep(0.5);
      gpioWrite(27, 0); 
      time_sleep(0.5);
   }
   gpioWrite(17, 0); 
   gpioWrite(22, 0); 
   gpioTerminate();

   return 0;
}
