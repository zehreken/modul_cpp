#include "audio_engine.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <algorithm>
#include <cmath>
#include <cstring>

struct AudioEngine::Impl {
    ma_context context_;
    ma_device device_;
    bool is_device_initialized_{false};
    bool is_context_initialized_{false};
};

AudioEngine::AudioEngine() : impl_(new Impl()) {}

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

    if (ma_context_get_devices(&impl_->context_, &playback_infos,
                               &playback_count, &capture_infos,
                               &capture_count) == MA_SUCCESS) {
        for (ma_uint32 i = 0; i < playback_count; ++i) {
            playback_devices.push_back({playback_infos[i].name,
                                        static_cast<int>(i),
                                        playback_infos[i].isDefault != 0});
        }
        for (ma_uint32 i = 0; i < capture_count; ++i) {
            capture_devices.push_back({capture_infos[i].name,
                                       static_cast<int>(i),
                                       capture_infos[i].isDefault != 0});
        }
    }

    AudioDevices devices{playback_devices, capture_devices};
    return devices;
}

bool AudioEngine::select_devices(int playback_device_index,
                                 int capture_device_index) {
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
    config.periodSizeInFrames = 128;
    config.dataCallback = AudioEngine::c_audio_callback;
    config.pUserData = this;

    ma_device_info* playback_infos;
    ma_uint32 playback_count;
    ma_device_info* capture_infos;
    ma_uint32 capture_count;
    if (ma_context_get_devices(&impl_->context_, &playback_infos,
                               &playback_count, &capture_infos,
                               &capture_count) != MA_SUCCESS) {
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

void AudioEngine::c_audio_callback(ma_device* device, void* output,
                                   const void* input, ma_uint32 frame_count) {
    auto* engine = static_cast<AudioEngine*>(device->pUserData);
    engine->process_audio(static_cast<float*>(output),
                          static_cast<const float*>(input), frame_count);
}

void AudioEngine::process_audio(float* output, const float* input,
                                unsigned int frame_count) {
    float freq = frequency_.load(std::memory_order_relaxed);
    float vol = volume_.load(std::memory_order_relaxed);
    float phase_incr = (TWO_PI * freq) / SAMPLE_RATE;

    for (unsigned int i = 0; i < frame_count; ++i) {
        float sample = std::sin(phase_) * vol;

        float in_left = input ? *input++ : 0.0f;
        float in_right = input ? *input++ : 0.0f;

        float out_left = (in_left * 0.5f) + (sample * 0.5f);
        float out_right = (in_right * 0.5f) + (sample * 0.5f);

        *output++ = out_left;
        *output++ = out_right;

        scope_buffer_[scope_write_index_] = out_left;
        scope_write_index_ = (scope_write_index_ + 1) % SCOPE_SIZE;

        phase_ += phase_incr;
        if (phase_ >= TWO_PI)
            phase_ -= TWO_PI;
    }
}

void AudioEngine::copy_scope_buffer(float* out_target, size_t count) {
    size_t copy_size = std::min(count, SCOPE_SIZE);
    std::memcpy(out_target, scope_buffer_, copy_size * sizeof(float));
}