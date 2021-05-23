/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SvtPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SvtPluginAudioProcessor();
    ~SvtPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void updateFilter();

    float minFilterCutoffValue = 20.0f;
    float maxFilterCutoffValue = 20000.0f;
    float defaultFilterCutoffValue = 600.0f;

    float minFilterResonanceValue = 0.1f;
    float maxFilterResonanceValue = 10.0f;
    float defaultFilterResonanceValue = 2.0f;

    float minFilterTypeValue = 0;
    float maxFilterTypeValue = 2;
    float defaultFilterTypeValue = 0;

    std::string filterCutoffId = "filterCutoff";
    std::string filterCutoffName = "Cutoff";

    std::string filterResonanceId = "filterResonance";
    std::string filterResonanceName = "Resonance";

    std::string filterTypeId = "filterType";
    std::string filterTypeName = "Filter Type";

    

    juce::AudioProcessorValueTreeState treeState;



private:

    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>, 
        juce::dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;

    float lastSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SvtPluginAudioProcessor)
};
