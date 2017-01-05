#!/usr/bin/python
"""
    FILE:           Leveling.py
    DESCRIPTION:    Finding the average of a level of a graph while finding out
                    if a point should be incorporated into that level

"""
"""
    What is a level?
        - a level is a group of adjacent equidistant points from the origin
        - a level represents an object or the lack of an object in front of a SENSOR
"""
import time
import math
from matplotlib import pyplot as plt

# lets test the idea of leveling a graph
VAR_SAMPLE = 50    # originally 50
VAR_SAMPLEx2 = 16  # originally 16
VAR_SAMPLEx5 = 4   # originally 4
VAR_SAMPLEx10 = 4  # originally 2
VAR_SAMPLEx30 = 1.5  # originally 2

# get graph information
# all samples with the box measurement
samplex30 = [13,13,13,14,14,14,14,23,42,43,43,41,41,41,41,41,41,40,41,41,41,40,40,40,39,40,40,39,39,39,39,40,38,38,39,38,38,39,38,39,38,38,39,38,38,39,38,39,39,38,39,38,38,38,39,38,39,39,38,38,38,38,39,39,38,39,38,38,39,38,39,39,39,39,39,40,39,40,40,39,42,42,41,40,41,40,41,41,41,41,41,41,41,15,13,12,12,12,12,11,12,12,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,10,10,10,10,10,10,10,10,10,10,11,11,11,10,11]
samplex10 = [13,13,14,14,14,14,17,28,37,43,42,42,41,41,42,39,40,41,41,40,41,41,40,37,40,40,40,39,37,38,38,40,39,38,38,38,38,38,38,39,39,39,38,38,38,38,39,39,39,39,38,38,38,38,38,39,39,37,39,38,38,38,38,38,38,38,39,39,39,38,38,38,38,40,37,40,39,39,39,39,42,41,41,41,41,38,37,41,41,38,38,41,41,15,15,12,12,12,12,12,11,11,11,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,10]
samplex5 = [13,13,14,13,14,14,14,15,37,37,43,42,39,42,42,36,41,41,41,41,35,39,40,40,40,40,39,40,35,40,35,38,39,37,39,36,39,34,39,36,39,37,38,39,36,39,36,39,37,39,37,39,36,38,38,38,39,37,39,37,39,36,39,37,38,37,37,37,38,39,37,39,36,39,36,38,37,38,41,41,40,42,40,41,41,37,41,41,41,40,41,35,41,24,13,12,12,12,11,12,12,12,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,11,11,10,11,10,11,11]

pop_samplex30 = [13,13,14,14,14,14,14,14,39,41,42,42,42,41,41,40,31,30,29,28,27,26,27,27,26,27,26,26,26,26,26,26,25,26,26,26,25,26,26,25,25,26,26,26,25,25,25,24,24,23,24,24,23,23,23,23,23,23,22,22,23,23,23,23,22,22,23,23,23,23,22,23,24,24,23,24,24,25,28,37,36,36,36,37,37,34,35,34,35,35,34,35,34,15,12,12,11,11,11,11,11,11,11,11,11,11,11,11,11,10,10,10,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,10,10,11,10,11]
pop_samplex5 = [13,13,14,14,13,14,14,20,31,42,42,36,42,37,42,42,29,32,29,28,27,26,26,27,27,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,24,24,24,23,23,23,24,24,24,23,23,22,22,23,23,23,21,23,23,22,23,23,21,23,23,23,23,24,24,24,25,27,34,37,35,37,34,35,35,33,35,33,35,35,33,35,35,17,12,12,11,12,11,11,11,11,11,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,10,11,11,11,11]

# populated room sample
pop_room_samplex10 = [187,170,172,189,172,127,57,56,60,60,60,59,59,54,57,59,54,57,58,55,54,58,57,55,57,55,57,53,56,57,53,56,53,57,53,56,57,54,57,53,57,58,55,57,57,59,54,57,59,55,57,59,52,57,59,60,86,60,200,175,196,200,206,195,201,202,202,201,199,205,193,201,201,201,195,211,195,206,200,201,195,206,192,209,190,156,209,147,98,100,103,112,106,101,111,108,113,109,102,106,111,103,112,109,102,111,107,109,117,112,123,151,103,102,112,261,131,112,243,243,273,247,249,294,199,259,271,227,154,139,153,142,153,142,152,143,152,144,152,143,139,152,136,152,142,152,143,152,144,151,145,151,144,151,144,151,144,151,144,152]

# just for one level on the graph
def levelGraph(graph, sample_variance):
    """
        Find the different levels of the graph and display them
    """
    level_low = level_high = 0
    level_average = current_value = graph[0]

    for i in range(1, len(graph)):
        current_value = graph[i]

        if ( -1*sample_variance <= (current_value - level_average) <= sample_variance):
            level_average = (current_value + level_average)/2
        else:
            level_high = i;
            break

    if (level_high == level_low):
        level_high = len(graph)-1

    return [level_low, level_high, level_average]

