#pragma once
#include "JuceHeader.h"

class MidiProcessor {


public:

    std::array<int, 12> chord = {1,0,0,0,0,0,0,0,0,0,0,0};
    std::array<std::array<int, 12>, 12> inversions;

    int counter = 0;        //for cycling through inversions
    int numCTs;              //num chords - same as num chord tones/possible inversions
    int lastOn;


    juce::MidiBuffer processedBuffer;  //holds temporary altered notes at any given time before swapping


    MidiProcessor() {
        //chord = { 0, 2, 4, 7, 11 }; //default to Maj9 on instatiation
        //chord = {1,0,1,0,1,0,0,1,0,0,0,1}; //default to Maj9 on instatiation
        updateChord();
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

    void process(juce::MidiBuffer& midiMessages) {      //pass in midi buffer by reference so we can change it in memory

        processedBuffer.clear();
        processMidiInput(midiMessages);             //clear and refill it. swap contents w the reference
        midiMessages.swapWith(processedBuffer);
    }


    void processMidiInput(const juce::MidiBuffer& midiMessages) {

        juce::MidiBuffer::Iterator it(midiMessages);    //use this iterator to iterate through midi events in the block
        juce::MidiMessage currentMessage;
        int samplePos;                                  //to store the sample position from the iterator
        while (it.getNextEvent(currentMessage, samplePos)) {

            if (currentMessage.isNoteOn()) {


                lastOn = currentMessage.getNoteNumber();
                counter = (counter + 1) % numCTs;

                //monophonic - turn off all other notes when new note played
                for (int i = 0; i <= 127; i++) {
                    auto off = juce::MidiMessage::noteOff(currentMessage.getChannel(), i, (float)0.5);
                    processedBuffer.addEvent(off, samplePos);
                }

                harmonize(currentMessage, samplePos);

            }
            else if (currentMessage.isNoteOff()) {
                if (currentMessage.getNoteNumber() == lastOn) { //ignore if not turning off most recent
                    harmonize(currentMessage, samplePos);
                }

            }
            else {
                processedBuffer.addEvent(currentMessage, samplePos);    //add to the processed buffer at the same time as original note
            }

        }
      

    }

    void harmonize(juce::MidiMessage currentMessage, int samplePos) {
        DBG("reached harm, counter: " << counter << " inversion is: " << inversions[counter][0] << inversions[counter][1] << inversions[counter][2]);
        
        processedBuffer.addEvent(currentMessage, samplePos);  //not necessary rn

        for (int i = 0; i < numCTs; i++) {
            DBG(inversions[counter][i]);
            auto transposedMessage = currentMessage;
            transposedMessage.setNoteNumber(currentMessage.getNoteNumber() + inversions[counter][i]);
            processedBuffer.addEvent(transposedMessage, samplePos);
        }
    }



};
