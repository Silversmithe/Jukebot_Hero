#include "Arduino.h"
#include <Servo.h>

/* SERVO MOTOR FUNCTIONS */
void goHere(Servo servo, int degree, int angle_rate, int write_delay=100){
  /*
    @param: (int) degree
    @param: (int) angle_rate
    @param: (Servo) servo
  */
  // Serial.println("entered!!");
  int angle = servo.read();
  // Serial.println(degree);

  while(true){
    // Serial.println(angle);
    int diff = degree - angle;
    if ( (diff >= (-1*angle_rate)) && (diff <= (angle_rate)) ){ break; }

    // so while you are not close enough
    if(angle > degree){
      servo.write(angle-angle_rate);
    } else if (angle < degree){
      servo.write(angle+angle_rate);
    }
    angle = servo.read();
    delay(write_delay);
  }
}
