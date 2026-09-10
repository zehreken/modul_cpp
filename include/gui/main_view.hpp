#pragma once

#include "gui/device_view.hpp"
#include "gui/metronome_view.hpp"
#include "gui/mixer_view.hpp"
#include "gui/scope_view.hpp"

class AudioEngine; // Forward declaration

class MainView {
  public:
    MainView(AudioEngine& audio_engine);
    void render(AudioEngine& audio_engine);

  private:
    DeviceView device_view_;
    ScopeView scope_view_;
    MixerView mixer_view_;
    MetronomeView metronome_view_;
};