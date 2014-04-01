

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

def undo_move(tower, prevr, prevc): #used in the simulator
    endr = tower[len(tower) - 1];
    if (endr[2] == True):
        endr[2] = False;
    elif(endr[0] == True):
        endr[0] = False;
    elif(endr[1] == True):
        endr[1] = False;
        tower.remove([False, False, False]);
    else:
        assert(False);
    tower[prevr][prevc] = True;
    return

def side_score(tside):
    score = 0;
    l = len(tside);
    for i in range(0, l):
        scoret = 0; #used to make realistic moves
        sub_score = 0;
        if (tside[i][0] == False):
            sub_score += 1;
            scoret += 1;
        if (tside[i][2] == False):
            sub_score -= 1;
            scoret += 1;
        if (tside[i][1] == False):
            scoret += 1;
        if (sub_score != 0 and scoret == 2):
            score += 10000000 #tower will break, so make it really shitty
        elif (scoret ==3):
            score += 10000000 #removes a row, make sure our robot can do this before we try letting it do this
        else:
            score += sub_score;
    return abs(score)

def get_score(tower):
    score = 0;
    t1 = tower[::2];#t1 and t2 are the two faces of each tower
    t2 = tower[1::2];
    l= len(tower);
    for i in range(0,l):
        score_sub = l-i-1;
        for j in range(0,3):
            if (tower[i][j] == False):
                score += score_sub;
    score += 5*side_score(t1);
    score += 5*side_score(t2);
    return score;

def max_legal(tower):
    l = len(tower);
    if is_full(tower[l-1]):
        return l-1;
    return l-2;


def make_best_move(tower):
    min_score = 10000000000000000000000 #stupid high
    best_r = -1;
    best_c = -1;
    l = max_legal(tower);
    for i in range(1, l): #we can't get the first row right now
        for j in range(0,3):
            if(tower[i][j]):
                make_move(tower,i,j);
                scor = get_score(tower);
                if (scor < min_score):
                    min_score = scor;
                    best_r = i;
                    best_c = j;
                undo_move(tower, i, j);
    return (best_r, best_c)


