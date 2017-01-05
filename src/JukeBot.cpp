#include <Arduino.h>
#include <NewPing.h>
#include "ServoMotion.hpp"
#include "Profile.hpp"
#include "Wiring.hpp"
#include "Leveling.hpp"
// #include "Level.hpp"
#include "State.hpp"

/* GLOBAL VARIABLES */
int AGENT_STATE;
const int SCAN_RANGE = RANGEx5; // change rangex5 to whatever range you want
const int DEGREE_RESOLUTION = FIVE_DEGREE_RESOLUTION;
const unsigned int SAMPLE_Q = SAMPLEx10;
const unsigned int VARIANCE = V_SAMPLEx10;
// Graph measurements
int queuedGraph[SCAN_RANGE];   // the most recent scan of environment
// Graph Levels
Level queuedLevels[SCAN_RANGE];
Level currentLevels[SCAN_RANGE];
// agent position
int AGENT_POSITION;

// scanning queue (later, for now just scan the whole room)
// PHYSICAL
Servo scanner, mono_speaker;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

/* Analysis functions */
/* check to see if within the variance */
bool isInVarRange(int currentPos, int queuedPos, int variance){
  int diff = currentPos - queuedPos;
  if((diff >= -1*variance) && (diff <= variance)){
    return true;
  }
  return false;
}
/* FUNCTIONS THAT LEVEL THE GRAPH */
/*
  group levels together and output them in a list
  Assuming that these arrays are passed by reference
*/
void mapGraphLevels(int graph[SCAN_RANGE], int sample_variance){
  /*
    @param: (int*) graph : a list of distances based on the angle
                            of the robot
    @param: (int) sample_variance : the variance based on the sampling type (resolution)
                                    chosen
    @description: given a set of points measured from the robot
                  group the points that are meant to be close to each other
                  and average them to "smooth" out the graph and ease object analysis
                  and distance comparison of the graph.
    @return: (Level*) : list of the levels
  */
  // global function variable declaration
  int level_index = 0;
  int current_level_low, current_level_high;
  int current_level_average, current_value;
  int difference;
  // global function variable initialization
  // set the current point to the first point in the graph
  current_level_low = current_level_high = 0;
  current_level_average = current_value = graph[0];

  /*
    Iterate through each point on the graph and determine its level
    positioning
  */

  for (int i = 1; i < SCAN_RANGE; i++) {
    current_value = graph[i];   // current value is the current index
    difference = current_value - current_level_average;
    if( (difference >= (-1*sample_variance)) && (difference <= (sample_variance)) ){
      // the current value is within the average +- variance
      // set the new level average by averaging the average and the current value
      current_level_average = (current_value + current_level_average)/2;
    }else{
      // current value is not withing the avg +- variance
      // *** look ahead & look behind method ***
      // check ahead and behind if these points are within the average variance
      //   if these points are withing the average variance, then the current point must be too
      if ((i > 0) && (i < SCAN_RANGE-1)){
        // can only look ahead if there is space in front of you and space behind you to do so
        // if the one in front and behind the current point are close to each other AND close to average
        //    the current point most likely is as well
        int back = graph[i-1];
        int ahead = graph[i+1];
        difference = ahead - back;
        // compare
        if(!((difference >= (-1*sample_variance)) && (difference <= (sample_variance)))){
          // REDEMPTION ATTEMPT FAILED
          // set the high index of the level
          current_level_high = i-1;
          // store the level in the currentLevels array
          queuedLevels[level_index++] = Level(current_level_low, current_level_high, current_level_average);
          // set the current low level for the next level range
          current_level_low = current_level_high = i;
          current_level_average = graph[i];
        }
      }else{
        // MUST BE AT ONE OF THE ENDPOINTS
        // NOT POSSIBLE TO LOOK AHEAD
        current_level_high = i-1;
        // store the level in the currentLevels array
        queuedLevels[level_index++] = Level(current_level_low, current_level_high, current_level_average);
        // set the current low level for the next level range
        current_level_low = current_level_high = i;
        current_level_average = graph[i];
      }//END IF
    }//END IF
  }// END FOR

  // once everything is done, the highest limit is the last index
  current_level_high = (SCAN_RANGE-1);
  queuedLevels[level_index] = Level(current_level_low, current_level_high, current_level_average);
}

