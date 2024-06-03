#include "ModeIndicator.h"

ModeIndicator::ModeIndicator (AudioProcessorValueTreeState& thePluginState)
    : pluginState (thePluginState)
{
    pluginState.addParameterListener ("mode", this);
    modeLabel.setColour (Label::textColourId, Colours::white);
    modeLabel.setJustificationType (Justification::centred);
    addAndMakeVisible(modeLabel);
    updateCurrentMode();
}

ModeIndicator::~ModeIndicator()
{
    pluginState.removeParameterListener ("mode", this);
}

void ModeIndicator::updateCurrentMode ()
{
    auto& modeParam = *dynamic_cast<juce::AudioParameterChoice*> (
        pluginState.getParameter ("mode"));
    auto currentChoiceName = modeParam.getCurrentChoiceName();
    jassert (modeMap.find (currentChoiceName) != modeMap.end());
    currentMode = modeMap[currentChoiceName];
    updateUi();
}

void ModeIndicator::parameterChanged (const String& parameterID, float /* newValue */)
{
    if (parameterID != "mode")
        return;
    updateCurrentMode();
}

void ModeIndicator::paint (Graphics& g)
{
    float borderRadius = 10.0f;
    float borderThickness = 2.0f;
    auto area = getLocalBounds();
    g.setColour (Colour (0x7f192226));
    g.fillRoundedRectangle (area.toFloat(), borderRadius);
    g.setColour (Colours::white);
    g.drawRoundedRectangle (area.toFloat(), borderRadius, borderThickness);
}

void ModeIndicator::resized() { modeLabel.setBounds (getLocalBounds()); }

void ModeIndicator::updateUi()
{
    switch (currentMode)
    {
#if LR_MODE
        case PluginMode::Stereo:
            modeLabel.setText (TRANS ("stereo"), dontSendNotification);
            break;
        case PluginMode::Mid:
            modeLabel.setText (TRANS ("left"), dontSendNotification);
            break;
        case PluginMode::Side:
            modeLabel.setText (TRANS ("right"), dontSendNotification);
            break;
        case PluginMode::MidSolo:
            modeLabel.setText (TRANS ("left (solo)"), dontSendNotification);
            break;
        case PluginMode::SideSolo:
            modeLabel.setText (TRANS ("right (solo)"), dontSendNotification);
            break;
#else
        case PluginMode::Stereo:
            modeLabel.setText (TRANS ("stereo"), dontSendNotification);
            break;
        case PluginMode::Mid:
            modeLabel.setText (TRANS ("mid"), dontSendNotification);
            break;
        case PluginMode::Side:
            modeLabel.setText (TRANS ("side"), dontSendNotification);
            break;
        case PluginMode::MidSolo:
            modeLabel.setText (TRANS ("mid (solo)"), dontSendNotification);
            break;
        case PluginMode::SideSolo:
            modeLabel.setText (TRANS ("side (solo)"), dontSendNotification);
            break;
#endif
    }
    repaint();
}
