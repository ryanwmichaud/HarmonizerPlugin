
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
    
    setLookAndFeel(&customLookAndFeel);


    addAndMakeVisible(title);
    title.setText("d H a r m o n i z e", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    juce::FontOptions options = juce::FontOptions(20.0f,1);
    juce::Font titleFont(options);
    title.setFont(titleFont);

    DBG("constructor");

    std::array<int, 12> chord = audioProcessor.getChord();
    bool cycle = audioProcessor.isCycleOn();

    initializeButton(m2, "m2", chord[1]);
    initializeButton(M2, "M2", chord[2]);
    initializeButton(m3, "m3", chord[3]);
    initializeButton(M3, "M3", chord[4]);
    initializeButton(p4, "p4", chord[5]);
    initializeButton(tt, "tt", chord[6]);
    initializeButton(p5, "p5", chord[7]);
    initializeButton(m6, "m6", chord[8]);
    initializeButton(M6, "M6", chord[9]);
    initializeButton(m7, "m7", chord[10]);
    initializeButton(M7, "M7", chord[11]);

    addAndMakeVisible(cycleButton);
    cycleButton.setButtonText("cycle");
    cycleButton.setToggleState(cycle, juce::dontSendNotification);
    cycleButton.onClick = [this] {
        cycleButtonClicked(&cycleButton);
    };
}

HarmonizerPluginAudioProcessorEditor::~HarmonizerPluginAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void HarmonizerPluginAudioProcessorEditor::cycleButtonClicked(juce::Button* button) {
    audioProcessor.toggleCycle();
}


void HarmonizerPluginAudioProcessorEditor::initializeButton(juce::ToggleButton& button, const juce::String& buttonText, bool initialState)
{
    addAndMakeVisible(button);
    button.setButtonText(buttonText);
    //customLookAndFeel.changeToggleButtonWidthToFitText(button);    
    button.setToggleState(initialState, juce::dontSendNotification);
    button.onClick = [this, &button] { buttonClicked(&button); };
}

void HarmonizerPluginAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    DBG("statecahnged");
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




void HarmonizerPluginAudioProcessorEditor::paint(juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


}



void HarmonizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto rightSection = getBounds();
    title.setBounds(rightSection.removeFromTop(30));
    rightSection.removeFromTop(10);

    int buttonHeight = 15;
    int buttonWidth = 30;
    int cycleButtonWidth = 50;
    int vSpace = 10;

    rightSection.removeFromLeft(getWidth() / 10);
    auto leftSection = rightSection.removeFromLeft(buttonWidth);
    rightSection.removeFromLeft(getWidth() / 10);
    auto secondSection = rightSection.removeFromLeft(buttonWidth);
    rightSection.removeFromLeft(getWidth() / 10);
    auto thirdSection = rightSection.removeFromLeft(cycleButtonWidth);
         

    

    /*
    for (int i = 0; i < 6; ++i) {
        juce::ToggleButton* button = buttons[i];
        button->setBounds(leftSection.removeFromTop(buttonHeight));
    }
    for (int i = 7; i < 11; ++i) {
        juce::ToggleButton* button = buttons[i];
        button->setBounds(r.removeFromTop(buttonHeight));
    }
    */



    m2.setBounds(leftSection.removeFromTop(buttonHeight));
    leftSection.removeFromTop(vSpace);
    M2.setBounds(leftSection.removeFromTop(buttonHeight));
    leftSection.removeFromTop(vSpace);
    m3.setBounds(leftSection.removeFromTop(buttonHeight));
    leftSection.removeFromTop(vSpace);
    M3.setBounds(leftSection.removeFromTop(buttonHeight));
    leftSection.removeFromTop(vSpace);
    p4.setBounds(leftSection.removeFromTop(buttonHeight));
    leftSection.removeFromTop(vSpace);
    tt.setBounds(leftSection.removeFromTop(buttonHeight));

    secondSection.removeFromTop((buttonHeight+vSpace)/2);
    p5.setBounds(secondSection.removeFromTop(buttonHeight));
    secondSection.removeFromTop(vSpace);
    m6.setBounds(secondSection.removeFromTop(buttonHeight));
    secondSection.removeFromTop(vSpace);
    M6.setBounds(secondSection.removeFromTop(buttonHeight));
    secondSection.removeFromTop(vSpace);
    m7.setBounds(secondSection.removeFromTop(buttonHeight));
    secondSection.removeFromTop(vSpace);
    M7.setBounds(secondSection.removeFromTop(buttonHeight));

    cycleButton.setBounds(thirdSection.removeFromTop(buttonHeight));







}