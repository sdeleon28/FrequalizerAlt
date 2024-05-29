#include "ModeControlsComponent.h"

ModeControlsComponent::ModeControlsComponent (
    AudioProcessorValueTreeState& thePluginState)
    : pluginState (thePluginState)
{
    // FIXME: Move these constants to where they can be used without
    // back-referencing the processor
    pluginState.addParameterListener ("mode", this);
    auto setupButton = [this] (TextButton& button, String text)
    {
        button.setButtonText (text);
        button.setClickingTogglesState (false);
        button.setColour (TextButton::buttonColourId, Colours::grey);
        button.setColour (TextButton::buttonOnColourId, Colours::green);
        button.setColour (TextButton::textColourOnId, Colours::white);
        button.setColour (TextButton::textColourOffId, Colours::white);
        addAndMakeVisible (button);
        button.addListener (this);
    };
    setupButton (stereoButton, TRANS ("stereo"));
    setupButton (midSideButton, TRANS ("mid/side"));
    setupButton (midButton, TRANS ("mid"));
    setupButton (sideButton, TRANS ("side"));
    setupButton (midSoloButton, TRANS ("solo"));
    setupButton (sideSoloButton, TRANS ("solo"));
}

ModeControlsComponent::~ModeControlsComponent()
{
    pluginState.removeParameterListener ("mode", this);
}

void ModeControlsComponent::paint (Graphics& /* g */) {}

void ModeControlsComponent::resized()
{
    auto bounds = getLocalBounds();
    int buttonHeight = bounds.proportionOfHeight (0.25f);
    int buttonWidth = bounds.proportionOfWidth (0.5f);
    auto topRow = bounds.removeFromTop (buttonHeight);
    stereoButton.setBounds (topRow.removeFromLeft (buttonWidth));
    midSideButton.setBounds (topRow);
    bounds.removeFromTop (buttonHeight);
    auto midOrSideRow = bounds.removeFromTop (buttonHeight);
    midButton.setBounds (midOrSideRow.removeFromLeft (buttonWidth));
    sideButton.setBounds (midOrSideRow);
    midSoloButton.setBounds (bounds.removeFromLeft (buttonWidth));
    sideSoloButton.setBounds (bounds);
}

void ModeControlsComponent::buttonClicked (Button* button)
{
    String stateString = "";
    if (button == &stereoButton)
        stateString = "Stereo";
    else if (button == &midSideButton)
        stateString = "Mid";
    else if (button == &midButton)
        stateString = "Mid";
    else if (button == &sideButton)
        stateString = "Side";
    else if (button == &midSoloButton)
        stateString = "MidSolo";
    else if (button == &sideSoloButton)
        stateString = "SideSolo";
    auto& modeParam = *dynamic_cast<juce::AudioParameterChoice*> (
        pluginState.getParameter ("mode"));
    if (stateString.isNotEmpty())
        modeParam.setValueNotifyingHost (
            stringStateToParamValue (stateString, modeParam.choices));
}

float ModeControlsComponent::stringStateToParamValue (const String& stateString,
                                                      StringArray choices)
{
    int index = choices.indexOf (stateString);
    if (index >= 0)
        return static_cast<float> (index)
               / static_cast<float> (choices.size() - 1);
    return 0.0f;
}

void ModeControlsComponent::parameterChanged (const String& parameterID,
                                              float newValue)
{
    if (parameterID != "mode")
        return;
    auto& modeParam = *dynamic_cast<juce::AudioParameterChoice*> (
        pluginState.getParameter ("mode"));
    auto stringState = modeParam.choices[(int) newValue];
    auto currentState = modeMap[stringState];
    auto setMidSideButtonsVisible = [this] (bool visible)
    {
        midButton.setVisible (visible);
        sideButton.setVisible (visible);
        midSoloButton.setVisible (visible);
        sideSoloButton.setVisible (visible);
    };
    switch (currentState)
    {
        case Mode::Stereo:
            stereoButton.setToggleState (true, dontSendNotification);
            midSideButton.setToggleState (false, dontSendNotification);
            midButton.setToggleState (false, dontSendNotification);
            sideButton.setToggleState (false, dontSendNotification);
            midSoloButton.setToggleState (false, dontSendNotification);
            sideSoloButton.setToggleState (false, dontSendNotification);
            setMidSideButtonsVisible (false);
            break;
        case Mode::Mid:
            stereoButton.setToggleState (false, dontSendNotification);
            midSideButton.setToggleState (true, dontSendNotification);
            midButton.setToggleState (true, dontSendNotification);
            sideButton.setToggleState (false, dontSendNotification);
            midSoloButton.setToggleState (false, dontSendNotification);
            sideSoloButton.setToggleState (false, dontSendNotification);
            setMidSideButtonsVisible (true);
            break;
        case Mode::Side:
            stereoButton.setToggleState (false, dontSendNotification);
            midSideButton.setToggleState (true, dontSendNotification);
            midButton.setToggleState (false, dontSendNotification);
            sideButton.setToggleState (true, dontSendNotification);
            midSoloButton.setToggleState (false, dontSendNotification);
            sideSoloButton.setToggleState (false, dontSendNotification);
            setMidSideButtonsVisible (true);
            break;
        case Mode::MidSolo:
            stereoButton.setToggleState (false, dontSendNotification);
            midSideButton.setToggleState (true, dontSendNotification);
            midButton.setToggleState (true, dontSendNotification);
            sideButton.setToggleState (false, dontSendNotification);
            midSoloButton.setToggleState (true, dontSendNotification);
            sideSoloButton.setToggleState (false, dontSendNotification);
            setMidSideButtonsVisible (true);
            break;
        case Mode::SideSolo:
            stereoButton.setToggleState (false, dontSendNotification);
            midSideButton.setToggleState (true, dontSendNotification);
            midButton.setToggleState (false, dontSendNotification);
            sideButton.setToggleState (true, dontSendNotification);
            midSoloButton.setToggleState (false, dontSendNotification);
            sideSoloButton.setToggleState (true, dontSendNotification);
            setMidSideButtonsVisible (true);
            break;
    }
}
