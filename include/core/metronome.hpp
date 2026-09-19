#pragma once

class Metronome {
  public:
    Metronome(int bpm = 120, float frame_rate = 48000);
    void update(size_t frame_index);
    bool can_beep() const;

  private:
    float tick_period_;
    bool can_beep_{false};
};