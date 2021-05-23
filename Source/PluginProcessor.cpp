/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SvtPluginAudioProcessor::SvtPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),treeState(*this,nullptr)
#endif
{
    juce::NormalisableRange<float> filterCutoffParam(minFilterCutoffValue, maxFilterCutoffValue);

    filterCutoffParam.setSkewForCentre(1000);
    juce::NormalisableRange<float> filterResonanceParam(minFilterResonanceValue, maxFilterResonanceValue);
    juce::NormalisableRange<float> filterTypeParam(minFilterTypeValue, maxFilterTypeValue);

    treeState.createAndAddParameter(filterCutoffId, filterCutoffName, filterCutoffName, filterCutoffParam, defaultFilterCutoffValue, nullptr, nullptr);
    treeState.createAndAddParameter(filterResonanceId, filterResonanceName, filterResonanceName, filterResonanceParam, defaultFilterResonanceValue, nullptr, nullptr);

    treeState.createAndAddParameter(filterTypeId, filterTypeName, filterTypeName, filterTypeParam, defaultFilterTypeValue, nullptr, nullptr);
    
    treeState.state = juce::ValueTree("Foo");
}

SvtPluginAudioProcessor::~SvtPluginAudioProcessor()
{
}

//==============================================================================
const juce::String SvtPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SvtPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SvtPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SvtPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SvtPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SvtPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SvtPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SvtPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SvtPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void SvtPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SvtPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getNumOutputChannels();
    
    stateVariableFilter.reset();
    updateFilter();
    stateVariableFilter.prepare(spec);

    
}

void SvtPluginAudioProcessor::updateFilter()
{
    int filterChoice = *treeState.getRawParameterValue(filterTypeId);
    float cutoff = *treeState.getRawParameterValue(filterCutoffId);
    float resonance = *treeState.getRawParameterValue(filterResonanceId);

    if (filterChoice == 0)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);
    }

    if (filterChoice == 1)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);
    }

    if (filterChoice == 2)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);
    }

}

void SvtPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SvtPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void SvtPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

   
    updateFilter();
    juce::dsp::AudioBlock<float> block(buffer);
    stateVariableFilter.process(juce::dsp::ProcessContextReplacing<float> (block));


}

//==============================================================================
bool SvtPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SvtPluginAudioProcessor::createEditor()
{
    return new SvtPluginAudioProcessorEditor (*this);
}

//==============================================================================
void SvtPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SvtPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SvtPluginAudioProcessor();
}
