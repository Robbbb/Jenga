#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define nParameter 31
FILE* fr;
double parameters[nParameter];

double floor_height=112;
double l0_s1=-1690,l0_s2=-0888.0,l0_s3=-2000.0,l0_sv=0058.0,l0_lv=5;
double l1_s1=-1618,l1_s2=-0913.0,l1_s3=-1921.0,l1_sv=0060.0,l1_lv=5;
double l2_s1=-1138,l2_s2=-0075.0,l2_s3=-1465.0,l2_sv=0063.0,l2_lv=6;
double r0_s1=-1781,r0_s2=-2421.0,r0_s3=-0913.0,r0_sv=0145.0,r0_lv=6;
double r1_s1=-1781,r1_s2=-2421.0,r1_s3=-0913.0,r1_sv=0145.0,r1_lv=6;
double r2_s1=-1781,r2_s2=-2421.0,r2_s3=-0913.0,r2_sv=0145.0,r2_lv=6;

void GenerateMotorCoord(int layer, int position,int firstLayerBlockAlignWithX,int *s1,int* s2,int *s3,int *sv)
{
    int left=0;//take from left or not
    if (firstLayerBlockAlignWithX) {
        //first layer 0
        if (layer%2) {
            //this layer 0
            //left
            left=1;
        }
        else {
            //this layer 90
            //right
            left=0;
        }
    }
    else
    {
        //first layer 90
        if (layer%2) {
            //this layer90
            //right
            left=0;
        }
        else{
            //this layer0
            //left
            left=1;
        }
    }
    if (left) {
        switch (position) {
            case 0:
                //l0
                *sv=l0_sv;
                *s1=l0_s1+floor_height*(layer-l0_lv);
                *s2=l0_s2+floor_height*(layer-l0_lv);
                *s3=l0_s3+floor_height*(layer-l0_lv);
                break;
            case 1:
                *sv=l1_sv;
                *s1=l1_s1+floor_height*(layer-l1_lv);
                *s2=l1_s2+floor_height*(layer-l1_lv);
                *s3=l1_s3+floor_height*(layer-l1_lv);
                break;
            case 2:
                *sv=l2_sv;
                *s1=l2_s1+floor_height*(layer-l2_lv);
                *s2=l2_s2+floor_height*(layer-l2_lv);
                *s3=l2_s3+floor_height*(layer-l2_lv);
                break;
                
            default:
                break;
        }
    }
    else{
        switch (position) {
            case 0:
                //r0
                *sv=r0_sv;
                *s1=r0_s1+floor_height*(layer-r0_lv);
                *s2=r0_s2+floor_height*(layer-r0_lv);
                *s3=r0_s3+floor_height*(layer-r0_lv);
                break;
            case 1:
                *sv=r1_sv;
                *s1=r1_s1+floor_height*(layer-r1_lv);
                *s2=r1_s2+floor_height*(layer-r1_lv);
                *s3=r1_s3+floor_height*(layer-r1_lv);
                break;
            case 2:
                *sv=r2_sv;
                *s1=r2_s1+floor_height*(layer-r2_lv);
                *s2=r2_s2+floor_height*(layer-r2_lv);
                *s3=r2_s3+floor_height*(layer-r2_lv);
                break;
                
            default:
            break;
        }
    }
}

void GeneratePath(int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX)//layer number from 0, position number is from 0 to 2 and 0 is the closest to the origin
{
    
    //0 -100，-100，-100
    //goto position
    //extend
    //pause
    //vacuum on
    //retract
    //goto put position
    //extend
    
    printf("V,0\n");//vacuum off
    printf("L,1\n");//pneumatic cylinder retracted
    printf("M,%d,%d,%d,%d\n",0,0,0,100);//go to intial position, above the take position and oriented
    
    int s1,s2,s3,sv;
    GenerateMotorCoord(fromLayer,fromPosition,firstLayerBlockAlignWithX,&s1,&s2,&s3,&sv);
    printf("M,%d,%d,%d,%d\n",s1,s2,s3,sv);//go to  position, above the take position and oriented
    printf("L,0\n");//pneumatic cylinder extend
    printf("P\n");//pause for cv
    printf("V,1\n");//vacuum on
    printf("L,1\n");//pneumatic cylinder retracted
    
    //int s1,s2,s3,sv;
    GenerateMotorCoord(toLayer,toPosition,firstLayerBlockAlignWithX,&s1,&s2,&s3,&sv);
    printf("M,%d,%d,%d,%d\n",s1,s2,s3,sv);//go to put position, above the take position and oriented
    
    printf("L,0\n");//pneumatic cylinder extend
    printf("P\n");//pause for cv
    printf("V,0\n");//vacuum off
    
    

    

}




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
    
    fr = fopen ("step_calibrate.txt", "rt");
    
    for (int i=0; i<nParameter; i++) {
        if(fgets(line, 80, fr)==NULL)
        {
            break;
        }
        parameters[i]=getNumber(line,strlen(line));
    }
    
    fclose(fr);
    
    l0_s1=parameters[0];
    l0_s2=parameters[1];
    l0_s3=parameters[2];
    l0_sv=parameters[3];
    l0_lv=parameters[4];
    
    l1_s1=parameters[5];
    l1_s2=parameters[6];
    l1_s3=parameters[7];
    l1_sv=parameters[8];
    l1_lv=parameters[9];
    
    l2_s1=parameters[10];
    l2_s2=parameters[11];
    l2_s3=parameters[12];
    l2_sv=parameters[13];
    l2_lv=parameters[14];
    
    r0_s1=parameters[15];
    r0_s2=parameters[16];
    r0_s3=parameters[17];
    r0_sv=parameters[18];
    r0_lv=parameters[19];
    
    r1_s1=parameters[20];
    r1_s2=parameters[21];
    r1_s3=parameters[22];
    r1_sv=parameters[23];
    r1_lv=parameters[24];
    
    r2_s1=parameters[25];
    r2_s2=parameters[26];
    r2_s3=parameters[27];
    r2_sv=parameters[28];
    r2_lv=parameters[29];
    
    
    floor_height=parameters[30];
    
    
}