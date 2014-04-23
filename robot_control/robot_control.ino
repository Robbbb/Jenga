//#include <SoftwareServo.h>
#include <Servo.h>

///TEAM MECHATRONIQUE MAR 19 MANUAL CONTROL OF DELTA ROBOT

////New! Servo included. from Knob example in Servo Library


#include <Stepper.h>


//SoftwareServo endEffectorServo;  // create servo object to control a servo
Servo endEffectorServo;

// Define some steppers and the pins the will use
Stepper stepper1(200,5,4,2,3);
Stepper stepper2(200,6,7,8,9);
Stepper stepper3(200,13,12,11,10);

//fw


int pos1;
int pos2;
int pos3;
int spinServoPin = 14; //(A0)
int rodSpanInches = 11;//desired span of lead screws
int maxSpeed = 100;//I have not yet played with this. Higher is faster, obvi
//float maxAccel = 300.0;
int need_new;
int first_goal;
int mov; //do we need to move?
int v; //do we need to change vaccume
int lin; //do we need to change the linear actuator state
int lin_pin = 16;
int vac_pin = 15;


void move_ee(int goal1, int goal2, int goal3, int svgoal){
  endEffectorServo.write(svgoal);
  int deadBand = 1;
  int error1, error2, error3;
  int val1, val2, val3;
  int sign1, sign2, sign3;
  int max_step = 1;
  //SoftwareServo::refresh();
  while(1)
  {
    error1 = goal1-pos1;
    error2 = goal2-pos2;
    error3 = goal3-pos3;
    sign1 = error1>0?1:-1;
    sign2 = error2>0?1:-1;
    sign3 = error3>0?1:-1;
    val1 = abs(error1);
    val1 = val1>max_step?max_step:val1;
    val2 = abs(error2);
    val2 = val2>max_step?max_step:val2;
    val3 = abs(error3);
    val3 = val3>max_step?max_step:val3;
    if (val1>0)
    {
      stepper1.step(val1*sign1);//if it is far from its goal, spin the motor
    }



    if (val2 > 0)
    {
      stepper2.step(val2*sign2);
    }

  
    if (val3 > 0)
    {
      stepper3.step(val3*sign3);
    }
    pos1 += val1*sign1;
    pos2 += val2*sign2;
    pos3 += val3*sign3;
    
    if (!val1 && !val2 && !val3)
    {
      break;
    }
    
   //SoftwareServo::refresh();
  }
  
  mov = 0;
  
  delay(20);
  //SoftwareServo::refresh();
  
  
  return;

}

char vac; //vaccume
char lin_ac; //linear actuator
void setup()
{

  ////TO ADD: move up until limit switch colllision, reset accel stepper zero

  Serial.begin(9600);
  first_goal = 1;
  mov = 0;//initially don't assume we have anything to do. 
  v = 0;
  lin = 0;
  vac = 0;
  lin_ac = 0;
  mov = 0;
  pos1 = 0;
  pos2 = 0;
  pos3 = 0;
  endEffectorServo.attach(spinServoPin);
  endEffectorServo.write(90);  // attaches the servo on pin 9 to the servo object
  pinMode(lin_pin, OUTPUT);
  pinMode(vac_pin, OUTPUT);
  digitalWrite(lin_pin, LOW);
  digitalWrite(vac_pin, LOW);
  stepper1.setSpeed(maxSpeed);
  stepper2.setSpeed(maxSpeed);
  stepper3.setSpeed(maxSpeed);

  //  float initShift = 0.0; //unknown units to move all motors at boot
  /*
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
*/
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

int myAtoi(char *str){
  int res = 0;
  int sign = 1;
  int i= 0;
  if (str[0] == '-'){
    sign = -1;
    i ++;
  }
  
  for (; str[i] != '\0'; ++i)
    res = res*10 + (str[i] - '0');
  
  return sign*res;
}


void read_jenga_serial(){
  int i = 0;
  //Serial.println("in reading");
  memset(read_string, 0, 30);
  memset(step1, 0, 8);
  memset(step2, 0, 8);
  memset(step3, 0, 8);
  memset(theta, 0, 8);
  if (!first_goal){
    Serial.println("done"); 
  }
  else{
    first_goal = 0;
  }
  while(1){
    //SoftwareServo::refresh();
    delay(20);
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
    memcpy(step2, read_string+8, 5);
    memcpy(step3, read_string+14, 5);
    memcpy(theta, read_string+20, 4);
    mov = 1;
    //Serial.println(theta);
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
  //SoftwareServo::refresh();
  return;
  
}

void set_vacuum(int state){
  if (state){
    digitalWrite(vac_pin, HIGH);
  }
  else{
    digitalWrite(vac_pin, LOW);
  }
  v = 0;
  int i = 0;
  while (i < 20)
  {
    //SoftwareServo::refresh();
    delay(25);
    i++;
  }
  return;
}

void set_linear(int state){
  if (state){
    digitalWrite(lin_pin, HIGH);
  }
  else{
    digitalWrite(lin_pin, LOW);
  }
  lin = 0;
  int i = 0;
  while (i < 20){
   //SoftwareServo::refresh();
   delay(50);
   i++ ;    
  }
  return;
}

void loop()
{
  read_jenga_serial();
  if (mov){
      char test[3];
      test[0] = '4';
      test[1] = '2';
      test[2] = 0;
      int goal1,goal2,goal3,svgoal;
      goal1 = myAtoi(step1);
      goal2 = myAtoi(step2);
      goal3 = myAtoi(step3);
      svgoal = myAtoi(theta);
      
      move_ee(goal1, goal2, goal3, svgoal);
      
      mov = 0;
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
    int i = 0;
    delay(5); 
    ////SoftwareServo::refresh();
}
