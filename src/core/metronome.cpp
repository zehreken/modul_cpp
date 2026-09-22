#include "core/metronome.hpp"

// Miniaudio provides frame rate, it is sample_rate / channel_count
// I think this is better than how cpal does it, simpler
Metronome::Metronome(int bpm, float frame_rate) {
    tick_period_ = frame_rate * 60.0f / bpm;
}

void Metronome::update(const size_t frame_index) {
    float remainder = frame_index % static_cast<size_t>(tick_period_);
    bool can_beep = remainder > 0 && remainder < 6000;
    can_beep_.store(can_beep, std::memory_order_relaxed);
}

bool Metronome::can_beep() const {
    return can_beep_.load(std::memory_order_relaxed);
}
