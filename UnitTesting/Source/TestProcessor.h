#pragma once
#include "JuceHeader.h"



class MidiProcessor {


public:

    std::array<int, 12> chord = { 1,0,0,0,0,0,0,0,0,0,0,0 };
    std::array<std::array<int, 12>, 12> inversions = { {
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    } };
    
    int counter = 0;        //for cycling through inversions
    int numCTs = 1;              //num chords - same as num chord tones/possible inversions
    int lastOn;

    std::array<int, 12> distancesBetween = { 0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 };






    MidiProcessor() {       
        updateChord();  //populate inversion matrix
    }



    std::array<int, 12> getChord() {
        return chord;
    }


    void removeChordTone(int interval) {
        DBG("removing");


        if (chord[interval] == 1) {
            chord[interval] = 0;
            numCTs -= 1;
            updateChord();
        }

    }
    void addChordTone(int interval) {

        DBG("adding");

        if (chord[interval] == 0) {
            chord[interval] = 1;
            numCTs += 1;
            updateChord();
        }
    }

    void updateChord() { //populate inversions w top down inversions of given intervals
        
        DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);


        int i = 1;      
        int currentDistance = 1;    //start here so we don't count a distance of 0. probably a cleaner way but this is efficient
        int distancesFound = 0;
        const int numDistancesToFind = numCTs -1;  // we'll quit early and find the last distance from subtraction not iteration. 


        while (distancesFound < numDistancesToFind) {  //get distances from each member to the next. quit early and get the last one w subtraction. 

            if (chord[i] == 1) {
                distancesBetween[distancesFound] = currentDistance;
                currentDistance = 1;
                distancesFound += 1;
            }
            else {
                currentDistance += 1;
            }
            i += 1;
        }
        distancesBetween[distancesFound] = (13 - i); //calc distance from the last ct til we wrap at index 0. (index 0 will always be a 1)



        //now, take running totals of these distances from each point saving at each step. 

        for (int i = 0; i < numCTs; i++) { //i is the row we write to and also the offset to the new starting point 

            int runningTotalRow = 0;
            for (int j = distancesFound; j >= 1; j--) { // iterate backwards through dists, save running totals into inversions in the right place
                runningTotalRow -= distancesBetween[(j+i)%numCTs];
                inversions[i][j] = runningTotalRow;
            }
        }

        DBG(inversions[0][0] << inversions[0][1] << inversions[0][2]);
        DBG(inversions[1][0] << inversions[1][1] << inversions[1][2]);
        DBG(inversions[2][0] << inversions[2][1] << inversions[2][2]);


    }

 


/* 
got rid of 
int dist = j-i;
if (dist > 0) { //if not below, put it below
    dist -= 12;
    dist %= 12;
by counting down from 0



     DBG(inversions[0][0] << ", " << inversions[0][1] << ", " << inversions[0][2]);
        DBG(inversions[1][0] << ", " << inversions[1][1] << ", " << inversions[1][2]);
        DBG(inversions[2][0] << ", " << inversions[2][1] << ", " << inversions[2][2]);
        counter = 0;
}*/







};


/*
* 
* 

void updateChord() { //populate inversions w top down inversions of given intervals

        //we wont clear inversions. we'll overwrite what we need and don't look past that.

        DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);
        std::array<int, 12> rowCounters = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        std::array<int, 12> numDistancesFound = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        int lastRowStarted = -1;


        for (int i = 33; i >= 0; i--) { //step backwards looking for 1s


            DBG("looking at index " << i);

            if (chord[i%12] == 0) {

                DBG("got a 0");

                for (int j = 0; j < lastRowStarted+1; j++) {  //incr each of the live counters
                    rowCounters[j] = rowCounters[j] + 1;
                    DBG("incremented counter " << j << " to " << rowCounters[j]);
                }
            } else {                //when we get a 1
                DBG("got a 1");

                lastRowStarted += 1;      //start a counter
                DBG("started counter at" << lastRowStarted << ". lastRowStarted is now " << lastRowStarted);

                for (int j = 0; j < lastRowStarted+1; j++) {  //incr and add a distance for each of the live counters
                    rowCounters[j] = rowCounters[j] + 1 ;
                    DBG("incremented counter " << j << " to " << rowCounters[j]);
                    inversions[j][numDistancesFound[j]] = rowCounters[j];
                    DBG("added row " << j << " index " << numDistancesFound[j] << " value of " << rowCounters[j]);
                    numDistancesFound[j] = numDistancesFound[j] + 1;
                }

                DBG(inversions[0][0] << inversions[0][1] << inversions[0][2]);
                DBG(inversions[1][0] << inversions[1][1] << inversions[1][2]);
                DBG(inversions[2][0] << inversions[2][1] << inversions[2][2]);
            }


            DBG("done if numdistancesfound at " << numCTs << " is " << numCTs << ". it is " << numDistancesFound[numCTs]);
            if (numDistancesFound[numCTs] == numCTs) {
                DBG("done and resetting");
                rowCounters = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
                numDistancesFound = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                lastRowStarted = -1;
            }

        }

        counter = 0;


    }



       void updateChord() { //populate inversions w top down inversions of given intervals

        DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);


        int row = 0;
        for (int i = 0; i < 12; i++) { //look for cts as 1s

            if (chord[i] == 0) {
                continue;
            }

            else {

                DBG("found a 1 at " << i);

                int inversionIndex = 0;
                int j = i-1;
                int distance = -1;

                while (inversionIndex < numCTs) { //come back to this

                    if (j < 0) { j = 11; }

                    if (chord[j] == 1) { //found another ct to get distance
                        inversions[row][inversionIndex] = distance;
                        inversionIndex += 1;
                        DBG("found another 1 at " << j << ". distance is " << distance << ". put it at " << row << " , " << inversionIndex);
                    }

                    j -= 1;
                    distance -= 1;
                    if (distance == -12) { distance = 0; }

                }
                row += 1;

            }
        }


    }

*/