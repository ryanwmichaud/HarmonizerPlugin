
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class HarmonizerPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Button::Listener

{
public:
    HarmonizerPluginAudioProcessorEditor(HarmonizerPluginAudioProcessor&);
    ~HarmonizerPluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;


private:

    //juce::OwnedArray<juce::ToggleButton> buttons;
    //const juce::StringArray buttonNames{ "m2", "M2", "m3", "M3", "p4","tt", "p5", "m6", "M6", "m7", "M7" };



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



    HarmonizerPluginAudioProcessor& audioProcessor;

    

    juce::Label title;

    void buttonClicked(juce::Button* button) override;
    void buttonStateChanged(juce::Button* button) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HarmonizerPluginAudioProcessorEditor)
};