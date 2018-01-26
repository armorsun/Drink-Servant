#include "pump.h"

void pump_init(){
  pinMode(w1, OUTPUT);
  pinMode(w2, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
}

//0  water
//1  milk
//2  black T
//3  milk T
void stop_pump(){
  digitalWrite(w1, LOW);
  digitalWrite(w2, LOW);
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
}

void pump_drinks(int d){
  if(d == 0){ //water
    Serial.println("pumping water!");
    water(6000);
    return;
  }
  if(d==1){ //milk
    Serial.println("pumping milk!");
    milk(6000);
    return;
  }
  if(d==2){ //black tea
    Serial.println("pumping black tea!");
    black_T(6000);
    return;
  }
  if(d==3){ //milk tea
    Serial.println("pumping milk tea!");
    black_T(4000);
    milk(2000);
    return;
    
  }
  return;
}

void water(int s){
    digitalWrite(w1, HIGH);
    digitalWrite(w2, LOW);
    delay(s);
    stop_pump();
}

void milk(int s){   
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    delay(s);
    stop_pump();
}

void black_T(int s){
  digitalWrite(b1, HIGH);
  digitalWrite(b2, LOW);
  delay(s);
  stop_pump();
}





