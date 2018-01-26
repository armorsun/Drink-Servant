#include <Arduino.h>
#include "Angle.h"
#include <Servo.h>

#define PI 3.141592
#define R0 1250
#define R90 1920
#define L0 1450
#define L90 1950
#define L_len 21.0
#define R_len 24.0
#define _0 1590
#define _90 1180
#define restrict 20

class Arm{
  public:    
    Arm(int msL,int msR, int msRotate, Servo L, Servo R, Servo M);   
    void toX(double len);
    void toY(double len);
    void toZ(double len);   
    void rotate(double degree);
    void goHome();
    void printConfig();
    
   private:
    //home
    Angle default_config;
    
    //turning point config
    Angle turning_config;
    
    //temporary config
    Angle temp_config;

    Servo sL;
    Servo sR;
    Servo s_;

  //  Arm();
    void servoAttach(int l, int r, int m);  
    void changeTurning();
    Angle straight(Angle A);
    void moveStraight(double stepLen);
    void moveTo(Angle A);
    void moveX(double step_len);
    void moveY(double step_len);
    void moveZ(double step_len);
    double straight_len(Angle A);
    double height(Angle A);
    Angle fix_straight(double len, Angle A);
    Angle converge(double len, Angle A);
    double toPolar(double x, double y);   //return rad
    double CosineRule(double a, double b, double c);
    bool Restrict(Angle A);
    void rotate_step(double degree);
};