void smoothLevels(Level (&graph)[SCAN_RANGE], unsigned int sample_variance){
  /*
    @param: (Level*) graph: list of levels mapped
    @param: (int) sample_variance: the variance based on the sampling type (resolution)
                                    chosen
    @description: Given the levels and variance, smooth out and organize the levels
                  now that we have all the levels in order, use the level averages to find out what levels seem off
                  Use the level averages and the level ranges, clean out the graph even further
                  only dealing with the levels with a level above and below, NOT a limit point
    @return: (Level*)
  */

    Level removeList[SCAN_RANGE];
    int removeIndex = 0;

    // add first level to result

    for(int i = 1; i < (SCAN_RANGE-1); i++){
      // lets look only at the levels that are one points
      // pull out the current level
      Level level = graph[i];
      Level level_behind = graph[i-1];
      Level level_ahead = graph[i+1];
      unsigned int difference = (level_behind.Average() - level_ahead.Average());
      // make sure each level is actually a valid level
      if ((level.UpperLimit() - level.LowerLimit()) == 0){
        // if the difference of the high limit & low limits are zero
        // the high and low limit are the same
        if(level_behind.UpperLimit() < level_ahead.UpperLimit()){
          // if one limit is above the other, its probably between the two
          level.setAverage((level_ahead.Average()+level_behind.Average())/2);
        } else if (level_behind.UpperLimit() > level_ahead.UpperLimit()){
          // if one limit is above the other, its probably between the two
          level.setAverage((level_ahead.Average()+level_behind.Average())/2);
        } else if (level_behind.UpperLimit() == level_ahead.UpperLimit()){
          // if these two are equal, then they are equal
          level.setAverage(level_behind.Average());
        }
      }else if ((difference >= (-1*sample_variance)) && ((difference <= sample_variance))){
        // so if the level in front and the level behind have similar averages
        // and if the variance you are measuring is very small
        if (level.UpperLimit() - level.LowerLimit() < 10){
          int ave = (level_ahead.Average() + level_behind.Average())/2;
          level.setAverage(ave);
          level.setLowerLimit(level_behind.LowerLimit());
          level.setUpperLimit(level_ahead.UpperLimit());
          removeList[removeIndex++] = level_behind;
          removeList[removeIndex++] = level_ahead;
        }
      }
    }
    // to remove levels, just set them as invalid
    // go through the list of levels and dont add the resulting levels that have been
    // added to the remove list
    for (int i = 0; i < SCAN_RANGE; i++) {
      for (int r = 0; r < removeIndex; i++) {
        // check if the level is the same as the one to remove
        bool ave = (graph[i].Average() == removeList[r].Average());
        bool low = (graph[i].LowerLimit() == removeList[r].LowerLimit());
        bool high = (graph[i].UpperLimit() == removeList[r].UpperLimit());
        if(!(ave && low && high)){
          graph[i].setValidLevel(false);
        }
      }
    }
}

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

/* SERVO & ULTRASONIC COMBO */
void sweep(int range_min, int range_max, int sample_rate, int sample_resolution, int delay_per_sample=T_STD, int degree_delay=T_STDx2){
  /*
    @param: (int) range_min
    @param: (int) range_max
    @param: (int) sample_rate
    @param: (int) sample_resolution
    @param: (int) delay_per_sample
    @param: (int) degree_delay

    @description: given a range, measure the distance between the ultrasonic sensor
                  and the closest object in that range
  */
  // for a range of values, sample the information
  // int range[RANGE];
  if(!(range_min < range_max)){ return; }
  int index = 0;

  for (int i = range_min; i < range_max; i+=sample_resolution){
    scanner.write(i);
    queuedGraph[index++] = ping_sample(sample_rate, delay_per_sample);
    // Serial.println(queuedGraph[index++]);
    delay(degree_delay);
  }
  // int* result = range;
  // return result;
  // Serial.println("done");
}

/* BEHAVIOR FUNCTION & MAIN LOOP ARDUINO */
void setup(){
  // INITIALIZE EVERYTHING
  // connect serial
  Serial.begin(115200);

  // initialize hardware
  scanner.attach(SCANNER_PIN);
  mono_speaker.attach(MONO_SPEAKER_PIN);

  /* Initialize variables */
  // after done with it make sure to remove it
  AGENT_STATE = STARTUP;
}

