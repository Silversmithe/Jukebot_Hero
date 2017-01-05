#include "Arduino.h"
#include <NewPing.h>
#include "Sampling.hpp"

/* ULTRASONIC SENSOR FUNCTIONS */
int ping_sample(int sample_rate, int delay_per_sample=T_STD){
  /*
    @param: (int) sample_rate: the number of times the sonar collects and averages a given angle distance
    @param: (int) delay_per_sample: the delay in miliseconds between each sonar ping

    @description: The command for the ultrasonic sensor to collect a given number of samples of the
                  distance at the given position and average based on those samples
  */
  int average = 0;
  for (int i = 0; i < sample_rate; i++) {
    delay(delay_per_sample);
    average += sonar.ping_cm();
  }
  average /= sample_rate;
  // Serial.println(average);

  if (average == 0){
    // beyond the scope of the sensor
    // set at the maximum distance
    average = MAX_DISTANCE;
  }
  return average;
}
