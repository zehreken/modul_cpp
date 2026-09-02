#pragma once

#include <vector>

class Tape {
  public:
    Tape(size_t length);
    float get_volume();
    float get_pan();
    void toggle_mute();
    void toggle_solo();
    bool is_solo();
    void volume_up();
    void volume_down();
    void pan_left();
    void pan_right();
    void clear();
    void add(const std::vector<float> other);
    float read(size_t index);
    void write(float sample);

  private:
    float volume_;
    float pan_;
    bool is_muted_;
    bool is_solo_;
    size_t record_index_;      // Cursor
    std::vector<float> audio_; // Tape is mono by design
};