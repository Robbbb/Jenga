//
//  main.cpp
//  jengatest
//
//  Created by Fan Wang on 3/26/14.
//  Copyright (c) 2014 Fan Wang. All rights reserved.
//

//#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include "jengacontrol.h"


int main(int argc, const char * argv[])
{
    //argv 1,2,3,4 are global x,y,z coordinates(in mm) and end effector orientation(in radian)
    
    if (argc!=5) {
        printf("check arguments");
        return 0;
    }
    
    const char* stringx=argv[1], *stringy=argv[2],*stringz=argv[3],*stringtheta=argv[4];
    
    float x=atof(stringx),y=atof(stringy),z=atof(stringz),theta=atof(stringtheta);
    int h1,h2,h3,sv;
    //int hh1,hh2,hh3;
    move2globalxyz(x,y,z,theta,&h1,&h2,&h3,&sv);
    //move2localxyz(x, y, z, hh1, hh2, hh3);
    printf("steps and servo angle: %d, %d, %d, %d\n",h1,h2,h3,sv);
    //std::cout << "xyzt "<<x<<", "<<y<<", "<<z<<", "<<theta<<"\n";
    
    //float cx,cy,cz;
    //cx=globalxyz2controlx(x,y,z);
    //cy=globalxyz2controly(x,y,z);
    //cz=globalxyz2controlz(x,y,z);
    //std::cout << "local xyz "<<cx<<", "<<cy<<", "<<cz<<"\n";
    
    
    //std::cout << "hs as global "<<h1<<", "<<h2<<", "<<h3<<", "<<sv<<"\n";
    //std::cout << "hs as local "<<hh1<<", "<<hh2<<", "<<hh3<<"\n";
    return 0;
}
