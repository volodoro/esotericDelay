

// ========================================================================
// Created by Ricardo Saldias on 12-11-25.
// Ingenieria en Sonido
// Programacion de plugins
// Universidad de Chile
// ========================================================================

#pragma once

#include <JuceHeader.h>
#include "CustomAudioProcess.h"
#include <vector>

/**
 * Procesador de Delay con realimentación
 * Ahora usa processEffect() en lugar de process()
 */
class DelayProcessor : public CustomAudioProcess
{
public:
    enum Parameters
    {
        DelayTime = 0,      // Tiempo de delay en milisegundos (0-2000ms)
        Feedback = 1,       // Realimentación en porcentaje (0-95%)
        NumParameters
    };
    
    DelayProcessor()
    {
        delayTimeMs.setCurrentAndTargetValue(500.0f);
        feedbackAmount.setCurrentAndTargetValue(0.5f);
    }
    
    ~DelayProcessor() override = default;
    
    // ========================================================================
    // Implementación de CustomAudioProcess
    // ========================================================================
    
    void prepare(double sampleRate, int samplesPerBlock, int numChannels) override
    {
        currentSampleRate = sampleRate;
        currentBlockSize = samplesPerBlock;
        currentNumChannels = numChannels;
        
        const int maxDelaySamples = static_cast<int>(sampleRate * 2.0);
        maxDelayBufferSize = maxDelaySamples;
        
        delayBuffers.clear(); // Limpia la variable.

        // El tamaño de vector se corresponde al número de canales. Ahora podemos hacer
        // delayBuffers[ch].
        delayBuffers.resize(numChannels);
        
        writePositions.clear();
        writePositions.resize(numChannels, 0);
        
        for (int ch = 0; ch < numChannels; ++ch)
        {
            // Inicializa los buffers de cada canal al tamaño máximo de samples
            // adelantados (maxDelaySamples).
            delayBuffers[ch].setSize(1, maxDelaySamples, false, true, false);
            delayBuffers[ch].clear();
        }
        
        delayTimeMs.reset(sampleRate, 0.05);
        feedbackAmount.reset(sampleRate, 0.05);
    }
    
    void reset() override
    {
        for (auto& buffer : delayBuffers)
        {
            buffer.clear();
        }
        
        for (auto& pos : writePositions)
        {
            pos = 0;
        }
    }
    
    // ========================================================================
    // LOOP PRINCIPAL DE PROCESAMIENTO (REAL-TIME SAFE)
    // ========================================================================
    void processEffect(float* const* buffer, int numChannels, int numSamples) override
    {
        const float targetDelayMs = delayTimeMs.getTargetValue();
        const float delaySamples = (targetDelayMs / 1000.0f) * static_cast<float>(currentSampleRate);
        const int delayInSamples = juce::jlimit(1, maxDelayBufferSize - 1, static_cast<int>(delaySamples));


        for (int channel = 0; channel < juce::jmin(numChannels, static_cast<int>(delayBuffers.size())); ++channel) // Descubrimos que esta función de juce 
//obtiene la variable del número de canales según el archivo ingresado, ya que funciona igual con archivos multicanal (probamos con un archivo de 4 y funciona)
        {
            auto* channelData = const_cast<float*>(buffer[channel]);
            auto* delayBuffer = delayBuffers[channel].getWritePointer(0);
            int writePos = writePositions[channel];
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                float currentFeedback = feedbackAmount.getNextValue();
                
                int readPos = writePos - delayInSamples;
                while (readPos < 0)
                    readPos += maxDelayBufferSize;
                readPos = readPos % maxDelayBufferSize;
                
                float delayedSample = delayBuffer[readPos]; //aquí hay que hacer algo??? crear un buffer por cada canal???
                float inputSample = channelData[sample];
                
                // Feedback
                float feedbackSample = inputSample + (delayedSample * currentFeedback);
                delayBuffer[writePos] = feedbackSample;
                
                // IMPORTANTE: Ahora solo retornamos la señal wet (delayed)
                // La mezcla wet/dry se hace automáticamente en la clase base
                channelData[sample] = delayedSample;
                
                writePos = (writePos + 1) % maxDelayBufferSize;
            }
            
            writePositions[channel] = writePos;
            
            if (channel == 0 && numChannels > 1)
            {
                feedbackAmount.skip(numSamples * (numChannels - 1));
            }
        }
    }
    
    // ========================================================================
    // Parámetros
    // ========================================================================
    
    void setParameter(int parameterIndex, float value) override
    {
        switch (parameterIndex)
        {
            case DelayTime:
                delayTimeMs.setTargetValue(juce::jlimit(0.0f, 2000.0f, value));
                break;
                
            case Feedback:
                feedbackAmount.setTargetValue(juce::jlimit(0.0f, 0.95f, value));
                break;
                
            default:
                break;
        }
    }
    
    float getParameter(int parameterIndex) const override
    {
        switch (parameterIndex)
        {
            case DelayTime:
                return delayTimeMs.getTargetValue();
                
            case Feedback:
                return feedbackAmount.getTargetValue();
                
            default:
                return 0.0f;
        }
    }
    
    juce::String getName() const override { return "Delay with Feedback"; }
    int getNumParameters() const override { return NumParameters; }
    
    juce::String getParameterName(int index) const override
    {
        switch (index)
        {
            case DelayTime: return "Delay Time";
            case Feedback: return "Feedback";
            default: return "Unknown";
        }
    }

    // Declaración de variables: búfer y posiciones de escritura.
    std::vector<juce::AudioBuffer<float>> delayBuffers;
    std::vector<int> writePositions;
    
    juce::LinearSmoothedValue<float> delayTimeMs;
    juce::LinearSmoothedValue<float> feedbackAmount;
    
    int maxDelayBufferSize = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayProcessor)
};


