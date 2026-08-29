#include "core/audio_system.hpp"
#include <iostream>

AudioSystem::AudioSystem(size_t length) : recording_tape_(length) {}

void AudioSystem::toggle_play_through() {
    can_play_through_ = !can_play_through_;
    std::cout << can_play_through_ << std::endl;
}

bool AudioSystem::can_play_through() { return can_play_through_; }