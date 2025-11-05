/*
Student : عبد الرحمن سيد محمود عبد العزيز
ID : 20242203
Section : S25

Documentation Link :https://1drv.ms/w/c/2ced87a452e9403c/ETxA6VKkh-0ggCzMAAAAAAAB3u_jWyC8rXzj613ML9YtxQ?e=VOeVi8
Video Demo Link :https://youtu.be/h4keucRt9uM
Repositories Link :https://github.com/abdosayedha/OOP_Assignment__2
*/

#include  <JuceHeader.h>
#include "MainComponent.h"

// Our application class
class SimpleAudioPlayer : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Simple Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        // Create and show the main window
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override
    {
        mainWindow = nullptr; // Clean up
    }

private:
    // The main window of the app
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Colours::lightgrey,
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true); // MainComponent = our UI + logic
            centreWithSize(500, 200);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

// This macro starts the app
START_JUCE_APPLICATION(SimpleAudioPlayer)
