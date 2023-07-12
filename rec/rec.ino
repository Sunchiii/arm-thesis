#include <ServoEasing.hpp>
#include <AccelStepper.h>



#define BASESERVOPIN A0
#define ARM1SERVOPIN A1
#define ARM2SERVOPIN A2
#define relayIN1 7
#define STEPPIN 2 
#define DIRPIN 5


int x,y;
int servoSpeed = 60;

ServoEasing arm1servo;
ServoEasing arm2servo;
ServoEasing baseservo;

AccelStepper stepWeel(1,STEPPIN,DIRPIN);

void toPos(int b,int a1, int a2){
  baseservo.startEaseTo(b+170,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm1servo.startEaseTo(188-a1,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm2servo.startEaseTo(a2+101,servoSpeed,START_UPDATE_BY_INTERRUPT);
}


int angleToMicroseconds(double angle) {
  double val = 460.0 + (((2400.0 - 460.0) / 180.0) * angle);
  return (int)val;
}
void moveToAngle(double b, double a1, double a2) {
  arm1servo.writeMicroseconds(angleToMicroseconds(188 - a1));
  arm2servo.writeMicroseconds(angleToMicroseconds(a2+101));
  baseservo.writeMicroseconds(angleToMicroseconds(b+170));
}

 
void moveToPos(double x, double y, double z) {
  double b = atan2(y,x) * (180 / 3.1415); // base angle

  double l = sqrt(x*x + y*y); // x and y extension 

  double h = sqrt (l*l + z*z);

  double phi = atan(z/l) * (180 / 3.1415);

  double theta = acos((h/2)/134) * (180 / 3.1415);

  
  double a1 = phi + theta; // angle for first part of the arm
  double a2 = phi - theta; // angle for second part of the ar


  
  //moveToAngle(b,a1,a2);
  toPos(b,a1,a2);
}

void resetPo(){
  baseservo.startEaseTo(90,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm1servo.startEaseTo(90,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm2servo.startEaseTo(45,servoSpeed,START_UPDATE_BY_INTERRUPT);
  }

void toTriAngle(){
  baseservo.startEaseTo(40,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm1servo.startEaseTo(90,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm2servo.startEaseTo(45,servoSpeed,START_UPDATE_BY_INTERRUPT);
}
void toCir(){
  baseservo.startEaseTo(30,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm1servo.startEaseTo(120,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm2servo.startEaseTo(45,servoSpeed,START_UPDATE_BY_INTERRUPT);
}
void toSquare(){
  baseservo.startEaseTo(20,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm1servo.startEaseTo(140,servoSpeed,START_UPDATE_BY_INTERRUPT);
  arm2servo.startEaseTo(60,servoSpeed,START_UPDATE_BY_INTERRUPT);
}

void workflow(int x,int y,String shape){
  //resetPo();
  delay(1000);
  moveToPos(x-10,y,10);
  suck();
  delay(2000);
  moveToPos(x-10,y,-30);
  delay(2000);
  moveToPos(x-10,y,10);
  delay(2000);
  if(shape == "3"){
    toTriAngle();
  }
  else if(shape == "4"){
    toSquare();
  }
  else if(shape == "9"){
    toCir();
  }
  delay(4000);
  sick();
  delay(1500);
  Serial.print("finish");
  delay(1000);
  // reset board
  //asm volatile ("jmp 0");
}

void testx(){
  int zp = 10;
  moveToPos(40,0,zp);
  delay(1000);
  moveToPos(70,0,zp);
  delay(1000);
  moveToPos(100,0,zp);
  delay(1000);
  moveToPos(115,0,zp);
  delay(1000);
  moveToPos(130,0,zp);
  delay(1000);
  moveToPos(170,0,zp);
  delay(1000);
  }

void testy(){
  moveToPos(100,-70,10);
  delay(2000);
  moveToPos(100,-40,10);
  delay(1000);
  moveToPos(100,-30,10);
  delay(1000);
  moveToPos(100,-20,10);
  delay(1000);
  moveToPos(100,-10,10);
  delay(1000);
//  moveToPos(100,0,10);
//  delay(1000);
  }



void suck(){
  digitalWrite(relayIN1,HIGH);
  }

void sick(){
  digitalWrite(relayIN1,LOW);
  }



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  baseservo.attach(BASESERVOPIN,90);
  arm1servo.attach(ARM1SERVOPIN,90);
  arm2servo.attach(ARM2SERVOPIN,45);
  stepWeel.setMaxSpeed(1000);
  stepWeel.setSpeed(15);
  pinMode(relayIN1,OUTPUT);
}


void loop() {
   //put your main code here, to run repeatedly:
  
  stepWeel.runSpeed();
  while(Serial.available()){
    String c = Serial.readStringUntil('\n');
    int state = 1;
    String wid,hei,shape;
    for(int i=0;i < c.length();i++){
      if(c[i] == ','){
        state = 2;
        continue;
      }
      if(c[i] == '!'){
        state = 3;
        continue;
      }
      if(state == 1){
        wid += c[i];
      }
      else if(state == 2){
        hei += c[i];
      }
      else{
        shape += c[i];
      }
    }
    x = map(hei.toInt(),0,480,50,170);
    y = map(wid.toInt(),640,0,-70,60);

    if(x < 120){
      workflow(x,y+15,shape);
    }else{
      workflow(x,y,shape);
      }
    state = 1;
  }
  
  //delay(100);
}
