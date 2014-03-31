

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

