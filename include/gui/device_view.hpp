#pragma once

#include "core/audio_engine.hpp"

class AudioEngine; // Forward decleration

class DeviceView {
  public:
    explicit DeviceView(AudioEngine& audio_engine);

    void render(AudioEngine& audio_engine);

  private:
    AudioDevices audio_devices_;
    int selected_playback_device_id_{0};
    int selected_capture_device_id_{0};
};