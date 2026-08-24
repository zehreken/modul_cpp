#pragma once

#include <vector>
#include <string>
#include <atomic>
#include <cstddef>

struct AudioDeviceInfo
{
    std::string name;
    int id_index;
    bool is_default;
};

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    AudioEngine(const AudioEngine &) = delete;
    AudioEngine &operator=(const AudioEngine &) = delete;

    bool init();
    void shutdown();

    std::vector<AudioDeviceInfo> get_playback_devices();
    bool select_playback_device(int device_index);

    std::vector<AudioDeviceInfo> get_capture_devices();
    bool select_capture_device(int device_index);

    void set_frequency(float freq) { frequency_.store(freq, std::memory_order_relaxed); };
    float get_frequency() const { return frequency_.load(std::memory_order_relaxed); };

    void set_volume(float volume) { volume_.store(volume, std::memory_order_relaxed); };
    float get_volume() const { return volume_.load(std::memory_order_relaxed); };

    void copy_scope_buffer(float *out_target, size_t count);

    static constexpr size_t SCOPE_SIZE = 512;

private:
    static constexpr float SAMPLE_RATE = 48000.0f;
    static constexpr float TWO_PI = 6.28318530717958647692f;

    static void c_audio_callback(struct ma_device *pDevice, void *pOutput, const void *pInput, unsigned int frameCount);
    void process_audio(float *output, const float *input, unsigned int frameCount);

    struct Impl;
    Impl *impl_;

    std::atomic<float> frequency_{440.0f};
    std::atomic<float> volume_{0.2f};
    float phase_{0.0};

    float scope_buffer_[SCOPE_SIZE]{0.0f};
    size_t scope_write_index_{0};
};