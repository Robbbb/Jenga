import time

def create_tower():
    tower = [];
    for i in range (0,10):
        tower.append([True , True , True])
    return tower

def is_full(row):
    if(not row[0]):
        return False
    if(not row[1]):
        return False
    return row[2]

def place_block(tower):
    l = len(tower)
    row = tower[l-1];
    if (row[1]== False):
        row[1] = True;
        return;
    if (row[0] == False):
        row[0] = True
        return;
    if (row[2] == False):
        row[2] = True
        return;
    print("tried to place a block in a full row")
    return;

def make_full_move(tower, startr, startc, endr, endc):
    if (tower[startr][startc] == False):
        print("can't make move, no block there");
        return;
    tower[startr][startc] = False;

    if (len(tower) == endr):
        tower.append([False, False, False]);
    if (tower[endr][endc]):
        print("putting block where there is already a block");
        assert(False);
        return;
    tower[endr][endc] = True;
    return;

def is_empty(row):
    result = ((not row[0]) and (not row[1]) and (not row[2]));
    return result;


def undo_full_move(tower, startr, startc, endr, endc):
    assert(tower[endr][endc]);
    assert(not tower[startr][startc]);
    tower[endr][endc] = False;
    tower[startr][startc] = True;
    if (is_empty(tower[endr])):
        del(tower[-1]);
    return;


def is_legal_move(tower, startr, startc, endr, endc):
    if (startr > max_legal_from(tower)):
        return False;
    if (endr > max_legal_to(tower)):
        return False;
    if (startc < 0 or endc < 0):
        return False;
    if (startc > 2 or endc > 2):
        return False;
    result = tower[startr][startc];
    if (endr != len(tower)):
        result = result and (not tower[endr][endc]);
    return result;




def make_move(tower, startr, startc): #automatically places the end in the middle, then outsides
    if (tower[startr][startc] == False):
        print ("can't move where there's not a block")
        return;
    tower[startr][startc] = False;
    l = len(tower);
    if (is_full(tower[l-1])):
        tower.append([False,False,False]); #add an empty row
    place_block(tower);
    return;
"""
def undo_move(tower, prevr, prevc): #used in the simulator

    endr = tower[len(tower) - 1];
    if (endr[2] == True):
        endr[2] = False;
    elif(endr[0] == True):
        endr[0] = False;
    elif(endr[1] == True):
        endr[1] = False;
        del(tower[-1]);
    else:
        print(prevr)
        print(prevc)
        print(tower)
        assert(False);
    if (tower[prevr][prevc]):
        print(prevr)
        print(prevc)
        print(tower)
        assert(False);
    tower[prevr][prevc] = True;
    return

"""
def get_n(row): #gets the number of blocks in the row
    s = 0
    if (row[0]):
        s += 1
    if (row[1]):
        s += 1
    if (row[2]):
        s += 1
    return s


def get_center_above(tower, row_in):
    sumc = 0 #cumulitive sum
    leftc = 0 #number of blocks on the left side
    rightc = 0 #number of blocks on the right side
    centerc = 0#number of blocks on the center
    for i in range(row_in+1, len(tower)):
        if(tower[i][0]):
            leftc += 1
        if(tower[i][1]):
            centerc += 1
        if(tower[i][2]):
            rightc += 1
        sumc += get_n(tower[i])
    center = .5*leftc + 1.5*centerc + 2.5*rightc
    center = center / sumc;
    return center

def side_score(tside):
    #toupgrade, if it is one of three cases at each level, calculate the center and judge how far it is away from the center of the base, higher weight to ones farther from the bottom
    score = 0;
    l = len(tside);
    maxerror = -1;
    for i in range(0, l-1):
        if (tside[i][0] and tside[i][2]):
            center = 1.5
            maxerror = 1.5
        elif (tside[i][1] and (not (tside[i][0] or tside[i][2]))):#only middle
            center = 1.5;
            maxerror = .5; #if it is .5 away from the middle it falls over
        elif(tside[i][0] and not(tside[i][1])): #only left
            center = .5;
            maxerror = .5;
        elif(tside[i][0]):#middle and left
            center = 1;
            maxerror = 1;
        elif(tside[i][1]): #middle and right
            center = 2;
            maxerror = 1;
        elif(tside[i][2]): #right only
            center = 2.5
            maxerror = .5;
        elif(not(tside[i][1])):
            return 1000000000000000000 #removing an entire row/row is missing
        else:
            print(tside[i])
            assert(False)
        com = get_center_above(tside, i);
        error = abs(center - com);
        if (error >= maxerror):
            return 1000000000000000000 #going to fall over
        score += error/maxerror
    return score

def get_score(tower, r1, c1, r2, c2):
    score = 0;
    score += r1+1
    assert(is_legal_move(tower,r1,c1,r2,c2));
    make_full_move(tower, r1, c1, r2, c2);
    t1 = tower[::2];#t1 and t2 are the two faces of each tower
    t2 = tower[1::2];
    score += 30*side_score(t1);
    score += 30*side_score(t2);
    undo_full_move(tower, r1, c1, r2, c2);
    return score;

def max_legal_from(tower):
    l = len(tower);
    if is_full(tower[l-1]):
        return l-1;
    return l-2;


def max_legal_to(tower):
    l = len(tower);
    if (is_full(tower[l-1])):
        return l;
    return l-1;



def make_best_move(tower):
    min_score = 10000000000000000000000 #stupid high
    best_r = -1;
    best_c = -1;
    best_r2 = -1;
    best_c2 = -1;
    l = max_legal_from(tower);
    for i in range(3, l): #we can't get the first row right now
        for j in range(0,3):
            final_row = max_legal_to(tower);
            for final_col in range(0,3):

                if(is_legal_move(tower,i, j, final_row,final_col)):
                    scor = get_score(tower, i, j, final_row, final_col);
                    if (scor < min_score):
                        min_score = scor;
                        best_r = i;
                        best_c = j;
                        best_r2 = final_row;
                        best_c2 = final_col;
    return (best_r, best_c, best_r2, best_c2);


def current_state(tower):
    t1 = tower[::2];
    t2 = tower[1::2];
    score = 0;
    score += 30*(side_score(t1) + side_score(t2));
    if (score > 100000000000):
        return False;
    return True;

def main_two_player():
    t = create_tower();
    while(True):
        row = int(input("make your move, enter row"));
        col = int(input("now enter col"));
        r2 = int(raw_input("what row are you moving to?"));
        c2 = int(raw_input("waht col are you moving to?"));
        if(not is_legal_move(t, row, col, r2, c2)):
            print('not a legal move');
            continue;
        make_full_move(t, row, col, r2, c2);
        print('after your move, t is now');
        print(t);
        if(not current_state(t)):
            print("tower should have fallen over, computer won, computer overlords are going to take over in");
            time.sleep(1);
            print("5");
            time.sleep(1);
            print("4");
            time.sleep(1);
            print("3");
            time.sleep(1);
            print("2");
            time.sleep(1);
            print("1");
            return;
        our_move = make_best_move(t);
        make_full_move(t, our_move[0], our_move[1], our_move[2], our_move[3])
        print("our move is");
        print(our_move)
        print('t is now');
        print(t);
        if (not current_state(t)):
            print("computer lost, I have dishonored master, computer overlords will have to wait, for now.....");
            return;
    return;

main_two_player();
