/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SvtPluginAudioProcessorEditor::SvtPluginAudioProcessorEditor (SvtPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    setSize (200, 200);

    filterTypeMenu.setJustificationType(juce::Justification::centred);
    filterTypeMenu.addItem("Low Pass", 1);
    filterTypeMenu.addItem("Band Pass", 2);
    filterTypeMenu.addItem("High Pass",3);
    addAndMakeVisible(&filterTypeMenu);
    filterTypeMenu.addListener(this);

    filterCutoffDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(audioProcessor.minFilterCutoffValue, audioProcessor.maxFilterCutoffValue);
    filterCutoffDial.setValue(audioProcessor.defaultFilterCutoffValue);
    filterCutoffDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);

    filterResonanceDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterResonanceDial.setRange(audioProcessor.minFilterResonanceValue, audioProcessor.maxFilterResonanceValue);
    filterResonanceDial.setValue(audioProcessor.defaultFilterResonanceValue);
    filterResonanceDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterResonanceDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterResonanceDial);

    filterCutoffTree = new juce::AudioProcessorValueTreeState::SliderAttachment
    (audioProcessor.treeState, audioProcessor.filterCutoffId, filterCutoffDial);

    filterResonanceTree = new juce::AudioProcessorValueTreeState::SliderAttachment
    (audioProcessor.treeState, audioProcessor.filterResonanceId, filterResonanceDial);

    filterTypeTree = new juce::AudioProcessorValueTreeState::ComboBoxAttachment
    (audioProcessor.treeState, audioProcessor.filterTypeId, filterTypeMenu);

    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f);

}

SvtPluginAudioProcessorEditor::~SvtPluginAudioProcessorEditor()
{
}

//==============================================================================
void SvtPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Filter", titleArea, juce::Justification::centredTop);
    g.drawText("CutOff", 38,70,50,25, juce::Justification::centredTop);
    g.drawText("Resonance", 101, 70, 70, 25, juce::Justification::centredTop);


    juce::Rectangle<float> area(25, 25, 150, 150);

    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void SvtPluginAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);

    filterTypeMenu.setBounds(area.removeFromTop(20));
    filterCutoffDial.setBounds(30, 90, 70, 70);
    filterResonanceDial.setBounds(100, 90, 70, 70);

}

void SvtPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{

}
