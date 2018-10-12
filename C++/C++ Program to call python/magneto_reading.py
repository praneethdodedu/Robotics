import smbus
import time
import math
bus = smbus.SMBus(1)
address = 0x0d

def magneto_reading():
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
		#print "Bearing:", math.degrees(bearing)
		#print "x: ", x_out
		#print "y: ", y_out
		#print "z: ", z_out
		#time.sleep(3)
		list.append(math.degrees(bearing))
	degree_str = str(reduce(lambda x, y: x + y, list) / len(list))
	degree = "degree which robot turned: %s" % degree_str
	return degree_str
