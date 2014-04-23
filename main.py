import os
import vision_main
import serial
import commands
import time
import jenga_logic




def versus_human_smart():
    ser = serial.Serial('/dev/ttyS0', 9600);
    print ("serial successful");
    t = jenga_logic.create_tower();
    currentxyz =[];
    while(jenga_logic.current_state(t)): #while tower is still up
        t_full = vision_main.get_tower_from_vision();
        t = t_full[0];
        print("robot thinks the tower is");
        print(t);
        next_move = jenga_logic.make_best_move(t);
        in_str = str(next_move[0]) + ' ';
        in_str += str(next_move[1]) + ' ';
        in_str += str(next_move[2]) + ' ';
        in_str += str(next_move[3]) + ' 0  >waypoints.txt';
        in_str = './waypoints '+ in_str;
        os.system(in_str);
        vision_mode = 'take'
        way_point_file = open('./waypoints.txt');
        for line in way_point_file:
            print (line);
            print("line was printed");
            inpt = line;
            if (line[0] == 'P'):
                if (currentxyz == []):
                    assert(False);
                startr, startc, endr, endc = next_move;
                if (vision_mode == 'take'):
                    r_goal = startr;
                    c_goal = startc;
                else:
                    r_goal = endr;
                    c_goal = endc;
                error = vision_main.distance_from_goal(r_goal, c_goal, t_full);
                print('error is');
                print(error);
                i = raw_input("test_error")
                new_goal = [];
                for i in range(3):
                    new_goal.append(current_xyz[i] + error[i]);
                lin = [];
                for i in range(3):
                    st = str(new_goal[i]).zfill(5);
                lin.append(theta_servo);
                lin = ','.join(lin);
                lin = 'M,' + lin + '\n';
                print('lin is now');
                print(lin);
                i = raw_input('testing error correction');
            if (line[0] == 'M'):
                inpt = inpt[2::];#get rid of the M,
                inpt = inpt.split(',');
                theta_goal = int(float(inpt[-1])); #save the theta goal
                inpt = inpt[:-1];
                inpt = ' '.join(inpt);
                cmd_str = "./jenga_test "+inpt;
                lin = commands.getoutput(cmd_str);
                lin = lin.split(',');
                xyz_temp = lin[:-1];
                currentxyz = map(int, xyz_temp);
                theta_ee = int(float(lin[-1]));
                lin = lin[:-1];
                theta_servo = theta_goal - theta_ee;
                theta_servo = theta_servo + 100;#should be 90
                theta_servo = str(theta_servo).zfill(4);
                #print(theta_ee);
                lin.append(theta_servo);
                lin = ','.join(lin);
                lin = lin.replace(' ', '');
                lin = 'M,' + lin + '\n';
            else:
                lin = line;
            print (lin)
            print("lin was printed");
            time.sleep(1)
            ser.setDTR(level=0)
            time.sleep(1)
            temp = ser.write(lin);
            result = ser.readline();
            print result;
        print('move was')
        print(next_move);
        print(tower_is);
        way_point_file.close();
        jenga_logic.make_full_move(t, next_move[0], next_move[1], next_move[2], next_move[3]);
        print (t);
    ser.close()
    return;


def one_player_main():
    ser = serial.Serial('/dev/ttyS0', 9600);
    print ("serial successful");
    t = jenga_logic.create_tower();
    currentxyz =[];
    while(jenga_logic.current_state(t)): #while tower is still up
        t_full = vision_main.get_tower_from_vision();
        if (t_full[0] != t):
            print("tower seen is different from tower stored");
            print(t);
            print('tower seen is');
            print(t_full[0]);
            t_full[0] = t;
        next_move = jenga_logic.make_best_move(t);
        in_str = str(next_move[0]) + ' ';
        in_str += str(next_move[1]) + ' ';
        in_str += str(next_move[2]) + ' ';
        in_str += str(next_move[3]) + ' 0  >waypoints.txt';
        in_str = './waypoints '+ in_str;
        os.system(in_str);
        vision_mode = 'take'
        way_point_file = open('./waypoints.txt');
        for line in way_point_file:
            print (line);
            print("line was printed");
            inpt = line;
            if (line[0] == 'P'):
                if (currentxyz == []):
                    assert(False);
                startr, startc, endr, endc = next_move;
                if (vision_mode == 'take'):
                    r_goal = startr;
                    c_goal = startc;
                else:
                    r_goal = endr;
                    c_goal = endc;
                error = vision_main.distance_from_goal(r_goal, c_goal, t_full);
                print('error is');
                print(error);
                i = raw_input("test_error")
                new_goal = [];
                for i in range(3):
                    new_goal.append(current_xyz[i] + error[i]);
                lin = [];
                for i in range(3):
                    st = str(new_goal[i]).zfill(5);
                lin.append(theta_servo);
                lin = ','.join(lin);
                lin = 'M,' + lin + '\n';
                print('lin is now');
                print(lin);
                i = raw_input('testing error correction');
            if (line[0] == 'M'):
                inpt = inpt[2::];#get rid of the M,
                inpt = inpt.split(',');
                theta_goal = int(float(inpt[-1])); #save the theta goal
                inpt = inpt[:-1];
                inpt = ' '.join(inpt);
                cmd_str = "./jenga_test "+inpt;
                lin = commands.getoutput(cmd_str);
                lin = lin.split(',');
                xyz_temp = lin[:-1];
                currentxyz = map(int, xyz_temp);
                theta_ee = int(float(lin[-1]));
                lin = lin[:-1];
                theta_servo = theta_goal - theta_ee;
                theta_servo = theta_servo + 100;#should be 90
                theta_servo = str(theta_servo).zfill(4);
                #print(theta_ee);
                lin.append(theta_servo);
                lin = ','.join(lin);
                lin = lin.replace(' ', '');
                lin = 'M,' + lin + '\n';
            else:
                lin = line;
            print (lin)
            print("lin was printed");
            time.sleep(1)
            ser.setDTR(level=0)
            time.sleep(1)
            temp = ser.write(lin);
            result = ser.readline();
            print result;
        print('move was')
        print(next_move);
        print(tower_is);
        way_point_file.close();
        jenga_logic.make_full_move(t, next_move[0], next_move[1], next_move[2], next_move[3]);
        print (t);
    ser.close()
    return;


def callibrate():
`    ser = serial.Serial('/dev/ttyS0', 9600);
    print ("serial successful");
    while (True):
        lin = raw_input("what input should I do?");
        print('lin is ' + lin);
        if (lin == 'break'):
            return
        ser.setDTR(level=0)
        time.sleep(.1)
        temp = ser.write(lin);
        result = ser.readline();
    print(result);
callibrate()
#main()
