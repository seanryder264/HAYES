import serial
import time

#probably this port, might need to test and rename if it doesn't work
SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 9600

try:
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
        print ("Port opened")
        while True:
            #reads data and strips new line \n
            data = ser.readline().strip()
            if data:
                print ("Data present")

except serial.SerialException as e:
    print (e)
except KeyboardInterrup:
    print ("Program stopped")