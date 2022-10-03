/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChaseGP01PandaAudioProcessorEditor::ChaseGP01PandaAudioProcessorEditor (ChaseGP01PandaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    float pi = juce::MathConstants<float>::pi;
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 350);

    // connect front to back
    auto& processorParams = processor.getParameters();

    // add listeners
    mVolumeSlider.addListener(this);
    mPanSlider.addListener(this);

    // GUI ===========================================================================================

    getLookAndFeel().setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::lightgrey);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::black);

    // volume slider gui
    mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 30);

    juce::AudioParameterFloat* volume = (juce::AudioParameterFloat*)processorParams.getUnchecked(0);
    mVolumeSlider.setRange(volume->range.start, volume->range.end);
    mVolumeSlider.setValue(*volume);
    mVolumeSlider.setSkewFactorFromMidPoint(0.0f);

    mVolumeLabel.setText("Volume", juce::dontSendNotification);
    mVolumeLabel.attachToComponent(&mVolumeSlider, true);
    mVolumeLabel.setBoundsRelative(1.0f, 1.0f, 1.0f, 0.2f);

    // pan slider gui
    mPanSlider.setRotaryParameters((-3 * pi) / 4, (3 * pi) / 4, true);
    mPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 30);
    mPanSlider.getChildComponent(0)->setColour(0, juce::Colours::red);

    juce::AudioParameterFloat* pan = (juce::AudioParameterFloat*)processorParams.getUnchecked(1);
    mPanSlider.setRange(pan->range.start, pan->range.end);
    mPanSlider.setValue(*pan);

    mPanLabel.setText("Pan", juce::dontSendNotification);
    mPanLabel.attachToComponent(&mPanSlider,true);
    //mVolumeLabel.setBoundsRelative(1.0f, 1.0f, 1.0f, 0.2f);

    // panda
    juce::Image panda = juce::ImageCache::getFromMemory(BinaryData::panda_png, BinaryData::panda_pngSize);
    pandaLogo.setImage(panda);

    // make components visible
    addAndMakeVisible(mVolumeSlider);
    addAndMakeVisible(mVolumeLabel);
    addAndMakeVisible(mPanSlider);
    addAndMakeVisible(mPanLabel);
    addAndMakeVisible(pandaLogo);
}

ChaseGP01PandaAudioProcessorEditor::~ChaseGP01PandaAudioProcessorEditor()
{
}

//==============================================================================
void ChaseGP01PandaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);

    g.setColour (juce::Colours::black);
    g.setOpacity(0.5f);
    g.setFont (15.0f);
}

void ChaseGP01PandaAudioProcessorEditor::resized()
{
    juce::FlexBox mainFb;

    mainFb.flexWrap = juce::FlexBox::Wrap::noWrap;
    mainFb.justifyContent = juce::FlexBox::JustifyContent::center;
    mainFb.alignContent = juce::FlexBox::AlignContent::center;

    mainFb.items.add(juce::FlexItem(mVolumeSlider).withMinWidth(75.0f).withMinHeight(100.0f).withMargin(10.0f));
    mainFb.items.add(juce::FlexItem(pandaLogo).withMinWidth(200.0f).withMinHeight(200.0f).withMargin(20.0f));
    mainFb.items.add(juce::FlexItem(mPanSlider).withMinWidth(75.0f).withMinHeight(50.0f).withMargin(10.0f));

    mainFb.performLayout(getLocalBounds().toFloat());
}

void ChaseGP01PandaAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    auto& processorParams = processor.getParameters();

    if (slider == &mVolumeSlider) {
        juce::AudioParameterFloat* volumeParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(0);

        float sliderValue = mVolumeSlider.getValue();
        *volumeParam = sliderValue;
    }
    if (slider == &mPanSlider) {
        juce::AudioParameterFloat* panParam = (juce::AudioParameterFloat*)processorParams.getUnchecked(1);

        float sliderValue = mPanSlider.getValue();
        *panParam = sliderValue;
    }
}
