// ========================================================================
// Created by Ricardo Saldias on 12-11-25.
// Ingenieria en Sonido
// Programacion de plugins
// Universidad de Chile
// ========================================================================

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor(DelayAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // ================== CARGA DE IMÁGENES ==================
    {
        // OM_logo (esquina superior izquierda)
        juce::Image logoImg = juce::ImageFileFormat::loadFrom(
            BinaryData::OM_logo_png,
            BinaryData::OM_logo_pngSize
        );
        omLogoImage.setImage(logoImg,
            juce::RectanglePlacement::centred
            | juce::RectanglePlacement::onlyReduceInSize);
        addAndMakeVisible(omLogoImage);

        // OM_titulo (reemplaza texto "ESOTERIC DELAY PLUGIN")
        juce::Image tituloImg = juce::ImageFileFormat::loadFrom(
            BinaryData::OM_titulo_png,
            BinaryData::OM_titulo_pngSize
        );
        omTituloImage.setImage(tituloImg,
            juce::RectanglePlacement::centred
            | juce::RectanglePlacement::onlyReduceInSize);
        addAndMakeVisible(omTituloImage);

        // OM_nombres (reemplaza texto de nombres)
        juce::Image nombresImg = juce::ImageFileFormat::loadFrom(
            BinaryData::OM_nombres_png,
            BinaryData::OM_nombres_pngSize
        );
        omNombresImage.setImage(nombresImg,
            juce::RectanglePlacement::centred
            | juce::RectanglePlacement::onlyReduceInSize);
        addAndMakeVisible(omNombresImage);
    }

    // Opcional: esconder los labels de texto antiguos (por si los dejas en el código)
    titleLabel.setVisible(false);
    byLabel.setVisible(false);
    // ================== LABELS / TEXTO ==================

    // Configurar título
    titleLabel.setText("ESOTERIC DELAY PLUGIN", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(22.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);

    // Configurar "by"
    byLabel.setText("Ignacio Marino - Nicolas Vasquez | OM Pluigns", juce::dontSendNotification);
    byLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    byLabel.setJustificationType(juce::Justification::centred);
    byLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(byLabel);

    // ========================================================================
    // Delay Time L
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
    // Delay Time R
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
    // Feedback L
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
    // Feedback R
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
    // Wet/Dry Mix
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
    // Botones Mute / Bypass
    // ========================================================================
    muteButton.setButtonText("MUTE");
    muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    muteButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(muteButton);

    bypassButton.setButtonText("BYPASS");
    bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::grey);
    bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(bypassButton);

    // ========================================================================
    // Attachments
    // ========================================================================
    auto& apvts = audioProcessor.getAPVTS();

    delayTimeAttachment_L = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
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

    // Tamaño de la ventana
    setSize(900, 600);
    setResizable(true, true);
    // minW, minH, maxW, maxH  (maxH ≥ 600 para que no se corte)
    setResizeLimits(600, 500, 900, 1100);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
// Fondo con gradiente
void DelayAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fondo con gradiente
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient gradient(
        juce::Colour(0xff3b3b3b), bounds.getTopLeft().toFloat(),
        juce::Colour(0xff5c3bc8), bounds.getBottomRight().toFloat(),
        false);

    g.setGradientFill(gradient);
    g.fillRect(bounds);

    // (Opcional) borde
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.drawRect(bounds, 1);
}
void DelayAudioProcessorEditor::resized()
{
    // Área total del editor con un pequeño margen
    auto bounds = getLocalBounds().reduced(10);

    // ================== LOGOTIPO ESQUINA IZQUIERDA (OM_logo) ==================
    // Controles PARA AJUSTAR TAMAÑO / POSICIÓN:
    int logoWidth = 300;   // <-- cambia ancho del logo
    int logoHeight = 200;   // <-- cambia alto del logo
    int logoX = 0;   // <-- distancia desde el borde izquierdo
    int logoY = 5;   // <-- distancia desde el borde superior

    omLogoImage.setBounds(logoX, logoY, logoWidth, logoHeight);

    // ================== TÍTULO (OM_titulo) ==================
    // Controles PARA AJUSTAR TAMAÑO / POSICIÓN:
    int tituloWidth = 450;  // ancho del PNG de título
    int tituloHeight = 70;   // alto del PNG de título
    int offsetX = 20;   //offset para ajustar la distancia respecto al margen derecho
    int tituloY = 30;   // posición vertical del título

    int tituloX = (getWidth() - (tituloWidth + offsetX)); // centrado horizontalmente
    omTituloImage.setBounds(tituloX, tituloY, tituloWidth, tituloHeight);

    // ================== NOMBRES (OM_nombres) ==================
    // Controles PARA AJUSTAR TAMAÑO / POSICIÓN:
    int nombresWidth = 320;   // ancho del PNG de nombres
    int nombresHeight = 30;    // alto del PNG de nombres
    int nombresY = tituloY + tituloHeight + 5; // justo bajo el título (+5 px de margen)

    int nombresX = (getWidth() - (nombresWidth + offsetX)) ; // centrado horizontalmente
    omNombresImage.setBounds(nombresX, nombresY, nombresWidth, nombresHeight);

    // ================== ESPACIO RESERVADO BAJO EL HEADER ==================
    // Calculamos cuánto espacio ocupa todo el header (logo, título, nombres)
    int headerBottom = juce::jmax(logoY + logoHeight,
        nombresY + nombresHeight);

    // A partir de esta Y arrancan los controles (botones + sliders)
    auto controlsBounds = getLocalBounds().reduced(10);
    controlsBounds.removeFromTop(headerBottom + 20); // +20 px de margen abajo del header

    // Ahora usamos controlsBounds como antes
    auto b = controlsBounds;

    // ================== CONTROLES ==================

    // Botones Mute y Bypass
    auto buttonsArea = b.removeFromTop(40);
    auto muteArea = buttonsArea.removeFromLeft(buttonsArea.getWidth() / 2 - 5);
    buttonsArea.removeFromLeft(10); // espaciado
    muteButton.setBounds(muteArea);
    bypassButton.setBounds(buttonsArea);

    b.removeFromTop(15);

    // Delay Time L
    auto delayLArea = b.removeFromTop(60);
    delayTimeLabel_L.setBounds(delayLArea.removeFromTop(25));
    delayLArea.removeFromTop(5);
    delayTimeSlider_L.setBounds(delayLArea);

    b.removeFromTop(15);

    // Delay Time R
    auto delayRArea = b.removeFromTop(60);
    delayTimeLabel_R.setBounds(delayRArea.removeFromTop(25));
    delayRArea.removeFromTop(5);
    delayTimeSlider_R.setBounds(delayRArea);

    b.removeFromTop(15);

    // Feedback L
    auto feedbackLArea = b.removeFromTop(60);
    feedbackLabel_L.setBounds(feedbackLArea.removeFromTop(25));
    feedbackLArea.removeFromTop(5);
    feedbackSlider_L.setBounds(feedbackLArea);

    b.removeFromTop(15);

    // Feedback R
    auto feedbackRArea = b.removeFromTop(60);
    feedbackLabel_R.setBounds(feedbackRArea.removeFromTop(25));
    feedbackRArea.removeFromTop(5);
    feedbackSlider_R.setBounds(feedbackRArea);

    b.removeFromTop(15);

    // Wet/Dry Mix
    auto wetDryArea = b.removeFromTop(60);
    wetDryLabel.setBounds(wetDryArea.removeFromTop(25));
    wetDryArea.removeFromTop(5);
    wetDrySlider.setBounds(wetDryArea);
}

