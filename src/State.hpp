/*
  FILE:         State.hpp

  DESCRIPTION:  Defining the states that will control the state machine of the
                robot

  NOTES:

*/
/*
  STATES/ STATE machine

  ON, OFF, SCANNING, EVALUATION, IDLE, STARTUP, SHUTDOWN, (RESET, ERROR)?
  ADJUST (servos)
*/
// SUPERSTATE
// ON and OFF may be unessesarry
#define ON 0
#define OFF 1
#define ERROR 2
// ON SUBSTATES
#define STARTUP 3
#define SHUTDOWN 4
#define SCANNING 5
#define EVALUATE 6
#define ADJUST 7
#define IDLE 8
#define RESET 9
