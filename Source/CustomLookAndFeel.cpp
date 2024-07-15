#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);
    setColour(juce::Label::textColourId, juce::Colours::wheat);
    setColour(juce::ToggleButton::textColourId, juce::Colours::white);
}

void CustomLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown){
    auto fontSize = 15.0f;
    float cornerSize = 10.0f; 
    float borderSize = 2.0f;

    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    auto textBounds = button.getLocalBounds();


    g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, 10);

    if (button.getToggleState()) {
        g.setColour(juce::Colours::wheat);
        g.fillRoundedRectangle(textBounds.toFloat(), cornerSize);
        g.setColour(juce::Colours::black);
        g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, 10);
    }

    else {
        g.setColour(juce::Colours::grey);
        g.drawRoundedRectangle(textBounds.toFloat(), cornerSize, borderSize);
        g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, 10);
    }


    if (shouldDrawButtonAsHighlighted)
    {
        // while hovered 
        g.setColour(juce::Colours::black);
        g.drawRoundedRectangle(textBounds.toFloat(), cornerSize, borderSize);
        g.setColour(juce::Colours::grey);
        g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, 10);


    }

    if (shouldDrawButtonAsDown)
    {
        // while clicking
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(textBounds.toFloat(), cornerSize, borderSize);
    }

}

void CustomLookAndFeel::changeToggleButtonWidthToFitText(juce::ToggleButton& button) {
    LookAndFeel_V4::changeToggleButtonWidthToFitText(button);

}


