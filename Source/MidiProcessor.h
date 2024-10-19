#pragma once
#include "JuceHeader.h"


class MidiProcessor {


public:

    std::array<int, 12> chord = { 1,0,0,0,0,0,0,0,0,0,0,0 };
    std::array<std::array<int, 12>, 12> inversions;
    std::array<int, 12> activeNoteNums = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

    int counter = 0;        //for cycling through inversions
    int numCTs;              //num chords - same as num chord tones/possible inversions
    int lastOn;

    std::array<int, 11> distancesBetween;
    

    bool cycle = true;


    juce::MidiBuffer processedBuffer;  //holds temporary altered notes at any given time before swapping


    MidiProcessor() {
        //chord = { 0, 2, 4, 7, 11 }; //default to Maj9 on instatiation
        //chord = {1,0,1,0,1,0,0,1,0,0,0,1}; //default to Maj9 on instatiation
        updateChord(); //populate inversion matrix
   
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

    void toggleCycle() {
        if (cycle) {
            cycle = false;
        }
        else {
            cycle = true;
        }
        DBG("cycle changed to: ", cycle);
    }

    bool isCycleOn() {
        DBG("cycle: ", cycle);
        return cycle;
    }

    /*


 
    //v1
    void updateChord() { //populate inversions w top down inversions of given intervals
        numCTs = 0;

        //we wont clear inversions. we'll overwrite what we need and don't look past that. 

        //DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);


        for (int i = 0; i < 12; i++) { //one inversion with each top note
            if (chord[i] == 0) {
                continue;
            }
            else {
                numCTs += 1;                       //1 inversion for every true CT
                std::array<int, 12>& inversion = inversions[numCTs-1];
                //DBG("array number "<< numCTs - 1<<":");
                int place = 0;
                for (int j = 0; j < 12; j++) {
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
                        //DBG("index " << place << " gets " << dist);
                        place += 1;

                    }
                    
                }
            }
        }
        counter = 0;

    }

    //v2
    void updateChord() { //populate inversions w top down inversions of given intervals

    //we wont clear inversions. we'll overwrite what we need and don't look past that. 

        DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);
        std::array<int, 12> rowCounters = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        std::array<int, 12> numDistancesFound = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        int lastRowStarted = -1;
        int lastRowEnded = -1;


        for (int i = 33; i >= 0; i--) { //step backwards looking for 1s


            DBG("looking at index " << i);

            if (chord[i % 12] == 0) {

                DBG("got a 0");

                for (int j = 0; j < lastRowStarted + 1; j++) {  //incr each of the live counters
                    rowCounters[j] = rowCounters[j] + 1;
                    DBG("incremented counter " << j << " to " << rowCounters[j]);
                }
            }
            else {                //when we get a 1
                DBG("got a 1");

                lastRowStarted += 1;      //start a counter
                DBG("started counter at" << lastRowStarted << ". lastRowStarted is now " << lastRowStarted);

                for (int j = 0; j < lastRowStarted + 1; j++) {  //incr and add a distance for each of the live counters
                    rowCounters[j] = rowCounters[j] + 1;
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

  

    //v3

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
                int j = i - 1;
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

    void updateChord() {

        int count = 0;
        for (int i = 0; i < 12; i++) {
            if (chord[i] == 1) {
                distancesBetween[i] = count;
                count = 0;
            }
            else {
                count += 1;
            }

        }
        DBG(distancesBetween);

    }




    void process(juce::MidiBuffer& midiMessages) {      //pass in midi buffer by reference so we can change it in memory

        processedBuffer.clear();
        processMidiInput(midiMessages);             //clear and refill it. swap contents w the reference
        midiMessages.swapWith(processedBuffer);
    }


    void processMidiInput(const juce::MidiBuffer& midiMessages) {

        for (const auto metadata : midiMessages) {
            const auto& currentMessage = metadata.getMessage();
            int samplePos = metadata.samplePosition;

            if (currentMessage.isNoteOn()) {

                lastOn = currentMessage.getNoteNumber();
                if (cycle) {
                    counter = (counter + 1) % numCTs;
                }
                

                //monophonic - turn off active notes when new note played
                for (int i = 0; i < 12; i++) {
                    if (activeNoteNums[i] != -1) {
                        auto off = juce::MidiMessage::noteOff(currentMessage.getChannel(), activeNoteNums[i], (float)0.5);
                        processedBuffer.addEvent(off, samplePos);
                    }
                    DBG("turned off " << activeNoteNums[i]);
                }
                

                harmonize(currentMessage, samplePos);

            }
            else if (currentMessage.isNoteOff()) {
                if (currentMessage.getNoteNumber() == lastOn) { //ignore if not turning off most recent and "harmonizing" the off
                    for (int i = 0; i < 12; i++) {
                        if (activeNoteNums[i] != -1) {
                            auto off = juce::MidiMessage::noteOff(currentMessage.getChannel(), activeNoteNums[i], (float)0.5);
                            processedBuffer.addEvent(off, samplePos);
                        }
                        DBG("turned off " << activeNoteNums[i]);
                    }
                }
            }
            else {
                processedBuffer.addEvent(currentMessage, samplePos);    //if not note on/off, add to the processed buffer at the same time as original note unchanged
            }

        }
      

    }

    void harmonize(juce::MidiMessage currentMessage, int samplePos) {
        //DBG("reached harm, counter: " << counter << " inversion is: " << inversions[counter][0] << inversions[counter][1] << inversions[counter][2]);
        
        processedBuffer.addEvent(currentMessage, samplePos);  //not necessary rn

        for (int i = 0; i < numCTs; i++) {
            //DBG(inversions[counter][i]);
            juce::MidiMessage transposedMessage = currentMessage;
            int newNum = currentMessage.getNoteNumber() + inversions[counter][i];
            transposedMessage.setNoteNumber(newNum);
            activeNoteNums[i] = newNum;
            processedBuffer.addEvent(transposedMessage, samplePos);
        }

        DBG("activenotes: " << activeNoteNums[0] << activeNoteNums[1] << activeNoteNums[2] << activeNoteNums[3] << activeNoteNums[4] << activeNoteNums[5] << activeNoteNums[6] << activeNoteNums[7] << activeNoteNums[8] << activeNoteNums[9] << activeNoteNums[10] << activeNoteNums[11]  );
    }



};
