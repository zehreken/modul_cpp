#include "core/tape.hpp"
#include "constants.hpp"
// #include <iostream>

Tape::Tape(size_t length) : audio_(length) {}

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

void Tape::write(size_t index, float sample) {
    if (audio_.size() == 0) {
        return;
    }
    auto record_index = index % audio_.size();
    audio_[record_index] = sample;
}

std::vector<float> Tape::get_view_copy() const {
    auto bin_size = audio_.size() / constants::TAPE_VIEW_SIZE;
    auto view_copy = std::vector<float>(constants::TAPE_VIEW_SIZE);
    for (int i = 0; i < constants::TAPE_VIEW_SIZE; ++i) {
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
