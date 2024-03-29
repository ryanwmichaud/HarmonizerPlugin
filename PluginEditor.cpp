/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiEffectAudioProcessorEditor::MidiEffectAudioProcessorEditor (MidiEffectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);

    addAndMakeVisible(title);
    title.setText("Harmonizer", juce::dontSendNotification);
    title.setColour(juce::Label::textColourId, juce::Colours::cadetblue);
    title.setJustificationType(juce::Justification::centred);
    title.setFont(juce::Font(20.0f, juce::Font::bold));

    

    addAndMakeVisible(m2);
    addAndMakeVisible(M2);
    addAndMakeVisible(m3);
    addAndMakeVisible(M3);
    addAndMakeVisible(p4);
    addAndMakeVisible(tt);
    addAndMakeVisible(p5);
    addAndMakeVisible(m6);
    addAndMakeVisible(M6);
    addAndMakeVisible(m7);
    addAndMakeVisible(M7);

    m2.setButtonText("m2");
    M2.setButtonText("M2");
    m3.setButtonText("m3");
    M3.setButtonText("M3");
    p4.setButtonText("p4");
    tt.setButtonText("tt");
    p5.setButtonText("p5");
    m6.setButtonText("m6");
    M6.setButtonText("M6");
    m7.setButtonText("m7");
    M7.setButtonText("M7");


    m2.addListener(this);
    M2.addListener(this);
    m3.addListener(this);
    M3.addListener(this);
    p4.addListener(this);
    tt.addListener(this);
    p5.addListener(this);
    m6.addListener(this);
    M6.addListener(this);
    m7.addListener(this);
    M7.addListener(this);

    


}

void MidiEffectAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    //hover over?
}

void MidiEffectAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    bool stateBool = button->getToggleState();
    juce::String name = button->getButtonText();
    DBG("click and now " << name << " is " << (stateBool ? "true" : "false"));

    int interval = 0;

    std::vector<juce::String> list = { "m2","M2","m3","M3","p4","tt","p5","m6","M6","m7","M7" };
    for (int i = 0; i < 11; i++) {
        if (name.compare(list[i]) == 0) {
            interval = i + 1;
        }
    }

    DBG("name was "<<name<<", interval is" << interval);


    MidiProcessor& midiProcessor = audioProcessor.getMidiProcessor();

    if (stateBool) {
        midiProcessor.addChordTone(interval);
    }
    else {
        midiProcessor.removeChordTone(interval);
    }




}

MidiEffectAudioProcessorEditor::~MidiEffectAudioProcessorEditor()
{
}

//==============================================================================
void MidiEffectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    /*
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::wheat);
    g.setFont (15.0f);
    g.drawFittedText ("Dharmonizer", getLocalBounds(), juce::Justification::centred, 1);
    
    */
    
}

void MidiEffectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto r = getBounds();
    title.setBounds(r.removeFromTop(30));
    r.removeFromLeft(20);

    auto leftSection = r.removeFromLeft(getWidth()/2);

    int boxSize = 25;

    m2.setBounds(leftSection.removeFromTop(boxSize));
    M2.setBounds(leftSection.removeFromTop(boxSize));
    m3.setBounds(leftSection.removeFromTop(boxSize));
    M3.setBounds(leftSection.removeFromTop(boxSize));
    p4.setBounds(leftSection.removeFromTop(boxSize));
    tt.setBounds(leftSection.removeFromTop(boxSize));

    p5.setBounds(r.removeFromTop(boxSize));
    m6.setBounds(r.removeFromTop(boxSize));
    M6.setBounds(r.removeFromTop(boxSize));
    m7.setBounds(r.removeFromTop(boxSize));
    M7.setBounds(r.removeFromTop(boxSize));
    
    

    


}
