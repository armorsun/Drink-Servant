#include <math.h>
#include <Arduino.h>
#include "Arm.h"
#include <Servo.h>


Arm::Arm(int msL, int msR, int msRotate, Servo L, Servo R, Servo M){
  Angle A;
  A.set_msR(msR);
  A.set_msL(msL);
  A.set_msRotate(msRotate);
  default_config = A;
  temp_config = A;
  sL = L;
  sR = R;
  s_ = M;
}

void Arm::changeTurning(){
  turning_config = temp_config;
}
void Arm::goHome(){
  int disL = default_config.get_msL() - temp_config.get_msL();
  int indL = 10*disL/abs(disL);
  int disR = default_config.get_msR() - temp_config.get_msR();
  int indR = 10*disR/abs(disR);
  int dis_ = default_config.get_msRotate() - temp_config.get_msRotate();
  int ind_ = 10*dis_/abs(dis_);
  while(abs(disL) > 1){
    temp_config.set_msL(temp_config.get_msL()+indL);
    moveTo(temp_config);
    delay(10);
    disL = default_config.get_msL() - temp_config.get_msL();
    if (abs(disL) <= 10) indL = disL/abs(disL);
  }
  while(abs(disR) > 1){
    temp_config.set_msR(temp_config.get_msR()+indR);
    moveTo(temp_config);
    delay(10);
    disR = default_config.get_msR() - temp_config.get_msR();
    if (abs(disR) <= 10) indR = disR/abs(disR);
  }
  while(abs(dis_) > 1){
    temp_config.set_msRotate(temp_config.get_msRotate()+ind_);
    moveTo(temp_config);
    delay(10);
    dis_ = default_config.get_msRotate() - temp_config.get_msRotate();
    if (abs(dis_) <= 10) ind_ = dis_/abs(dis_);
  }
}

Angle Arm::straight(Angle A){
  double offset = L_len*sin(temp_config.get_angleL('r')) - R_len*sin(temp_config.get_angleR('r'));
  double var_h = L_len*sin(A.get_angleL('r')) - offset;
  double thetaR = asin(var_h/R_len)/PI*180.0; 
  A.set_angleR(thetaR);
  return A;    
}

void Arm::moveStraight(double stepLen){
  //if (temp_config.get_angleR('d') <= 20 || temp_config.get_angleL('d') <= 20) return;
  double len = straight_len(temp_config) + stepLen;
  Angle A = converge(len, temp_config);
  moveTo(A);
  
}
double Arm::straight_len(Angle A){
  return L_len*cos(A.get_angleL('r')) + R_len*cos(A.get_angleR('r'));
}

double Arm::height(Angle A){
  return L_len*sin(A.get_angleL('r')) - R_len*sin(A.get_angleR('r'));
}
void Arm::moveTo(Angle A){
  if (Restrict(A)) return;
  Serial.println("moveto");
  sL.writeMicroseconds(A.get_msL());
  Serial.println(A.get_msL());
  sR.writeMicroseconds(A.get_msR());
  Serial.println(A.get_msR());
  s_.writeMicroseconds(A.get_msRotate());
  Serial.println(A.get_msRotate());
  delay(50);
  temp_config = A;
}

void Arm::toX(double len){
  Serial.println("toX------");
  Serial.println(len);
  if(abs(len) < 1.0){
    moveX(len);
    return;
  }
  double step_len = len/abs(len);
  moveX(step_len);
  len -= step_len;
  toX(len);
}

void Arm::toY(double len){
  if(abs(len) < 1.0){
    moveY(len);
    return;
  }
  double step_len = len/abs(len);
  moveY(step_len);
  len -= step_len;
  toY(len);
}

void Arm::toZ(double len){
  if(abs(len) < 1.0){
    moveZ(len);
    return;
  }
  double step_len = len/abs(len);
  moveZ(step_len);
  len -= step_len;
  toZ(len);
}


