#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    resampleSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio()
{
    stop();
    releaseResources();
    transportSource.setSource(nullptr);
    readerSource.reset();
    resampleSource.reset();
}

void PlayerAudio::loadAudioFile(const juce::File& file)
{
    stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    if (!file.existsAsFile())
        return;

    if (auto* reader = formatManager.createReaderFor(file))
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(
            new juce::AudioFormatReaderSource(reader, true));

        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource = std::move(newSource);
    }
}

void PlayerAudio::play()
{
    if (!transportSource.isPlaying() && readerSource != nullptr)
        transportSource.start();
}

void PlayerAudio::stop()
{
    if (transportSource.isPlaying())
        transportSource.stop();
}

void PlayerAudio::restart()
{
    if (readerSource != nullptr)
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::toggleLoop()
{
    if (readerSource != nullptr)
        readerSource->setLooping(!readerSource->isLooping());
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(juce::jlimit(0.0f, 1.0f, gain));
}

void PlayerAudio::setSpeed(double ratio)
{
    if (resampleSource)
        resampleSource->setResamplingRatio(juce::jlimit(0.1, 4.0, ratio));
}

void PlayerAudio::setPositionRelative(double pos)
{
    if (readerSource == nullptr) return;

    pos = juce::jlimit(0.0, 1.0, pos);
    double len = transportSource.getLengthInSeconds();
    if (len > 0.0)
        transportSource.setPosition(len * pos);
}

void PlayerAudio::setPositionSeconds(double seconds)
{
    double total = getTotalTime();
    if (total > 0.0)
        setPositionRelative(seconds / total);
}

float PlayerAudio::getPositionRelative() const
{
    double len = transportSource.getLengthInSeconds();
    if (len > 0.0)
        return static_cast<float>(transportSource.getCurrentPosition() / len);
    return 0.0f;
}

double PlayerAudio::getCurrentTime() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getTotalTime() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setAB_A()
{
    pointA = transportSource.getCurrentPosition();
}

void PlayerAudio::setAB_B()
{
    pointB = transportSource.getCurrentPosition();
}

void PlayerAudio::enableABLoop()
{
    if (pointA >= 0.0 && pointB > pointA)
        abLoopEnabled = true;
}

void PlayerAudio::clearABLoop()
{
    abLoopEnabled = false;
    pointA = -1.0;
    pointB = -1.0;
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    if (resampleSource)
        resampleSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::releaseResources()
{
    if (resampleSource)
        resampleSource->releaseResources();
    transportSource.releaseResources();
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr || resampleSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    if (abLoopEnabled && pointA >= 0.0 && pointB > pointA)
    {
        double pos = transportSource.getCurrentPosition();
        if (pos >= pointB)
            transportSource.setPosition(pointA);
    }

    resampleSource->getNextAudioBlock(bufferToFill);
}
