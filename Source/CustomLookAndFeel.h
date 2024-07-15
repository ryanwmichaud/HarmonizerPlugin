
#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void changeToggleButtonWidthToFitText(juce::ToggleButton& button) override;
    //juce::Font getLabelFont(juce::Label& label) override;

};