void Arm::moveX(double step_len){
  if (temp_config.get_angleRotate() >= 179 &&temp_config.get_angleRotate() <= 181){
    moveStraight(-step_len);
    return;
  }
  double len = straight_len(temp_config);
  double thetaRotate = temp_config.get_angleRotate('r');
  double x = len*cos(thetaRotate);
  double y = len*sin(thetaRotate);
  x += step_len;
  thetaRotate = toPolar(x,y);
  double newlen = sqrt(x*x + y*y);
  Angle A = temp_config;
  A.set_angleRotate(thetaRotate/PI*180);
  moveTo(A);
  moveStraight(newlen - len);
}
void Arm::moveY(double step_len){
  if (temp_config.get_angleRotate() >= 89 && temp_config.get_angleRotate() <= 91){
    moveStraight(step_len);
    return;
  } //
  double len = straight_len(temp_config);
  double thetaRotate = temp_config.get_angleRotate('r');
  double x = len*cos(thetaRotate);
  double y = len*sin(thetaRotate);
  y += step_len;
  thetaRotate = toPolar(x,y);
  double newlen = sqrt(x*x + y*y);
  Angle A = temp_config;
  A.set_angleRotate(thetaRotate/PI*180);
  moveTo(A);
  moveStraight(newlen - len);
}

void Arm::moveZ(double step_len){
   double z = height(temp_config);
   Serial.println(z);
   z += step_len;
   double fix_len = straight_len(temp_config);
   double slopeLen = sqrt(pow(z,2) + pow(fix_len,2));
   double thetaL = CosineRule(R_len, L_len, slopeLen) + asin(z/slopeLen);
   Angle A = temp_config;
   A.set_angleL(thetaL*180/PI);
   moveTo(fix_straight(fix_len, A));
}

Angle Arm::converge(double len, Angle A){
  double thetaL;
  if (len > straight_len(A)){
     while(len >= straight_len(A)){
        thetaL = A.get_angleL('d');
        thetaL -= 0.5;
        A.set_angleL(thetaL);
        A = straight(A);
        
     }
     return A;
  }
  while(len < straight_len(A)){
        thetaL = A.get_angleL('d');
        thetaL += 0.5;
        A.set_angleL(thetaL);
        A = straight(A);
     }
  return A;
}

//Fix straight_len
Angle Arm::fix_straight(double len, Angle A){
  double thetaL = A.get_angleL('r');
  double req_len = len - L_len*cos(thetaL);
  double thetaR = acos(req_len/R_len);
  A.set_angleR(thetaR/PI*180.0);
  return A;
}

void Arm::rotate(double degree){
  if(abs(degree) < 5.0){
    rotate_step(degree);
    return;
  }
  double degree_step = degree/abs(degree);
  rotate_step(degree_step);
  degree -= degree_step;
  rotate(degree);
}

void Arm::rotate_step(double degree){
  double d = temp_config.get_angleRotate();
  d += degree;
  Angle A = temp_config;
  A.set_angleRotate(d);
  moveTo(A);
}

void Arm::printConfig(){
  Serial.print(temp_config.get_angleR('d'));
  Serial.print("\t");
  Serial.print(temp_config.get_msR());
  Serial.print("\t");
  Serial.print(temp_config.get_angleL('d'));
  Serial.print("\t");
  Serial.print(temp_config.get_msL());
  Serial.print("\t");
  Serial.print(temp_config.get_msRotate());
  Serial.println();
}
double Arm::toPolar(double x, double y){
  if(x > 0){
    if (y >= 0) return atan(y/x);
    return atan(y/x) + 2*PI;
  }
  else if (x < 0){
    if (y >= 0)return atan(y/x) + PI;
    return atan(y/x) + PI;
  }
  else{
    if (y >=0)return PI/2;
    return PI/2 + PI;
  }
}
//return A
double Arm::CosineRule(double a, double b, double c){
  return acos((b*b + c*c - a*a)/(2*b*c));
}

bool Arm::Restrict(Angle A){
  double thetaL = A.get_angleL();
  double thetaR = A.get_angleR();
  double safe = 90.0 - (thetaL - 90.0 + 15.0);
  if (thetaR > safe) return true;
  if (thetaL < 10) return true;
  if (thetaR < 10) return true;
  return false;
}

