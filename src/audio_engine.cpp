#include "audio_engine.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <cmath>
#include <cstring>

struct AudioEngine::Impl
{
    ma_context context;
    ma_device device;
    bool is_device_initialized{false};
};

AudioEngine::AudioEngine() : impl_(new Impl()) {}

AudioEngine::~AudioEngine()
{
    shutdown();
    delete impl_;
};

bool AudioEngine::init()
{
    if (ma_context_init(nullptr, 0, nullptr, &impl_->context) != MA_SUCCESS)
    {
        return false;
    }
    return select_devices(-1, -1); // init with default device
}

void AudioEngine::shutdown()
{
    if (impl_->is_device_initialized)
    {
        ma_device_uninit(&impl_->device);
        impl_->is_device_initialized = false;
    }
    ma_context_uninit(&impl_->context);
}

AudioDevices AudioEngine::get_audio_devices()
{
    std::vector<AudioDeviceInfo> playback_devices;
    ma_device_info *pPlaybackInfos;
    ma_uint32 playbackCount;

    std::vector<AudioDeviceInfo> capture_devices;
    ma_device_info *pCaptureInfos;
    ma_uint32 captureCount;

    if (ma_context_get_devices(&impl_->context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount) == MA_SUCCESS)
    {
        for (ma_uint32 i = 0; i < playbackCount; ++i)
        {
            playback_devices.push_back({pPlaybackInfos[i].name, static_cast<int>(i), pPlaybackInfos[i].isDefault != 0});
        }
        for (ma_uint32 i = 0; i < captureCount; ++i)
        {
            capture_devices.push_back({pCaptureInfos[i].name, static_cast<int>(i), pCaptureInfos[i].isDefault != 0});
        }
    }

    AudioDevices devices{playback_devices, capture_devices};
    return devices;
}

bool AudioEngine::select_devices(int playback_device_index, int capture_device_index)
{
    if (impl_->is_device_initialized)
    {
        ma_device_uninit(&impl_->device);
        impl_->is_device_initialized = false;
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

    ma_device_info *pPlaybackInfos;
    ma_uint32 playbackCount;
    ma_device_info *pCaptureInfos;
    ma_uint32 captureCount;
    ma_context_get_devices(&impl_->context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount);
    if (playback_device_index >= 0)
    {
        config.playback.pDeviceID = &pPlaybackInfos[playback_device_index].id;
    }

    if (capture_device_index >= 0)
    {
        config.capture.pDeviceID = &pCaptureInfos[capture_device_index].id;
    }

    if (ma_device_init(&impl_->context, &config, &impl_->device) != MA_SUCCESS)
    {
        return false;
    }

    impl_->is_device_initialized = true;
    return ma_device_start(&impl_->device) == MA_SUCCESS;
}

void AudioEngine::c_audio_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    auto *engine = static_cast<AudioEngine *>(pDevice->pUserData);
    engine->process_audio(static_cast<float *>(pOutput), static_cast<const float *>(pInput), frameCount);
}

void AudioEngine::process_audio(float *output, const float *input, unsigned int frameCount)
{
    float freq = frequency_.load(std::memory_order_relaxed);
    float vol = volume_.load(std::memory_order_relaxed);
    float phase_incr = (TWO_PI * freq) / SAMPLE_RATE;

    for (unsigned int i = 0; i < frameCount; ++i)
    {
        float sample = sin(phase_) * vol;

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

void AudioEngine::copy_scope_buffer(float *out_target, size_t count)
{
    size_t copy_size = min(count, SCOPE_SIZE);
    std::memcpy(out_target, scope_buffer_, copy_size * sizeof(float));
}