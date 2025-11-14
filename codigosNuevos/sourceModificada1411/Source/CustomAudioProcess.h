// ========================================================================
// Created by Ricardo Saldias on 12-11-25.
// Ingenieria en Sonido
// Programacion de plugins
// Universidad de Chile
// ========================================================================

#pragma once

#include <JuceHeader.h>

/**
 * Clase base abstracta para procesamiento de audio personalizado
 * Incluye controles comunes: Mute, Wet/Dry Mix, Bypass
 */
class CustomAudioProcess //clase de la que pueda heredar y concentrarme en cosas puntuales. De aquí hereda delay processor
{
public:
    CustomAudioProcess() = default;
    virtual ~CustomAudioProcess() = default;
    
    /**
     * Preparar el procesador para reproducción
     * @param sampleRate Frecuencia de muestreo
     * @param samplesPerBlock Tamaño máximo del buffer
     * @param numChannels Número de canales
     */
    virtual void prepare(double sampleRate, int samplesPerBlock, int numChannels) = 0;
    
    /**
     * Resetear el estado del procesador (limpiar buffers, etc.)
     */
    virtual void reset() = 0;
    
    /**
     * LOOP PRINCIPAL DE PROCESAMIENTO (REAL-TIME SAFE)
     * Esta es la función virtual pura que debe implementarse
     *
     * IMPORTANTE: Esta función debe procesar el audio "wet" (procesado)
     * La mezcla wet/dry se hace automáticamente después
     *
     * @param buffer Array de punteros a los canales de audio
     * @param numChannels Número de canales en el buffer
     * @param numSamples Número de samples en el buffer
     */
    virtual void processEffect(float* const* buffer, int numChannels, int numSamples) = 0; //aquí es donde ocurre el procesamiento 
    
    /**
     * Función de procesamiento completa con controles (mute, wet/dry)
     * Esta función NO debe ser sobrescrita, sobrescribe processEffect() en su lugar
     */
    void process(juce::AudioBuffer<float>& buffer)
    {
        // Si está en bypass, no hacer nada
        if (isBypassed.load(std::memory_order_acquire))
            return;
        
        // Si está muteado, silenciar
        if (isMuted.load(std::memory_order_acquire))
        {
            buffer.clear();
            return;
        }
        
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();
        
        // Obtener nivel de wet/dry
        float wetLevel = wetDryMix.load(std::memory_order_acquire);
        float dryLevel = 1.0f - wetLevel;
        
        // Si es 100% dry, no procesar
        if (wetLevel < 0.001f)
            return;
        
        // Si es 100% wet, solo procesar sin mezclar
        if (wetLevel > 0.999f)
        {
            processEffect(buffer.getArrayOfWritePointers(), numChannels, numSamples);
            return;
        }
        
        // ====================================================================
        // Mezcla Wet/Dry: Guardar señal original, procesar, y mezclar
        // ====================================================================
        
        // Asegurar que el buffer temporal tiene el tamaño correcto
        if (dryBuffer.getNumChannels() != numChannels ||
            dryBuffer.getNumSamples() < numSamples)
        {
            dryBuffer.setSize(numChannels, numSamples, false, false, true);
        }
        
        // Copiar señal original (dry)
        for (int ch = 0; ch < numChannels; ++ch)
        {
            dryBuffer.copyFrom(ch, 0, buffer, ch, 0, numSamples);
        }
        
        // Procesar efecto (wet)
        processEffect(buffer.getArrayOfWritePointers(), numChannels, numSamples);
        
        // Mezclar dry y wet
        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* wetData = buffer.getWritePointer(ch);
            const auto* dryData = dryBuffer.getReadPointer(ch);
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                wetData[sample] = (dryData[sample] * dryLevel) + (wetData[sample] * wetLevel);
            }
        }
    }
    
    // ========================================================================
    // Control de parámetros específicos del efecto
    // ========================================================================
    
    /**
     * Establecer parámetros específicos del efecto
     */
    virtual void setParameter(int parameterIndex, float value) = 0;
    
    /**
     * Obtener valor de parámetro específico del efecto
     */
    virtual float getParameter(int parameterIndex) const = 0;
    
    /**
     * Obtener información del procesador
     */
    virtual juce::String getName() const = 0;
    virtual int getNumParameters() const = 0;
    virtual juce::String getParameterName(int index) const = 0;
    
    // ========================================================================
    // Controles comunes (Mute, Wet/Dry, Bypass)
    // ========================================================================
    
    /**
     * Establecer mute (silenciar salida)
     */
    void setMuted(bool shouldBeMuted)
    {
        isMuted.store(shouldBeMuted, std::memory_order_release);
    }
    
    bool getMuted() const
    {
        return isMuted.load(std::memory_order_acquire);
    }
    
    /**
     * Establecer bypass (pasar señal sin procesar)
     */
    void setBypassed(bool shouldBeBypassed)
    {
        isBypassed.store(shouldBeBypassed, std::memory_order_release);
    }
    
    bool getBypassed() const
    {
        return isBypassed.load(std::memory_order_acquire);
    }
    
    /**
     * Establecer mezcla Wet/Dry
     * @param mix Valor entre 0.0 (100% dry) y 1.0 (100% wet)
     */
    void setWetDryMix(float mix)
    {
        wetDryMix.store(juce::jlimit(0.0f, 1.0f, mix), std::memory_order_release);
    }
    
    float getWetDryMix() const
    {
        return wetDryMix.load(std::memory_order_acquire);
    }
    
    /**
     * Obtener porcentaje de wet (0-100%)
     */
    float getWetPercentage() const
    {
        return getWetDryMix() * 100.0f;
    }
    
    /**
     * Obtener porcentaje de dry (0-100%)
     */
    float getDryPercentage() const
    {
        return (1.0f - getWetDryMix()) * 100.0f;
    }

protected:
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
    int currentNumChannels = 2;

private:
    // ========================================================================
    // Estado interno de controles comunes
    // =============FUNCIONES HEREDADAS A DELAY PROCESSOR==================
    
    std::atomic<bool> isMuted { false };
    std::atomic<bool> isBypassed { false };
    std::atomic<float> wetDryMix { 1.0f };  // Default: 100% wet
    
    // Buffer para almacenar señal dry durante el procesamiento
    juce::AudioBuffer<float> dryBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomAudioProcess)
};

