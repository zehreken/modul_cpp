#include "core/tape.hpp"

Tape::Tape(size_t length) : audio_(length) {}

float Tape::get_volume() { return is_muted_ ? 0.0f : volume_; }

void Tape::toggle_mute() { is_muted_ = !is_muted_; }

void Tape::toggle_solo() { is_solo_ = !is_solo_; }

bool Tape::is_solo() { return is_solo_; }

void Tape::volume_up() {
    if (volume_ < 1.0f) {
        volume_ += 0.01f;
    } else {
        volume_ = 1.0f;
    }
}

void Tape::volume_down() {
    if (volume_ > 0.0f) {
        volume_ -= 0.01f;
    } else {
        volume_ = 0.0f;
    }
}

void Tape::clear() { audio_.clear(); }

void Tape::add(const std::vector<float> other) {
    for (int i = 0; i < other.size(); ++i) {
        audio_[i] += other[i];
    }
}