import vision
import os
import cv2
import cv2.cv as cv


def combine_towers(t1, t2, state):
    if (not state):
        temp = t1;
        t2 = t1;
        t1 = temp;
    i = 0;
    t = [];
    while (i < len(t1)):
        t.append(t1[i]);
        t.append(t2[i]);
        i += 1;
    return t;

def get_tower_from_vision():
    savestr1 = '~/jenga/current_pics/left_cam.JPG';
    savestr2 = '~/jenga/current_pics/right_cam.JPG';
    command_string1 = "fswebcam -S 40 -d /dev/video4 " + savestr1;
    command_string2 = "fswebcam -S 40 -d /dev/video3 " + savestr2;
    os.system(command_string1);
    os.system(command_string2)
    outstr1 = '~/jenga/current_pics/left_out.JPG';
    outstr2 = '~/jenga/current_pics/right_out.JPG';
    (t1, rowsl, colsl) = vision.main(savestr1, outstr1, vision.left_tower);
    (t2, rowsr, colsr) = vision.main(savestr2, output2, vision.right_tower);
    t = combine_towers(t1, t2, True);
    t2 = (t, rowsl, colsl, rowsr, colsr)
    return t2;


#def distance_from_goal(t, r_goal, c_goal):
