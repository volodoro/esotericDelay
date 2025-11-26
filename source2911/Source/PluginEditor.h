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
    DelayAudioProcessorEditor(DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==========================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    //==========================================================================
    // Componentes UI
    //==========================================================================
      
    // === NUEVOS LOGOS ===
    juce::ImageComponent omLogoImage;     // OM_logo (esquina superior izquierda)
    juce::ImageComponent omTituloImage;   // OM_titulo (reemplaza título)
    juce::ImageComponent omNombresImage;  // OM_nombres (reemplaza nombres)

    // === NUEVAS IMÁGENES PARA LOS SLIDERS ===
    juce::ImageComponent delayTimeLImage;
    juce::ImageComponent delayTimeRImage;
    juce::ImageComponent feedbackLImage;
    juce::ImageComponent feedbackRImage;
    juce::ImageComponent wetDryMixImage;

   

    // Título
    juce::Label titleLabel;

    // by
    juce::Label byLabel;

    // Delay Time L
    juce::Label delayTimeLabel_L;
    juce::Slider delayTimeSlider_L;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment_L;

    // Delay Time R
    juce::Label delayTimeLabel_R;
    juce::Slider delayTimeSlider_R;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment_R;

    // Feedback L
    juce::Label feedbackLabel_L;
    juce::Slider feedbackSlider_L;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment_L;

    // Feedback R
    juce::Label feedbackLabel_R;
    juce::Slider feedbackSlider_R;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment_R;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayAudioProcessorEditor)
};
