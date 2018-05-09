#####################################
# Arduino Serial Communication Test #
# Version: alpha 0.3                #
# Thomas Kaunzinger                 #
# May 7, 2018                       #
#                                   #
# A dumb program used to test       #
# serial communication between the  #
# computer and Arduino for use      #
# in future applications.           #
#####################################

import serial
import time

com_port = 'COM4'

serial_port = serial.Serial(port=com_port, baudrate=9600)


# Execution
def main():
    while True:
        serial_port.write(b'w')     # Read/write
        serial_port.write(b'a')     # DAC Address
        serial_port.write(b'a')     # Data bits. Can be whatever.
        serial_port.write(b'a')
        serial_port.write(b'!')     # Execution
        serial_port.close()
        time.sleep(7)
        serial_port.open()


# I have to keep looking at the documentation. Supposed to send the DAC address and desired voltage through the COM port
#   and tell the arduino what voltage it wants to send to the DAC.
def send_voltage(dac_address, voltage):
    serial_port.write(dac_address)
    serial_port.write(voltage)


# Reads back the data being sent by the arduino through the serial port
def readback_data():
    # Serial port recieves the string encoded in binary, thus needing decode(). Arduino also sends newline characters,
    # which we do not need, so that's why I've trimmed the string
    return str(serial_port.readline().decode()[:-2])


# Execute main method
if __name__ == "__main__":
    main()
    print()
    input("Press <Enter> to close...")

