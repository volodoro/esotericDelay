

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
		DelayTimeL = 0,      // Tiempo de delay en milisegundos (0-2000ms)
		FeedbackL = 1,       // Realimentación en porcentaje (0-95%)
		DelayTimeR = 2,      // Tiempo de delay en milisegundos (0-2000ms)
		FeedbackR = 3,       // Realimentación en porcentaje (0-95%)
		NumParameters
	};

	DelayProcessor()
	{
		delayTimeMsL.setCurrentAndTargetValue(500.0f);
		feedbackAmountL.setCurrentAndTargetValue(0.5f);
		delayTimeMsR.setCurrentAndTargetValue(500.0f);
		feedbackAmountR.setCurrentAndTargetValue(0.5f);
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

		delayBuffers.clear();
		delayBuffers.resize(numChannels);

		writePositions.clear();
		writePositions.resize(numChannels, 0);

		for (int ch = 0; ch < numChannels; ++ch)
		{ // Creación de buffer
			delayBuffers[ch].setSize(1, maxDelaySamples, false, true, false);
			delayBuffers[ch].clear();
		}

		delayTimeMsL.reset(sampleRate, 0.05);
		feedbackAmountL.reset(sampleRate, 0.05);

		delayTimeMsR.reset(sampleRate, 0.05);
		feedbackAmountR.reset(sampleRate, 0.05);
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
		const float targetDelayMsL = delayTimeMsL.getTargetValue();
		const float delaySamplesL = (targetDelayMsL / 1000.0f) * static_cast<float>(currentSampleRate);
		const int delayInSamplesL = juce::jlimit(1, maxDelayBufferSize - 1, static_cast<int>(delaySamplesL));

		const float targetDelayMsR = delayTimeMsR.getTargetValue();
		const float delaySamplesR = (targetDelayMsR / 1000.0f) * static_cast<float>(currentSampleRate);
		const int delayInSamplesR = juce::jlimit(1, maxDelayBufferSize - 1, static_cast<int>(delaySamplesR));

		for (int channel = 0; channel < juce::jmin(numChannels, static_cast<int>(delayBuffers.size())); ++channel)
		{
			auto* channelData = const_cast<float*>(buffer[channel]);


			if (channel == 0) {
				// canal 0 o L

				auto* delayBufferL = delayBuffers[channel].getWritePointer(0);
				int writePosL = writePositions[channel];

				for (int sample = 0; sample < numSamples; ++sample)
				{
					float currentFeedbackL = feedbackAmountL.getNextValue();

					int readPosL = writePosL - delayInSamplesL;
					while (readPosL < 0)
						readPosL += maxDelayBufferSize;
					readPosL = readPosL % maxDelayBufferSize;

					float delayedSampleL = delayBufferL[readPosL];  // aquí hay que crear un buffer por cada canal? (Delay procesor line 104
					float inputSampleL = channelData[sample];

					// Feedback
					float feedbackSampleL = inputSampleL + (delayedSampleL * currentFeedbackL);
					delayBufferL[writePosL] = feedbackSampleL;

					// IMPORTANTE: Ahora solo retornamos la señal wet (delayed)
					// La mezcla wet/dry se hace automáticamente en la clase base
					channelData[sample] = delayedSampleL;

					writePosL = (writePosL + 1) % maxDelayBufferSize;
				}

				writePositions[channel] = writePosL;

				if (channel == 0 && numChannels > 1)
				{
					feedbackAmountL.skip(numSamples * (numChannels - 1));

				}
			}
			

			
			if (channel == 1) {
				// canal 1 o R

				auto* delayBufferR = delayBuffers[channel].getWritePointer(0);
				int writePosR = writePositions[channel];

				for (int sample = 0; sample < numSamples; ++sample)
				{
					float currentFeedbackR = feedbackAmountR.getNextValue();

					int readPosR = writePosR - delayInSamplesR;
					while (readPosR < 0)
						readPosR += maxDelayBufferSize;
					readPosR = readPosR % maxDelayBufferSize;

					float delayedSampleR = delayBufferR[readPosR];  // aquí hay que crear un buffer por cada canal? (Delay procesor line 104
					float inputSampleR = channelData[sample];

					// Feedback
					float feedbackSampleR = inputSampleR + (delayedSampleR * currentFeedbackR);
					delayBufferR[writePosR] = feedbackSampleR;

					// IMPORTANTE: Ahora solo retornamos la señal wet (delayed)
					// La mezcla wet/dry se hace automáticamente en la clase base
					channelData[sample] = delayedSampleR;

					writePosR = (writePosR + 1) % maxDelayBufferSize;
				}

				writePositions[channel] = writePosR;

				if (channel == 1 && numChannels > 1)
				{
					feedbackAmountR.skip(numSamples * (numChannels - 1));

				}
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
		case DelayTimeL:
			delayTimeMsL.setTargetValue(juce::jlimit(0.0f, 2000.0f, value));
			break;

		case FeedbackL:
			feedbackAmountL.setTargetValue(juce::jlimit(0.0f, 0.95f, value));
			break;
		case DelayTimeR:
			delayTimeMsR.setTargetValue(juce::jlimit(0.0f, 2000.0f, value));
			break;

		case FeedbackR:
			feedbackAmountR.setTargetValue(juce::jlimit(0.0f, 0.95f, value));
			break;

		default:
			break;
		}
	}

	float getParameter(int parameterIndex) const override
	{
		switch (parameterIndex)
		{
		case DelayTimeL:
			return delayTimeMsL.getTargetValue();

		case FeedbackL:
			return feedbackAmountL.getTargetValue();

		case DelayTimeR:
			return delayTimeMsR.getTargetValue();

		case FeedbackR:
			return feedbackAmountR.getTargetValue();

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
		case DelayTimeL: return "Delay Time L";
		case FeedbackL: return "Feedback L";
		case DelayTimeR: return "Delay Time R";
		case FeedbackR: return "Feedback R";
		default: return "Unknown";
		}
	}

private:
	std::vector<juce::AudioBuffer<float>> delayBuffers;
	std::vector<int> writePositions;



	juce::LinearSmoothedValue<float> delayTimeMsL;
	juce::LinearSmoothedValue<float> feedbackAmountL;
	juce::LinearSmoothedValue<float> delayTimeMsR;
	juce::LinearSmoothedValue<float> feedbackAmountR;

	int maxDelayBufferSize = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayProcessor)
};

