# -*- coding: utf-8 -*
import serial
import logging


logging.basicConfig(filename='logfile-new.log',level=logging.DEBUG)
ser = serial.Serial("/dev/ttyAMA0", 115200)

def getTFminiData():
    while True:
        try:
            count = ser.in_waiting
            recv = ser.read(1)
            ser.reset_input_buffer()
            print "recv",recv
		#ser.write("recieved")
        except KeyboardInterrupt:   # Ctrl+C
            break
        except Exception as exception:
            excThrown = "Exception :: " + str(exception)
            print excThrown
            logging.info(excThrown)
            break



if __name__ == '__main__':
    try:
        if ser.is_open == False:
            ser.open()
        getTFminiData()
    except KeyboardInterrupt:   # Ctrl+C
        if ser != None:
            ser.close()
