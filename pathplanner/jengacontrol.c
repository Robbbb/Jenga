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
#include <stdio.h>
#include "jengacontrol.h"



//fw

float screwDistance=325.269;
float armLength=200;
float nutLength=23.775+4.1+9.625;
float endLength=38.6+4.05+7.35;
float pi=3.1416;
float stepperrev=200.0;
float pitch=25.4;
float zoneLenth=230;
float endfXinit=-28.8953;//TODO those three values are not exact
float endfYinit=-28.8953;
float endfZinit=439.20;
//equation: float endfZinit=20*25.4-sqrt(armLength*armLength-(zoneLenth*sqrt(2.0)/sqrt(3.0))*(zoneLenth*sqrt(2.0)/sqrt(3.0)));

float equationH(float par1,float par2,float z)
{
    float equation=z+sqrt(armLength*armLength-(-endLength-nutLength+sqrt(par1*par1+par2*par2))*(-endLength-nutLength+sqrt(par1*par1+par2*par2)));
    return equation;
}

float equationSteps(float h,float theta)
{
    return -(-stepperrev*h/pitch+theta*stepperrev/(2.0*pi));
}


float equationStep1(float x,float y,float z)
{
    float par1=screwDistance/sqrt(3.0)+x;
    float par2=y;
    float h1=equationH(par1,par2,z);
    float theta1=atan2(par1,par2);
    return equationSteps(h1,theta1);
}

float equationStep2(float x,float y,float z)
{
    float par1=-sqrt(3.0)/2.0*screwDistance+screwDistance/sqrt(3.0)+x;
    float par2=y-screwDistance/2;
    float h2=equationH(par1,par2,z);
    float theta2=atan2(par1,par2);
    return equationSteps(h2,theta2);
}

float equationStep3(float x,float y,float z)
{
    float par1=-sqrt(3.0)/2.0*screwDistance+screwDistance/sqrt(3.0)+x;
    float par2=y+screwDistance/2;
    float h3=equationH(par1,par2,z);
    float theta3=atan2(par1,par2);
    return equationSteps(h3,theta3);
}

float equationServo(float x,float y,float z, float theta)
{
    float par1=screwDistance/sqrt(3.0)+x;
    float par2=y;
    //float h1=equationH(par1,par2,z);
    float theta1=atan2(par1,par2);
    return (theta-(pi/4.0+theta1))*180.0/pi;
}

float xyz2Step1(float x,float y,float z)
{
    return equationStep1(x,y,z)-equationStep1(0,0,0);
}

float xyz2Step2(float x,float y,float z)
{
    return equationStep2(x,y,z)-equationStep2(0,0,0);
}

float xyz2Step3(float x,float y,float z)
{
    return equationStep3(x,y,z)-equationStep3(0,0,0);
}

//coordinate transformation

float globalxyz2controlx(float x,float y,float z)
{
    return ((x-endfXinit)/sqrt(2.0)+(y-endfYinit)/sqrt(2.0));
}

float globalxyz2controly(float x,float y,float z)
{
    return ((y-endfYinit)/sqrt(2.0)-(x-endfXinit)/sqrt(2.0));
}

float globalxyz2controlz(float x,float y,float z)
{
    return z-endfZinit;
}

//calculate destination in steps according to global xyz
void move2globalxyz(float x,float y,float z,float theta,int *ns1,int *ns2,int *ns3,int *sv)
{
    float cx,cy,cz;
    cx=globalxyz2controlx(x,y,z);
    cy=globalxyz2controly(x,y,z);
    cz=globalxyz2controlz(x,y,z);
    
    
    
    float nstep1,nstep2,nstep3;
    nstep1=xyz2Step1(cx,cy,cz);
    nstep2=xyz2Step2(cx,cy,cz);
    nstep3=xyz2Step3(cx,cy,cz);
    
    
    *ns1=(int)nstep1;
    *ns2=(int)nstep2;
    *ns3=(int)nstep3;
    
    *sv=(int)equationServo(cx, cy, cz, theta);
}

void move2localxyz(float x,float y,float z,int *ns1,int *ns2,int *ns3)
{
    
    
    float nstep1,nstep2,nstep3;
    nstep1=xyz2Step1(x,y,z);
    nstep2=xyz2Step2(x,y,z);
    nstep3=xyz2Step3(x,y,z);
    
    *ns1=(int)nstep1;
    *ns2=(int)nstep2;
    *ns3=(int)nstep3;
}
//fw

//fw path

float JengaLength=75;
float JengaWidth=25;
float JengaHeight=15;
float EndEffectorHeightOffset=7*25.4;
float EndEffectorLengthOffset=-4*25.4;
float EndEffectorLongerLengthOffset=-4.3*25.4;

