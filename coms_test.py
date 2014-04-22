import time
import serial
import jenga_logic

def full_move_ser(t, opposition_move):
    print('their move is');
    print(opposition_move);
    s = opposition_move[:-3]
    s = s.split(',');
    s = map(int, s);
    s[:] = [x - 1 for x in s];
    print(s);
    jenga_logic.make_full_move(t, s[0],s[1],s[2],s[3]);
    return;

def main():
    first_move = raw_input('am I making the first move?');
    ser = serial.Serial('/dev/tty.usbserial-FTG9K5QZ', 115200);
    print ("serial successful");
    t = jenga_logic.create_tower()
    while(True):
        if (first_move == 'false'):
            opposition_move = ser.readline();
            print('opposition move is');
            print(opposition_move);
            full_move_ser(t, opposition_move);
        r1, c1, r2,c2 = jenga_logic.make_best_move(t);
        print(t);
        jenga_logic.make_full_move(t, r1,c1,r2,c2);
        move = str(r1+1);
        move += ','
        move += str(c1+1);
        move += ',';
        move += str(r2 +1);
        move += ',';
        move +=str(c2 +1);
        move += ';'
        print('our move is');
        print(move);
        ser.setDTR(level=0)
        time.sleep(.1)
        temp = ser.write(move);
        if (first_move != 'false'):
            opposition_move = ser.readline();
            full_move_ser(t, opposition_move);


main()

