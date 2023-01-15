
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class MidiEffectAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                        public juce::Button::Listener

{
public:
    MidiEffectAudioProcessorEditor (MidiEffectAudioProcessor&);
    ~MidiEffectAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    std::vector<juce::Button> buttons;


private:
    
    MidiEffectAudioProcessor& audioProcessor;

    juce::ToggleButton m2;
    juce::ToggleButton M2;
    juce::ToggleButton m3;
    juce::ToggleButton M3;
    juce::ToggleButton p4;
    juce::ToggleButton tt;
    juce::ToggleButton p5;
    juce::ToggleButton m6;
    juce::ToggleButton M6;
    juce::ToggleButton m7;
    juce::ToggleButton M7;

    juce::Label title;

    void buttonClicked(juce::Button* button) override;
    void buttonStateChanged(juce::Button* button) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEffectAudioProcessorEditor)
};
