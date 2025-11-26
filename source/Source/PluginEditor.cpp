// ========================================================================
// Created by Ricardo Saldias on 12-11-25.
// Ingenieria en Sonido
// Programacion de plugins
// Universidad de Chile
// ========================================================================


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configurar título
    titleLabel.setText("ESOTERIC DELAY PLUGIN", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(22.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    // Configurar by
    byLabel.setText("Ignacio Marino - Nicolas Vasquez | OM Plugins", juce::dontSendNotification);
    byLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    byLabel.setJustificationType(juce::Justification::centred);
    byLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(byLabel);
    
    // ========================================================================
    // Configurar Delay Time L Slider
    // ========================================================================
    delayTimeLabel_L.setText("Delay Time L (ms)", juce::dontSendNotification);
    delayTimeLabel_L.setFont(juce::Font(16.0f, juce::Font::bold));
    delayTimeLabel_L.setJustificationType(juce::Justification::centredLeft);
    delayTimeLabel_L.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(delayTimeLabel_L);
    
    delayTimeSlider_L.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeSlider_L.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    delayTimeSlider_L.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    delayTimeSlider_L.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    delayTimeSlider_L.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    delayTimeSlider_L.setTextValueSuffix(" ms");
    addAndMakeVisible(delayTimeSlider_L);
    
    // ========================================================================
// Configurar Delay Time R Slider
// ========================================================================
    delayTimeLabel_R.setText("Delay Time R (ms)", juce::dontSendNotification);
    delayTimeLabel_R.setFont(juce::Font(16.0f, juce::Font::bold));
    delayTimeLabel_R.setJustificationType(juce::Justification::centredLeft);
    delayTimeLabel_R.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(delayTimeLabel_R);

    delayTimeSlider_R.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeSlider_R.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    delayTimeSlider_R.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    delayTimeSlider_R.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    delayTimeSlider_R.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    delayTimeSlider_R.setTextValueSuffix(" ms");
    addAndMakeVisible(delayTimeSlider_R);

    // ========================================================================
    // Configurar Feedback L Slider
    // ========================================================================
    feedbackLabel_L.setText("Feedback L (%)", juce::dontSendNotification);
    feedbackLabel_L.setFont(juce::Font(16.0f, juce::Font::bold));
    feedbackLabel_L.setJustificationType(juce::Justification::centredLeft);
    feedbackLabel_L.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(feedbackLabel_L);
    
    feedbackSlider_L.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackSlider_L.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    feedbackSlider_L.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    feedbackSlider_L.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    feedbackSlider_L.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(feedbackSlider_L);
    
// ========================================================================
// Configurar Feedback R Slider
// ========================================================================
    feedbackLabel_R.setText("Feedback R (%)", juce::dontSendNotification);
    feedbackLabel_R.setFont(juce::Font(16.0f, juce::Font::bold));
    feedbackLabel_R.setJustificationType(juce::Justification::centredLeft);
    feedbackLabel_R.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(feedbackLabel_R);

    feedbackSlider_R.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackSlider_R.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    feedbackSlider_R.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    feedbackSlider_R.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    feedbackSlider_R.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(feedbackSlider_R);

    // ========================================================================
    // Configurar Wet/Dry Slider
    // ========================================================================
    wetDryLabel.setText("Dry/Wet Mix", juce::dontSendNotification);
    wetDryLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    wetDryLabel.setJustificationType(juce::Justification::centredLeft);
    wetDryLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(wetDryLabel);
    
    wetDrySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    wetDrySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    wetDrySlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    wetDrySlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    wetDrySlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(wetDrySlider);
    
    // ========================================================================
    // Configurar Mute Button
    // ========================================================================
    muteButton.setButtonText("MUTE");
    muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    muteButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(muteButton);
    
    // ========================================================================
    // Configurar Bypass Button
    // ========================================================================
    bypassButton.setButtonText("BYPASS");
    bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::grey);
    bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(bypassButton);
    
    // ========================================================================
    // Crear attachments
    // ========================================================================
    auto& apvts = audioProcessor.getAPVTS();
    
    delayTimeAttachment_L = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>( //"delayTime" es un texto alternativo?
        apvts, "delayTimeL", delayTimeSlider_L);
    
    delayTimeAttachment_R = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "delayTimeR", delayTimeSlider_R);

    feedbackAttachment_L = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "feedbackL", feedbackSlider_L);

    feedbackAttachment_R = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "feedbackR", feedbackSlider_R);
    
    wetDryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "wetDry", wetDrySlider);
    
    muteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "mute", muteButton);
    
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "bypass", bypassButton);
    
    // Tamaño de la ventana (aumentado para los nuevos controles)
    setSize (500, 600);
    setResizable(true, true);
    setResizeLimits(400, 300, 800, 500);

}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Logo
    auto logoImage = juce::ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);

    if (!logoImage.isNull())
      logoComponent.setImage (logoImage, juce::RectanglePlacement::stretchToFit);
    else 
      jassert(!logoImage.isNull());

    addAndMakeVisible(logoComponent);

    // Gradiente
    juce::ColourGradient gradient(
        juce::Colour(0xff3b3b3b), bounds.getTopLeft().toFloat(),
        juce::Colour(0xff5c3bc8), bounds.getBottomRight().toFloat(),
        false);

    g.setGradientFill(gradient);
    g.fillRect(bounds);

    // (Opcional) borde
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.drawRect(bounds, 1);}

}

void DelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Logo
    logoComponent.setBounds(0, 0, 90, 75);

    // Título
    auto titleArea = bounds.removeFromTop(40);
    titleLabel.setBounds(titleArea);
    
    // by
    auto byArea = bounds.removeFromTop(10);
    byLabel.setBounds(byArea);
    
    bounds.removeFromTop(10);
    
    // Botones Mute y Bypass (lado a lado en la parte superior)
    auto buttonsArea = bounds.removeFromTop(40);
    auto muteArea = buttonsArea.removeFromLeft(buttonsArea.getWidth() / 2 - 5);
    buttonsArea.removeFromLeft(10); // Espaciado
    muteButton.setBounds(muteArea);
    bypassButton.setBounds(buttonsArea);
    
    bounds.removeFromTop(15);
    
    // Delay Time L
    auto delayLArea = bounds.removeFromTop(60);
    delayTimeLabel_L.setBounds(delayLArea.removeFromTop(25));
    delayLArea.removeFromTop(5);
    delayTimeSlider_L.setBounds(delayLArea);
    
    bounds.removeFromTop(15);

   //  Delay Time R

    auto delayRArea = bounds.removeFromTop(60);
    delayTimeLabel_R.setBounds(delayRArea.removeFromTop(25));
    delayRArea.removeFromTop(5);
    delayTimeSlider_R.setBounds(delayRArea);

    bounds.removeFromTop(15);
    
    // Feedback L
    auto feedbackLArea = bounds.removeFromTop(60);
    feedbackLabel_L.setBounds(feedbackLArea.removeFromTop(25));
    feedbackLArea.removeFromTop(5);
    feedbackSlider_L.setBounds(feedbackLArea);
    
    bounds.removeFromTop(15);

    //// Feedback R
    auto feedbackRArea = bounds.removeFromTop(60);
    feedbackLabel_R.setBounds(feedbackRArea.removeFromTop(25));
    feedbackRArea.removeFromTop(5);
    feedbackSlider_R.setBounds(feedbackRArea);

    bounds.removeFromTop(15);
    
    // Wet/Dry Mix
    auto wetDryArea = bounds.removeFromTop(60);
    wetDryLabel.setBounds(wetDryArea.removeFromTop(25));
    wetDryArea.removeFromTop(5);
    wetDrySlider.setBounds(wetDryArea);
}



