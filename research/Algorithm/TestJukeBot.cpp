// #include <Arduino.h>
// #include <Servo.h>
// #include <NewPing.h>
// #include "Wiring.hpp"
// #include "Leveling.hpp"
// #include "State.hpp"

// /* INITIALIZING MAJOR VARIABLES */
// // distances read from the sensor
// int queued_distance_map[180];
// // corrected distance
// int current_distance_map[180];
// // list of all sequences of points at the same distance
// // each level will just be defined by its distance from the robot
// int level_map[MAX_DISTANCE/2];
// // ULTRASONIC SENSOR OBJECT
// NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// // SERVO OBJECT
// Servo survey;
//
// /*
//   SERVO FUNCTIONS
// */
// void goHere(int degree){
//   int angle = survey.read();
//   if (angle == degree){
//     // angle == MID_ANGLE
//     // dont need to do anything
//   } else if (angle > degree){
//     // angle > MID_ANGLE
//     for(int i = angle; i > degree; i-=5){
//       survey.write(i);
//       delay(100);
//     }
//   } else {
//     // angle < MID_ANGLE
//     for(int i = angle; i < degree; i+=5){
//       survey.write(i);
//       delay(100);
//     }
//   }
// }
//
// /*
//   ULTRASONIC SENSOR FUNCTIONS
// */
// // sampling over a degree
// int ping_sample(int sample_rate, int delay_per_sample=T_STD){
//   int average = 0;
//   for (int i = 0; i < sample_rate; i++) {
//     /* code */
//     delay(delay_per_sample);
//     average += sonar.ping_cm();
//   }
//   average /= sample_rate;
//   // Serial.println(average);
//
//   if (average == 0){
//     // beyond the scope of the sensor
//     // set at the maximum distance
//     average = MAX_DISTANCE;
//   }
//   return average;
// }
//
// // ping samples over an angle range
// void sweep(int range_min, int range_max, int sample_rate, int sample_resolution, int delay_per_sample=T_STD, int degree_delay=T_STDx2){;
//   // for a range of values, sample the information
//   for (int i = range_min; i < range_max; i+=sample_resolution){
//     survey.write(i);
//     queued_distance_map[i] = ping_sample(sample_rate, delay_per_sample);
//     delay(degree_delay);
//   }
// }
//
// /*
//   MAIN ARDUINO FUNCTIONS
// */
// void setup(){
//   // connect serial
//   Serial.begin(115200);
//   Serial.println("Hello");
//   // connect servos
//   // survey.attach(SURVEY_PIN);
//   //connect buzzer
//   // goHere(MID_ANGLE);
// }
//
// void loop(){
//   // loop through all the angles of the servo the surveyal
//   // sweep(MIN_ANGLE, MAX_ANGLE, SAMPLEx5, FIVE_DEGREE_RESOLUTION, T_STD, T_STDx2);
//   delay(5000);
// }
