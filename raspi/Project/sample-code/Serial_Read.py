#! /usr/bin/python
import serial
import string
import time

# Raspberry Pi GPIO Serial Port settings
rpiCOM = '/dev/ttyAMA0'
baud = 9400
xtimes = 0
inbuff = 0
msgCOM = ""

# Setup - if serial port can't be open an Exception will be raised
while True:
    try:
        print 'Opening port'
        ser = serial.Serial(rpiCOM, baud, timeout=0, stopbits=1)
        #time.sleep(10)
        print 'Port OPEN'
        # go out of while loop when connection is made
        break

    except serial.SerialException:
        print 'COM port ' + rpiCOM + ' not available. Wait...'
        time.sleep(3)

# Get input from serial buffer
while True:
    try:
        str = ""
        print " Ready to check inbuff: " + msgCOM
        while 1:
         inbuff = ser.inWaiting()
         if inbuff > 0:
           msgCOM = ser.read(1)

           if msgCOM != '':
                xtimes += 1
                print xtimes
                print "PIC Says: " + msgCOM
                print "inWaiting: %d" %inbuff
                ser.flushInput()
                inbuff = ser.inWaiting()
                print "Clear inbuff: %d" %inbuff
                ser.write('$OK')
                msgCOM = ""
                break
           else:
                print "NOK Message: " + msgCOM

         else:

          # print "Buffer empty:  " + msgCOM
           time.sleep(1)
        
    except serial.serialutil.SerialException:
        print "Serial Exception raised"
        pass
