#include "Arduino.h"
#include "Motor.h"


Motor::Motor(int dir, int pwm, int en_a, int en_b) {
  pinMode(dir,OUTPUT);
  pinMode(pwm,OUTPUT);
  pinMode(en_a,INPUT_PULLUP);
  pinMode(en_b,INPUT_PULLUP);
  Motor::dir = dir;
  Motor::pwm = pwm;
  Motor::en_a = en_a;
  Motor::en_b = en_b;
}

void Motor::rotate(int value) {
  if(value>=0){
    //Max Voltage with 16V battery with 12V required
    //(12/16)*255 ~=190
//    Serial.println("called");
//    Serial.println(dir);
    int out = map(value, 0, 100, 0, 255);
    digitalWrite(dir, HIGH);
    analogWrite(pwm,out);
  }else{
    //Max Voltage with 16V battery with 12V required
    //(12/16)*255 ~=190
    int out = map(value, 0, -100, 0, 255);
    digitalWrite(dir, LOW);
    analogWrite(pwm,out);
}
  // int out = map(fabs(value), 0, 100, 0, 190);
  // digitalWrite(dir, value>0);
  // analogWrite(pwm,out);
  
}
