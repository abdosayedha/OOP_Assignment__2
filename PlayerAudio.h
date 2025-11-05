#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void loadAudioFile(const juce::File& file);
    void play();
    void stop();
    void restart();
    void toggleLoop();

    void setGain(float gain);
    void setSpeed(double ratio);
    void setPositionRelative(double pos);
    void setPositionSeconds(double seconds);

    float getPositionRelative() const;
    double getCurrentTime() const;
    double getTotalTime() const;

    void setAB_A();
    void setAB_B();
    void enableABLoop();
    void clearABLoop();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resampleSource;

    bool abLoopEnabled = false;
    double pointA = -1.0;
    double pointB = -1.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
