#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include "PluginMode.h"

class ModeControlsComponent : public Component,
                              public TextButton::Listener,
                              public AudioProcessorValueTreeState::Listener
{
public:
    std::map<String, PluginMode> modeMap = {
        { "Stereo", Stereo },   { "Mid", Mid },           { "Side", Side },
        { "MidSolo", MidSolo }, { "SideSolo", SideSolo },
    };

    ModeControlsComponent (AudioProcessorValueTreeState& thePluginState);
    ~ModeControlsComponent() override;

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;
    void parameterChanged (const String& parameterID, float newValue) override;
    float stringStateToParamValue (const String& stateString,
                                   StringArray choices);

private:
    void updateUi();

    AudioProcessorValueTreeState& pluginState;
    TextButton stereoButton;
    TextButton midSideButton;
    TextButton midButton;
    TextButton sideButton;
    TextButton midSoloButton;
    TextButton sideSoloButton;
    PluginMode currentMode = Stereo;
};
