#include "FullscreenButton.h"

#include "BinaryData.h"

FullscreenButton::FullscreenButton (
    AudioProcessorValueTreeState& thePluginState)
    : pluginState (thePluginState),
      expandIcon { Drawable::createFromImageData (
          FFAudioData::Expand_svg,
          FFAudioData::Expand_svgSize) },
      compressIcon { Drawable::createFromImageData (
          FFAudioData::Compress_svg,
          FFAudioData::Compress_svgSize) }
{
    // FIXME: Move these constants to where they can be used without
    // back-referencing the processor
    pluginState.addParameterListener ("fullscreen", this);
}

FullscreenButton::~FullscreenButton()
{
    pluginState.removeParameterListener ("fullscreen", this);
}

void FullscreenButton::paint (Graphics& g)
{
    if (isFullscreen)
    {
        if (compressIcon)
            compressIcon->drawWithin (g,
                                      getLocalBounds().toFloat(),
                                      RectanglePlacement::centred,
                                      1.0f);
    }
    else if (expandIcon)
        expandIcon->drawWithin (
            g, getLocalBounds().toFloat(), RectanglePlacement::centred, 1.0f);
}

void FullscreenButton::parameterChanged (const String& parameterID,
                                         float newValue)
{
    if (parameterID != "fullscreen")
        return;
    isFullscreen = newValue >= 0.5f;
    repaint();
}

void FullscreenButton::mouseUp (const MouseEvent& event)
{
    if (! event.mouseWasClicked())
        return;
    isFullscreen = ! isFullscreen;
    pluginState.getParameterAsValue ("fullscreen")
        .setValue (isFullscreen ? 1.0f : 0.0f);
    repaint();
}
