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
    titleLabel.setText("CUSTOM DELAY PLUGIN", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(22.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    // Configurar by
    byLabel.setText("Prof. Ricardo Saldias | DSON - Universidad de Chile", juce::dontSendNotification);
    byLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    byLabel.setJustificationType(juce::Justification::centred);
    byLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(byLabel);
    
    // ========================================================================
    // Configurar Delay Time Slider
    // ========================================================================
    delayTimeLabel.setText("Delay Time (ms)", juce::dontSendNotification);
    delayTimeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    delayTimeLabel.setJustificationType(juce::Justification::centredLeft);
    delayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(delayTimeLabel);
    
    delayTimeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    delayTimeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    delayTimeSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    delayTimeSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    delayTimeSlider.setTextValueSuffix(" ms");
    addAndMakeVisible(delayTimeSlider);
    
    // ========================================================================
    // Configurar Feedback Slider
    // ========================================================================
    feedbackLabel.setText("Feedback (%)", juce::dontSendNotification);
    feedbackLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    feedbackLabel.setJustificationType(juce::Justification::centredLeft);
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(feedbackLabel);
    
    feedbackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    feedbackSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    feedbackSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    feedbackSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(feedbackSlider);
    
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
    
    delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "delayTime", delayTimeSlider);
    
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "feedback", feedbackSlider);
    
    wetDryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "wetDry", wetDrySlider);
    
    muteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "mute", muteButton);
    
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "bypass", bypassButton);
    
    // Tamaño de la ventana (aumentado para los nuevos controles)
    setSize (500, 350);
    setResizable(true, true);
    setResizeLimits(400, 300, 800, 500);

}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fondo con gradiente
    auto bounds = getLocalBounds();
    
    juce::ColourGradient gradient(
        juce::Colour(0xff1a1a2e), bounds.getTopLeft().toFloat(),
        juce::Colour(0xff16213e), bounds.getBottomRight().toFloat(),
        false);
    
    g.setGradientFill(gradient);
    g.fillRect(bounds);
    
    // Borde
    g.setColour(juce::Colours::cyan.withAlpha(0.5f));
    g.drawRect(bounds, 2);
}

void DelayAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
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
    
    // Delay Time
    auto delayArea = bounds.removeFromTop(60);
    delayTimeLabel.setBounds(delayArea.removeFromTop(25));
    delayArea.removeFromTop(5);
    delayTimeSlider.setBounds(delayArea);
    
    bounds.removeFromTop(15);
    
    // Feedback
    auto feedbackArea = bounds.removeFromTop(60);
    feedbackLabel.setBounds(feedbackArea.removeFromTop(25));
    feedbackArea.removeFromTop(5);
    feedbackSlider.setBounds(feedbackArea);
    
    bounds.removeFromTop(15);
    
    // Wet/Dry Mix
    auto wetDryArea = bounds.removeFromTop(60);
    wetDryLabel.setBounds(wetDryArea.removeFromTop(25));
    wetDryArea.removeFromTop(5);
    wetDrySlider.setBounds(wetDryArea);
}


