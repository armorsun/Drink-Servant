#include <math.h>
#include "Angle.h"

//R------------------
void Angle::set_angleR(double t){
  thetaR = t;
  msR = R0 + (int)(thetaR * double(R90 - R0)/90.0);
}
void Angle::set_msR(int ms){
  msR = ms;
  thetaR = double(msR - R0)*90.0/double(R90 - R0);
}
double Angle::get_angleR(char type = 'd'){
  if (type == 'r')  return thetaR/180.0*PI;
  return thetaR;
}
int Angle::get_msR(){
  return msR;
}

//L-------------------
void Angle::set_angleL(double t){
  thetaL = t;
  msL = L0 + (int)(thetaL * double(L90 - L0)/90.0);
}
void Angle::set_msL(int ms){
  msL = ms;
  thetaL = double(msL - L0)*90.0/double(L90 - L0);
}
double Angle::get_angleL(char type = 'd'){
  if (type == 'r') return thetaL/180.0*PI;
  return thetaL;
}
int Angle::get_msL(){
  return msL;
}

//Rotate
void Angle::set_angleRotate(double t){
  thetaRotate = t;
  msRotate = _0 + (int)(thetaRotate * double(_180 - _90)/90.0);
}
void Angle::set_msRotate(int ms){
  msRotate = ms;
  thetaRotate = double(msRotate - _0)*90.0/double(_180 - _90);
}

double Angle::get_angleRotate(char type = 'd'){
  if (type == 'r') return thetaRotate/180.0*PI;
  return thetaRotate;
}
int Angle::get_msRotate(){
  return msRotate;
}

