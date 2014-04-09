///TEAM MECHATRONIQUE MAR 19 MANUAL CONTROL OF DELTA ROBOT

////New! Servo included. from Knob example in Servo Library


#include <AccelStepper.h>
#include <Servo.h>

//Servo endEffectorServo;  // create servo object to control a servo
// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::FULL4WIRE,5,4,2,3);
AccelStepper stepper3(AccelStepper::FULL4WIRE,7,6,8,9);
AccelStepper stepper2(AccelStepper::FULL4WIRE,13,12,11,10);

//fw



int spinPotentiometerPin = 2;  // analog pin used to connect the potentiometer
int spinPotValue;    // variable to read the value from the analog pin
int revoluteDestination;
int spinServoPin = A0; //(A0)
int rodSpanInches = 11;//desired span of lead screws
int heightAtStartOnPot = 1023; //value of pot at boot
float maxSpeed = 400.0;//I have not yet played with this. Higher is faster, obvi
float maxAccel = 300.0;
char need_new;
char first_goal;
char mov; //do we need to move?
char v; //do we need to change vaccume
char lin; //do we need to change the linear actuator state
int lin_pin;
int vac_pin;


void move_ee(int goal1, int goal2, int goal3, int angle){
  int deadBand = 1;
  stepper1.moveTo(goal1); //moveTo only sets goal, run actually moves it.
  stepper1.setSpeed(maxSpeed); //set speed only sets speed, run actually moves it.
  stepper2.moveTo(goal2);
  stepper2.setSpeed(maxSpeed);
  stepper3.moveTo(goal3);
  stepper3.setSpeed(maxSpeed);
  
  while(1){
    if (abs(stepper1.distanceToGo()) > deadBand)
    {
      stepper1.runSpeedToPosition();//if it is far from its goal, spin the motor
    }



    if (abs(stepper2.distanceToGo()) > deadBand)
    {
      stepper2.runSpeedToPosition();
    }

  
    if (abs(stepper3.distanceToGo()) > deadBand)
    {
      stepper3.runSpeedToPosition();
    }
  
    if ((abs(stepper1.distanceToGo() <= deadBand)) && (abs(stepper2.distanceToGo()) <= deadBand) && (abs(stepper3.distanceToGo()) <= deadBand)){
      break;
    }
  }
  //endEffectorServo.write(angle);
  mov = 0;
  
  return;

}

void setup()
{

  ////TO ADD: move up until limit switch colllision, reset accel stepper zero

  Serial.begin(9600);
  first_goal = 1;
  mov = 0;//initially don't assume we have anything to do. 
  v = 0;
  lin = 0;
  //endEffectorServo.attach(spinServoPin);
  //endEffectorServo.write(0);  // attaches the servo on pin 9 to the servo object
  //digitalWrite(lin_pin, LOW);
  //digitalWrite(vac_pin, HIGH);

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
char vac; //vaccume
char lin_ac; //linear actuator



void read_jenga_serial(){
  int i = 0;
  //Serial.println('in reading');
  memset(read_string, 0, 30);
  memset(step1, 0, 8);
  memset(step2, 0, 8);
  memset(step3, 0, 8);
  memset(theta, 0, 8);
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
        //Serial.println(read_string);
      break;
    }
  }
  if (read_string[0] == 'M')
  {
    memcpy(step1, read_string+2, 5);
    memcpy(step2, read_string+9, 5);
    memcpy(step3, read_string+16, 5);
    memcpy(theta, read_string+23, 4);
    mov = 1;
    Serial.print(step1);
    Serial.print(',');
    Serial.print(step2);
    Serial.print(',');
    Serial.print(step3);
    Serial.println(',');
    //Serial.println(read_string);
  }
  else if (read_string[0] == 'L'){
    lin_ac = read_string[2];
    lin = 1;
      
  }
  else if (read_string[0] == 'V'){
    vac = read_string[2];
    v = 1;
  }      
  return;
  
}

void set_vacuum(int state){
  if (state){
    //digitalWrite(vac_pin, LOW);
  }
  else{
    //digitalWrite(vac_pin, HIGH);
  }
  v = 0;
  delay(500);
  return;
}

void set_linear(int state){
  if (state){
    //digitalWrite(lin_pin, HIGH);
  }
  else{
    //digitalWrite(vac_pin, LOW);
  }
  lin = 0;
  delay(500);
  return;
}

void loop()
{
  read_jenga_serial();
  if (mov){
      int goal1,goal2,goal3,svgoal;
      goal1 = atoi(step1);
      goal2 = atoi(step2);
      goal3 = atoi(step3);
      svgoal = atoi(theta);
      move_ee(goal1, goal2, goal3, svgoal);
    }
    else if (v){
      int vac_state;
      if (vac == '0') {
        vac_state = 0;
      }
      else{
        vac_state = 1;
      }     
      set_vacuum(vac_state);
    }
    else if (lin){
      int lin_state;
      if (lin_ac == '0'){
        lin_state = 0;
      }
      else{
        lin_state = 1;
      }
      set_linear(lin_state);
    }
}
