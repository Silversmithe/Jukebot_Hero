/*
  FILE:         Sampling.hpp

  DESCRIPTION:  A file with constants specifically for Ultrasonic sampling
                as well as their meaning

*/

// sampling rate Information
/*
  For each degree where a distance is taken, these constants specify how many
  times a distance is taken and averages at a given degree. For example, a scan
  with MONO_SAMPLE only takes one distance and averages it by one. A HQ_SAMPLE
  takes 30 samples of the distance and finds the average.
*/
#define SAMPLE      1  // Only sample one
#define SAMPLEx2    2  // Only sample two
#define SAMPLEx5    5  // Number of samples per degree balance speed/ quality
#define SAMPLEx10   10 // double the "best" sample
#define SAMPLEx30   30 // Number of samples for central limit theorem (focus entirely on quality)

// Variance per sample
#define V_SAMPLE 50
#define V_SAMPLEx2 16
#define V_SAMPLEx5 4
#define V_SAMPLEx10 4     // originally 2
#define V_SAMPLEx30 2   // originally 2

// delay per sample OR delay per degree where sampling is done
// the amound of time between each ultrasonic measurement
// this can affect quality
#define T_STD     50    // 50ms
#define T_STDx2   100   // 100ms
#define T_STDx3   150   // 150ms
#define T_STDx4   200   // 200ms
#define T_STDx10  500   // 500ms (1/2 second)
#define T_STDxS   1000  // 1000ms (1 second)

// sampling angle Information (resolution)
/*
  Based on simple geometry, X degrees of resolution results in rougly a datapoint
  per 7(x) centimeters.

  Since the radius of our half circle is about 400 centimeters, the arc length
  between two points of ONE_DEGREE_RESOLUTION is 7*(1) or roughly 7 centimeters

  As a result,the more X is increased, the less "resolution" a scan will have.
*/
// ~7 cm between each sample point
#define ONE_DEGREE_RESOLUTION     1   // one sample per degree
// ~14 cm between each sample point
#define TWO_DEGREE_RESOLUTION     2   // one sample per 2 degrees
// ~21 cm between each sample point
#define THREE_DEGREE_RESOLUTION   3   // one sample per 3 degrees
// ~28 cm between each sample point
#define FOUR_DEGREE_RESOLUTION    4   // one sample per 4 degrees
// ~35 cm between each sample point
#define FIVE_DEGREE_RESOLUTION    5   // one sample per 5 degrees
// ~70 cm between each sample point
#define TEN_DEGREE_RESOLUTION     10  // one sample per 10 degrees

// angle symbolic constants
// specifying servo constraints
#define MIN_ANGLE     10  // smallest angle allowed for Servo
#define MID_ANGLE     90  // middle angle
#define MAX_ANGLE     170 // largest angle allowed for servo

// VARIABLES FOR LEVELING
const unsigned int RANGE = (MAX_ANGLE - MIN_ANGLE);  // should be 160
const unsigned int RANGEx2 = RANGE/2;
const unsigned int RANGEx5 = RANGE/5; // should be 32
const unsigned int RANGEx10 = RANGE/10;
