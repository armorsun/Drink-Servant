#include <Servo.h>
#include <Wire.h>

#include "Arm.h"
#include "claw_control.h"
#include "pump.h"

Servo servo_;
Servo servoR;
Servo servoL;
Arm A(2050, 1350, 1180, servoL, servoR, servo_);   //L R M

void setup() {
  // put your setup code here, to run once:
  servo_.attach(2);
  servoR.attach(3);
  servoL.attach(6);
  
  servoR.writeMicroseconds(1350);  //r 
  servoL.writeMicroseconds(2050);//l 
  servo_.writeMicroseconds(1180);    //1180   1590

  Wire.begin(0x2A);
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);

  claw_init();
  pump_init();
  Serial.println("ready");

  
  delay(2000);
  claw_control("Close");
}

void loop() {

  String received = "";
    while (0 < Wire.available()) //pra rolar com i2cset no shell
  {
    char c = Wire.read(); // receive byte as a character
    received.concat(String(c));
  }
  
  Serial.println(received);
  
  if (received.indexOf("arm") >= 0) {
    received = received.substring(3);
    //Serial.println("received---------");
    //Serial.println(received.substring(1).toDouble());
    if(received.indexOf("X") >= 0) A.toX(received.substring(1).toDouble());
    else if(received.indexOf("Y") >= 0) A.toY(received.substring(1).toDouble());
    else if(received.indexOf("Z") >= 0) A.toZ(received.substring(1).toDouble());
    else if(received.indexOf("R") >= 0) A.rotate(received.substring(1).toDouble());
    else if(received.indexOf("HOME") >= 0) A.goHome();
  }
  else if (received.indexOf("claw") >= 0) {
    if(received.indexOf("Open") >= 0)
    {
        claw_control("Open");
    }else if(received.indexOf("Close") >= 0)
    {
      claw_control("Close");
    }
  }
  else if (received.indexOf("pump") >= 0) {
    pump_drinks(received.substring(4).toInt());
  }
  else if (received.indexOf("move") >= 0) {
    Serial.print("motor value: ");
    Serial.println(received.substring(4).toInt());
  //  move_car(received.substring(4).toInt());
  }else if (received.indexOf("stop") >= 0)
  {
    //stop();
  }
  
//  delay(2000);
//  for(int i= 0; i < 20; i++){
//    A.moveX(1);
//    delay(10);
//  }
//  for(int i= 0; i < 15; i++){
//    A.moveZ(1);
//    delay(10);
//  }
//  delay(1000);
//  A.printConfig();
}

void receiveEvent(int howMany){
}
