// ========================================================================
// Created by Ricardo Saldias on 12-11-25.
// Ingenieria en Sonido
// Programacion de plugins
// Universidad de Chile
// ========================================================================


#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class DelayAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==========================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==========================================================================
    // Componentes UI
    //==========================================================================
    
    // Título
    juce::Label titleLabel;
    
    // by
    juce::Label byLabel;
    
    // Delay Time L
    juce::Label delayTimeLLabel;
    juce::Slider delayTimeLSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeLAttachment;

    // Delay Time R
    juce::Label delayTimeRLabel;
    juce::Slider delayTimeRSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeRAttachment;
    
    // Feedback L
    juce::Label feedbackLLabel;
    juce::Slider feedbackLSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackLAttachment;

    // Feedback R
    juce::Label feedbackRLabel;
    juce::Slider feedbackRSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackRAttachment;
    
    // Wet/Dry Mix
    juce::Label wetDryLabel;
    juce::Slider wetDrySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetDryAttachment;
    
    // Mute Button
//    juce::TextButton muteButton;
    juce::ToggleButton muteButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteAttachment;
    
    // Bypass Button
    juce::ToggleButton bypassButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    
    //==========================================================================
    DelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};