# VERSION 1
# gets all levels in the graph
def mapLevelGraph(graph, sample_variance, debug=False):
    """
        Find the different levels of the graph and display them
        these level indexes are inclusive
    """
    levels = [] # array that contains all of the levels
    current_level_low = current_level_high = 0
    current_level_average = current_value = graph[0]

    for i in range(1, len(graph)):
        current_value = graph[i]
        if debug:
            print(i)
        if ( -1*sample_variance <= (current_value - current_level_average) <= sample_variance):
            if debug:
                print("Matches current level-> value:", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))
            current_level_average = math.ceil((current_value + current_level_average)/2)
        else:
            if debug:
                print("** Outside current level", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))

            """
                I have some thoughts here:
                Just because a point is outside of the average variance does not make it actually outside of the level
                is there not a possibility that this point is an outlier?

                it might be usefull look at the point after the current point (look at the trend of points after the current one)
                to determine wether the current point should be apart of the current level or the next level
            """

            current_level_high = i-1;
            # store levels in levels array
            levels.append([current_level_low, current_level_high, current_level_average])
            current_level_low = current_level_high = i
            current_level_average = graph[i]

    # once its done, the highest limit is the last index
    current_level_high = len(graph)-1
    levels.append([current_level_low, current_level_high, current_level_average])
    return levels

# VERSION 2
# gets all levels in the graph
def mapLevelGraphLookAhead(graph, sample_variance, debug=False):
    """
        Find the different levels of the graph and display them
        these level indexes are inclusive
    """
    levels = [] # array that contains all of the levels
    current_level_low = current_level_high = 0
    current_level_average = current_value = graph[0]

    for i in range(1, len(graph)):
        current_value = graph[i]
        if debug:
            print(i)
        if ( -1*sample_variance <= (current_value - current_level_average) <= sample_variance):
            if debug:
                print("Matches current level-> value:", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))
            current_level_average = math.ceil((current_value + current_level_average)/2)
        # elif (i > 0) and (i < len(graph)-1) and ( -1*sample_variance <= (current_value - graph[i-1]) <= sample_variance):
        #         if debug:
        #             print("Matches current level-> value:", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))
        #         current_level_average = math.ceil((current_value + current_level_average)/2)
        else:
            if debug:
                print("** Outside current level", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))

            """
                I have some thoughts here:
                Just because a point is outside of the average variance does not make it actually outside of the level
                is there not a possibility that this point is an outlier?

                it might be usefull look at the point after the current point (look at the trend of points after the current one)
                to determine wether the current point should be apart of the current level or the next level

                this is the most useful if your degree resolution is high! meaning you take more samples
            """
            # LOOK AHEAD/ LOOK BEHIND
            # check ahead if the next one is within the limit
            if 0 < i < len(graph)-1:
                # if the one behind you and the one in front of you are close to each other
                # you should most likely be with them
                behind_value = graph[i-1]
                ahead_value = graph[i+1]
                if (( -1*sample_variance <= (ahead_value - current_level_average) <= sample_variance)) and (( -1*sample_variance <= (ahead_value - current_level_average) <= sample_variance)):
                     # if not then actually end level
                     # its an outlier (that belongs) and should not be counted towards the average
                     #current_level_average = math.ceil((current_value + current_level_average)/2)
                     # REDEMPTION
                     pass
                else:
                    # STILL NOT REDEEMED
                    current_level_high = i-1;
                    # store levels in levels array
                    levels.append([current_level_low, current_level_high, current_level_average])
                    current_level_low = current_level_high = i
                    current_level_average = graph[i]
            else:
                current_level_high = i-1;
                # store levels in levels array
                levels.append([current_level_low, current_level_high, current_level_average])
                current_level_low = current_level_high = i
                current_level_average = graph[i]

    # once its done, the highest limit is the last index
    current_level_high = len(graph)-1
    levels.append([current_level_low, current_level_high, current_level_average])
    return levels

