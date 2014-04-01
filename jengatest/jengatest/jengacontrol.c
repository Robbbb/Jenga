//
//  jengacontrol.c
//  jengatest
//
//  Created by Fan Wang on 3/31/14.
//  Copyright (c) 2014 Fan Wang. All rights reserved.
//
#include <math.h>
#include <stdio.h>
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
