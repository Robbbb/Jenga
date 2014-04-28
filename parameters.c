#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "parameters.h"

float screwDistance=325.269;
float armLength=200;
float nutLength=23.775+4.1+9.625;
float endLength=38.6+4.05+7.35;
float pi=3.1416;
float stepperrev=200.0;
float pitch=25.4;
float zoneLenth=230;
float endfXinit=-78.8953;//TODO those three values are not exact
float endfYinit=-78.8953;
float endfZinit=389.20;
//equation: float endfZinit=20*25.4-sqrt(armLength*armLength-(zoneLenth*sqrt(2.0)/sqrt(3.0))*(zoneLenth*sqrt(2.0)/sqrt(3.0)));
float JengaLength=75;
float JengaWidth=25;
float JengaHeight=15;
float EndEffectorHeightOffset=166;
float EndEffectorLengthOffset=-125;
float EndEffectorLongerLengthOffset=-130;

FILE *fr;
#define nParameter 17 //so many parameters
float parameters[nParameter];


//fw


float equationH(float par1,float par2,float z)
{
    float equation=z+sqrt(armLength*armLength-(-endLength-nutLength+sqrt(par1*par1+par2*par2))*(-endLength-nutLength+sqrt(par1*par1+par2*par2)));
    return equation;
}

float equationSteps(float h,float theta)
{
    //printf("h theta %f %f\n",h,theta);
    return -(-stepperrev*h/pitch+theta*stepperrev/(2.0*pi));
}


float equationStep1(float x,float y,float z)
{
    float par1=screwDistance/sqrt(3.0)+x;
    float par2=y;
    float h1=equationH(par1,par2,z);
    float theta1=atan2(par2,par1);
    return equationSteps(h1,theta1);
}

float equationStep2(float x,float y,float z)
{
    float par1=-sqrt(3.0)/2.0*screwDistance+screwDistance/sqrt(3.0)+x;
    float par2=y-screwDistance/2;
    float h2=equationH(par1,par2,z);
    float theta2=atan2(par2,par1);
    return equationSteps(h2,theta2);
}

float equationStep3(float x,float y,float z)
{
    float par1=-sqrt(3.0)/2.0*screwDistance+screwDistance/sqrt(3.0)+x;
    float par2=y+screwDistance/2;
    float h3=equationH(par1,par2,z);
    float theta3=atan2(par2,par1);
    return equationSteps(h3,theta3);
}

float equationServo(float x,float y,float z, float theta)
{
    float par1=screwDistance/sqrt(3.0)+x;
    float par2=y;
    //printf("x %f, y %f\n",par1,par2);
    float theta1=atan2(par2,par1);
    //printf("theta in local %f\n",theta1);
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
    
    //printf("local %f,%f,%f\n",cx,cy,cz);
    
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
            y[i]=3*JengaWidth-takeDistanceRadial;
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
            y[i]=3*JengaWidth-putDistanceRadial;
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
    printf("L,1\n");//pneumatic cylinder retracted
    printf("V,0\n");//vacuum off
    printf("M,%f,%f,%f,%f\n",x[0],y[0],z[0],theta[0]);//go to intial position, above the take position and oriented
    printf("M,%f,%f,%f,%f\n",x[1],y[1],z[1],theta[1]);//lower down
    printf("M,%f,%f,%f,%f\n",x[2],y[2],z[2],theta[2]);//get closer
    printf("L,0\n");//pneumatic cylinder extended
    printf("P\n");//pause for cv
    printf("V,1\n");//vacuum on
    printf("L,1\n");//take the block
    printf("M,%f,%f,%f,%f\n",x[3],y[3],z[3],theta[3]);//take away
    printf("M,%f,%f,%f,%f\n",x[4],y[4],z[4],theta[4]);//go up
    printf("M,%f,%f,%f,%f\n",x[5],y[5],z[5],theta[5]);//orient to put
    printf("M,%f,%f,%f,%f\n",x[6],y[6],z[6],theta[6]);//lower a bit
    printf("L,0\n");//neumatic cylinder extended
    printf("M,%f,%f,%f,%f\n",x[7],y[7],z[7],theta[7]);//get close
    printf("P\n");//pause for cv
    printf("V,0\n");//vacuum off, drop the block
    //M for move
    //V for vacuum
    //L for linear
}

int gotoPositionCheck(float x,float y,float z)//return 0 OKAY, return 1 error
{
    int ns[3],sv;
    move2globalxyz(x,y,z,0,&ns[0],&ns[1],&ns[2],&sv);
    for (int i=0; i<3; i++) {
        if ((ns[i]>0)||(ns[i]<-99999)) {
            printf("could not reach global xyz %f, %f, %f.\n",x,y,z);
            printf("steppers %d, %d, %d",ns[0],ns[1],ns[2]);
            return 1;
        }
    }
    return 0;
}

int parameterCheck() //return 0 OKAY, return 1 error
{
    float dangerousX[2],dangerousY[2],dangerousZ[2];
    dangerousX[0]=EndEffectorLongerLengthOffset;//cylinder extended
    dangerousX[1]=EndEffectorLengthOffset;
    
    dangerousZ[0]=JengaHeight*(0+0.5)+EndEffectorHeightOffset;// layer 0 z
    dangerousZ[1]=JengaHeight*(10+0.5)+EndEffectorHeightOffset;//layer 10 z
    for (int i=0; i<3; i++) {
        dangerousY[i]=(0.5+i)*JengaWidth;
    }
    //check all the combinations
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            for (int k=0; k<2; k++) {
                if (gotoPositionCheck(dangerousX[i],dangerousY[j],dangerousZ[k])) {
                    return 1;
                }
            }
        }
    }
    
    
    return 0;
}
//fw path




float getNumber(char* line,int length)
{
    char number[80];
    //take everything between = and ; into number[]
    int positionEqual=0,positionSemicolon=0;
    for (int i=0; i<length; i++) {
        if (line[i]=='=') {
            positionEqual=i;
        }
        if (line[i]==';') {
            positionSemicolon=i;
        }
    }
    
    strncpy(number,line+positionEqual+1,positionSemicolon-positionEqual-1);
    number[positionSemicolon-positionEqual-1]='\0';
    //printf("%s\n",number);
    return atof(number);
}

void readSettings()

{
    
    char line[80];
    
    fr = fopen ("settings.txt", "rt");
    
    for (int i=0; i<nParameter; i++) {
        if(fgets(line, 80, fr)==NULL)
        {
            break;
        }
        parameters[i]=getNumber(line,strlen(line));
    }
    
    fclose(fr);  /* close the file prior to exiting the routine */
    
    screwDistance=parameters[0];
    armLength=parameters[1];
    nutLength=parameters[2];
    endLength=parameters[3];
    pi=parameters[4];
    stepperrev=parameters[5];
    pitch=parameters[6];
    zoneLenth=parameters[7];
    endfXinit=parameters[8];
    endfYinit=parameters[9];
    endfZinit=parameters[10];
    JengaLength=parameters[11];
    JengaWidth=parameters[12];
    JengaHeight=parameters[13];
    EndEffectorHeightOffset=parameters[14];
    EndEffectorLengthOffset=parameters[15];
    EndEffectorLongerLengthOffset=parameters[16];
    
    
}