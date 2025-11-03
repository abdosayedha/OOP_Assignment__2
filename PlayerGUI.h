#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI(); // constructor عادي
    PlayerGUI(PlayerAudio& externalPlayer); // constructor بياخد reference لو حبيت تستخدم player خارجي

    void paint(juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

private:
    PlayerAudio localPlayer;       // لو مش هتستخدم external
    PlayerAudio* playerRef = nullptr; // pointer عشان تقدر تستخدم إما local أو external

    // Buttons
    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton loopButton{ "Loop" };

    // Sliders
    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    // Labels
    juce::Label volumeLabel{ "Volume", "Volume" };
    juce::Label speedLabel{ "Speed", "Speed" };
    juce::Label positionLabel{ "Position", "Position" };
    juce::Label timeLabel;

    void updateTimeLabel();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
