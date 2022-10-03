/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ChaseGP01PandaAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    ChaseGP01PandaAudioProcessorEditor (ChaseGP01PandaAudioProcessor&);
    ~ChaseGP01PandaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChaseGP01PandaAudioProcessor& audioProcessor;
    juce::Slider mVolumeSlider;
    juce::Slider mPanSlider;
    juce::Label mVolumeLabel;
    juce::Label mPanLabel;
    juce::ImageComponent pandaLogo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChaseGP01PandaAudioProcessorEditor)
};
