import os

def make_sources():
    i = 0
    while (i < 15):
        savestr = "~/jenga/sources/front" + str(i) + ".JPG"
        command_string = "fswebcam -S 30 -d /dev/video3 " + savestr;
        os.system(command_string);
        i = i + 1
    return;


make_sources();
