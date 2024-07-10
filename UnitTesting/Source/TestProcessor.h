#pragma once
#include "JuceHeader.h"


class MidiProcessor {


public:

    std::array<int, 12> chord = {1,0,0,0,0,0,0,0,0,0,0,0};
    std::array<std::array<int, 12>, 12> inversions;

    int counter = 0;        //for cycling through inversions
    int numCTs;              //num chords - same as num chord tones/possible inversions
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

        chord[interval] = 0;
        updateChord();
    }
    void addChordTone(int interval) {

        chord[interval] = 1;
        updateChord();

    }


    void updateChord() { //populate inversions w top down inversions of given intervals
        numCTs = 0;
        //we wont clear inversions. we'll overwrite what we need and don't look past that. 

        DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);


        for (int i = 0; i < chord.size(); i++) { //one inversion with each top note
            if (chord[i] == 0) {
                continue;
            }
            else {
                numCTs += 1;                       //1 inversion for every true CT
                std::array<int, 12>& inversion = inversions[numCTs-1];
                DBG("array number "<< numCTs - 1<<":");
                int place = 0;
                for (int j = 0; j < chord.size(); j++) {
                    if (chord[j] == 0) {
                        continue;
                    }
                    else {
                        int dist = j - i;
                        if (dist > 0) { //if not below, put it below
                            dist -= 12;
                            dist %= 12;
                        }
                        inversion[place] = dist;
                        DBG("index " << place << " gets " << dist);
                        place += 1;

                    }
                    
                }
            }
        }
        counter = 0;

    }



    
    



};
