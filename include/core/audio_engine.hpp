#pragma once

#include <atomic>
#include <cstddef>
#include <string>
#include <vector>

struct AudioDeviceInfo {
    std::string name_;
    int channels_;
    int format_;
    int sample_rate_;
    int id_index_;
    bool is_default_;
};

struct AudioDevices {
    std::vector<AudioDeviceInfo> playback_devices_;
    std::vector<AudioDeviceInfo> capture_devices_;
};

class AudioEngine {
  public:
    AudioEngine();
    ~AudioEngine();

    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    bool init();
    void shutdown();

    AudioDevices get_audio_devices();
    bool select_devices(int playback_device_index, int capture_device_index);

    void set_frequency(float freq) {
        frequency_.store(freq, std::memory_order_relaxed);
    };
    float get_frequency() const {
        return frequency_.load(std::memory_order_relaxed);
    };

    void set_volume(float volume) {
        volume_.store(volume, std::memory_order_relaxed);
    };
    float get_volume() const {
        return volume_.load(std::memory_order_relaxed);
    };

    void toggle_play_through();
    bool can_play_through();

    void copy_scope_buffer(float* out_target, size_t count);

    static constexpr size_t SCOPE_SIZE = 512;

  private:
    static constexpr float SAMPLE_RATE = 48000.0f;
    static constexpr float TWO_PI = 6.28318530717958647692f;

    static void c_audio_callback(
        struct ma_device* device,
        void* output,
        const void* input,
        unsigned int frame_count
    );
    void process_audio(
        ma_device* device,
        float* output,
        const float* input,
        unsigned int frame_count
    );

    struct Impl;
    Impl* impl_;

    std::atomic<float> frequency_{440.0f};
    std::atomic<float> volume_{0.2f};
    float phase_{0.0};

    float scope_buffer_[SCOPE_SIZE]{0.0f};
    size_t scope_write_index_{0};

    bool can_play_through_;
};