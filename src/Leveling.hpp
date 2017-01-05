/*
    FILE:           Leveling.hpp
    DESCRIPTION:    Finding the average of a level of a graph while finding out
                    if a point should be incorporated into that level

    What is a level?
        - a level is a group of adjacent equidistant points from the origin
        - a level represents an object or the lack of an object in front of a SENSOR
*/
#include "Level.hpp"
const int GRAPH_RANGE = RANGEx5;

/* FUNCTIONS THAT EVALUATE THE LEVELS AND GRAPHS */
int findDeepestLevel(Level levels[GRAPH_RANGE], int sample_variance){
  /*
    @param: (Level*) levels
    @description: given the current graph levels, which define the mapping, output
                  the position that the speaker should be at to aim it's audio
                  at the deepest part of the environment as to fill up the most
                  space in the room.
  */
  int largest_index = 0;

  for(int i = 1; i < GRAPH_RANGE; i++){
    // iterate through each level
    // check the average
    int ave_diff = levels[largest_index].Average() - levels[i].Average();

    // checking if they are close enough to be very similar
    if((ave_diff >= -1*sample_variance) && (ave_diff <= sample_variance)){
      // if the averages are within the sample variance, the larger range wins
      int range = levels[largest_index].UpperLimit() - levels[largest_index].LowerLimit();
      int rangeI = levels[i].UpperLimit() - levels[i].LowerLimit();

      if(rangeI > range){
        // if the averages are roughly the same
        // and the range size of i is greater than the current largest level
        // i becomes the new largest index
        largest_index = i;
      }

    } else if (levels[i].Average() > levels[largest_index].Average()){
      // if the average is larger then it is the larger index
      largest_index = i;
    }
  }

  // take the largest index and get the middle value
  return (levels[largest_index].UpperLimit() + levels[largest_index].LowerLimit())/2;
}

int findMostActiveLevel(Level queued[GRAPH_RANGE], Level current[GRAPH_RANGE], int sample_variance){
  return 0;
}

