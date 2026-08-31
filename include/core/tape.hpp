#pragma once

#include <vector>

class Tape {
  public:
    explicit Tape(size_t length);
    float get_volume();
    void toggle_mute();
    void toggle_solo();
    bool is_solo();
    void volume_up();
    void volume_down();
    void clear();
    void add(const std::vector<float> other);
    float read(size_t index);
    void write(float sample);

  private:
    float volume_;
    bool is_muted_;
    bool is_solo_;
    size_t record_index_; // Cursor
    std::vector<float> audio_;
};