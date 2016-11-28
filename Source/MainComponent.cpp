#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public AudioAppComponent,
                               public Button::Listener 
{
public:
    //==============================================================================
    MainContentComponent() : fftObj(5,false)
    {
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
        
        setSize (600, 200);
        setAudioChannels (2, 62);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double /*sampleRate*/) override
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        std::cout << bufferToFill.numSamples << std::endl;
        AudioIODevice* device = deviceManager.getCurrentAudioDevice();
        const BigInteger activeInputChannels = device->getActiveInputChannels();
        const BigInteger activeOutputChannels = device->getActiveOutputChannels();
        const int maxInputChannels = activeInputChannels.getHighestBit() + 1;
        const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

        const float level = (float) levelSlider.getValue();
        
        for (int channel = 0; channel < maxOutputChannels; ++channel)
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
                    
                    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
                        outBuffer[sample] = inBuffer[sample] * level;// * random.nextFloat();
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
    }
    
    void buttonClicked (Button* button) override
    {
        const float xPos = (float) XSlider.getValue();
        const float yPos = (float) YSlider.getValue();
        
        /* Call Matlab WFS function, get new impulses */
    }

private:
    Random random;
    Slider levelSlider;
    Label levelLabel;
    Slider XSlider;
    Label XLabel;
    Slider YSlider;
    Label YLabel;
    FFT fftObj;
    TextButton submitButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
