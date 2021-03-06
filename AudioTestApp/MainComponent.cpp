#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

class MainContentComponent   : public AudioAppComponent,
                               public Button::Listener 
{
public:
    //==============================================================================
    MainContentComponent()
    {
        sampleRate = 48000;
        outputSpeakers = 2;
        
		setSize(600, 450);
        setAudioChannels (1, outputSpeakers);
		deviceManager.setCurrentAudioDeviceType("ASIO", true);
		selector = new AudioDeviceSelectorComponent(deviceManager, 0, 0, 0, outputSpeakers, false, false, false, false);
		selector->setBounds(0, 200, getWidth(), getHeight());
		addAndMakeVisible(selector);
        
        levelSlider.setRange (0.0, 0.25);
        levelSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        levelLabel.setText ("Noise Level", dontSendNotification);
        
        addAndMakeVisible (levelSlider);
        addAndMakeVisible (levelLabel);
        
        XSlider.setRange (-5.0, 5.0);
        XSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        XLabel.setText ("X Position", dontSendNotification);
        
        addAndMakeVisible (XSlider);
        addAndMakeVisible (XLabel);
        
        YSlider.setRange (-5.0, 5.0);
        YSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        YLabel.setText ("Y Position", dontSendNotification);
        
        addAndMakeVisible (YSlider);
        addAndMakeVisible (YLabel);
        
        addAndMakeVisible (submitButton);
        submitButton.setButtonText ("Set Position");
        submitButton.addListener (this);
        
        chunkSize = 512;
        for(int j = 0; j < outputSpeakers; j++)
        {
            std::vector<float> temp;
            for(int i = 0; i < chunkSize; i++)
            {
                temp.push_back(0);
            }
            prevBuffer.push_back(temp);
        }
        
        delay.push_back(0);
        delay.push_back(479);
        
        for(int j = 0; j < outputSpeakers; j++)
        {
            gain.push_back(1);
        }
    }

    ~MainContentComponent()
    {
		shutdownAudio();
		delete selector;
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        AudioIODevice* device = deviceManager.getCurrentAudioDevice();
        const BigInteger activeInputChannels = device->getActiveInputChannels();
        const BigInteger activeOutputChannels = device->getActiveOutputChannels();
        const int maxInputChannels = activeInputChannels.getHighestBit() + 1;
        const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

        const float level = (float) levelSlider.getValue();
        
        for (int channel = 0; channel < outputSpeakers; channel++)
        {
            if ((! activeOutputChannels[channel]) || maxInputChannels == 0)
            {
                bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
            }
            else
            {
                const int actualInputChannel = channel % maxInputChannels; // [1]
                
                if (! activeInputChannels[channel]) // [2]
                {
                    bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
                }
                else // [3]
                {
                    const float* inBuffer = bufferToFill.buffer->getReadPointer (actualInputChannel,
                                                                                 bufferToFill.startSample);
                    float* outBuffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
                    
                    for (int sample = 0; sample < bufferToFill.numSamples; sample++)
                    {
						prevBuffer[channel][sample] = inBuffer[sample];
                        
                        if(sample - delay[channel] < 0)
                            outBuffer[sample] = prevBuffer[channel][(chunkSize)+(sample-delay[channel])] * gain[channel] * level;
                        else
                            outBuffer[sample] = prevBuffer[channel][sample - delay[channel]] * gain[channel] * level;
                    }
                }
            }
        }
    }

    void releaseResources() override
    {
    }
    
    void resized() override
    {
        levelLabel.setBounds (10, 10, 90, 20);
        levelSlider.setBounds (100, 10, getWidth() - 110, 20);
        
        XLabel.setBounds (10, 70, 90, 20);
        XSlider.setBounds (100, 70, getWidth() - 110, 20);
        
        YLabel.setBounds (10, 110, 90, 20);
        YSlider.setBounds (100, 110, getWidth() - 110, 20);
        
        submitButton.setBounds (10, 150, getWidth() - 20, 40);

		if (selector) {
			selector->setBounds(0, 200, getWidth(), getHeight());
		}
    }
    
    void buttonClicked (Button* button) override
    {
        const float xPos = (float) XSlider.getValue();
        const float yPos = (float) YSlider.getValue();
        
        /* Call Matlab WFS function, get new impulses */
        /*std::vector<std::vector<float> > ArraySignals;
        for(int i = 0; i < outputSpeakers; i++)
        {
			std::vector<float> temp;
            for(int j = 0; j < chunkSize; j++)
            {
				temp.push_back(0);
            }
			ArraySignals.push_back(temp);
        }
        
        *//* Get gains and delay *//*
        for(int i = 0; i < outputSpeakers; i++)
        {
            gain[i] = 1;
            delay[i] = 0;
            for(int j = 0; j < chunkSize; j++)
            {
                if(ArraySignals[i][j] != 0)
                {
                    gain[i] = ArraySignals[i][j];
                    delay[i] = j;
                    break;
                }
            }
        }*/
    }

private:
    Random random;
    Slider levelSlider;
    Label levelLabel;
    Slider XSlider;
    Label XLabel;
    Slider YSlider;
    Label YLabel;
    TextButton submitButton;
    
    int chunkSize;
    int outputSpeakers;
    int sampleRate;
    
	std::vector<std::vector<float> > prevBuffer;
    std::vector<int> delay;
    std::vector<int> gain;
    
    AudioDeviceSelectorComponent  * selector = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
