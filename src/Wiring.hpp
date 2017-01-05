/*
  FILE:         Wiring.hpp

  DESCRIPTION:  A file with constants specifically for electrical Wiring
                as well as their meaning
*/

// Ultrasonic sensor information
#define TRIGGER_PIN   12  // Arduino Pin tied to TRIGGER_PIN
#define ECHO_PIN      11  // Arduino pin tied to echo
#define MAX_DISTANCE  400 // Max distance to ping in cm

// Servo wiring constants
#define SCANNER_PIN    9    // the pin that controls the survo for surveyal
#define MONO_SPEAKER_PIN 5  // pin to control servo for single speaker setup
#define LEFT_SPEAKER_PIN 5  // the pin that controls the servo for the left speaker
#define RIGHT_SPEAKER_PIN 6 // the pin that controls the servo for the right speaker
