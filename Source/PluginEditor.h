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
class SvtPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::ComboBox::Listener
{
public:
    SvtPluginAudioProcessorEditor (SvtPluginAudioProcessor&);
    ~SvtPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SvtPluginAudioProcessor& audioProcessor;

    juce::Slider filterCutoffDial;
    juce::Slider filterResonanceDial;

    juce::ComboBox filterTypeMenu;

    juce::ScopedPointer <juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffTree;
    juce::ScopedPointer <juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceTree;
    juce::ScopedPointer <juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeTree;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvtPluginAudioProcessorEditor)
};
