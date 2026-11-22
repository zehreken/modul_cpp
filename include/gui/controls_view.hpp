#pragma once

class AudioEngine; // Forward declaration

class ControlsView {
  public:
    void render(AudioEngine& audio_engine, bool* show);
};