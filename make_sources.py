import os

def make_sources():
    i = 0
    while (i < 100):
        savestr = "~/jenga/sources/output" + str(i) + ".JPG"
        command_string = "fswebcam -D 1 -S 30 -d /dev/video1 " + savestr;
        os.system(command_string);
        i = i + 1
    return;


make_sources();
