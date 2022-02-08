#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<iostream>
#include <wiringSerial.h>

using namespace std;

int main (int argc, char **argv)
{

	int fd;
	int distance;
	int low;
	int high;
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	while (1)
	{
		cout << "received :: " << serialGetchar(fd)<< endl;
	fflush (stdout) ;
	}
	return 0;
}