/* FUNCTIONS THAT LEVEL THE GRAPH */
/*
  group levels together and output them in a list
  Assuming that these arrays are passed by reference
*/
// void mapGraphLevels(int graph[GRAPH_RANGE], Level levels[GRAPH_RANGE], int sample_variance){
//   /*
//     @param: (int*) graph : a list of distances based on the angle
//                             of the robot
//     @param: (int) sample_variance : the variance based on the sampling type (resolution)
//                                     chosen
//     @description: given a set of points measured from the robot
//                   group the points that are meant to be close to each other
//                   and average them to "smooth" out the graph and ease object analysis
//                   and distance comparison of the graph.
//     @return: (Level*) : list of the levels
//   */
//   // global function variable declaration
//   int level_index = 0;
//   int current_level_low, current_level_high;
//   int current_level_average, current_value;
//   int difference;
//   // global function variable initialization
//   // set the current point to the first point in the graph
//   current_level_low = current_level_high = 0;
//   current_level_average = current_value = graph[0];
//
//   /*
//     Iterate through each point on the graph and determine its level
//     positioning
//   */
//
//   for (int i = 1; i < GRAPH_RANGE; i++) {
//     current_value = graph[i];   // current value is the current index
//     difference = current_value - current_level_average;
//     if( (difference >= (-1*sample_variance)) && (difference <= (sample_variance)) ){
//       // the current value is within the average +- variance
//       // set the new level average by averaging the average and the current value
//       current_level_average = (current_value + current_level_average)/2;
//     }else{
//       // current value is not withing the avg +- variance
//       // *** look ahead & look behind method ***
//       // check ahead and behind if these points are within the average variance
//       //   if these points are withing the average variance, then the current point must be too
//       if ((i > 0) && (i < GRAPH_RANGE-1)){
//         // can only look ahead if there is space in front of you and space behind you to do so
//         // if the one in front and behind the current point are close to each other AND close to average
//         //    the current point most likely is as well
//         int back = graph[i-1];
//         int ahead = graph[i+1];
//         difference = ahead - back;
//         // compare
//         if(!((difference >= (-1*sample_variance)) && (difference <= (sample_variance)))){
//           // REDEMPTION ATTEMPT FAILED
//           // set the high index of the level
//           current_level_high = i-1;
//           // store the level in the levels array
//           levels[level_index++] = Level(current_level_low, current_level_high, current_level_average);
//           // set the current low level for the next level range
//           current_level_low = current_level_high = i;
//           current_level_average = graph[i];
//         }
//       }else{
//         // MUST BE AT ONE OF THE ENDPOINTS
//         // NOT POSSIBLE TO LOOK AHEAD
//         current_level_high = i-1;
//         // store the level in the levels array
//         levels[level_index++] = Level(current_level_low, current_level_high, current_level_average);
//         // set the current low level for the next level range
//         current_level_low = current_level_high = i;
//         current_level_average = graph[i];
//       }//END IF
//     }//END IF
//   }// END FOR
//
//   // once everything is done, the highest limit is the last index
//   current_level_high = (GRAPH_RANGE-1);
//   levels[level_index] = Level(current_level_low, current_level_high, current_level_average);
// }
//
// void smoothLevels(Level (&graph)[GRAPH_RANGE], unsigned int sample_variance){
//   /*
//     @param: (Level*) graph: list of levels mapped
//     @param: (int) sample_variance: the variance based on the sampling type (resolution)
//                                     chosen
//     @description: Given the levels and variance, smooth out and organize the levels
//                   now that we have all the levels in order, use the level averages to find out what levels seem off
//                   Use the level averages and the level ranges, clean out the graph even further
//                   only dealing with the levels with a level above and below, NOT a limit point
//     @return: (Level*)
//   */
//
//     // Level removeList[GRAPH_RANGE];
//     // int removeIndex = 0;
//     //
//     // // add first level to result
//     //
//     // for(int i = 1; i < (GRAPH_RANGE-1); i++){
//     //   // lets look only at the levels that are one points
//     //   // pull out the current level
//     //   Level level = graph[i];
//     //   Level level_behind = graph[i-1];
//     //   Level level_ahead = graph[i+1];
//     //   unsigned int difference = (level_behind.Average() - level_ahead.Average());
//     //   // make sure each level is actually a valid level
//     //   if ((level.UpperLimit() - level.LowerLimit()) == 0){
//     //     // if the difference of the high limit & low limits are zero
//     //     // the high and low limit are the same
//     //     if(level_behind.UpperLimit() < level_ahead.UpperLimit()){
//     //       // if one limit is above the other, its probably between the two
//     //       level.setAverage((level_ahead.Average()+level_behind.Average())/2);
//     //     } else if (level_behind.UpperLimit() > level_ahead.UpperLimit()){
//     //       // if one limit is above the other, its probably between the two
//     //       level.setAverage((level_ahead.Average()+level_behind.Average())/2);
//     //     } else if (level_behind.UpperLimit() == level_ahead.UpperLimit()){
//     //       // if these two are equal, then they are equal
//     //       level.setAverage(level_behind.Average());
//     //     }
//     //   }else if ((difference >= (-1*sample_variance)) && ((difference <= sample_variance))){
//     //     // so if the level in front and the level behind have similar averages
//     //     // and if the variance you are measuring is very small
//     //     if (level.UpperLimit() - level.LowerLimit() < 10){
//     //       int ave = (level_ahead.Average() + level_behind.Average())/2;
//     //       level.setAverage(ave);
//     //       level.setLowerLimit(level_behind.LowerLimit());
//     //       level.setUpperLimit(level_ahead.UpperLimit());
//     //       removeList[removeIndex++] = level_behind;
//     //       removeList[removeIndex++] = level_ahead;
//     //     }
//     //   }
//     // }
//     // // to remove levels, just set them as invalid
//     // // go through the list of levels and dont add the resulting levels that have been
//     // // added to the remove list
//     // for (int i = 0; i < GRAPH_RANGE; i++) {
//     //   for (int r = 0; r < removeIndex; i++) {
//     //     // check if the level is the same as the one to remove
//     //     bool ave = (graph[i].Average() == removeList[r].Average());
//     //     bool low = (graph[i].LowerLimit() == removeList[r].LowerLimit());
//     //     bool high = (graph[i].UpperLimit() == removeList[r].UpperLimit());
//     //     if(!(ave && low && high)){
//     //       graph[i].setValidLevel(false);
//     //     }
//     //   }
//     // }
// }
