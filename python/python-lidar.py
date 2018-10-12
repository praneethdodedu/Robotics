# -*- coding: utf-8 -*
import serial
import logging


logging.basicConfig(filename='logfile-new.log',level=logging.DEBUG)
ser = serial.Serial("/dev/ttyAMA0", 115200)

def getTFminiData():
    while True:
        try:
            count = ser.in_waiting
            if count > 8:
                recv = ser.read(9)
                ser.reset_input_buffer()
                if recv[0] == 'Y' and recv[1] == 'Y': # 0x59 is 'Y'
                    low = int(recv[2].encode('hex'), 16)
                    high = int(recv[3].encode('hex'), 16)
                    distance = low + high * 256
                    print "distance:: ", distance
                    low_sig = int(recv[4].encode('hex'), 16)
                    high_sig = int(recv[5].encode('hex'), 16)
                    strength = low_sig + high_sig * 256
                    print "strength:: ",strength
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
