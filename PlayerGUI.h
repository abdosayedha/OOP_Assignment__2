#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void loadFile(const juce::File& file);
    void start();
    void stop();
    void restart();
    void toggleMute();
    void toggleLoop();
    void setVolume(float newVolume);
    void setSpeed(float newSpeed);
    bool isPlaying() const;
    bool isLooping() const;

    double getCurrentPosition() const;
    double getLengthInSeconds() const;
    void setPositionRelative(double pos);
    double getPositionRelative() const;

    double getPosition() const;
    double getLength() const;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;

    bool muted = false;
    bool looping = false;
    float volume = 1.0f;
};

