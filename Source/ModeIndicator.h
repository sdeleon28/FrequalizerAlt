#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include "PluginMode.h"

class ModeIndicator : public Component, AudioProcessorValueTreeState::Listener
{
public:
    std::map<String, PluginMode> modeMap = {
        { "Stereo", Stereo },   { "Mid", Mid },           { "Side", Side },
        { "MidSolo", MidSolo }, { "SideSolo", SideSolo },
    };

    ModeIndicator (AudioProcessorValueTreeState& thePluginState);
    ~ModeIndicator() override;

    void paint (Graphics& g) override;
    void resized() override;
    void parameterChanged (const String& parameterID, float newValue) override;

private:
    void updateCurrentMode();
    void updateUi();

    AudioProcessorValueTreeState& pluginState;
    PluginMode currentMode = Stereo;
    Label modeLabel;
};
