#pragma once

#include "dj_fft/dj_fft.h"

class AudioEngine; // Forward declaration

class ScopeView {
  public:
    ScopeView();
    void render(AudioEngine& audio_engine, bool* show);

  private:
    dj::fft_arg<float> fft_input_;
};