#include <string.h>
FILE *fr;
#define nParameter 17 //so many parameters
float parameters[nParameter];


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