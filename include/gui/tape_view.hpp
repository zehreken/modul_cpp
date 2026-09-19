#pragma once

class AudioEngine; // Forward declaration

class TapeView {
  public:
    explicit TapeView();
    void render(AudioEngine& audio_engine, bool* show);

  private:
};