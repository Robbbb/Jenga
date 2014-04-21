import time
import serial
import jenga_logic

def full_move_ser(t, opposition_move);
    s = opposition_move[:-1]
    s = s.split(',');
    s = map(int, s);
    s[:] = [x - 1 for x in s];
    jenga_logic.make_full_move(t, s[0],s[1],s[2],s[3]);
    return;

def main()
    first_move = raw_input('am I making the first move?');
    ser = serial.Serial('/dev/ttyS0', 9600);
    print ("serial successful");
    t = jenga_logic.create_tower()
    while(True):
        if (first_move == 'false'):
            opposition_move = ser.readline();
            print('opposition move is');
            print(opposition_move);
            full_move_ser(t, opposition_move);
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
        move = str(next_move_from[0]+1);
        move += ','
        move += str(next_move_from[1]+1);
        move += ',';
        move += str(next_move_end[0] +1);
        move += ',';
        move +=str(next_move_end[1] +1);
        move += ';'
        print('our move is');
        print(move);
        ser.setDTR(level=0)
        time.sleep(.1)
        temp = ser.write(move);
        if (first_move != 'false'):
            opposition_move = ser.readline();
            full_move_ser(t, opposition_move);




