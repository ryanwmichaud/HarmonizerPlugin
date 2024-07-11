/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HarmonizerPluginAudioProcessor::HarmonizerPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsHarmonizerPlugin
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    MidiProcessor* mpPtr = new MidiProcessor();
    midiProcessor = *mpPtr;

}

HarmonizerPluginAudioProcessor::~HarmonizerPluginAudioProcessor()
{
}

//==============================================================================
const juce::String HarmonizerPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmonizerPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool HarmonizerPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool HarmonizerPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsHarmonizerPlugin
    return true;
#else
    return false;
#endif
}

double HarmonizerPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmonizerPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HarmonizerPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmonizerPluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String HarmonizerPluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void HarmonizerPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void HarmonizerPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void HarmonizerPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmonizerPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsHarmonizerPlugin
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void HarmonizerPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    midiProcessor.process(midiMessages);

}

MidiProcessor& HarmonizerPluginAudioProcessor::getMidiProcessor() {
    MidiProcessor& ref = midiProcessor;
    return ref;
}

std::array<int, 12> HarmonizerPluginAudioProcessor::getChord() {
    return midiProcessor.getChord();
}

void HarmonizerPluginAudioProcessor::addChordTone(int interval) {
    midiProcessor.addChordTone(interval);

}

void HarmonizerPluginAudioProcessor::removeChordTone(int interval) {
    midiProcessor.removeChordTone(interval);
}


//==============================================================================
bool HarmonizerPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HarmonizerPluginAudioProcessor::createEditor()
{
    return new HarmonizerPluginAudioProcessorEditor(*this);
}

//==============================================================================
void HarmonizerPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HarmonizerPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonizerPluginAudioProcessor();
}