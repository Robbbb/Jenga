///TEAM MECHATRONIQUE MAR 19 MANUAL CONTROL OF DELTA ROBOT

////New! Servo included. from Knob example in Servo Library


#include <AccelStepper.h>
#include <Servo.h>

Servo endEffectorServo;  // create servo object to control a servo
// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::FULL4WIRE,5,4,2,3);
AccelStepper stepper3(AccelStepper::FULL4WIRE,7,6,8,9);
AccelStepper stepper2(AccelStepper::FULL4WIRE,13,12,11,10);

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


int spinPotentiometerPin = 2;  // analog pin used to connect the potentiometer
int spinPotValue;    // variable to read the value from the analog pin
int revoluteDestination;
int spinServoPin = A0; //(A0)
int pot1 = 3;//analog pins for slide pots
int pot2 = 4;
int pot3 = 5;
int rodSpanInches = 11;//desired span of lead screws
int heightAtStartOnPot = 1023; //value of pot at boot
float maxSpeed = 400.0;//I have not yet played with this. Higher is faster, obvi
float maxAccel = 300.0;
char need_new;
char first_goal;

void setup()
{

  ////TO ADD: move up until limit switch colllision, reset accel stepper zero

  Serial.begin(9600);
  first_goal = 1;
  need_new = 1;
  endEffectorServo.attach(spinServoPin);  // attaches the servo on pin 9 to the servo object

  //  float initShift = 0.0; //unknown units to move all motors at boot
  //
  stepper1.setMaxSpeed(maxSpeed);
  stepper1.setAcceleration(maxAccel);
  //  stepper1.moveTo(initShift);     //moveTo:The desired absolute position. Negative is anticlockwise.
  //
  stepper2.setMaxSpeed(maxSpeed);
  stepper2.setAcceleration(maxAccel);
  //  stepper2.moveTo(initShift);
  //
  stepper3.setMaxSpeed(maxSpeed);
  stepper3.setAcceleration(maxAccel);
  //  stepper3.moveTo(initShift);
}
// void zeroOutAxis(int limitSwitchPin){///ooop doesn't work....
//   stepper.setMaxSpeed(maxSpeed);
//   stepper.setAcceleration(maxAccel);
//   //  stepper.moveTo(initShift);

// }

char read_string[30];
char step1[8];
char step2[8];
char step3[8];
char theta[8];

//float path[]={37.500000, -109.220001, 252.800003,37.500000, -109.220001, 200.300003,37.500000, -101.599998, 200.300003,37.500000, -109.220001, 200.300003, 37.500000, -109.220001, 252.800003,37.500000, -109.220001, 252.800003 , 37.500000, -109.220001, 245.300003 ,37.500000, -101.599998, 245.300003};
//int destination=0;
void loop()
{

  //spinPotValue = analogRead(spinPotentiometerPin);// reads the value of the potentiometer (value between 0 and 1023)
  //revoluteDestination = map(spinPotValue, 0, 1023, 0, 179); // scale it to use it with the servo (value between 0 and 180)
//  endEffectorServo.write(revoluteDestination);  // sets the servo position according to the scaled value , spins servo
  //delay(1);
  
  if (need_new){
    int i = 0;
    if (!first_goal){
      Serial.println('d'); 
    }
    else{
      first_goal = 0;
    }
    while(1){
      while (Serial.available()){
        char c = Serial.read();
        read_string[i] = c;
        i++;
        //Serial.println("possible success");
        delay(3);
      }
      if (i != 0){
        i = 0;
        need_new = 0;
        //Serial.println(read_string);
        break;
      }
    }
  }
  memcpy(step1, read_string, 5);
  memcpy(step2, read_string+7, 5);
  memcpy(step3, read_string+14, 5);
  memcpy(theta, read_string+21, 4);
  //Serial.println("out of dat shit");
  

  //int potVal1 = analogRead(pot1);//update value of slide pots on manual controller
  //int potVal2 = analogRead(pot2);
  //int potVal3 = analogRead(pot3);
  //int minnn = -1600; //This is the number of steps from the top of the rod to the bottom of the rod.
  ///200 steps per revolution on this motor.

//  int goal1 =  map(potVal2,1023,0,0,minnn);
//  int goal2 =  map(potVal1,1023,0,0,minnn);/////LOOK CLOSE! I swapped 1 & 2 so the controller would work.
//  int goal3 =  map(potVal3,1023,0,0,minnn);



  int deadBand = 1; //how close it needs to be to its goal before stopping. Proportional control may be better.
  //float x=40,y=-100,z=250,theta=0.0;//ATTENSION HERE IS THE DESTINATION.
  int goal1,goal2,goal3,svgoal;
  goal1 = atoi(step1);
  goal2 = atoi(step2);
  goal3 = atoi(step3);
  //move2globalxyz(x,y,z,theta,&goal1,&goal2,&goal3,&svgoal);
  //temporary testing start
  //goal1 = -1578;
  //goal2 = -2624;
  //goal3 = -1258;
  //end
    stepper1.moveTo(goal1); //moveTo only sets goal, run actually moves it.
    stepper1.setSpeed(maxSpeed); //set speed only sets speed, run actually moves it.
    if (abs(stepper1.distanceToGo()) > deadBand)
    {
      stepper1.runSpeedToPosition();//if it is far from its goal, spin the motor
    }

    stepper2.moveTo(goal2);
    stepper2.setSpeed(maxSpeed);
    if (abs(stepper2.distanceToGo()) > deadBand)
    {
      stepper2.runSpeedToPosition();
    }

    stepper3.moveTo(goal3);
    stepper3.setSpeed(maxSpeed);
    if (abs(stepper3.distanceToGo()) > deadBand)
    {
      stepper3.runSpeedToPosition();
    }
    if ((abs(stepper1.distanceToGo() <= deadBand)) && (abs(stepper2.distanceToGo()) <= deadBand) && (abs(stepper3.distanceToGo()) <= deadBand)){
      //Serial.println(read_string); //telll computer we are done
      need_new = 1;
    }
   
    //:Serial.println(goal1);
    //Serial.println(goal2);
    //Serial.println(goal3);
  ////PRINTING SLOWS SPEEED signifiantly!

  //  Serial.print("distanceToGo1: ");py
  //  Serial.println(stepper1.distanceToGo());
  // Serial.print("\tpotVal1: ");
  //  Serial.println(potVal1);
  // //Serial.print("distanceToGo2: ");
  //  Serial.println(stepper2.distanceToGo());
  //  Serial.print("\tpotVal2: ");
  //  Serial.println(potVal2);
  // Serial.print("distanceToGo1: ");
  //  Serial.println(stepper1.distanceToGo());
  //  Serial.print("\tpotVal1: ");
  //  Serial.println(potVal1);
}
