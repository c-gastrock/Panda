/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChaseGP01PandaAudioProcessor::ChaseGP01PandaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // destructor handled by juce
    addParameter(mVolParam = new juce::AudioParameterFloat("volume", "Volume", -60.0f, 10.0f, 0.0f)); // 0.001 = -60db, 3.16 = 10db
    addParameter(mPanParam = new juce::AudioParameterFloat("pan", "Pan", -50.0f, 50.0f, 0.0f));
}

ChaseGP01PandaAudioProcessor::~ChaseGP01PandaAudioProcessor()
{
}

//==============================================================================
const juce::String ChaseGP01PandaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChaseGP01PandaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChaseGP01PandaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChaseGP01PandaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChaseGP01PandaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChaseGP01PandaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChaseGP01PandaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChaseGP01PandaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ChaseGP01PandaAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChaseGP01PandaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ChaseGP01PandaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    float pi = juce::MathConstants<float>::pi;

    mGainLinear = 1.0f;
    constPowLeft = pi/4;
    constPowRight = pi/4;
}

void ChaseGP01PandaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChaseGP01PandaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

// Self-defined funcs
void ChaseGP01PandaAudioProcessor::calcAlgorithmParams() {
    mGainLinear = juce::Decibels::decibelsToGain(mVolParam->get()); // change internal var based on user changed param

    // constant power panning calculation
    float panVal = (-1*mPanParam->get() + 50.0f)*(juce::MathConstants<float>::halfPi/100.0f); // pi/2 is full left, 0 is full right
    constPowLeft = std::sin(panVal);
    constPowLeft *= constPowLeft;
    constPowRight = std::cos(panVal);
    constPowRight *= constPowRight;
}

void ChaseGP01PandaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //===========================================================================
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    calcAlgorithmParams();
    // loop through sample indices
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        leftChannel[sample] *= mGainLinear * constPowLeft;
        rightChannel[sample] *= mGainLinear * constPowRight;
    }
}

//==============================================================================
bool ChaseGP01PandaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ChaseGP01PandaAudioProcessor::createEditor()
{
    return new ChaseGP01PandaAudioProcessorEditor (*this);
}

//==============================================================================
void ChaseGP01PandaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChaseGP01PandaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChaseGP01PandaAudioProcessor();
}
