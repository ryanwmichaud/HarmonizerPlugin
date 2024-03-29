
#pragma once
#include "JuceHeader.h"

class MidiProcessor {
   

public:

    std::vector<int> chord = {0};
    std::vector<std::vector<int>> inversions;

    int counter = 0;        //for cycling through inversions
    int limit;              //num chords - same as num chord tones/possible inversions
    int lastOn;


    juce::MidiBuffer processedBuffer;  //holds temporary altered notes at any given time before swapping


    MidiProcessor() {
        //chord = { 0, 2, 4, 7, 11 }; //default to Maj9 on instanstiation
        updateChord();
    }

    void removeChordTone(int interval) {

        std::vector<int> newChord = {};

        for (int i = 0; i < chord.size(); i++) {
            if (chord[i] != interval) {
                newChord.push_back(chord[i]);
            }
        }
        chord = newChord;
        updateChord();
    }
    void addChordTone(int interval) {

       chord.push_back(interval);
       updateChord();

    }


    void updateChord() { //populate inversions w top down inversions of given intervals
        inversions.clear();
        for (int i = 0; i < chord.size(); i++) { //one inversion with each top note
            std::vector<int> inversion = {};
            for (int j = 0; j < chord.size(); j++) {
                int dist = chord[j] - chord[i];
                if (dist > 0) { //if not below, put it below
                    dist -= 12;
                    dist %= 12;
                }
                inversion.push_back(dist);
            }
            inversions.push_back(inversion);
        }
        counter = 0;
        limit = chord.size();
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
                    counter = (counter + 1) % limit;
                    
                    //monophonic - turn off all other notes when new note played
                    for (int i = 0; i <= 127; i++) {
                        auto off = juce::MidiMessage::noteOff(currentMessage.getChannel(),i,(float)0.5);
                        processedBuffer.addEvent(off, samplePos);
                    }
                    
                    harmonize(currentMessage, samplePos);
                    
            }
            else if (currentMessage.isNoteOff()) {
                if (currentMessage.getNoteNumber() == lastOn) { //ignore if not turning off most recent
                    harmonize(currentMessage, samplePos);
                }

            }else {
                processedBuffer.addEvent(currentMessage, samplePos);    //add to the processed buffer at the same time as original note
            }

        }

    }

    void harmonize(juce::MidiMessage currentMessage, int samplePos) {
        DBG("reached harm");
        processedBuffer.addEvent(currentMessage, samplePos);
        
        for (int i = 0; i < inversions[counter].size(); i++) {
            auto transposedMessage = currentMessage;
            transposedMessage.setNoteNumber(currentMessage.getNoteNumber() + inversions[counter][i]);
            DBG(transposedMessage.getDescription());
            processedBuffer.addEvent(transposedMessage, samplePos);
        }

        
        
        
    }

};
