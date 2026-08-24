#pragma once

class AudioEngine; // Forward decleration

class DeviceView
{
public:
    void render(AudioEngine &audio_engine);

private:
    int selected_playback_device_id_{0};
    int selected_capture_device_id_{0};
};