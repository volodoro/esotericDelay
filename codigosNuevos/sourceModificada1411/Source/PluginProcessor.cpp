// ========================================================================
// Created by Ricardo Saldias on 12-11-25.
// Ingenieria en Sonido
// Programacion de plugins
// Universidad de Chile
// ========================================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
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
    , apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout DelayAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Parámetro: Tiempo de Delay (0 - 2000 ms)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(PARAM_DELAY_TIME, 1),
        "Delay Time",
        juce::NormalisableRange<float>(0.0f, 2000.0f, 1.0f),
        500.0f,  // Default: 500ms
        "ms"
    ));
    
    // Parámetro: Feedback (0 - 95%)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(PARAM_FEEDBACK, 1),
        "Feedback",
        juce::NormalisableRange<float>(0.0f, 0.95f, 0.01f),
        0.5f,    // Default: 50%
        "%",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value * 100.0f, 1) + "%"; },
        nullptr
    ));
    
    // Parámetro Wet/Dry Mix (0 - 100%)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(PARAM_WET_DRY, 1),
        "Wet/Dry Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f,  // Default: 50% wet, 50% dry
        "%",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value * 100.0f, 1) + "%"; },
        nullptr
    ));
    
    // Parámetro Mute
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(PARAM_MUTE, 1),
        "Mute",
        false
    ));
    
    // Parámetro Bypass
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID(PARAM_BYPASS, 1),
        "Bypass",
        false
    ));
    
    
    return layout;
}

//==============================================================================
const juce::String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    // El delay tiene tail debido al feedback
    auto* delayTimeParam = apvts.getRawParameterValue(PARAM_DELAY_TIME);
    auto* feedbackParam = apvts.getRawParameterValue(PARAM_FEEDBACK);
    
    if (delayTimeParam && feedbackParam)
    {
        float delayMs = delayTimeParam->load();
        float feedback = feedbackParam->load();
        
        // Calcular tiempo de tail aproximado basado en feedback
        if (feedback > 0.01f)
        {
            float tailSeconds = (delayMs / 1000.0f) * (1.0f / (1.0f - feedback));
            return juce::jlimit(0.0, 10.0, static_cast<double>(tailSeconds));
        }
    }
    
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String DelayAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Preparar el procesador de delay
    delayProcessor.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    
    // Establecer valores iniciales de parámetros
    auto* delayTimeParam = apvts.getRawParameterValue(PARAM_DELAY_TIME);
    auto* feedbackParam = apvts.getRawParameterValue(PARAM_FEEDBACK);
    
    if (delayTimeParam)
        delayProcessor.setParameter(DelayProcessor::DelayTime, delayTimeParam->load());
    
    if (feedbackParam)
        delayProcessor.setParameter(DelayProcessor::Feedback, feedbackParam->load());
}

void DelayAudioProcessor::releaseResources()
{
    delayProcessor.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // ========================================================================
    // LEER PARÁMETROS Y ACTUALIZAR EL DELAY PROCESSOR
    // ========================================================================
    auto* delayTimeParam = apvts.getRawParameterValue(PARAM_DELAY_TIME);
    auto* feedbackParam = apvts.getRawParameterValue(PARAM_FEEDBACK);
    auto* wetDryParam = apvts.getRawParameterValue(PARAM_WET_DRY);
    auto* muteParam = apvts.getRawParameterValue(PARAM_MUTE);
    auto* bypassParam = apvts.getRawParameterValue(PARAM_BYPASS);
    
    if (delayTimeParam)
        delayProcessor.setParameter(DelayProcessor::DelayTime, delayTimeParam->load());
    
    if (feedbackParam)
        delayProcessor.setParameter(DelayProcessor::Feedback, feedbackParam->load());
    
    if (wetDryParam)
            delayProcessor.setWetDryMix(wetDryParam->load());
        
    if (muteParam)
        delayProcessor.setMuted(muteParam->load() >= 0.5f);
    
    if (bypassParam)
        delayProcessor.setBypassed(bypassParam->load() >= 0.5f);
    
    // ========================================================================
    // PROCESAR AUDIO (con mute, wet/dry y bypass integrados)
    // ========================================================================
    delayProcessor.process(buffer);
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}

