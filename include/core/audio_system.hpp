#pragma once

#include "core/tape.hpp"

class AudioSystem {
  public:
    explicit AudioSystem(size_t length);

    void toggle_play_through();
    bool can_play_through();

  private:
    Tape recording_tape_;
    bool can_play_through_;
};