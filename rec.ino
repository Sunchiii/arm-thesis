#include <Servo.h>



#define BASESERVOPIN A0
#define ARM1SERVOPIN A1
#define ARM2SERVOPIN A2



Servo arm1servo;
Servo arm2servo;
Servo baseservo;

int x,y;

void toPos(double b,double a1, double a2){
  baseservo.write(b+90);
  arm1servo.write(188-a1);
  arm2servo.write(a2+101);
}


int angleToMicroseconds(double angle) {
  double val = 460.0 + (((2400.0 - 460.0) / 180.0) * angle);
  return (int)val;
}
void moveToAngle(double b, double a1, double a2) {
  arm1servo.writeMicroseconds(angleToMicroseconds(188 - a1));
  arm2servo.writeMicroseconds(angleToMicroseconds(a2+101));
  baseservo.writeMicroseconds(angleToMicroseconds(b+90));
}


void moveToPos(double x, double y, double z) {
  double b = atan2(y,x) * (180 / 3.1415); // base angle

  double l = sqrt(x*x + y*y); // x and y extension 

  double h = sqrt (l*l + z*z);

  double phi = atan(z/l) * (180 / 3.1415);

  double theta = acos((h/2)/75) * (180 / 3.1415);
  
  double a1 = phi + theta; // angle for first part of the arm
  double a2 = phi - theta; // angle for second part of the arm
  
  //moveToAngle(b,a1,a2);
  toPos(b,a1,a2);
  
  //smooth(b+90,188-a1,a2+101);
}
void resetPo(){
  baseservo.write(30);
  arm1servo.write(90);
  arm2servo.write(60);
  }

void workflow(int x,int y){
  resetPo();
  delay(1000);
  moveToPos(x,y,20);
  delay(1000);
  moveToPos(x,y,0);
  delay(1000);
  resetPo();
  delay(1000);
  // reset board
  asm volatile ("jmp 0");
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  baseservo.attach(BASESERVOPIN,460,2400);
  arm1servo.attach(ARM1SERVOPIN,460,2400);
  arm2servo.attach(ARM2SERVOPIN,460,2400);
  
}



void loop() {
  
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    String c = Serial.readStringUntil('\n');
    int state = 1;
    String wid,hei;
    for(int i=0;i < c.length();i++){
      if(c[i] == ','){
        state = 2;
        continue;
      }
      if(state == 1){
        wid += c[i];
      }else{
        hei += c[i];
      }
    }
    x = map(hei.toInt(),0,480,30,100);
    y = map(wid.toInt(),0,640,-50,50);
    workflow(x,y);
    state = 1;
  }
  
  delay(100);
}
