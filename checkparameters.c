//
//  jengacontrol.c
//  jengatest
//
//  Created by Fan Wang on 3/31/14.
//  Copyright (c) 2014 Fan Wang. All rights reserved.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"


int main(int argc, const char * argv[])
{
    readSettings();
    //argv 1,2,3,4,5 are int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX
    
    if (parameterCheck()) {
        printf("error");
    }
    else{
        printf("okay");
    }
    
    return 0;
}