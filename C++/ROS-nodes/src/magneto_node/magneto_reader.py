#!/usr/bin/env python
## This Publisher will send the degree data from magneto meter into magneto_topic over roscore

import rospy
from std_msgs.msg import String
import smbus
import time
import math
bus = smbus.SMBus(1)
address = 0x0d

def talker():
    pub = rospy.Publisher('magneto_topic', String, queue_size=10)
    rospy.init_node('magneto_node', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        list = []
        for i in range(0,20):
            def read_byte(adr): #communicate with compass
                return bus.read_byte_data(address, adr)

            def read_word(adr):
                low = bus.read_byte_data(address, adr)
                high = bus.read_byte_data(address, adr+1)
                #high = bus.read_byte_data(address, adr)
                #low = bus.read_byte_data(address, adr+1)
                val = (high<< 8) + low
                return val

            def read_word_2c(adr):
                val = read_word(adr)
                if (val>= 0x8000):
                    return -((65535 - val)+1)
                else:
                    return val

            def write_byte(adr,value):
                bus.write_byte_data(address, adr, value)
            write_byte(11, 0b01110000)
            write_byte(10, 0b00100000)
            write_byte(9, 0b00011101)
            scale = 0.92
            x_offset = -10
            y_offset = 10
            x_out = (read_word_2c(0)- x_offset+2) * scale  #calculating x,y,z coordinates
            y_out = (read_word_2c(2)- y_offset+2)* scale
            z_out = read_word_2c(4) * scale
            bearing = math.atan2(y_out, x_out)+.48  #0.48 is correction value
            if(bearing < 0):
                bearing += 2* math.pi
            print "Bearing:", math.degrees(bearing)
            #print "x: ", x_out
            #print "y: ", y_out
            #print "z: ", z_out
            list.append(math.degrees(bearing))
            #time.sleep(0.1)
        #print list
        degree_str = str(reduce(lambda x, y: x + y, list) / len(list))
        degree = "degree which robot turned: %s" % degree_str
        rospy.loginfo(degree)
        pub.publish(degree_str)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
