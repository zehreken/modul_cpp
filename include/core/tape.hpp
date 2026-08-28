#pragma once

#include <vector>

class Tape {
  public:
    explicit Tape(size_t length);
    ~Tape();
    float get_volume();
    void toggle_mute();
    void toggle_solo();
    bool is_solo();
    void volume_up();
    void volume_down();
    void clear();
    void add(const std::vector<float> other);

  private:
    float volume_;
    bool is_muted_;
    bool is_solo_;
    std::vector<float> audio_;
};