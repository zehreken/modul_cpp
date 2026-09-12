#pragma once

class AudioEngine; // Forward declaration

class MetronomeView {
  public:
    void render(AudioEngine& audio_engine, bool* show);

  private:
    bool can_metronome_run_ = false;
};