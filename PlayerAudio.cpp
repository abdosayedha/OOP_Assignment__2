#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio()
{
    releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::releaseResources()
{
    resamplingSource->releaseResources();
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resamplingSource->getNextAudioBlock(bufferToFill);
}

void PlayerAudio::loadFile(const juce::File& file)
{
    auto* reader = formatManager.createReaderFor(file);
    if (reader != nullptr)
    {
        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void PlayerAudio::start() { transportSource.start(); }
void PlayerAudio::stop() { transportSource.stop(); }

void PlayerAudio::restart()
{
    transportSource.setPosition(0);
    transportSource.start();
}

void PlayerAudio::toggleMute()
{
    muted = !muted;
    transportSource.setGain(muted ? 0.0f : volume);
}

void PlayerAudio::toggleLoop()
{
    looping = !looping;
    if (readerSource)
        readerSource->setLooping(looping);
}

void PlayerAudio::setVolume(float newVolume)
{
    volume = newVolume;
    if (!muted)
        transportSource.setGain(volume);
}

void PlayerAudio::setSpeed(float newSpeed)
{
    resamplingSource->setResamplingRatio(newSpeed);
}

bool PlayerAudio::isPlaying() const { return transportSource.isPlaying(); }
bool PlayerAudio::isLooping() const { return looping; }

double PlayerAudio::getCurrentPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setPositionRelative(double pos)
{
    if (auto length = getLengthInSeconds(); length > 0)
        transportSource.setPosition(length * pos);
}

double PlayerAudio::getPositionRelative() const
{
    auto length = getLengthInSeconds();
    return length > 0 ? transportSource.getCurrentPosition() / length : 0.0;
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

