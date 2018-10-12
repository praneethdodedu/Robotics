#include <iostream>
#include <errno.h>
#include <wiringPiI2C.h>
#include <math.h>

using namespace std;

int read_reg16(int fd, int reg);

int main()
{
   int fd, result;
   fd = wiringPiI2CSetup(0x0d);

   cout << "Init result: "<< fd << endl;

	 int res1 = wiringPiI2CWriteReg16 (fd, 11, 0b01110000) ;
	 int res2 = wiringPiI2CWriteReg16 (fd, 10, 0b00100000) ;
	 int res3 = wiringPiI2CWriteReg16 (fd, 9, 0b00011101) ;

	 	 float scale = 0.92;
     int x_offset = -10;
     int y_offset = 10;
     float x_out = (read_reg16(fd,0)- x_offset+2) * scale  ;
		 cout << "X :: " << x_out << endl;
     float y_out = (read_reg16(fd,2)- y_offset+2)* scale;
		 cout << "Y :: " << y_out << endl;
     float z_out = read_reg16(fd, 4) * scale;
		 cout << "Z :: " << z_out << endl;
     //int bearing = math.atan2(y_out, x_out)+.48;

		 float bearing = atan2(y_out, x_out)+.48;
	     if(bearing < 0)
	         bearing += 2* M_PI;

		 float heading = (180 * bearing/M_PI);
		 if(heading < 0)
 			heading += 360;
	 cout << "heading ::  " << heading << endl;

}

int read_reg16(int fd, int reg){
			int val = wiringPiI2CReadReg16(fd,reg);
			if (val>= 0x8000)
					return -((65535 - val)+1);
			else
					return val;
}
