/*
  Motor.h - Library for working with the Cytron SPG30E-30K.
  Created by Vinay Lanka, January 27, 2021.
*/
#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
  public:
    //Constructor - dir and pwm are the Motor output / en_a and en_b are the encoder inputs
    Motor(int dir, int pwm, int en_a, int en_b);
    //Spin the motor with a percentage value
    void rotate(int value);
    //Motor Outputs - dir is one direction and pwm is the other
    int dir;
    int pwm;
    //Encoder Inputs
    int en_a;
    int en_b;
};

#endif
