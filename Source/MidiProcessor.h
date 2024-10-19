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


    std::array<int, 12> activeNoteNums = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

    int counter = 0;        //for cycling through inversions
    int numCTs = 1;              //num chords - same as num chord tones/possible inversions
    int lastOn;

    std::array<int, 12> distancesBetween = { 0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 };


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

        if (chord[interval] == 1) {
            chord[interval] = 0;
            numCTs -= 1;
            updateChord();
        }

    }
    void addChordTone(int interval) {

        if (chord[interval] == 0) {
            chord[interval] = 1;
            numCTs += 1;
            updateChord();
        }
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

    

 //v4
    void updateChord() { //populate inversions w top down inversions of given intervals

        DBG("new: " << chord[0] << chord[1] << chord[2] << chord[3] << chord[4] << chord[5] << chord[6] << chord[7] << chord[8] << chord[9] << chord[10] << chord[11]);
        DBG("numcts: " << numCTs);

        int i = 1;
        int currentDistance = 1;    //start here so we don't count a distance of 0. probably a cleaner way but this is efficient
        int distancesFound = 0;
        const int numDistancesToFind = numCTs - 1;  // we'll quit early and find the last distance from subtraction not iteration. 


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

        DBG(distancesBetween[0] << " " << distancesBetween[1] << " " << distancesBetween[2] << " " << distancesBetween[3] << " " << distancesBetween[4]);

        //now, take running totals of these distances from each point saving at each step. 

        for (int i = 0; i < numCTs; i++) { //i is the row we write to and also the offset to the new starting point 

            int runningTotalRow = 0;
            for (int j = distancesFound; j >= 1; j--) { // iterate backwards through dists, save running totals into inversions in the right place
                runningTotalRow -= distancesBetween[(j + i) % numCTs];
                inversions[i][j] = runningTotalRow;
            }
        }

        DBG(inversions[0][0] << inversions[0][1] << inversions[0][2]);
        DBG(inversions[1][0] << inversions[1][1] << inversions[1][2]);
        DBG(inversions[2][0] << inversions[2][1] << inversions[2][2]);


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
