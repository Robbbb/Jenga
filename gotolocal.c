//
//  jengacontrol.c
//  jengatest
//
//  Created by Fan Wang on 3/31/14.
//  Copyright (c) 2014 Fan Wang. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

#include "parameters.h"



int main(int argc, const char * argv[])
{
    readSettings();
    //argv 1,2,3,4 are global x,y,z coordinates(in mm)
    
    if (argc!=4) {
        printf("check arguments");
        return 0;
    }
    
    const char* stringx=argv[1], *stringy=argv[2],*stringz=argv[3];
    
    float x=atof(stringx),y=atof(stringy),z=atof(stringz),theta=0.0;
    int h1,h2,h3,sv;
    //int hh1,hh2,hh3;
    move2localxyz(x,y,z,&h1,&h2,&h3);
    //move2localxyz(x, y, z, hh1, hh2, hh3);
    //sv*=-1;
    printf("%05d, %05d, %05d\n",h1,h2,h3);


    return 0;
}

