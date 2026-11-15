#pragma once

#include <vector>

class Tape {
  public:
    explicit Tape(size_t length = 0);
    void clear();
    void add(const std::vector<float> other);
    float read(size_t index);
    void write(size_t index, float sample);
    std::vector<float> get_view_copy() const;

  private:
    std::vector<float> audio_; // Tape is mono by design
};