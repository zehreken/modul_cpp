#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "core/metronome.hpp"
#include "core/tape.hpp"

struct ProjectConfig {
    int bpm_{120};
    int bar_count_{4};
    int selected_playback_device_id_{0};
    int seelcted_capture_device_id_{0};
};

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
    explicit AudioEngine();
    ~AudioEngine();

    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    bool init(ProjectConfig project_config);
    void update();
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
        master_volume_.store(volume, std::memory_order_relaxed);
    };
    float get_volume() const {
        return master_volume_.load(std::memory_order_relaxed);
    };

    void toggle_play_through();
    bool can_play_through();

    void toggle_record();
    bool can_record();

    void toggle_metronome();
    bool can_metronome_beep() { return metronome_->can_beep(); };
    int get_beat_count() { return metronome_->get_beat_count(); };

    void copy_recording(float* out_target, size_t count);
    Tape& get_tape(size_t id);

    void set_selected_tape(size_t id);
    size_t get_selected_tape();

    float* get_scope_buffer();

    static constexpr size_t BUFFER_SIZE = 128; // In frames

    int get_bpm() { return bpm_; };

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

    int bpm_{120};

    std::unique_ptr<Metronome> metronome_;

    std::atomic<float> frequency_{440.0f};
    std::atomic<float> master_volume_{0.2f};
    float phase_{0.0};

    size_t frame_index_{0};

    float scope_buffer_[BUFFER_SIZE * 2]{0.0f};
    size_t scope_write_index_{0};

    Tape recording_tape_;
    Tape tapes_[8];

    std::atomic<bool> can_play_through_{false};
    std::atomic<bool> can_record_{false};
    std::atomic<bool> can_metronome_run_{false};
    std::atomic<size_t> selected_tape_{0};
};