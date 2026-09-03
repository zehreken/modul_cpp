#include "core/audio_engine.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

struct AudioEngine::Impl {
    ma_context context_;
    ma_device device_;
    bool is_device_initialized_{false};
    bool is_context_initialized_{false};
    ma_pcm_rb rb_;
};

AudioEngine::AudioEngine(size_t length)
    : impl_(new Impl()), recording_tape_(length),
      tapes_{length, length, length, length, length, length, length, length} {
    if (ma_pcm_rb_init(
            ma_format_f32,
            2,
            AudioEngine::SCOPE_SIZE,
            nullptr,
            nullptr,
            &impl_->rb_
        ) == MA_SUCCESS) {
        std::cout << "Successfully created ring buffer" << std::endl;
    } else {
        std::cout << "Failed to create ring buffer" << std::endl;
    }
}

AudioEngine::~AudioEngine() {
    shutdown();
    delete impl_;
};

bool AudioEngine::init() {
    if (ma_context_init(nullptr, 0, nullptr, &impl_->context_) != MA_SUCCESS) {
        return false;
    }
    impl_->is_context_initialized_ = true;
    return select_devices(-1, -1); // init with default device
}

void AudioEngine::shutdown() {
    ma_pcm_rb_uninit(&impl_->rb_);
    if (impl_->is_device_initialized_) {
        ma_device_uninit(&impl_->device_);
        impl_->is_device_initialized_ = false;
    }
    if (impl_->is_context_initialized_) {
        ma_context_uninit(&impl_->context_);
        impl_->is_context_initialized_ = false;
    }
}

AudioDevices AudioEngine::get_audio_devices() {
    std::vector<AudioDeviceInfo> playback_devices;
    ma_device_info* playback_infos;
    ma_uint32 playback_count;

    std::vector<AudioDeviceInfo> capture_devices;
    ma_device_info* capture_infos;
    ma_uint32 capture_count;

    if (ma_context_get_devices(
            &impl_->context_,
            &playback_infos,
            &playback_count,
            &capture_infos,
            &capture_count
        ) == MA_SUCCESS) {
        for (ma_uint32 i = 0; i < playback_count; ++i) {
            auto info = playback_infos[i];

            AudioDeviceInfo device_info;
            device_info.name_ = info.name;
            device_info.id_index_ = static_cast<int>(i);
            device_info.is_default_ = info.isDefault != 0;
            if (ma_context_get_device_info(
                    &impl_->context_, ma_device_type_playback, &info.id, &info
                ) == MA_SUCCESS) {
                device_info.channels_ =
                    static_cast<int>(info.nativeDataFormats->channels);
                device_info.format_ =
                    static_cast<int>(info.nativeDataFormats->format);
                device_info.sample_rate_ =
                    static_cast<int>(info.nativeDataFormats->sampleRate);
            }
            playback_devices.push_back(device_info);
        }
        for (ma_uint32 i = 0; i < capture_count; ++i) {
            auto info = capture_infos[i];

            AudioDeviceInfo device_info;
            device_info.name_ = info.name;
            device_info.id_index_ = static_cast<int>(i);
            device_info.is_default_ = info.isDefault != 0;
            if (ma_context_get_device_info(
                    &impl_->context_, ma_device_type_capture, &info.id, &info
                ) == MA_SUCCESS) {
                device_info.channels_ =
                    static_cast<int>(info.nativeDataFormats->channels);
                device_info.format_ =
                    static_cast<int>(info.nativeDataFormats->format);
                device_info.sample_rate_ =
                    static_cast<int>(info.nativeDataFormats->sampleRate);
            }
            capture_devices.push_back(device_info);
        }
    }

    AudioDevices devices{playback_devices, capture_devices};
    return devices;
}

bool AudioEngine::select_devices(
    int playback_device_index, int capture_device_index
) {
    if (impl_->is_device_initialized_) {
        ma_device_uninit(&impl_->device_);
        impl_->is_device_initialized_ = false;
    }

    ma_device_config config = ma_device_config_init(ma_device_type_duplex);
    config.playback.format = ma_format_f32;
    config.playback.channels = 2;
    config.capture.format = ma_format_f32;
    config.capture.channels = 2;
    config.sampleRate = static_cast<ma_uint32>(SAMPLE_RATE);
    config.periodSizeInFrames = SCOPE_SIZE; // periodSizeInFrames <> latency
    config.dataCallback = AudioEngine::c_audio_callback;
    config.pUserData = this;

    ma_device_info* playback_infos;
    ma_uint32 playback_count;
    ma_device_info* capture_infos;
    ma_uint32 capture_count;
    if (ma_context_get_devices(
            &impl_->context_,
            &playback_infos,
            &playback_count,
            &capture_infos,
            &capture_count
        ) != MA_SUCCESS) {
        return false;
    }
    if (playback_device_index >= 0 && playback_device_index < playback_count) {
        config.playback.pDeviceID = &playback_infos[playback_device_index].id;
    }

    if (capture_device_index >= 0 && capture_device_index < capture_count) {
        config.capture.pDeviceID = &capture_infos[capture_device_index].id;
    }

    if (ma_device_init(&impl_->context_, &config, &impl_->device_) !=
        MA_SUCCESS) {
        return false;
    }

    impl_->is_device_initialized_ = true;
    return ma_device_start(&impl_->device_) == MA_SUCCESS;
}

