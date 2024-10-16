#pragma once
#include "JuceHeader.h"



class MidiProcessor {


public:

    std::array<int, 12> chord = { 1,0,0,0,0,0,0,0,0,0,0,0 };
    std::array<std::array<int, 12>, 12> inversions; /*= { {
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},
        {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13}
    } };
    */
    int counter = 0;        //for cycling through inversions
    int numCTs = 1;              //num chords - same as num chord tones/possible inversions
    int lastOn;






    MidiProcessor() {
        //chord = { 0, 2, 4, 7, 11 }; //default to Maj9 on instatiation
        //chord = {1,0,1,0,1,0,0,1,0,0,0,1}; //default to Maj9 on instatiation
        updateChord();
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



*/