void GeneratePath(int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX)//layer number from 0, position number is from 0 to 2 and 0 is the closest to the origin
{
    //path 0 inital,1 lower down, 2 get closer, take, 3 get away, 4 go up, 5 orient to put, 6 lower a bit, 7 get close, put
    float x[8],y[8],z[8],theta[8];
    float takeZ=JengaHeight*(fromLayer+0.5)+EndEffectorHeightOffset;
    float putZ=JengaHeight*(toLayer+0.5)+EndEffectorHeightOffset;
    float initialZ=JengaHeight*(toLayer+2)+EndEffectorHeightOffset;
    z[0]=initialZ;
    z[1]=takeZ;
    z[2]=takeZ;
    z[3]=takeZ;
    z[4]=initialZ;
    z[5]=initialZ;
    z[6]=putZ;
    z[7]=putZ;
    
    float takeDistanceRadial=(fromPosition+0.5)*JengaWidth,putDistanceRadial=(toPosition+0.5)*JengaWidth;
    int takeAlignWithX, putAlignWithX;
    
    if (fromLayer%2) {
        takeAlignWithX=!firstLayerBlockAlignWithX;
    }
    else{
        takeAlignWithX=firstLayerBlockAlignWithX;
    }
    if (toLayer%2) {
        putAlignWithX=!firstLayerBlockAlignWithX;
    }
    else{
        putAlignWithX=firstLayerBlockAlignWithX;
    }

    if (takeAlignWithX) {
        for (int i=0; i<5; i++) {
            y[i]=takeDistanceRadial;
            theta[i]=0;
        }
        x[0]=EndEffectorLongerLengthOffset;
        x[1]=EndEffectorLongerLengthOffset;
        x[2]=EndEffectorLengthOffset;
        x[3]=EndEffectorLongerLengthOffset;
        x[4]=EndEffectorLongerLengthOffset;
    }
    else {
        for (int i=0; i<5; i++) {
            x[i]=takeDistanceRadial;
            theta[i]=90;
        }
        y[0]=EndEffectorLongerLengthOffset;
        y[1]=EndEffectorLongerLengthOffset;
        y[2]=EndEffectorLengthOffset;
        y[3]=EndEffectorLongerLengthOffset;
        y[4]=EndEffectorLongerLengthOffset;
    }
    
    if (putAlignWithX) {
        for (int i=5; i<8; i++) {
            y[i]=putDistanceRadial;
            theta[i]=0;
        }
        x[5]=EndEffectorLongerLengthOffset;
        x[6]=EndEffectorLongerLengthOffset;
        x[7]=EndEffectorLengthOffset;
    }
    else {
        for (int i=5; i<8; i++) {
            x[i]=putDistanceRadial;
            theta[i]=90;
        }
        y[5]=EndEffectorLongerLengthOffset;
        y[6]=EndEffectorLongerLengthOffset;
        y[7]=EndEffectorLengthOffset;
    }
    

    //path 0 inital,1 lower down, 2 get closer, take, 3 get away, 4 go up, 5 orient to put, 6 lower a bit, 7 get close, put
    printf("L,0\n");//pneumatic cylinder extended
    printf("V,0\n");//vacuum off
    printf("M,%f,%f,%f,%f\n",x[0],y[0],z[0],theta[0]);//go to intial position, above the take position and oriented
    printf("M,%f,%f,%f,%f\n",x[1],y[1],z[1],theta[1]);//lower down
    printf("M,%f,%f,%f,%f\n",x[2],y[2],z[2],theta[2]);//get closer
    printf("V,1\n");//vacuum on
    printf("L,1\n");//take the block
    printf("M,%f,%f,%f,%f\n",x[3],y[3],z[3],theta[3]);//take away
    printf("M,%f,%f,%f,%f\n",x[4],y[4],z[4],theta[4]);//go up
    printf("M,%f,%f,%f,%f\n",x[5],y[5],z[5],theta[5]);//orient to put
    printf("M,%f,%f,%f,%f\n",x[6],y[6],z[6],theta[6]);//lower a bit
    printf("L,0\n");//neumatic cylinder extended
    printf("M,%f,%f,%f,%f\n",x[7],y[7],z[7],theta[7]);//get close
    printf("V,0\n");//vacuum off, drop the block
    //M for move
    //V for vacuum
    //L for linear
}


//fw path



int main(int argc, const char * argv[])
{
    //argv 1,2,3,4,5 are int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX
    
    if (argc!=6) {
        printf("check arguments");
        return 0;
    }
    
    
    int fromLayer=atoi(argv[1]),fromPosition=atoi(argv[2]),toLayer=atoi(argv[3]),toPosition=atoi(argv[4]),firstLayerBlockAlignWithX=atoi(argv[5]);
    
    GeneratePath(fromLayer,fromPosition,toLayer,toPosition,firstLayerBlockAlignWithX);
    
    return 0;
}