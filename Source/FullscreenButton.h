#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

class FullscreenButton : public Component,
                         public AudioProcessorValueTreeState::Listener
{
public:
    FullscreenButton (AudioProcessorValueTreeState& thePluginState);
    ~FullscreenButton() override;

    void paint (Graphics&) override;
    void parameterChanged (const String& parameterID, float newValue) override;
    void mouseUp (const MouseEvent&) override;

private:
    bool isFullscreen = false;
    AudioProcessorValueTreeState& pluginState;
    std::unique_ptr<Drawable> expandIcon;
    std::unique_ptr<Drawable> compressIcon;
};
