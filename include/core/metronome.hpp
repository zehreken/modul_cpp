#pragma once

class Metronome {
  public:
    Metronome(unsigned int bpm, float frame_rate);
    void update(size_t frame_index);
    bool can_beep() const;

  private:
    float tick_period_;
    bool can_beep_{false};
};