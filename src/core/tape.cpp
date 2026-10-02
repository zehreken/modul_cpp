#include "core/tape.hpp"
// #include <iostream>

Tape::Tape(size_t length) : audio_(length) {
    volume_ = 1.0f;
    pan_ = 0.0f;
    is_muted_ = false;
    is_solo_ = false;
    record_index_ = 0;
}

float Tape::get_volume() { return is_muted_ ? 0.0f : volume_; }

void Tape::set_volume(float vol) { volume_ = vol; }

float Tape::get_pan() { return pan_; }

void Tape::set_pan(float pan) { pan_ = pan; }

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

void Tape::pan_left() {
    if (pan_ > -1.0) {
        pan_ -= 0.01;
    } else {
        pan_ = -1.0f;
    }
}

void Tape::pan_right() {
    if (pan_ < 1.0f) {
        pan_ += 0.01;
    } else {
        pan_ = 1.0f;
    }
}

void Tape::clear() {
    // This and the for loop are equivalent
    // std::fill(audio_.begin(), audio_.end(), 0.0f)
    for (int i = 0; i < audio_.size(); ++i) {
        audio_[i] = 0.0f;
    }
}

void Tape::add(const std::vector<float> other) {
    for (int i = 0; i < other.size(); ++i) {
        audio_[i] += other[i];
    }
}

float Tape::read(size_t index) {
    if (audio_.size() == 0)
        return 0.0f;
    index = index % audio_.size();
    // std::cout << index << " " << audio_[index] << std::endl;
    return audio_[index];
}

void Tape::write(float sample) {
    if (audio_.size() == 0) {
        return;
    }
    audio_[record_index_] = sample;
    record_index_++;
    // std::cout << record_index_ << " " << sample << std::endl;
    if (record_index_ >= audio_.size()) {
        record_index_ = 0;
    }
}

std::vector<float> Tape::get_view_copy() const {
    auto bin_size = audio_.size() / 512;
    auto view_copy = std::vector<float>(512);
    for (int i = 0; i < 512; ++i) {
        float max = 0.0f;
        size_t start = i * bin_size;
        size_t end = start + bin_size;
        for (int j = start; j < end; ++j) {
            if (std::abs(max) < std::abs(audio_[j]))
                max = audio_[j];
        }
        view_copy[i] = max;
    }

    return view_copy;
}
