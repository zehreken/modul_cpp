#pragma once

#include <atomic>

class Metronome {
  public:
    Metronome(int bpm = 120, float frame_rate = 48000.0f);
    void update(size_t frame_index);
    bool can_beep() const;
    int get_beat_count() const;

  private:
    float tick_period_;
    std::atomic<bool> can_beep_{false};
    std::atomic<int> beat_count_{0};
};