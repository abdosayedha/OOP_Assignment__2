#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(PlayerAudio& extPlayer)
{
    playerRef = &extPlayer;

    // prepare thumbnail format manager
    thumbnailFormatManager.registerBasicFormats();

    // === Buttons ===
    addAndMakeVisible(loadButton); loadButton.addListener(this);
    addAndMakeVisible(playButton); playButton.addListener(this);
    addAndMakeVisible(stopButton); stopButton.addListener(this);
    addAndMakeVisible(restartButton); restartButton.addListener(this);
    addAndMakeVisible(loopButton); loopButton.addListener(this);

    addAndMakeVisible(setAButton); setAButton.addListener(this);
    addAndMakeVisible(setBButton); setBButton.addListener(this);
    addAndMakeVisible(enableABButton); enableABButton.addListener(this);
    addAndMakeVisible(clearABButton); clearABButton.addListener(this);

    // === Sliders ===
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.8);
    volumeSlider.addListener(this);

    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);

    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.addListener(this);

    // === Label ===
    addAndMakeVisible(timeLabel);
    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centredLeft);

    // start timer to update UI and waveform playhead
    startTimerHz(30);
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);

    // draw waveform area background
    auto area = getLocalBounds().reduced(10);
    auto waveformHeight = 120;
    juce::Rectangle<int> waveformArea = area.removeFromTop(waveformHeight).reduced(4);

    g.setColour(juce::Colours::black);
    g.fillRect(waveformArea);

    // draw thumbnail waveform if available
    if (thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::lightgrey);
        thumbnail.drawChannels(g, waveformArea, 0.0, thumbnail.getTotalLength(), 1.0f);

        // draw playhead (current position)
        double posRel = 0.0;
        if (playerRef != nullptr)
            posRel = playerRef->getPositionRelative();

        int x = waveformArea.getX() + (int)(posRel * (double)waveformArea.getWidth());
        g.setColour(juce::Colours::red);
        g.drawLine((float)x, (float)waveformArea.getY(), (float)x, (float)waveformArea.getBottom(), 2.0f);
    }
    else
    {
        g.setColour(juce::Colours::grey);
        g.drawText("No waveform loaded", waveformArea, juce::Justification::centred);
    }

    // small separator line
    g.setColour(juce::Colours::darkgrey.brighter());
    g.drawLine((float)waveformArea.getX(), (float)waveformArea.getBottom() + 2,
        (float)waveformArea.getRight(), (float)waveformArea.getBottom() + 2, 1.0f);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(10);

    // waveform area height (paint uses same)
    auto waveformHeight = 120;
    auto waveformArea = area.removeFromTop(waveformHeight);

    // below waveform, layout controls
    auto h = 30;
    auto row = area.removeFromTop(h);
    loadButton.setBounds(row.removeFromLeft(120).reduced(2));
    playButton.setBounds(row.removeFromLeft(80).reduced(2));
    stopButton.setBounds(row.removeFromLeft(80).reduced(2));
    restartButton.setBounds(row.removeFromLeft(80).reduced(2));
    loopButton.setBounds(row.removeFromLeft(80).reduced(2));

    auto row2 = area.removeFromTop(h);
    setAButton.setBounds(row2.removeFromLeft(80).reduced(2));
    setBButton.setBounds(row2.removeFromLeft(80).reduced(2));
    enableABButton.setBounds(row2.removeFromLeft(120).reduced(2));
    clearABButton.setBounds(row2.removeFromLeft(120).reduced(2));

    volumeSlider.setBounds(area.removeFromTop(h).reduced(2));
    speedSlider.setBounds(area.removeFromTop(h).reduced(2));
    positionSlider.setBounds(area.removeFromTop(h).reduced(2));
    timeLabel.setBounds(area.removeFromTop(h).reduced(2));
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (playerRef == nullptr) return;

    if (button == &loadButton)
    {
        // modern async chooser; capture shared_ptr to keep alive during async
        auto chooser = std::make_shared<juce::FileChooser>(
            "Select an audio file...", juce::File(), "*.wav;*.mp3;*.aiff;*.flac");

        chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile() && playerRef != nullptr)
                {
                    // load into player
                    playerRef->loadAudioFile(file);

                    // set thumbnail source so waveform is displayed
                    lastLoadedFile = file;
                    thumbnail.clear();
                    thumbnail.setSource(new juce::FileInputSource(file));
                }
            });
    }
    else if (button == &playButton) playerRef->play();
    else if (button == &stopButton) playerRef->stop();
    else if (button == &restartButton) playerRef->restart();
    else if (button == &loopButton) playerRef->toggleLoop();

    else if (button == &setAButton) playerRef->setAB_A();
    else if (button == &setBButton) playerRef->setAB_B();
    else if (button == &enableABButton) playerRef->enableABLoop();
    else if (button == &clearABButton) playerRef->clearABLoop();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (playerRef == nullptr) return;

    if (slider == &volumeSlider)
        playerRef->setGain((float)volumeSlider.getValue());
    else if (slider == &speedSlider)
        playerRef->setSpeed((float)speedSlider.getValue());
    else if (slider == &positionSlider)
        playerRef->setPositionRelative((float)positionSlider.getValue());
}

void PlayerGUI::timerCallback()
{
    if (playerRef == nullptr) return;

    // update position slider (without notification to avoid feedback)
    positionSlider.setValue(playerRef->getPositionRelative(), juce::dontSendNotification);

    // update time label
    updateTimeLabel();

    // repaint so waveform playhead updates smoothly
    repaint();
}

void PlayerGUI::updateTimeLabel()
{
    if (playerRef == nullptr) return;

    double pos = playerRef->getCurrentTime();
    double len = playerRef->getTotalTime();

    if (len <= 0.0)
    {
        timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
        return;
    }

    int posMin = static_cast<int>(pos) / 60;
    int posSec = static_cast<int>(pos) % 60;
    int lenMin = static_cast<int>(len) / 60;
    int lenSec = static_cast<int>(len) % 60;

    juce::String p = juce::String(posMin).paddedLeft('0', 2) + ":" + juce::String(posSec).paddedLeft('0', 2);
    juce::String l = juce::String(lenMin).paddedLeft('0', 2) + ":" + juce::String(lenSec).paddedLeft('0', 2);

    timeLabel.setText(p + " / " + l, juce::dontSendNotification);
}
