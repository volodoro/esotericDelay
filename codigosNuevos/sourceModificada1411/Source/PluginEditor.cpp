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
    // Configurar Delay Time L Slider
    // ========================================================================
    delayTimeLLabel.setText("Delay Time L (ms)", juce::dontSendNotification);
    delayTimeLLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    delayTimeLLabel.setJustificationType(juce::Justification::centredLeft);
    delayTimeLLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(delayTimeLLabel);
    
    delayTimeLSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeLSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    delayTimeLSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    delayTimeLSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    delayTimeLSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    delayTimeLSlider.setTextValueSuffix(" ms");
    addAndMakeVisible(delayTimeLSlider);
    
    // ========================================================================
// Configurar Delay Time R Slider
// ========================================================================
    delayTimeRLabel.setText("Delay Time R (ms)", juce::dontSendNotification);
    delayTimeRLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    delayTimeRLabel.setJustificationType(juce::Justification::centredLeft);
    delayTimeRLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(delayTimeRLabel);

    delayTimeRSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeRSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    delayTimeRSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    delayTimeRSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    delayTimeRSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    delayTimeRSlider.setTextValueSuffix(" ms");
    addAndMakeVisible(delayTimeRSlider);

    // ========================================================================
    // Configurar Feedback L Slider
    // ========================================================================
    feedbackLLabel.setText("Feedback L (%)", juce::dontSendNotification);
    feedbackLLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    feedbackLLabel.setJustificationType(juce::Justification::centredLeft);
    feedbackLLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(feedbackLLabel);
    
    feedbackLSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackLSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    feedbackLSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    feedbackLSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    feedbackLSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(feedbackLSlider);
    
// ========================================================================
// Configurar Feedback R Slider
// ========================================================================
    feedbackRLabel.setText("Feedback R (%)", juce::dontSendNotification);
    feedbackRLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    feedbackRLabel.setJustificationType(juce::Justification::centredLeft);
    feedbackRLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(feedbackRLabel);

    feedbackRSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackRSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
    feedbackRSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    feedbackRSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue);
    feedbackRSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(feedbackRSlider);

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
    
    delayTimeLAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>( //"delayTime" es un texto alternativo?
        apvts, "delayTimeL", delayTimeLSlider);
    
    delayTimeRAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "delayTimeR", delayTimeRSlider);

    feedbackLAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "feedbackL", feedbackLSlider);

    feedbackRAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "feedbackR", feedbackRSlider);
    
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
    
    // Delay Time L
    auto delayLArea = bounds.removeFromTop(60);
    delayTimeLLabel.setBounds(delayLArea.removeFromTop(25));
    delayLArea.removeFromTop(5);
    delayTimeLSlider.setBounds(delayLArea);
    
    bounds.removeFromTop(15);

    // Delay Time R
    auto delayRArea = bounds.removeFromTop(60);
    delayTimeRLabel.setBounds(delayRArea.removeFromTop(25));
    delayRArea.removeFromTop(5);
    delayTimeRSlider.setBounds(delayRArea);

    bounds.removeFromTop(15);
    
    // Feedback L
    auto feedbackLArea = bounds.removeFromTop(60);
    feedbackLLabel.setBounds(feedbackLArea.removeFromTop(25));
    feedbackLArea.removeFromTop(5);
    feedbackLSlider.setBounds(feedbackLArea);
    
    bounds.removeFromTop(15);

    // Feedback R
    auto feedbackRArea = bounds.removeFromTop(60);
    feedbackRLabel.setBounds(feedbackRArea.removeFromTop(25));
    feedbackRArea.removeFromTop(5);
    feedbackRSlider.setBounds(feedbackRArea);

    bounds.removeFromTop(15);
    
    // Wet/Dry Mix
    auto wetDryArea = bounds.removeFromTop(60);
    wetDryLabel.setBounds(wetDryArea.removeFromTop(25));
    wetDryArea.removeFromTop(5);
    wetDrySlider.setBounds(wetDryArea);
}


