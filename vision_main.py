import vision
import os
import cv2
import cv2.cv as cv
import numpy

def combine_towers(t1, t2, state):
    if (not state):
        temp = t2;
        t2 = t1;
        t1 = temp;
    i = 0;
    t = [];
    while (i < len(t1)):
        t.append(t1[i]);
        if (i == len(t2)):
            return t;
        t.append(t2[i]);
        i += 1;
    return t;

def remove_bullshit(t, rows, cols):
    distances = [];
    for i in range(1, len(rows)):
        dist = rows[i] - rows[i-1];
        distances.append(dist);
    print(distances);
    med_dist = numpy.median(distances);
    print(med_dist);
    check_rows = [];
    for i in range(len(distances)):
        deviation = distances[i] - med_dist;
        if (abs(deviation) > 5):
            check_rows.append(i);
            check_rows.append(i+1);
    for i in range(len(check_rows)):
        if(check_rows[i] == 0):
            if (len(check_rows) == 1):
                del(rows[0]);
                del(t[0]);
                return (t,rows, cols);
            elif(check_rows[i+1] != 1):
                del(rows[0]);
                del(t[0]);
                return remove_bullshit(t, rows, cols);
            else:
                continue
        elif(i == (len(check_rows) -1)):
            print('rows are');
            print(rows);
            print('checed are');
            print(check_rows);
            if (len(check_rows) == 1 or (len(check_rows) == 2)):
                del(rows[check_rows[i]]);
                del(t[check_rows[i]]);
                return(t, rows, cols);
            else:
                print ('rows is');
                print(rows);
                print('checked rows are');
                print(check_rows);
        
        else:
            if (check_rows[i] == check_rows[i+1]):
                del(rows[check_rows[i]]);
                del(t[check_rows[i]]);
                return remove_bullshit(t, rows, cols);
    return(t, rows, cols);





def get_tower_from_vision():
    savestr1 = './current_pics/left_cam.JPG';
    savestr2 = './current_pics/right_cam.JPG';    
    command_string1 = "fswebcam -D 1 -S 40 -d /dev/video4 " + savestr1;
    command_string2 = "fswebcam -D 1 -S 40 -d /dev/video3 " + savestr2;
    #os.system(command_string1);
    #os.system(command_string2)
    savestr1 = './left.JPG';
    savestr2 = './right.JPG';

    #outstr1 = './current_pics/left_out.JPG';
    #outstr2 = './current_pics/right_out.JPG';
    outstr1 = './current_pics/left_out.JPG';
    outstr2 = './current_pics/right_out.JPG';
    left_t = [48,15,198,286];
    right_t = [153,55,284,286]; 
    #(t1, rowsl, colsl) = vision.main(savestr1, outstr1, vision.left_tower);
    (t1, rowsl, colsl) = vision.main(savestr1, outstr1, left_t);
    (t1, rowsl, colsl) = remove_bullshit(t1, rowsl, colsl);
    #(t2, rowsr, colsr) = vision.main(savestr2, outstr2, vision.right_tower);
    (t2, rowsr, colsr) = vision.main(savestr2, outstr2, right_t);
    (t2, rowsr, colsr) = remove_bullshit(t2, rowsr, colsr);
    print("left is");
    print(t1);
    print(rowsl);
    print("right is");
    print(t2);
    print(rowsr);

    t = combine_towers(t1, t2, True);
    print("final tower is")
    print(t);
    t_full = [t, rowsl, colsl, rowsr, colsr]
    return t_full;
    #return t1;
get_tower_from_vision();

def get_end_error(bbox_end, r_block, c_block, in_left):
    if (len(bbox_end) == 0):
        return (0,0);
    best_x_error = -1;
    best_y_error = -1;
    best_error = 1000000000000;
    conversion = 1.53;
    for x1, y1, x2, y2 in bbox_end:
        x_mean = (x1+x2)/2;
        if (in_left):
            x_error = x_mean-c_block;
        else:
            x_error = c_block - x_mean;
        y_mean = (y1+y2)/2;
        y_error =r_block - y_mean;
        error = abs(x_error) + abs(y_error); #manhattan distance
        if (error < best_error):
            best_error = error;
            best_y_error = y_error;
            best_x_error = x_error;
    return (best_x_error * conversion, best_y_error*conversion);

def get_side_error(bbox_side, side_goal, in_left):
    if (len(bbox_side) == 0):
        return 0;

    best_side_error = 100000000000;
    if (len(bbox_sides) == 0):
        return 0;
    conversion = 1.53;
    for x1, y1, x2, y2 in bbox_side:
        if (in_left):
            error = x1-side_goal;
        else:
            error = side_goal - x1;
        if (abs(error) < abs(best_side_error)):
            best_side_error = error;
    return conversion*best_side_error;



def distance_from_goal(r_goal, c_goal, t_full):
    t, rowsl, colsl, rowsr, colsr = t_full;
    is_left = False;
    if ((r_goal % 2)== 0):
        is_left = True;
    savestr1 = '~/jenga/current_pics/left_cam_feedback.JPG';
    savestr2 = '~/jenga/current_pics/right_cam_feedback.JPG';
    command_string1 = "fswebcam -S 40 -d /dev/video4 " + savestr1;
    command_string2 = "fswebcam -D 1 -S 40 -d /dev/video3 " + savestr2;
    os.system(command_string1);
    os.system(command_string2);
    cascade_side = cv2.CascadeClassifier('./detectors/sides.xml');
    max_size_side = (33, 69);
    cascade_end = cv2.CascadeClassifier('./detectors/ends.xml');
    max_size_end = (75, 45);
    if (is_left):
        pic_end = cv2.imread(savestr1,cv2.CV_LOAD_IMAGE_GRAYSCALE);
        pic_side = cv2.imread(savestr2, cv2.CV_LOAD_IMAGE_GRAYSCALE);
        side_goal = vision.right_tower[2];
        r_block = rowsl[r_goal/2];
        c_block = colsl[c_goal];
    else:
        pic_end = cv2.imread(savestr2,cv2.CV_LOAD_IMAGE_GRAYSCALE);
        pic_side = cv2.imread(savestr1, cv2.CV_LOAD_IMAGE_GRAYSCALE);
        side_goal = vision.left_tower[0];
        r_block = rowsr[r_goal/2];
        c_block = colsr[c_goal];
    pic_end = cv2.GaussianBlur(pic_end, (15,15), 1);
    pic_end = cv2.Canny(pic_end, 70, 175);
    pic_side = cv2.GaussianBlur(pic_side, (15,15), 1);
    pic_side = cv2.Canny(pic_side, 70, 175);
    bbox_sides = vision.detect(pic_side, cascade_side, max_size_side);
    bbox_ends = vision.detect(pic_end, cascade_end, max_size_end);
    side_error = get_side_error(bbox_sides,r_block, c_block);
    end_error = get_end_error(bbox_ends, r_block, c_block);
    if (is_left):
        return (side_error, end_error[0], end_error[1]);
    else:
        return (end_error[0], side_error, end_error[1]);





