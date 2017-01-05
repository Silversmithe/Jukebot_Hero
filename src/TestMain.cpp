// #include <Arduino.h>
// #include <Servo.h>
// #include <NewPing.h>
// #include "Profile.hpp"
// #include "Wiring.hpp"
// #include "Leveling.hpp"
// #include "State.hpp"
//
// /* GLOBALS*/
// int counter;
// Level levels[180];
//
// void setup(){
//   Serial.begin(9600);
//   counter = 0;
// }
//
// void loop(){
//   Serial.println(counter++);
//   if (( counter >= 0 )&&(counter<=179)){
//     levels[counter] = Level(100, 200, 59, false);
//     levels[counter].print();
//   }
//   delay(500);
// }