void loop(){
  // loop through all the angles of the servo the surveyal
  switch (AGENT_STATE) {
    case STARTUP:
      /*
        @state: STARTUP
        @description: responsible for initializing and preparing variables for
                      the JukeBot system before starting the scanning process
      */
      Serial.println("state: startup");
      delay(1000);
      // fill the historical level graph
      // change state to scanning
      AGENT_POSITION = MID_ANGLE;

      AGENT_STATE = SCANNING;
      break;
    case SHUTDOWN:
      /*
        @state: SHUTDOWN
        @description: responsible for killing and de-initializing everything that
                      must be closed if the system enters an error state or is
                      forcefully shut off with warning
      */
      Serial.println("state: shutdown");
      delay(1000);
      // switch to startup
      break;
    case RESET:
        /*
          @state: RESET
          @description: responsible for killing and de-initializing everything that
                        must be closed if the system enters an error state or is
                        forcefully shut off with warning
        */
        Serial.println("state: reset");
        delay(1000);
        // switch to startup
        AGENT_STATE = STARTUP;
        break;
    case SCANNING:
      /*
        @state: SCANNING
        @description: responsible for deciding what in the environment to scan
                      and scanning it
      */
      Serial.println("state: scanning");
      delay(5000);
      // scan 180 degrees
      sweep(MIN_ANGLE, MAX_ANGLE, SAMPLE_Q, DEGREE_RESOLUTION);
      // just convert to levels and set as current level

      // !!! Be careful with these functions
      // DEBUG THE HELL OUT OF THESE
      // take int graph and convert to level graph
      mapGraphLevels(queuedGraph, V_SAMPLEx30);
      // for(int i = 0; i < SCAN_RANGE; i++){
      //   if(currentLevels[i].isLevel()){
      //     currentLevels[i].print();
      //   }
      // }
      // Serial.println("done!");
      // smoothLevels(currentLevels, V_SAMPLEx30);
      // !!!

      // change state to evaluate scan
      AGENT_STATE = EVALUATE;
      break;
    case EVALUATE:
      /*
        @state: EVALUATE
        @description: responsible for analyzing the new graph and the historical graph
                      to check for differences
      */
      Serial.println("state: evaluate");
      delay(1000);
      /*
        compare queued Levels and current Levels to see what is different/active
      */

      // create adjustment order in the queue
      // command queue
      // create command queue
      AGENT_POSITION = findDeepestLevel(currentLevels, VARIANCE);
      int pos;
      pos = findDeepestLevel(currentLevels, VARIANCE);
      pos *= DEGREE_RESOLUTION;
      pos = MAX_ANGLE - pos;
      // Serial.println(pos);
      // Serial.println(AGENT_POSITION);

      // AGENT_POSITION = pos;
      if (!isInVarRange(AGENT_POSITION, pos, VARIANCE)){
        AGENT_POSITION = pos;
      }

      // change state to adjust servos for evaluation
      AGENT_STATE = ADJUST;
      break;
    case ADJUST:
      /*
        @state: ADJUST
        @description: responsible for using the evaluation to adjust the positioning of the
                      speakers rationally
      */
      Serial.println("state: adjust");
      delay(1000);
      // change state to finding out what to scan and to scan it
      goHere(mono_speaker, AGENT_POSITION, 5, 100);
      AGENT_STATE = SCANNING;
      // AGENT_STATE = SHUTDOWN;
      break;
    case IDLE:
      /*
        @state: IDLE
        @description: responsible for power saving mode when there is not significant
                      change in the environment
      */
      Serial.println("state: idle");
      delay(1000);
      // once ready to not be idle, switch to scanning
      break;
    case ERROR:
      /*
        @state: ERROR
        @description: The system enters this state if there is any fault detected with the system.
                      The system then promptly enters the shutdown phase and then the startup phase,
                      creating a restart sequence.
      */
      Serial.println("state: error");
      delay(1000);
      // restart the system
      AGENT_STATE = RESET;
      break;
  }
  delay(1000);
}
