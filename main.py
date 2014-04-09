import serial
import commands
import time
import jenga_logic
def main():
    ser = '';
    #ser = serial.Serial('/dev/tty.usbmodem1411');
    #ser.baudrate = 9600;
    print ("serial successful");
    t = jenga_logic.create_tower();
    while(jenga_logic.current_state(t)): #while tower is still up
        next_move_from = jenga_logic.make_best_move(t);
        l = len(t);
        top_row = t[l-1];
        if (jenga_logic.is_full(top_row)):
            next_move_end = (l, 1);
        else:
            if (not top_row[1]):
                next_move_end = (l-1, 1);
            elif(not top_row[0]):
                next_move_end = (l-1, 0);
            else:
                next_move_end = (l-1, 2);
        in_str = str(next_move_from[0]) + ' ';
        in_str += str(next_move_from[1]) + ' ';
        in_str += str(next_move_end[0]) + ' ';
        in_str += str(next_move_end[1]) + ' >waypoints.txt';
        in_str = './waypoints '+ in_str;
        print(in_str);
        os.system(in_str);
        way_point_file = open('./waypoints.txt');
        for line in way_point_file:
            inpt = line;
            if (line[0] == 'M'):
                inpt = inpt[1::];#get rid of the M
                inpt = inpt.split(',');
                theta_goal = inpt[-1]; #save the theta goal
                inpt = str(inpt[:-1]);
                inpt = inpt.replace(",", " ");
                cmd_str = "./jenga_test "+inpt;
                lin = commands.getoutput(cmd_str);
                lin = lin.split(',');
                theta_ee = int(float(lin[-1]));
                lin = lin[:-1];
                theta_servo = theta_goal - theta_ee;
                lin.append(theta_servo);
                lin = str(lin);
                lin = 'M' + lin + '\r';
            else:
                lin = line;
            print(lin);
            break
            time.sleep(1)
            ser.setDTR(level=0)
            time.sleep(1)
            temp = ser.write(lin);
            result = ser.readline();
            print(result);
        ser.close()
    return

main()
