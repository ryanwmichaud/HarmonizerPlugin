
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiProcessor.h"

//==============================================================================
HarmonizerPluginAudioProcessorEditor::HarmonizerPluginAudioProcessorEditor(HarmonizerPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 200);

    addAndMakeVisible(title);
    title.setColour(juce::Label::textColourId, juce::Colours::wheat);
    title.setText("d H a r m o n i z e", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);

    juce::FontOptions options = juce::FontOptions(20.0f,1);
    juce::Font labelFont(options);
    title.setFont(labelFont);

    DBG("constructor");

    std::array<int, 12> chord = audioProcessor.getChord();

    
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


    if (chord[1] == 1) {
        m2.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[2] == 1) {
        M2.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[3] == 1) {
        m3.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[4] == 1) {
        M3.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[5] == 1) {
        p4.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[6] == 1) {
        tt.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[7] == 1) {
        p5.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[8] == 1) {
        m6.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[9] == 1) {
        M6.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[10] == 1) {
        m7.setToggleState(true, juce::dontSendNotification);
    }
    if (chord[11] == 1) {
        M7.setToggleState(true, juce::dontSendNotification);
    }


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

void HarmonizerPluginAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    //hover over?
}



void HarmonizerPluginAudioProcessorEditor::buttonClicked(juce::Button* button)
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


    if (stateBool) {
        audioProcessor.addChordTone(interval);
    }
    else {
        audioProcessor.removeChordTone(interval);
    }
}

HarmonizerPluginAudioProcessorEditor::~HarmonizerPluginAudioProcessorEditor()
{
}

//==============================================================================


void HarmonizerPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    /*
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::wheat);
    g.setFont (15.0f);
    g.drawFittedText ("d H a r m o n i z e", getLocalBounds(), juce::Justification::centred, 1);
    */
    
    
   

}



void HarmonizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto r = getBounds();
    title.setBounds(r.removeFromTop(30));
    r.removeFromLeft(20);

    auto leftSection = r.removeFromLeft(getWidth() / 2);

    int boxSize = 25;

    /*
    for (int i = 0; i < 6; ++i) {
        juce::ToggleButton* button = buttons[i];
        button->setBounds(leftSection.removeFromTop(boxSize));
    }
    for (int i = 7; i < 11; ++i) {
        juce::ToggleButton* button = buttons[i];
        button->setBounds(r.removeFromTop(boxSize));
    }
    */



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