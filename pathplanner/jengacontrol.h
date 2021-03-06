//
//  jengacontrol.h
//  jengatest
//
//  Created by Fan Wang on 3/31/14.
//  Copyright (c) 2014 Fan Wang. All rights reserved.
//

#ifndef jengatest_jengacontrol_h
#define jengatest_jengacontrol_h

void move2globalxyz(float x,float y,float z,float theta,int *ns1,int *ns2,int *ns3,int *sv);

void GeneratePath(int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX);//layer number from 0, position number is from 0 to 2 and 0 is the closest to the origin
//output V 0/1 vacum on/off, L 0/1 cylinder extended/retracted, M,x,y,z,theta in global system
#endif
