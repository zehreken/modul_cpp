#pragma once

#include "gui/device_view.hpp"
#include "gui/scope_view.hpp"

class AudioEngine; // Forward decleration

class MainView
{
public:
    MainView(AudioEngine &audio_engine);
    void render(AudioEngine &audio_engine);

private:
    DeviceView device_view_;
    ScopeView scope_view_;
};