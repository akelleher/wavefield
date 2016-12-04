#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public AudioAppComponent
{
public:
    MainContentComponent()
    {
        levelSlider.setRange (0.0, 0.25);
        levelSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        levelLabel.setText ("Noise Level", dontSendNotification);

		speakerSelectorSlider.setRange(0, 61);
		speakerSelectorSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
		speakerSelectorLabel.setText("Speaker Number", dontSendNotification);

        
        addAndMakeVisible (levelSlider);
        addAndMakeVisible (levelLabel);
		addAndMakeVisible(speakerSelectorSlider);
		addAndMakeVisible(speakerSelectorLabel);


        
        setSize (600, 340);
        setAudioChannels (0, 62);
		deviceManager.setCurrentAudioDeviceType("ASIO", true);
		selector = new AudioDeviceSelectorComponent(deviceManager, 0, 0, 0, 62, false, false, false, false);
		selector->setBounds(0, 50, getWidth(), getHeight());
		addAndMakeVisible(selector);

    }

    ~MainContentComponent()
    {
        shutdownAudio();
		delete selector;
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double /*sampleRate*/) override
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        const float level = (float) levelSlider.getValue();
		int selectedChannel = round(speakerSelectorSlider.getValue());
        
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
			if (channel == selectedChannel) {
				float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

				for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
				{
					const float noise = random.nextFloat() * 2.0f - 1.0f;
					buffer[sample] = noise * level;
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
		speakerSelectorLabel.setBounds(10, 30, 90, 20);
		speakerSelectorSlider.setBounds(100, 30, getWidth() - 110, 20);

		if (selector) {
			selector->setBounds(0, 50, getWidth(), getHeight());
		}
    }
    

private:
    Random random;
    Slider levelSlider;
    Label levelLabel;

	Slider speakerSelectorSlider;
	Label speakerSelectorLabel;

	AudioDeviceSelectorComponent  * selector = nullptr;
		;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
