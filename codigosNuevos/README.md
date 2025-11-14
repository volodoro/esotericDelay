## Aquí podemos comentar avances de los códigos que vayamos generando

# Observaciones 14/11

No tenemos claro de qué manera separar los delay

Sospechamos que la solución puede ser convertir los parámetros de delay time y feedback en vectores de dos columnas para poder separarlos por canal.
¿En ese caso deberíamos inicializar las variables delayTimeMs y feedbackAmount como punteros en lugares como estos?



```cpp
    void processEffect(float* const* buffer, int numChannels, int numSamples) override
    {
        const float targetDelayMs = delayTimeMs.getTargetValue();
        const float delaySamples = (targetDelayMs / 1000.0f) * static_cast<float>(currentSampleRate);
        const int delayInSamples = juce::jlimit(1, maxDelayBufferSize - 1, static_cast<int>(delaySamples));
        
        for (int channel = 0; channel < juce::jmin(numChannels, static_cast<int>(delayBuffers.size())); ++channel)
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
                
                float delayedSample = delayBuffer[readPos];  // aquí hay que crear un buffer por cada canal? (Delay procesor line 104
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

private:
    std::vector<juce::AudioBuffer<float>> delayBuffers;
    std::vector<int> writePositions;
    
    juce::LinearSmoothedValue<float> delayTimeMs;
    juce::LinearSmoothedValue<float> feedbackAmount;
    
    int maxDelayBufferSize = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayProcessor)
};
```
^^^ DelayProcessor.h Línea 83
