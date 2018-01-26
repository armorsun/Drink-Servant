#define w1 41
#define w2 43
#define m1 45
#define m2 47
#define b1 49
#define b2 51
#include <Arduino.h>

void pump_init();
void pump_drinks(int d);      //main funcion: 0 water / 1 milk / 2 blk T/ 3 milk T 
void water(int s);
void milk(int s);
void black_T(int s);
