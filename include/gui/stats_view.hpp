#pragma once

#include "core/audio_engine.hpp"

class AudioEngine; // Forward declaration

class StatsView {
  public:
    explicit StatsView(AudioEngine& audio_engine);

    void render(AudioEngine& audio_engine, bool* show);

  private:
    AudioDevices audio_devices_;
    int selected_playback_device_id_{0};
    int selected_capture_device_id_{0};
};