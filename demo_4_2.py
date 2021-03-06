import serial
import commands
import time

def main():
    ser = serial.Serial('/dev/tty.usbmodem1411');
    ser.baudrate = 9600;
    print ("serial successful");
    way_points = [[37.5, -109.220001, 252.800003]];
    way_points.append([37.5, -109.220001, 200.300003]);
    way_points.append([37.5, -101.599998, 200.300003]);
    way_points.append([37.5, -109.220001, 200.300003]);
    way_points.append([37.5, -109.220001, 245.300003]);
    way_points.append([37.5, -101.599998, 245.300003]);
    for i in range(0, len(way_points)):
        inpt = str(way_points[i]);
        inpt = inpt[1:(len(inpt)-1)];
        inpt = inpt.replace(",", "");
        cmd_str = "./jenga_test "+inpt;
        lin = commands.getoutput(cmd_str);
        lin = lin + '\r';
        print(lin);
        time.sleep(1)
        ser.setDTR(level=0)
        time.sleep(1)
        temp = ser.write(lin);
        result = ser.readline();
        print(result);
    ser.close()
    return

main()
