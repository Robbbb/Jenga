//
//  jengacontrol.c
//  jengatest
//
//  Created by Fan Wang on 3/31/14.
//  Copyright (c) 2014 Fan Wang. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

#include "pathplannermotor.h"



int main(int argc, const char * argv[])
{
    readSettings();
    //argv 1,2,3,4,5 are int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX
    
    if (argc!=6) {
        printf("check arguments");
        return 0;
    }
    
    
    int fromLayer=atoi(argv[1]),fromPosition=atoi(argv[2]),toLayer=atoi(argv[3]),toPosition=atoi(argv[4]),firstLayerBlockAlignWithX=atoi(argv[5]);
    
    GeneratePath(fromLayer,fromPosition,toLayer,toPosition,firstLayerBlockAlignWithX);
    
    return 0;
}
