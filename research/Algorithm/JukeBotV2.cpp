#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include "Profile.hpp"
#include "Wiring.hpp"
#include "Leveling.hpp"
#include "State.hpp"

/* GLOBAL VARIABLES */
int AGENT_STATE;
// Graph measurements
int queuedGraph[RANGE];   // the most recent scan of environment
// Graph Levels
Level* currentLevelGraph; // the objectification of the historical environment
Level* queuedLevelGraph;  // the objectification of the most current environment
// scanning queue (later, for now just scan the whole room)
// PHYSICAL
Servo scanner, left_speaker, right_speaker;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

/* SERVO MOTOR FUNCTIONS */
void goHere(int degree, int angle_rate, Servo servo){
  /*
    @param: (int) degree
    @param: (int) angle_rate
    @param: (Servo) servo
  */
  int angle = servo.read();
  if (angle == degree){
    // angle == MID_ANGLE
    // dont need to do anything
  } else if (angle > degree){
    // angle > MID_ANGLE
    for(int i = angle; i > degree; i-=angle_rate){
      servo.write(i);
      delay(100);
    }
  } else {
    // angle < MID_ANGLE
    for(int i = angle; i < degree; i+=angle_rate){
      servo.write(i);
      delay(100);
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
  for (int i = range_min; i < range_max; i+=sample_resolution){
    scanner.write(i);
    queuedGraph[i] = ping_sample(sample_rate, delay_per_sample);
    Serial.println(queuedGraph[i]);
    delay(degree_delay);
  }
  // int* result = range;
  // return result;
}

// MAIN FUNCTIONS
/*
  Initializes all data structures and objects for percepts, actions, and behavior
  to perform as expected
*/
void setup(){
  // INITIALIZE EVERYTHING
  // connect serial
  Serial.begin(115200);
  // attach servo
  scanner.attach(SCANNER_PIN);

  /* Initialize variables */
  // after done with it make sure to remove it

  goHere(MID_ANGLE, 5, scanner);
  AGENT_STATE = STARTUP;  // state of the agent
}

/*
  BEHAVIOR FUNCTION

  Describes the behavior of the agent
*/
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
      // currentLevelGraph = {new Level(MIN_ANGLE, MAX_ANGLE, MAX_DISTANCE)};
      // change state to scanning
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
      delete [] currentLevelGraph;
      delete [] queuedLevelGraph;
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
        delete [] currentLevelGraph;
        delete [] queuedLevelGraph;

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
      delay(1000);
      // scan 180 degrees
      sweep(MIN_ANGLE, MAX_ANGLE, SAMPLEx10, ONE_DEGREE_RESOLUTION);
      // proof of scanning
      // Serial.println("show queue");
      // for(int i = 0; i < 160; i++){
      //   Serial.println(queuedGraph[i]);
      // }
      // // process scan
      // Serial.println("processing...");
      // queuedLevelGraph = mapGraphLevels(queuedGraph, V_SAMPLEx10);
      // Serial.println("past 1st part");
      // queuedLevelGraph = smoothLevels(queuedLevelGraph, V_SAMPLEx10);
      // Serial.println("finished");
      //
      // // shift queue to current
      // delete [] currentLevelGraph;
      // currentLevelGraph = queuedLevelGraph;
      // delete [] queuedGraph;

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
      // create adjustment order in the queue
      // command queue
      // create command queue
      // Serial.println("reading smoothed level graph");
      // for(int i = 0; i < 160; i++){
      //   Level level = currentLevelGraph[i];
      //   if(level.isActiveObject()){
      //     Serial.print("(");
      //     Serial.print(level.LowerLimit());
      //     Serial.print(", ");
      //     Serial.print(level.UpperLimit());
      //     Serial.print(", ");
      //     Serial.print(level.Average());
      //     Serial.println(")");
      //   }
      // }

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
      // AGENT_STATE = SCANNING;
      AGENT_STATE = SHUTDOWN;
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
  // universal delay
  delay(1000);
}