void AudioEngine::c_audio_callback(
    ma_device* device, void* output, const void* input, ma_uint32 frame_count
) {
    auto* engine = static_cast<AudioEngine*>(device->pUserData);
    engine->process_audio(
        device,
        static_cast<float*>(output),
        static_cast<const float*>(input),
        frame_count
    );
}

void AudioEngine::process_audio(
    ma_device* device,
    float* output,
    const float* input,
    unsigned int frame_count
) {
    float freq = frequency_.load(std::memory_order_relaxed);
    float vol = volume_.load(std::memory_order_relaxed);
    float phase_incr = (TWO_PI * freq) / SAMPLE_RATE;

    ma_result result;
    ma_uint32 frames_written;
    frames_written = 0;
    while (frames_written < frame_count) {
        void* write_buffer;
        ma_uint32 frames_to_write = frame_count - frames_written;

        result = ma_pcm_rb_acquire_write(
            &impl_->rb_, &frames_to_write, &write_buffer
        );
        if (result != MA_SUCCESS) {
            break;
        }
        if (frames_to_write == 0) {
            break;
        }
        // std::cout << frame_count << " " << fc << std::endl;
        ma_copy_pcm_frames(
            write_buffer,
            ma_offset_pcm_frames_const_ptr_f32(
                (const float*)input, frames_written, device->capture.channels
            ),
            frames_to_write,
            device->capture.format,
            device->capture.channels
        );
        ma_result result = ma_pcm_rb_commit_write(&impl_->rb_, frames_to_write);
        if (result != MA_SUCCESS) {
            break;
        }
        frames_written += frames_to_write;
        // std::cout << "write r2: " << r2 << std::endl;
    }

    for (unsigned int i = 0; i < frame_count; ++i) {
        audio_index_++;
        float sample = std::sin(phase_);

        float in_left = input ? *input++ : 0.0f;
        float in_right = input ? *input++ : 0.0f;

        float out_left = sample;
        float out_right = sample;
        if (can_play_through_) {
            out_left = (out_left + in_left) * 0.5f;
            out_right = (out_right + in_right) * 0.5f;
        }

        if (can_record_) {
            // recording_tape_.write((in_left + in_right) * 0.5f);
            tapes_[selected_tape_].write((in_left + in_right) * 0.5f);
        } else {
            float tape_value = 0.0f;
            for (Tape& tape : tapes_) {
                tape_value += tape.read(audio_index_);
            }
            tape_value = tape_value * 0.125f;
            // float tape_value = recording_tape_.read(audio_index_);
            out_left = (out_left + tape_value) * 0.5f;
            out_right = (out_right + tape_value) * 0.5f;
        }

        out_left = out_left * vol;
        out_right = out_right * vol;

        *output++ = out_left;
        *output++ = out_right;

        // scope_buffer_[scope_write_index_] = out_left;
        // scope_write_index_ = (scope_write_index_ + 1) % SCOPE_SIZE;

        phase_ += phase_incr;
        if (phase_ >= TWO_PI)
            phase_ -= TWO_PI;
    }
}

void AudioEngine::toggle_play_through() {
    can_play_through_ = !can_play_through_;
}

bool AudioEngine::can_play_through() { return can_play_through_; }

// When load operation is not used, it is seq_cst load
void AudioEngine::toggle_record() { can_record_ = !can_record_; }

bool AudioEngine::can_record() { return can_record_; }

void AudioEngine::copy_scope_buffer(float* out_target, size_t count) {
    // TODO: This is currently broken, need to separate
    // L and R channel and also memcpy arithmetic is wrong
    void* read_buffer;
    ma_result result;
    ma_uint32 frame_count = static_cast<ma_uint32>(count);
    result = ma_pcm_rb_acquire_read(&impl_->rb_, &frame_count, &read_buffer);
    if (result != MA_SUCCESS) {
        return;
    }
    std::memcpy(out_target, read_buffer, frame_count * sizeof(float));
    result = ma_pcm_rb_commit_read(&impl_->rb_, frame_count);
    if (result != MA_SUCCESS) {
        return;
    }
    // std::cout << "read r2: " << r2 << std::endl;
    // size_t copy_size = std::min(count, SCOPE_SIZE);
    // std::memcpy(out_target, scope_buffer_, copy_size * sizeof(float));
}

void AudioEngine::copy_recording(float* out_target, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        *out_target++ = recording_tape_.read(i);
    }
}

void AudioEngine::set_selected_tape(size_t id) {
    selected_tape_.store(id, std::memory_order_relaxed);
}

size_t AudioEngine::get_selected_tape() {
    return selected_tape_.load(std::memory_order_relaxed);
}