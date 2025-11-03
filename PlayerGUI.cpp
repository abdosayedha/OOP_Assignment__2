#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    playerRef = &localPlayer;
    startTimerHz(10);

    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(loopButton);

    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    restartButton.addListener(this);
    muteButton.addListener(this);
    loopButton.addListener(this);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);

    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);

    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);
    addAndMakeVisible(timeLabel);

    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
}

PlayerGUI::PlayerGUI(PlayerAudio& externalPlayer)
    : playerRef(&externalPlayer)
{
    startTimerHz(10);
    // نفس إعدادات الـ constructor فوق
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(loopButton);

    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    restartButton.addListener(this);
    muteButton.addListener(this);
    loopButton.addListener(this);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);

    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);

    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);
    addAndMakeVisible(timeLabel);

    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto buttonHeight = 30;

    loadButton.setBounds(area.removeFromTop(buttonHeight));
    playButton.setBounds(area.removeFromTop(buttonHeight));
    stopButton.setBounds(area.removeFromTop(buttonHeight));
    restartButton.setBounds(area.removeFromTop(buttonHeight));
    muteButton.setBounds(area.removeFromTop(buttonHeight));
    loopButton.setBounds(area.removeFromTop(buttonHeight));

    area.removeFromTop(10);
    volumeLabel.setBounds(area.removeFromTop(20));
    volumeSlider.setBounds(area.removeFromTop(30));

    area.removeFromTop(10);
    speedLabel.setBounds(area.removeFromTop(20));
    speedSlider.setBounds(area.removeFromTop(30));

    area.removeFromTop(10);
    positionLabel.setBounds(area.removeFromTop(20));
    positionSlider.setBounds(area.removeFromTop(30));

    area.removeFromTop(10);
    timeLabel.setBounds(area.removeFromTop(30));
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    auto& player = *playerRef;

    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select an audio file...", {}, "*.mp3;*.wav");
        chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                    playerRef->loadFile(file);
            });
    }
    else if (button == &playButton)
        player.start();
    else if (button == &stopButton)
        player.stop();
    else if (button == &restartButton)
        player.restart();
    else if (button == &muteButton)
        player.toggleMute();
    else if (button == &loopButton)
        player.toggleLoop();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    auto& player = *playerRef;

    if (slider == &volumeSlider)
        player.setVolume((float)slider->getValue());
    else if (slider == &speedSlider)
        player.setSpeed((float)slider->getValue());
    else if (slider == &positionSlider)
        player.setPositionRelative((float)slider->getValue());
}

void PlayerGUI::timerCallback()
{
    updateTimeLabel();
}

void PlayerGUI::updateTimeLabel()
{
    auto& player = *playerRef;
    double pos = player.getPosition();
    double len = player.getLength();

    if (len > 0)
    {
        juce::String posStr = juce::String((int)(pos / 60)).paddedLeft('0', 2) + ":" + juce::String((int)pos % 60).paddedLeft('0', 2);
        juce::String lenStr = juce::String((int)(len / 60)).paddedLeft('0', 2) + ":" + juce::String((int)len % 60).paddedLeft('0', 2);
        timeLabel.setText(posStr + " / " + lenStr, juce::dontSendNotification);
    }
}