# VERSION 3
# gets all levels in the graph
def mapGraphLevelEvening(graph, sample_variance, debug=False):
    """
        Find the different levels of the graph and display them
        these level indexes are inclusive
    """
    levels = [] # array that contains all of the levels
    current_level_low = current_level_high = 0
    current_level_average = current_value = graph[0]

    for i in range(1, len(graph)):
        current_value = graph[i]
        if debug:
            print(i)
        if ( -1*sample_variance <= (current_value - current_level_average) <= sample_variance):
            if debug:
                print("Matches current level-> value:", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))
            current_level_average = math.ceil((current_value + current_level_average)/2)
        # elif (i > 0) and (i < len(graph)-1) and ( -1*sample_variance <= (current_value - graph[i-1]) <= sample_variance):
        #         if debug:
        #             print("Matches current level-> value:", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))
        #         current_level_average = math.ceil((current_value + current_level_average)/2)
        else:
            if debug:
                print("** Outside current level", current_value, ", ave: ", current_level_average, ", diff: ", (current_value - current_level_average))

            """
                I have some thoughts here:
                Just because a point is outside of the average variance does not make it actually outside of the level
                is there not a possibility that this point is an outlier?

                it might be usefull look at the point after the current point (look at the trend of points after the current one)
                to determine wether the current point should be apart of the current level or the next level

                this is the most useful if your degree resolution is high! meaning you take more samples
            """
            # LOOK AHEAD/ LOOK BEHIND
            # check ahead if the next one is within the limit
            if 0 < i < len(graph)-1:
                # if the one behind you and the one in front of you are close to each other
                # you should most likely be with them
                behind_value = graph[i-1]
                ahead_value = graph[i+1]
                if (( -1*sample_variance <= (ahead_value - current_level_average) <= sample_variance)) and (( -1*sample_variance <= (ahead_value - current_level_average) <= sample_variance)):
                     # if not then actually end level
                     # its an outlier (that belongs) and should not be counted towards the average
                     #current_level_average = math.ceil((current_value + current_level_average)/2)
                     # REDEMPTION
                     pass
                else:
                    # STILL NOT REDEEMED
                    current_level_high = i-1;
                    # store levels in levels array
                    levels.append([current_level_low, current_level_high, current_level_average])
                    current_level_low = current_level_high = i
                    current_level_average = graph[i]
            else:
                current_level_high = i-1;
                # store levels in levels array
                levels.append([current_level_low, current_level_high, current_level_average])
                current_level_low = current_level_high = i
                current_level_average = graph[i]

    # once its done, the highest limit is the last index
    current_level_high = len(graph)-1
    levels.append([current_level_low, current_level_high, current_level_average])

    # LETS SEE WHAT THIS DOES
    """
        now that we have all the levels in order, use the level averages to find out what levels seem off
        Use the level averages and the level ranges, clean out the graph even further
        only dealing with the levels with a level above and below, NOT a limit point
    """
    levels_to_remove = []
    for i in range(1, len(levels)-1):
        # lets look only at the levels that are one point
        # pull out the current level
        level = levels[i]
        level_behind = levels[i-1]
        level_ahead = levels[i+1]
        if level[1] - level[0] == 0:
            # if the difference of the high limit & low limit are zero
            # the high and low limit are the same
            # if ( -1*sample_variance <= (ahead_value - current_level_average) <= sample_variance):
            if level_behind[2] < level_ahead[2]:
                level[2] = (level_ahead[2] + level_behind[2])/2
            elif level_behind[2] > level_ahead[2]:
                level[2] = (level_ahead[2] + level_behind[2])/2
            elif level_behind[2] == level_ahead[2]:
                level[2] = level_behind[2]

        elif (-1*sample_variance <= (level_behind[2] - level_ahead[2]) <= sample_variance):
            # so if the level in front and the level behind have similar averages
            # and if the variance you are measuring is very small
            if level[1] - level[0] < 10:
                new_ave = (level_ahead[2] + level_behind[2])/2
                level[2] = new_ave
                level[0] = level_behind[0]
                level[1] = level_ahead[1]
                levels_to_remove.append(level_ahead)
                levels_to_remove.append(level_behind)
    # remove suggested levels
    for level in levels_to_remove:
        levels.remove(level)

    return levels

def smooth(name, samples, sample_variance, debug=False):
    """
        given the samples and the variance, find each level of the graph
        while predicting each level average
    """
    print(name)
    level_output = mapGraphLevelEvening(samples, sample_variance)
    print(level_output)
    # create a graph using the level averages to show distinction
    output_graph = []
    for level in level_output:
        # for each level
        for i in range(level[0], level[1]+1):
            # for each range fill in the correct average
            output_graph.append(level[2])

    plt.figure(name)
    plt.subplot(211)
    plt.plot(samples)

    plt.subplot(212)
    plt.plot(output_graph)
    plt.show()

def run():
    smooth("Sampling Rate 30",samplex30, VAR_SAMPLEx30)
    print()
    smooth("Sampling Rate 10", samplex10, VAR_SAMPLEx10)
    print()
    smooth("Sampling Rate 5", samplex5, VAR_SAMPLEx5)
    print()
    smooth("Populated Area, Sampling Rate 30", pop_samplex30, VAR_SAMPLEx30)
    print()
    smooth("Populated Area, Sampling Rate 5", pop_samplex5, VAR_SAMPLEx5)
    print()
    smooth("Populated Room, Sampling Rate 10", pop_room_samplex10, VAR_SAMPLEx10)

if __name__ == '__main__':
    run()
