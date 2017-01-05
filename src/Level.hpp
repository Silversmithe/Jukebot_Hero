/*
  FILE:         Level.hpp

  DESCRIPTION:  Container to define the a level object

  NOTES:        Possibly might want to make an object that
                can store all the levels and search through them
                in a specific fashion
                - just encapsulating it into a structure
*/
#include "Arduino.h"
#include "Sampling.hpp"

// defines a range of distance points that lie (with variance) around a common average distance
class Level{
public:
  Level(){
    lower_limit = 0;
    upper_limit = 0;
    average = 0;
    active_object = false;
    is_level = false;
  }

  Level(unsigned int low, unsigned int high, unsigned int avg, bool active=false){
    lower_limit = low;
    upper_limit = high;
    average = avg;
    active_object = active;
    is_level = true;
  }

  ~Level(){}

  // Operator overload
  void operator=(const Level& select){
    is_level = select.is_level;
    lower_limit = select.lower_limit;
    upper_limit = select.upper_limit;
    average = select.average;
    active_object = select.active_object;
  }

  bool operator==(const Level& rhs){
    bool low = (lower_limit == rhs.lower_limit);
    bool up = (upper_limit == rhs.upper_limit);
    bool ave = (average == rhs.average);
    return low && up && ave;
  }

  // Acessors
  unsigned int LowerLimit(){ return lower_limit; }
  unsigned int UpperLimit(){ return upper_limit; }
  unsigned int Average(){ return average; }
  bool isActiveObject(){ return active_object; }
  bool isLevel(){ return is_level; }

  void print(){
    Serial.print("(");
    Serial.print(lower_limit);
    Serial.print(", ");
    Serial.print(upper_limit);
    Serial.print(", ");
    Serial.print(average);
    Serial.println(")");
  }

  // Mutators
  void setLowerLimit(unsigned int value){ lower_limit = value; }
  void setUpperLimit(unsigned int value){ upper_limit = value; }
  void setAverage(unsigned int value){ average = value; }
  void setActiveObject(bool value){ active_object = value; }
  void setValidLevel(bool value){ is_level = value; }

private:
  bool is_level;
  unsigned int lower_limit;
  unsigned int upper_limit;
  unsigned int average;
  bool active_object;
};
