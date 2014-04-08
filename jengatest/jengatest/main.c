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
    //argv 1,2,3,4 are global x,y,z coordinates(in mm)
    
    if (argc!=4) {
        printf("check arguments");
        return 0;
    }
    
    const char* stringx=argv[1], *stringy=argv[2],*stringz=argv[3];
    
    float x=atof(stringx),y=atof(stringy),z=atof(stringz),theta=0.0;
    int h1,h2,h3,sv;
    //int hh1,hh2,hh3;
    move2globalxyz(x,y,z,theta,&h1,&h2,&h3,&sv);
    //move2localxyz(x, y, z, hh1, hh2, hh3);
    sv*=-1;
    printf("%04d, %04d, %04d, %04d\n",h1,h2,h3,sv);
    //std::cout << "xyzt "<<x<<", "<<y<<", "<<z<<", "<<theta<<"\n";
    
    //float cx,cy,cz;
    //cx=globalxyz2controlx(x,y,z);
    //cy=globalxyz2controly(x,y,z);
    //cz=globalxyz2controlz(x,y,z);
    //std::cout << "local xyz "<<cx<<", "<<cy<<", "<<cz<<"\n";
    
    
    //std::cout << "hs as global "<<h1<<", "<<h2<<", "<<h3<<", "<<sv<<"\n";
    //std::cout << "hs as local "<<hh1<<", "<<hh2<<", "<<hh3<<"\n";
    
    GeneratePath(1,2,2,2,1);//example

    return 0;
}

